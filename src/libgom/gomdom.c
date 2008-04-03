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

#include "gom/gomdom.h"

#include "gom/dom/gomdomimplementation.h"
#include "gom/gomdoc.h"

#include "gommacros.h"

static gboolean
gom_dom_has_feature (GomDOMImplementation *dom, const char *feature, const char *version)
{
    if (!feature) {
        return FALSE;
    }
    if (!g_ascii_strcasecmp (feature, "xml")) {
        return !version || !g_ascii_strcasecmp (version, "1.0");
    } else if (!g_ascii_strcasecmp (feature, "Core")) {
        return !version || !g_ascii_strcasecmp (version, "2.0");
    } else if (!g_ascii_strcasecmp (feature, "Events")) {
        return !version || !g_ascii_strcasecmp (version, "3.0") || !g_ascii_strcasecmp (version, "2.0");
    } else if (!g_ascii_strcasecmp (feature, "KeyboardEvents")) {
        return !version || !g_ascii_strcasecmp (version, "3.0");
    } else if (!g_ascii_strcasecmp (feature, "MouseEvents")) {
        return !version || !g_ascii_strcasecmp (version, "3.0") || !g_ascii_strcasecmp (version, "2.0");
#if 0
    } else if (!g_ascii_strcasecmp (feature, "MutationEvents")) {
        return !version || !g_ascii_strcasecmp (version, "3.0") || !g_ascii_strcasecmp (version, "2.0");
    } else if (!g_ascii_strcasecmp (feature, "MutationNameEvents")) {
        return !version || !g_ascii_strcasecmp (version, "3.0") || !g_ascii_strcasecmp (version, "2.0");
#endif
    } else if (!g_ascii_strcasecmp (feature, "UIEvents")) {
        return !version || !g_ascii_strcasecmp (version, "3.0") || !g_ascii_strcasecmp (version, "2.0");
#if 0
    } else if (!g_ascii_strcasecmp (feature, "Views")) {
        return !version || !g_ascii_strcasecmp (version, "2.0");
#endif
    }
    return FALSE;
}

static GomDocumentType *
gom_dom_create_document_type (GomDOMImplementation *dom,
                              const char           *qualified_name,
                              const char           *public_id,
                              const char           *system_id,
                              GError              **error)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomDocument *
gom_dom_create_document (GomDOMImplementation *dom,
                         const char           *namespace_uri,
                         const char           *qualified_name,
                         GomDocumentType      *doctype,
                         GError              **error)
{
    GError *err = NULL;
    GomDocument *doc;
    GomElement  *elem;

    doc = g_object_new (GOM_TYPE_DOC,
                        "doctype",         doctype,
                        "implementation",  dom,
                        NULL);

    elem = gom_document_create_element_ns (doc,
                                           namespace_uri,
                                           qualified_name,
                                           &err);
    if (!elem) {
        g_object_unref (doc);
        g_propagate_error (error, err);
        return NULL;
    }

    gom_node_append_child (GOM_NODE (doc), GOM_NODE (elem), &err);
    if (err) {
        g_object_unref (elem);
        g_object_unref (doc);
        g_propagate_error (error, err);
        return NULL;
    }
    return doc;
}

GOM_IMPLEMENT (DOM_IMPLEMENTATION, dom_implementation, gom_dom);

G_DEFINE_TYPE_WITH_CODE (GomDOM, gom_dom, G_TYPE_OBJECT, GOM_IMPLEMENT_INTERFACE (DOM_IMPLEMENTATION, dom_implementation, gom_dom));                         

typedef struct {
    GType                  type;
    guint                  n_construct_properties;
    GObjectConstructParam *construct_properties;
} OnceData;

static gpointer
gom_dom_once (gpointer data)
{
    OnceData *d = data;
    return G_OBJECT_CLASS (gom_dom_parent_class)->constructor (
        d->type, d->n_construct_properties, d->construct_properties);
}

static GObject *
gom_dom_constructor (GType                  type,
                     guint                  n_construct_properties,
                     GObjectConstructParam *construct_properties)
{
    static GOnce once = G_ONCE_INIT;
    OnceData data;

    data.type = type;
    data.n_construct_properties = n_construct_properties;
    data.construct_properties = construct_properties;

    return g_object_ref (g_once (&once, gom_dom_once, &data));
}

static void gom_dom_init (GomDOM *dom) { }

static void
gom_dom_class_init (GomDOMClass *klass)
{
    G_OBJECT_CLASS (klass)->constructor = gom_dom_constructor;
}
