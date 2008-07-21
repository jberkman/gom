/*
The MIT License

Copyright (c) 2008 jacob berkman <jacob@ilovegom.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef GOM_XML_HTTP_REQUEST_H
#define GOM_XML_HTTP_REQUEST_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomXMLHttpRequest          GomXMLHttpRequest; /* dummy object */
typedef struct _GomXMLHttpRequestInterface GomXMLHttpRequestInterface;

typedef enum {
    GOM_UNSENT = 0,
    GOM_OPENED = 1,
    GOM_HEADERS_RECEIVED = 2,
    GOM_LOADING = 3,
    GOM_DONE = 4
} GomXmlHttpRequestState;

typedef GomXmlHttpRequestState     GomXMLHttpRequestState;

#define GOM_XML_HTTP_REQUEST_OMITTED ((const char *)GINT_TO_POINTER(1))

G_END_DECLS

#include <gom/dom/gomdocument.h>

G_BEGIN_DECLS

#define GOM_TYPE_XML_HTTP_REQUEST             (gom_xml_http_request_get_type ())
#define GOM_XML_HTTP_REQUEST(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_XML_HTTP_REQUEST, GomXMLHttpRequest))
#define GOM_IS_XML_HTTP_REQUEST(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_XML_HTTP_REQUEST))
#define GOM_XML_HTTP_REQUEST_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_XML_HTTP_REQUEST, GomXMLHttpRequestInterface))

#define _GOM_IMPLEMENT_XML_HTTP_REQUEST(i, p, f) (((GomXMLHttpRequestInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_XML_HTTP_REQUEST(i, p)                            \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, open);                   \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, set_request_header);     \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, send);                   \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, send_string);            \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, send_document);          \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, abort);                  \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, get_all_response_headers); \
        _GOM_IMPLEMENT_XML_HTTP_REQUEST (i, p, get_response_header);    \
    } G_STMT_END

struct _GomXMLHttpRequestInterface {
    GTypeInterface parent;

    void (*open) (GomXMLHttpRequest *xml_http_request,
                  const char        *method,
                  const char        *url,
                  gboolean           async,
                  const char        *user,
                  const char        *password,
                  GError           **error);

    void (*set_request_header) (GomXMLHttpRequest *xml_http_request,
                                const char *header,
                                const char *value,
                                GError    **error);

    void (*send) (GomXMLHttpRequest *xml_http_request,
                  GError           **error);

    void (*send_string) (GomXMLHttpRequest *xml_http_request,
                         const char        *data,
                         GError           **error);

    void (*send_document) (GomXMLHttpRequest *xml_http_request,
                           const GomDocument *doc,
                           GError           **error);

    void (*abort) (GomXMLHttpRequest *xml_http_request);

    char *(*get_all_response_headers) (const GomXMLHttpRequest *xml_http_request,
                                       GError                 **error);

    char *(*get_response_header) (const GomXMLHttpRequest *xml_http_request,
                                  const char              *header,
                                  GError                 **error);
};

GType gom_xml_http_request_get_type (void);

void gom_xml_http_request_open (GomXMLHttpRequest *xml_http_request,
                                const char        *method,
                                const char        *url,
                                gboolean           async,
                                const char        *user,
                                const char        *password,
                                GError           **error);

void gom_xml_http_request_set_request_header (GomXMLHttpRequest *xml_http_request,
                                              const char *header,
                                              const char *value,
                                              GError    **error);

void gom_xml_http_request_send (GomXMLHttpRequest *xml_http_request,
                                GError           **error);

void gom_xml_http_request_send_string (GomXMLHttpRequest *xml_http_request,
                                       const char        *data,
                                       GError           **error);

void gom_xml_http_request_send_document (GomXMLHttpRequest *xml_http_request,
                                         const GomDocument *doc,
                                         GError           **error);

void gom_xml_http_request_abort (GomXMLHttpRequest *xml_http_request);

char *gom_xml_http_request_get_all_response_headers (const GomXMLHttpRequest *xml_http_request,
                                                     GError                 **error);

char *gom_xml_http_request_get_response_header (const GomXMLHttpRequest *xml_http_request,
                                                const char              *header,
                                                GError                 **error);

#endif /* GOM_XML_HTTP_REQUEST_H */
