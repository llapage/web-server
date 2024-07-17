#include "../../includes/request/Request.hpp"

RequestState::RequestState()
{
    m_content_length = 0;
    m_content_red = 0;
    m_finished = false;
    m_headers = false;
    m_initial = true;
    m_route = NULL;
}

bool RequestState::finished() const { return m_finished; }
bool RequestState::headers() const { return m_headers; }
bool RequestState::initial() const { return m_initial; }
int RequestState::getContentLength() const { return m_content_length; }
int RequestState::getContentRed() const { return m_content_red; }
IRoute *RequestState::getRoute() const { return m_route; }

void RequestState::finished(bool value) { m_finished = value; }
void RequestState::headers(bool value) { m_headers = value; }
void RequestState::initial(bool value) { m_initial = value; }
void RequestState::setContentLength(int value) { m_content_length = value; }
void RequestState::incrementContentRed() { m_content_red++; }
void RequestState::setContentRed(int value) { m_content_red = value; }
void RequestState::reset()
{
    m_finished = false;
    m_initial = true;
    m_content_red = 0;
    m_content_length = 0;
}

void RequestState::setRoute(IRoute *route) { m_route = route; }
