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
#include "config.h"

#include "gom/dom/gomxmlhttprequest.h"

#include "gom/gomxhr.h"

#include "gom/dom/gomeventtarget.h"
#include "gom/dom/gomdomexception.h"
#include "gom/gomuri.h"

#include "gommacros.h"

#include <string.h>
#include <curl/curl.h>

enum {
    PROP_DOCUMENT = 1,
    PROP_ONREADYSTATECHANGE,
    PROP_READY_STATE,
    PROP_RESPONSE_TEXT,
    PROP_RESPONSE_XML,
    PROP_STATUS,
    PROP_STATUS_TEXT
};

typedef struct {
    GomDocument *doc;

    GSList *reqhdr_names;
    GSList *reqhdr_values;

    GomURI *uri;

    GomXMLHttpRequestState state;

    gboolean send : 1;
} GomXhrPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_XHR, GomXhrPrivate)

static void
gom_xhr_get_property (GObject    *object,
                      guint       property_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
    GomXhrPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_ONREADYSTATECHANGE:
        break;
    case PROP_READY_STATE:
        g_value_set_enum (value, priv->state);
        break;
    case PROP_RESPONSE_TEXT:
        break;
    case PROP_RESPONSE_XML:
        break;
    case PROP_STATUS:
        break;
    case PROP_STATUS_TEXT:
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_xhr_set_property (GObject *object,
                      guint property_id,
                      const GValue *value,
                      GParamSpec *pspec)
{
    GomXhrPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_DOCUMENT:
        GOM_SET_WEAK (priv->doc, g_value_get_object (value));
        break;
    case PROP_ONREADYSTATECHANGE:
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GomEventTarget *
gom_xhr_get_parent_target (GomEventTargetInternal *target)
{
    return NULL;
}

#define gom_xhr_dispatch_listeners gom_evt_tgt_dispatch_listeners

#define TOK_SEPARATORS "()<>@,;:\"/[]?={} \t"

/*
       CTL            = <any US-ASCII control character
                        (octets 0 - 31) and DEL (127)>
       SP             = <US-ASCII SP, space (32)>
       HT             = <US-ASCII HT, horizontal-tab (9)>
       token          = 1*<any CHAR except CTLs or separators>
       separators     = "(" | ")" | "<" | ">" | "@"
                      | "," | ";" | ":" | "\" | <">
                      | "/" | "[" | "]" | "?" | "="
                      | "{" | "}" | SP | HT
*/
static gboolean
is_token (const char *s)
{
    gunichar c;
    while (*s) {
        c = g_utf8_get_char (s);
        if (c < 32 || c >= 127 || strchr (TOK_SEPARATORS, c)) {
            return FALSE;
        }
        s = g_utf8_next_char (s);
    }
    return TRUE;
}

const char *matched_methods[] = {
    "CONNECT",
    "DELETE",
    "GET",
    "HEAD",
    "OPTIONS",
    "POST",
    "PUT",
    "TRACE",
    "TRACK",
    NULL
};

const char *disallowed_methods[] = {
    "CONNECT",
    "TRACE",
    "TRACK",
    NULL
};

/* http://www.w3.org/TR/XMLHttpRequest/#open */
static void
gom_xhr_open (GomXMLHttpRequest *xml_http_request,
              const char        *method,
              const char        *url,
              gboolean           async,
              const char        *user,
              const char        *password,
              GError           **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    const char **s;
    GomURI *uri;
    char *scheme;
    const curl_version_info_data *curl_data;
    const char * const*protocol;

    /* 1. Let stored method be the method argument. */

    /* 
     * 2. If stored method does not match the Method production,
     * defined in section 5.1.1 of RFC 2616, raise a SYNTAX_ERR
     * exception and terminate these steps. [RFC2616]
     */
    if (!method || !is_token (method)) {
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_SYNTAX_ERR, "method is not a valid token");
        return;
    }

    /*
     * 3. If stored method case-insensitively matches CONNECT, DELETE,
     * GET, HEAD, OPTIONS POST, PUT, TRACE, or TRACK let stored method
     * be the canonical uppercase form of the matched method name.
     */
    for (s = matched_methods; *s; s++) {
        if (!strcasecmp (method, *s)) {
            method = *s;
            break;
        }
    }

    /*
     * 4. If stored method is one of CONNECT, TRACE, or TRACK the user
     * agent should raise a SECURITY_ERR exception and terminate these
     * steps.
     */
    for (s = disallowed_methods; *s; s++) {
        if (!strcasecmp (method, *s)) {
            g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_SECURITY_ERR,
                         "disallowed method specified");
            return;
        }
    }

    /*
     * 5.  Drop the fragment identifier (if any) from url and let
     * stored url be the result of that operation.
     */
    uri = g_object_new (GOM_TYPE_URI,
                        "uri", url,
                        "fragment", NULL,
                        NULL);

    /*
     * 6.  If stored url is a relative reference resolve it using the
     * current value of the baseURI attribute of the Document
     * pointer. If this fails raise a SYNTAX_ERR exception and
     * terminate these steps.
     */
    if (gom_uri_is_relative (uri)) {
        char *new_uri = NULL;
        if (priv->doc) {
            char *base_uri;
            g_object_get (priv->doc, "document-u-r-i", &base_uri, NULL);
            if (base_uri && *base_uri) {
                char *rel_uri;
                g_object_get (uri, "uri", &rel_uri, NULL);
                new_uri = gom_uri_join (base_uri, rel_uri);
                g_free (rel_uri);
            }
            g_free (base_uri);
        }
        g_object_unref (uri);
        if (!new_uri) {
            g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_SYNTAX_ERR,
                         "could not resolve relative uri");
            return;
        }
        uri = g_object_new (GOM_TYPE_URI, "uri", new_uri, NULL);
    }

    /*
     * 7. If stored url contains an unsupported scheme raise a
     * NOT_SUPPORTED_ERR and terminate these steps.
     */
    g_object_get (uri, "scheme", &scheme, NULL);
    curl_data = curl_version_info (CURLVERSION_NOW);
    for (protocol = curl_data->protocols; *protocol; protocol++) {
        if (!strcmp (scheme, *protocol)) {
            break;
        }
    }
    g_free (scheme);
    if (!*protocol) {
        g_object_unref (uri);
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_NOT_SUPPORTED_ERR,
                     "scheme not supported by cURL");
        return;
    }

    /*
     * 8. If the "user:password" format in the userinfo production
     * defined in section 3.2.1 of RFC 3986 is not supported for the
     * relevant scheme and stored url contains this format raise a
     * SYNTAX_ERR and terminate these steps. [RFC3986]
     */

    /*
     * 9. If stored url contains the "user:password" format let stored
     * user be the user part and stored password be the password part.
     */

    /*
     * 10. If stored url just contains the "user" format let stored
     * user be the user part.
     */

    /*
     * 11. If stored url is not of the same-origin as the origin of
     * the Document pointer the user agent should raise a SECURITY_ERR
     * exception and terminate these steps.
     */

    /*
     * 12. Let async be the value of the async argument or true if it
     * was omitted.
     */

    /*
     * 13. If the user argument was not omitted, and its syntax does
     * not match that specified by the relevant authentication scheme,
     * raise a SYNTAX_ERR exception and terminate these steps.
     */
    /*
     * 14. If the user argument was not omitted and is not null let
     * stored user be user encoded using the encoding specified in the
     * relevant authentication scheme or UTF-8 if the scheme fails to
     * specify an encoding.
     */
    /*
     * 15. If the user argument was not omitted and is null remove
     * stored user.
     */
    if (user != GOM_XML_HTTP_REQUEST_OMITTED) {
        g_object_set (uri, "user", user, NULL);
    }

    /*
     * 16.If the password argument was not omitted and its syntax does
     * not match that specified by the relevant authentication scheme
     * raise a SYNTAX_ERR exception and terminate these steps.
     */
    /*
     * 17. If the password argument was not omitted and is not null
     * let stored password be password encoded using the encoding
     * specified in the relevant authentication scheme or UTF-8 if the
     * scheme fails to specify an encoding.
     */
    /*
     * 18. If the password argument was not omitted and is null remove
     * stored password.
     */
    if (password != GOM_XML_HTTP_REQUEST_OMITTED) {
        g_object_set (uri, "password", password, NULL);
    }

    /*
     * 19. Abort the send() algorithm, set response entity body to
     * "null" and reset the list of request headers.
     */
    g_slist_foreach (priv->reqhdr_names, (GFunc)g_free, NULL);
    g_slist_free (priv->reqhdr_names);
    priv->reqhdr_names = NULL;

    g_slist_foreach (priv->reqhdr_values, (GFunc)g_free, NULL);
    g_slist_free (priv->reqhdr_values);
    priv->reqhdr_values = NULL;
    /*
     * 20. The user agent should cancel any network activity for which
     * the object is responsible.
     */
    /*
     * 21. Switch the object to the OPENED state, set the send() flag
     * to "false" and then synchronously dispatch a readystatechange
     * event on the object and return the method call.
     */
    priv->state = GOM_OPENED;
    priv->send = FALSE;
    if (priv->uri) {
        g_object_unref (priv->uri);
    }
    priv->uri = uri;
}

