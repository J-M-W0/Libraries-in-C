#include "/opt/localc/cheaders/crayon.h"
#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cstring.h"
#include "/opt/localc/cheaders/ckvmap.h"
#include "/opt/localc/cheaders/cmove.h"
#include "/opt/localc/cheaders/chttp.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static void chttp_warning(borrowed const char * fmt, ...)
{
    fprintf(stderr, "[" BOLD YELLOW "CHTTP" NOCRAYON "]: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

static void chttp_panic(borrowed const char * fmt, ...)
{
    fprintf(stderr, "[" BOLD RED "CHTTP" NOCRAYON "]: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(EXIT_FAILURE);
}

static void chttp_abort()
{
    exit(EXIT_FAILURE);
}

static copy HttpMethod chttp_parse_request_httpmethod(borrowed char ** contnt) 
{
    borrowed char * content = deref(contnt, char*);
    copy HttpMethod method = HTTP_UNKNOWN;
    owned char * http_method = cut_before_delim(content, ' ');
    scp(http_method);
    if (streq("GET", http_method)) {
        method = HTTP_GET;
    }
    else if (streq("PUT", http_method)) {
        method = HTTP_PUT;
    }
    else if (streq("HEAD", http_method)) {
        method = HTTP_HEAD;
    }
    else if (streq("POST", http_method)) {
        method = HTTP_POST;
    }
    else if (streq("TRACE", http_method)) {
        method = HTTP_TRACE;
    }
    else if (streq("DELETE", http_method)) {
        method = HTTP_DELETE;
    }
    else if (streq("CONNECT", http_method)) {
        method = HTTP_CONNECT;
    }
    else if (streq("OPTIONS", http_method)) {
        method = HTTP_OPTIONS;
    }
    else {
        chttp_warning("Unknown HTTP Method - " BOLD RED "%s" NOCRAYON, http_method);
        drop(http_method);
        chttp_abort();
    }
    content = truncate_from_left(content, strlen(http_method));
    deref(contnt, char*) = content;
    drop(http_method);
    return method;
}

static owned char * chttp_parse_request_uri(borrowed char ** contnt)
{
    borrowed char * content = deref(contnt, char*);
    content = trim_left(content, WHITESPACES);
    owned char * uri = cut_before_delim(content, ' ');
    if (!uri) chttp_panic("Failed at parsing the Target!");
    content = truncate_from_left(content, strlen(uri));
    deref(contnt, char*) = content;
    return uri;
}

static copy real chttp_parse_request_version(borrowed char ** contnt)
{
    borrowed char * content = deref(contnt, char*);
    content = trim_left(content, WHITESPACES);
    owned char * version = cut_before_delims(content, WHITESPACES);
    copy real http_version = 0;
    if (streq("HTTP/1.1", version)) {
        http_version = 1.1;
    }
    else if (streq("HTTP/1.2", version)) {
        http_version = 1.2;
    }
    else {
        chttp_warning("Unknown HTTP Version - " BOLD RED "%s" NOCRAYON, version);
    }
    content = truncate_from_left(content, strlen(version));
    drop(version);
    deref(contnt, char*) = content;
    return http_version;
}

owned KVMap * chttp_parse_headers(borrowed char ** contnt)
{
    borrowed char * content = deref(contnt, char*);
    content = trim_left(content, WHITESPACES);

    owned KVMap * headers = kvmap_create(); scp(headers);
    owned char * line = cut_before_delims(content, "\r\n");
    while (line[0] != '\0') {
        Option option = strloc(line, ':');
        if (none(&option)) {
            chttp_panic("Unknown header: " "%s", line);
        }
        u64 colonindex = deref(Some(&option), u64);                                         free(Some(&option));
        owned char * key   = trim(substr(line, 0, colonindex), WHITESPACES);                scp( key );
        owned char * value = trim(substr(line, colonindex+1, strlen(line)), WHITESPACES);   scp(value);
        kvmap_insert(headers, key, value);
        drop(key);

        // truncate the line + \r\n
        content = truncate_from_left(content, strlen(line) + 2);
        drop(line);
        line = cut_before_delims(content, "\r\n");
    }
    drop(line);

    deref(contnt, char*) = content;
    return headers;
}

static owned char * chttp_parse_body(owned char * content)
{
    return trim(content, WHITESPACES);
}

copy bool chttp_header_exist(borrowed HttpHeader * headers, borrowed const char * const key)
{
    if (!headers || !key) return false;
    return kvmap_has_key(headers, key);
}
owned char * chttp_header_get(borrowed HttpHeader * headers, borrowed const char * const key)
{
    if (!headers || !key) return nil;
    return strdup(kvmap_get(headers, key));
}

copy bool chttp_request_header_exist(borrowed HttpRequest * request, borrowed const char * const key)
{
    if (!request || !request->Headers || !key) return false;
    return kvmap_has_key(request->Headers, key);
}
owned char * chttp_request_header_get(borrowed HttpRequest * request, borrowed const char * const key)
{
    if (!request || !request->Headers || !key) return nil;
    return strdup(kvmap_get(request->Headers, key));
}

copy bool chttp_response_header_exist(borrowed HttpResponse * response, borrowed const char * const key)
{
    if (!response || !response->Headers || !key) return false;
    return kvmap_has_key(response->Headers, key);
}
owned char * chttp_response_header_get(borrowed HttpResponse * response, borrowed const char * const key)
{
    if (!response || !response->Headers || !key) return false;
    return strdup(kvmap_get(response->Headers, key));
}

owned char * chttp_request_target_path(borrowed HttpRequest * request)
{
    if (!request || !request->Target) return nil;
    Option option = strloc(request->Target, '?');
    if (none(&option)) return nil;
    u64 question_mark_index = deref(Some(&option), u64);
    drop(Some(&option));
    return substr(request->Target, 0, question_mark_index);
}

owned char * chttp_request_target_queries(borrowed HttpRequest * request)
{
    if (!request || !request->Target) return nil;
    Option option = strloc(request->Target, '?');
    if (none(&option)) return nil;
    u64 question_mark_index = deref(Some(&option), u64);
    free(Some(&option));
    return substr(request->Target, question_mark_index+1, strlen(request->Target));
}

owned HttpQuery * chttp_parse_request_query(borrowed HttpRequest * request)
{
    if (!request) return nil;
    return chttp_parse_query(chttp_request_target_queries(request));
}
owned HttpQuery * chttp_parse_query(owned char * querystr)
{
    if (!querystr) return nil;
    HttpQuery * queries = kvmap_create();

    Option option;
    bool done = false;
    do {
        option = strloc(querystr, '=');
        if (none(&option)) chttp_panic("Query string" BOLD RED " %s " NOCRAYON "has bad format!", querystr);
        u64 eqindex = deref(Some(&option), u64);
        drop(Some(&option));
        char * key = substr(querystr, 0, eqindex);
        char * value = nil;

        option = strloc(querystr, '&');
        if (none(&option)) {
            value = substr(querystr, eqindex+1, strlen(querystr));
            done = true;
        } else {
            u64 ampersand_index = deref(Some(&option), u64);
            value = substr(querystr, eqindex+1, ampersand_index);
            querystr = truncate_from_left(querystr, ampersand_index+1);
        }
        kvmap_insert(queries, key, value);
        drop(key);
        drop(Some(&option));
    } while (!done);
    drop(querystr);

    if (kvmap_size(queries) == 0) {
        kvmap_easy_free(queries);
        queries = nil;
    }
    return queries;
}

// {content} will be treated as an owned string, thus will be freed by this function.
owned HttpRequest * chttp_analyze_request(owned char * content)
{
    scp(content);
    content = trim(content, WHITESPACES);


    HttpRequest * request = new(HttpRequest); scp(request);


    // Parse the Request Line
    request->Method = chttp_parse_request_httpmethod(&content);
    request->Target = chttp_parse_request_uri(&content);
    request->Version = chttp_parse_request_version(&content);

    // Parse the Request Headers and the Empty Line
    request->Headers = chttp_parse_headers(&content);

    // Parse the Request Body (Optional)
    request->Body = chttp_parse_body(content);

    return request;
}


/* * 
 * === === Reponse === === 
 * */


static copy real chttp_parse_response_version(borrowed char ** contnt)
{
    borrowed char * content = deref(contnt, char*);
    content = trim_left(content, WHITESPACES);
    copy real http_version = 0;

    owned char * version = cut_before_delim(content, ' ');
    if (streq("HTTP/1.1", version)) {
        http_version = 1.1;
    }
    else if (streq("HTTP/1.2", version)) {
        http_version = 1.2;
    }
    else {
        chttp_warning("Unknown HTTP Version - " BOLD RED "%s" NOCRAYON, version);
        drop(version);
        chttp_abort();
    }

    content = truncate_from_left(content, strlen(version));
    drop(version);
    deref(contnt, char*) = content;
    return http_version;
}

static copy u16 chttp_parse_response_status_code(borrowed char ** contnt)
{
    borrowed char * content = deref(contnt, char*);
    content = trim_left(content, WHITESPACES);
    copy u16 status_code = 0;

    owned char * statuscode = cut_before_delim(content, deref(SPACE, char));
    owned Option option = tonumber(statuscode);
    if (none(&option)) {
        chttp_warning("Unknown HTTP Status Code - " BOLD RED "%s" NOCRAYON, statuscode);
        drop(statuscode);
        chttp_abort();
    }
    status_code = deref(Some(&option), u16);
    drop(Some(&option));


    borrowed char * status_message = chttp_status_message(status_code);
    if (status_message == nil) {
        chttp_warning("Unknown HTTP Status Code - " BOLD RED "%s" NOCRAYON, statuscode);
        drop(statuscode);
        chttp_abort();
    }
    content = truncate_from_left(content, strlen(statuscode));
    content = trim_left(content, WHITESPACES);
    drop(statuscode);


    owned char * statusmessage = cut_before_delims(content, HTTP_CRLF);
    if (!streq(status_message, statusmessage)) {
        chttp_warning(
            "Response Status Message Mismatch - should be " BOLD YELLOW "%s" NOCRAYON ", but received " BOLD RED "%s" NOCRAYON,
            status_message,
            statusmessage
        );
        drop(statusmessage);
        chttp_abort();
    }
    content = truncate_from_left(content, strlen(statusmessage));
    drop(statusmessage);

    deref(contnt, char*) = content;
    return status_code;
}


owned HttpResponse * chttp_analyze_response(owned char * content)
{
    scp(content);
    content = trim(content, WHITESPACES);

    owned HttpResponse * response = new(HttpResponse); scp(response);

    // Parse Status Line.
    response->Version = chttp_parse_response_version(&content);
    response->Status = chttp_parse_response_status_code(&content);

    // Parse Header(s)
    response->Headers = chttp_parse_headers(&content);

    // Parse Response Body (Optional)
    response->Body = chttp_parse_body(content);

    return response;
}


void chttp_free_request(owned HttpRequest * request)
{
    if (!request) return;
    if (request->Headers) {
        chttp_free_headers(request->Headers);
    }
    drop(request->Target);
    drop(request->Body);
    drop(request);
}

void chttp_free_response(owned HttpResponse * response)
{
    if (!response) return;
    if (response->Headers) {
        chttp_free_headers(response->Headers);
    }
    drop(response->Body);
    drop(response);
}

void chttp_free_queries(owned HttpQuery * queries)
{
    if (!queries) return;
    kvmap_easy_free(queries);
}

void chttp_free_headers(owned HttpHeader * headers)
{
    if (!headers) return;
    kvmap_easy_free(headers);
}

borrowed char * chttp_method(copy HttpMethod method)
{
    switch (method) 
    {
        case HTTP_GET:
            return "GET";
        case HTTP_PUT:
            return "PUT";
        case HTTP_HEAD:
            return "HEAD";
        case HTTP_POST:
            return "POST";
        case HTTP_TRACE:
            return "TRACE";
        case HTTP_DELETE:
            return "DELETE";
        case HTTP_CONNECT:
            return "CONNECT";
        case HTTP_OPTIONS:
            return "OPTIONS";
        default:
            return nil;
    }
}

borrowed char * chttp_status_message(copy u16 statuscode)
{
    switch (statuscode) 
    {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocols";

        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";

        case 300:
            return "Multiple Choices";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 305:
            return "Use Proxy";
        case 306:
            return "Unused";
        case 307:
            return "Temporary Redirect";

        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 402:
            return "Payment Required";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Request Entity Too Large";
        case 414:
            return "Request-url Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Requested Range Not Satisified";
        case 417:
            return "Expectation Failed";

        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        default:
            return nil;
    }
}
