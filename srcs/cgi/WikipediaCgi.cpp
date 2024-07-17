#include <iostream>
#include <sstream>
#include <string>

std::string httpGet(const std::string &url)
{
    std::ostringstream oss;

    // Open a pipe to run the curl command
    FILE *pipe = popen(("curl -s '" + url + "'").c_str(), "r");
    if (!pipe)
        return "";

    // Read the output of the curl command
    char buffer[ 128 ];
    while (!feof(pipe))
        if (fgets(buffer, 128, pipe) != NULL)
            oss << buffer;
    pclose(pipe);

    return oss.str();
}

std::string extractFromJson(const std::string &json)
{
    std::string extract;

    // Find the "extract" field in the JSON response
    size_t start = json.find("\"extract\":\"");
    if (start != std::string::npos)
    {
        // Get the first paragraph of the extract
        start = json.find("<b>", start);
        size_t end = json.find("</p>", start);
        extract = json.substr(start, end - start);
    }
    return extract;
}

int main(int argc, char **argv)
{
    // the first argument is the query
    std::string query(argv[ 1 ]);

    // initialize the response
    std::ostringstream response;

    // initialize the body
    std::string body;

    // Make a Wikipedia API request for the query
    std::string json_response =
        httpGet("https://en.wikipedia.org/w/"
                "api.php?action=query&format=json&prop=extracts&titles=" +
                query + "&redirects=true");

    // check if the request failed
    if (json_response.empty())
    {
        // pipe failed, set error Status Line
        response << "HTTP/1.1 500 Internal Server Error\r\n";

        // empty body
        body = "";
    }
    else
    {
        // success, set OK Status Line
        response << "HTTP/1.1 200 OK\r\n";

        // format the body
        // set it with the first paragraph of the extract
        body = "<!DOCTYPE html>\n"
               "<html lang=\"en\">\n"
               "<head>\n"
               "    <meta charset=\"UTF-8\">\n"
               "    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n"
               "    <meta name=\"viewport\" content=\"width=device-width, "
               "initial-scale=1.0\">\n"
               "    <title>Wikipedia Extract</title>\n"
               "    <style>\n"
               "        body {\n"
               "            font-family: Arial, sans-serif;\n"
               "            background-color: #333;\n"
               "            color: #f4f4f4;\n"
               "            margin: 0;\n"
               "            padding: 0;\n"
               "            display: flex;\n"
               "            align-items: center;\n"
               "            justify-content: center;\n"
               "            height: 100vh;\n"
               "        }\n"
               "        .container {\n"
               "            width: 80%;\n"
               "            text-align: center;\n"
               "            position: relative;\n"
               "        }\n"
               "        .extract {\n"
               "            background: #444;\n"
               "            border: #333 solid 1px;\n"
               "            padding: 20px;\n"
               "            position: relative;\n"
               "        }\n"
               "        .extract h1 {\n"
               "            color: #f90;\n"
               "            font-size: 33px;\n"
               "            margin-bottom: 5px;\n"
               "        }\n"
               "        .extract p {\n"
               "            font-size: 21px;\n"
               "        }\n"
               "        .version {\n"
               "            position: absolute;\n"
               "            bottom: 5px;\n"
               "            right: 5px;\n"
               "            font-size: 14px;\n"
               "            color: #999;\n"
               "        }\n"
               "    </style>\n"
               "</head>\n"
               "<body>\n"
               "    <div class=\"container\">\n"
               "        <div class=\"extract\">\n"
               "            <h1>Wikipedia Extract for '" +
               query +
               "'</h1>\n"
               "            <p>" +
               extractFromJson(json_response) +
               "</p>\n"
               "            <div class=\"version\">webserv/1.0</div>\n"
               "        </div>\n"
               "    </div>\n"
               "</body>\n"
               "</html>";
    }

    response << "content-type: text/html\r\n";
    response << "content-length: " << body.length() << "\r\n";
    response << "Connection: close\r\n";
    response << "Server: webserv/1.0\r\n";
    response << "\r\n";
    response << body;

    std::cout << response.str();
    return 0;
}
