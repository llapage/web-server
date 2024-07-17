#!/usr/bin/python3
import cgi
import os
import json
# write the response



print("HTTP/1.1 200 OK")
print("Content-type: application/json")
print("")

request = os.environ['REQUEST_METHOD']
print("Request Method:", request)

if (request == 'POST'):
	form = cgi.FieldStorage()
	post_data = {key: form.getvalue(key) for key in form.keys()}
	print('POST DATA: ', json.dumps(post_data))

