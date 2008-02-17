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

#include <gom/gomdom.h>
#include <gom/gomdoc.h>
#include <gom/dom/gomdomexception.h>

GQuark
gom_dom_exception_error_quark (void)
{
    return g_quark_from_static_string ("gom-dom-exception-error-quark");
}

static gboolean
gom_dom_has_feature (GomDOMImplementation *dom, const char *feature, const char *version)
{
    return !g_ascii_strcasecmp (feature, "xml") && !g_ascii_strcasecmp (version, "1.0");
}

static void
gom_dom_interface_init (gpointer g_iface, gpointer iface_data)
{
    GomDOMImplementationInterface *iface = (GomDOMImplementationInterface *)g_iface;

    iface->has_feature = gom_dom_has_feature;
}

G_DEFINE_TYPE_WITH_CODE (GomDOM, gom_dom, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GOM_TYPE_DOM_IMPLEMENTATION, gom_dom_interface_init));

GomDOMImplementation *
gom_dom_get_implementation (void)
{
    static GomDOMImplementation *dom = NULL;

    if (dom == NULL) {
        dom = g_object_new (GOM_TYPE_DOM, NULL);
    }

    return dom;
}

static void gom_dom_init (GomDOM *dom) { }
static void gom_dom_class_init (GomDOMClass *klass) { }