/*
       CR             = <US-ASCII CR, carriage return (13)>
       LF             = <US-ASCII LF, linefeed (10)>
       LWS            = [CRLF] 1*( SP | HT )
*/
static gboolean
is_lws (const char **s)
{
    gunichar c;

    g_assert (g_utf8_get_char (*s) == '\r');

    *s = g_utf8_next_char (*s);
    c = g_utf8_get_char (*s);
    if (c != '\n') {
        return FALSE;
    }
    *s = g_utf8_next_char (*s);
    c = g_utf8_get_char (*s);
    if (c != '\t' && c != ' ') {
        return FALSE;
    }
    /* this is valid LWS; eat the rest of leading ws */
    while (**s) {
        c = g_utf8_get_char (*s);
        if (c != '\t' && c != ' ') {
            break;
        }
        *s = g_utf8_next_char (*s);
    }
    return TRUE;
}

/*
       quoted-string  = ( <"> *(qdtext | quoted-pair ) <"> )
       CHAR           = <any US-ASCII character (octets 0 - 127)>
       quoted-pair    = "\" CHAR
       qdtext         = <any TEXT except <">>
*/
static gboolean
is_quoted_string (const char **s)
{
    gunichar c;

    g_assert (g_utf8_get_char (*s) == '"');

    while (**s) {
        c = g_utf8_get_char (*s);
        switch (c) {
        case '"':
            return TRUE;
        case '\r':
            if (is_lws (s)) {
                continue;
            } else {
                return FALSE;
            }
            g_assert_not_reached ();
        case '\\':
            *s = g_utf8_next_char (*s);
            if (g_utf8_get_char (*s) >= 128) {
                return FALSE;
            }
            break;
        default:
            if (c < 32 || c >= 127) {
                return FALSE;
            }
            break;                
        }
        *s = g_utf8_next_char (*s);
    }
    return FALSE;
}

