#ifndef C_HTTP_H
#define C_HTTP_H

#include "/opt/localc/cheaders/ckvmap.h"
#include "/opt/localc/cheaders/assertf.h"

#define HTTP_CRLF "\r\n"

typedef KVMap HttpHeader;
typedef KVMap HttpQuery;
typedef enum _HttpMethod HttpMethod;
typedef struct _HttpRequest HttpRequest;
typedef struct _HttpResponse HttpResponse;

enum _HttpMethod {
    HTTP_GET,
    HTTP_PUT,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_TRACE,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_UNKNOWN,
};

struct _HttpRequest {
    copy  HttpMethod    Method;
    owned char *        Target;
    copy  real          Version;
    owned HttpHeader *  Headers;
    owned char *        Body;
};

struct _HttpResponse {
    copy  real          Version;
    copy  u16           Status;
    owned HttpHeader *  Headers;
    owned char *        Body;
};

owned HttpQuery * chttp_parse_request_query(borrowed HttpRequest * request);
owned HttpQuery * chttp_parse_query(owned char * querystr);
owned char * chttp_request_target_path(borrowed HttpRequest * request);
owned char * chttp_request_target_queries(borrowed HttpRequest * request);
copy bool chttp_request_header_exist(borrowed HttpRequest * request, borrowed const char * const key);
owned char * chttp_request_header_get(borrowed HttpRequest * request, borrowed const char * const key);
copy bool chttp_response_header_exist(borrowed HttpResponse * response, borrowed const char * const key);
owned char * chttp_response_header_get(borrowed HttpResponse * response, borrowed const char * const key);
copy bool chttp_header_exist(borrowed HttpHeader * headers, borrowed const char * const key);
owned char * chttp_header_get(borrowed HttpHeader * headers, borrowed const char * const key);
owned HttpRequest * chttp_analyze_request(owned char * content);
owned HttpResponse * chttp_analyze_response(owned char * content);
void chttp_free_request(owned HttpRequest * request);
void chttp_free_response(owned HttpResponse * response);
borrowed char * chttp_method(copy HttpMethod method);
borrowed char * chttp_status_message(copy u16 statuscode);
void chttp_free_headers(owned HttpHeader * headers);
void chttp_free_queries(owned HttpQuery * queries);

#endif // C_HTTP_H
