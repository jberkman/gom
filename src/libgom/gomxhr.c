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

#include "gommacros.h"

enum {
    PROP_ONREADYSTATECHANGE = 1,
    PROP_READY_STATE,
    PROP_RESPONSE_TEXT,
    PROP_RESPONSE_XML,
    PROP_STATUS,
    PROP_STATUS_TEXT
};

typedef struct {
    char f;
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

static void
gom_xhr_open (GomXMLHttpRequest *xml_http_request,
              const char        *method,
              const char        *url,
              gboolean           async,
              const char        *user,
              const char        *password,
              GError           **error)
{GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_xhr_set_request_header (GomXMLHttpRequest *xml_http_request,
                            const char *header,
                            const char *value,
                            GError    **error)
{
    GomXhrPrivate *priv = PRIV (xml_http_request);
    GOM_NOT_IMPLEMENTED_ERROR (error);
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

    g_object_class_override_property (oclass, PROP_ONREADYSTATECHANGE, "onreadystatechange");
    g_object_class_override_property (oclass, PROP_READY_STATE,        "ready-state");
    g_object_class_override_property (oclass, PROP_RESPONSE_TEXT,      "response-text");
    g_object_class_override_property (oclass, PROP_RESPONSE_XML,       "response-x-m-l");
    g_object_class_override_property (oclass, PROP_STATUS,             "status");
    g_object_class_override_property (oclass, PROP_STATUS_TEXT,        "status-text");
}