/*
       TEXT           = <any OCTET except CTLs,
                        but including LWS>
       field-value    = *( field-content | LWS )
       field-content  = <the OCTETs making up the field-value
                        and consisting of either *TEXT or combinations
                        of token, separators, and quoted-string>
*/
static gboolean
is_value (const char *s)
{
    gunichar c;

    while (*s) {
        c = g_utf8_get_char (s);
        switch (c) {
        case '\r':
            if (is_lws (&s)) {
                /* don't read next char; is_lws() already read it */
                continue;
            } else {
                return FALSE;
            }
            break;
        case '\t':
            break;
        case '"':
            /* is_quoted_string() stops at " */
            if (!is_quoted_string (&s)) {
                return FALSE;
            }
            break;
        default:
            if (c < 32 || c >= 127) {
                return FALSE;
            }
            break;
        }
        s = g_utf8_next_char (s);
    }
    return TRUE;
}

static const char *disallowed_headers[] = {
    "Accept-Charset",
    "Accept-Encoding",
    "Connection",
    "Content-Length",
    "Content-Transfer-Encoding",
    "Date",
    "Expect",
    "Host",
    "Keep-Alive",
    "Referer",
    "TE",
    "Trailer",
    "Transfer-Encoding",
    "Upgrade",
    "Via",
    NULL
};

