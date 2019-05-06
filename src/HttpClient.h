#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>

#include <string>
#include <vector>
#include <map>
#include <sstream>

// F(id, str)
#define FOREACH_CONTENT_TYPE(F)                                             \
    F(TEXT_PLAIN,                   "text/plain")                           \
    F(TEXT_HTML,                    "text/html")                            \
    F(TEXT_XML,                     "text/xml")                             \
    F(APPLICATION_JSON,             "application/json")                     \
    F(APPLICATION_XML,              "application/xml")                      \
    F(APPLICATION_JAVASCRIPT,       "application/javascript")               \
    F(FORM_DATA,                    "multipart/form-data")                  \
    F(X_WWW_FORM_URLENCODED,        "application/x-www-form-urlencoded")    \
    F(QUERY_STRING,                 "text/plain")

#define ENUM_CONTENT_TYPE(id, _)    id,

typedef std::map<std::string, std::string> KeyValue;

struct FormData 
{
    enum FormDataType 
    {
        CONTENT,
        FILENAME
    } type;
    
    FormData() 
    {
        type = CONTENT;
    }

    FormData(const char* data, FormDataType type = CONTENT) 
    {
        this->type = type;
        this->data = data;
    }

    FormData(int n) 
    {
        this->type = CONTENT;
        this->data = std::to_string(n);
    }

    FormData(float f) 
    {
        std::ostringstream ss;
        ss << f;
        this->type = CONTENT;
        this->data = ss.str();
    }

    std::string data;
};

typedef std::map<std::string, FormData> Form;

struct http_request 
{
    enum ContentType 
    {
        NONE,
        FOREACH_CONTENT_TYPE(ENUM_CONTENT_TYPE)
        LAST
    } content_type;

    std::string method;
    std::string url;
    std::string text;
    KeyValue kvs;
    Form form;
};

typedef std::string http_response;

class http_client 
{
public:
    http_client();
    ~http_client();

    int send(http_request& req, http_response& res);
    void set_debug(bool b) { m_bDebug = b; }
    void set_timeout(int second) { m_timeout = second; }
    void add_header(std::string header) { m_headers.push_back(header); }
    void reset_header() { m_headers.clear(); }

protected:
    int curl(http_request& req, http_response& res);

private:
    static bool s_bInit;
    bool m_bDebug;
    std::vector<std::string> m_headers;
    int m_timeout;
};

#endif