static const char *disallowed_prefixes[] = {
    "Proxy-",
    "Sec-",
    NULL
};

static void
gom_xhr_set_request_header (GomXMLHttpRequest *xml_http_request,
                            const char *header,
                            const char *value,
                            GError    **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    const char **s;
    if (priv->state != GOM_OPENED) {
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_INVALID_STATE_ERR,                     
                     "request state is not OPENED");
        return;
    }
    if (priv->send) {
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_INVALID_STATE_ERR,
                     "request has already been sent");
        return;
    }
    if (!header || !is_token (header)) {
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_SYNTAX_ERR,
                     "invalid header");
        return;
    }
    if (!value) {
        return;
    }
    if (!is_value (value)) {
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_SYNTAX_ERR,
                     "invalid value");
        return;
    }
    for (s = disallowed_headers; *s; s++) {
        if (!strcasecmp (header, *s)) {
            return;
        }
    }
    for (s = disallowed_prefixes; *s; s++) {
        if (!strncasecmp (header, *s, strlen (*s))) {
            return;
        }
    }
    priv->reqhdr_names  = g_slist_prepend (priv->reqhdr_names, g_strdup (header));
    priv->reqhdr_values = g_slist_prepend (priv->reqhdr_values, g_strdup (value));
}

static void
gom_xhr_send (GomXMLHttpRequest *xml_http_request,
              GError           **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_xhr_send_string (GomXMLHttpRequest *xml_http_request,
                     const char        *data,
                     GError           **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_xhr_send_document (GomXMLHttpRequest *xml_http_request,
                       const GomDocument *doc,
                       GError           **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_xhr_abort (GomXMLHttpRequest *xml_http_request)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED;
}

static char *
gom_xhr_get_all_response_headers (const GomXMLHttpRequest *xml_http_request,
                                  GError                 **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static char *
gom_xhr_get_response_header (const GomXMLHttpRequest *xml_http_request,
                             const char              *header,
                             GError                 **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

GOM_IMPLEMENT (EVENT_TARGET_INTERNAL, event_target_internal, gom_xhr);
GOM_IMPLEMENT (XML_HTTP_REQUEST,      xml_http_request,      gom_xhr);

G_DEFINE_TYPE_WITH_CODE (GomXhr, gom_xhr, GOM_TYPE_EVT_TGT,
                         {
                             GOM_IMPLEMENT_INTERFACE (EVENT_TARGET_INTERNAL, event_target_internal, gom_xhr);
                             GOM_IMPLEMENT_INTERFACE (XML_HTTP_REQUEST,      xml_http_request,      gom_xhr);
                         });

static void gom_xhr_init (GomXhr *xhr) { }

static void
gom_xhr_dispose (GObject *object)
{
    GomXhrPrivate *priv = PRIV (object);

    G_OBJECT_CLASS (gom_xhr_parent_class)->dispose (object);
}

static void
gom_xhr_class_init (GomXhrClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomXhrPrivate));

    oclass->dispose      = gom_xhr_dispose;
    oclass->get_property = gom_xhr_get_property;
    oclass->set_property = gom_xhr_set_property;

    g_object_class_install_property (
        oclass, PROP_DOCUMENT,
        g_param_spec_object ("document", NULL,
                             "The owning document, if any.",
                             GOM_TYPE_DOCUMENT,
                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_override_property (oclass, PROP_ONREADYSTATECHANGE, "onreadystatechange");
    g_object_class_override_property (oclass, PROP_READY_STATE,        "ready-state");
    g_object_class_override_property (oclass, PROP_RESPONSE_TEXT,      "response-text");
    g_object_class_override_property (oclass, PROP_RESPONSE_XML,       "response-x-m-l");
    g_object_class_override_property (oclass, PROP_STATUS,             "status");
    g_object_class_override_property (oclass, PROP_STATUS_TEXT,        "status-text");
}

