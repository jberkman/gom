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

#include "xpgom/gomwrappedelement.hh"
#include "gom/dom/gomelement.h"
#include "xpgom/xgElement.hh"
#include "gom/dom/gomdomexception.h"

#include <nsIDOMElement.h>
#include <nsCOMPtr.h>
#include <nsStringAPI.h>

#include "gommacros.h"

enum {
    PROP_TAG_NAME = 1,
    PROP_NODE_TYPE,
    PROP_PREFIX,
    PROP_NAMESPACE_URI,
    PROP_LOCAL_NAME
};

#define GET_ELEMENT(i) GOM_WRAPPED_GET (i, nsIDOMElement, mElement)

static void
gom_wrapped_element_get_property (GObject    *object,
				  guint       property_id,
				  GValue     *value,
				  GParamSpec *pspec)
{
    GET_ELEMENT (object);

    switch (property_id) {
    case PROP_TAG_NAME:
	GOM_WRAPPED_GET_STRING (mElement, GetTagName);
	break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_wrapped_element_set_property (GObject *object,
				  guint property_id,
				  const GValue *value,
				  GParamSpec *pspec)
{
    GET_ELEMENT (object);

    switch (property_id) {
    case PROP_TAG_NAME:
	GOM_NOT_IMPLEMENTED;
	break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static char *
gom_wrapped_element_get_attribute (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
gom_wrapped_element_set_attribute (GomElement *elem, const char *name, const char *value, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_wrapped_element_remove_attribute (GomElement *elem, const char *name, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static GomAttr *
gom_wrapped_element_get_attribute_node (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomAttr *
gom_wrapped_element_set_attribute_node (GomElement *elem, GomAttr *new_attr, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomAttr *
gom_wrapped_element_remove_attribute_node (GomElement *elem, GomAttr *old_attr, GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNodeList *
gom_wrapped_element_get_elements_by_tag_name (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static char *
gom_wrapped_element_get_attribute_ns (GomElement *elem, const char *namespace_uri, const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static void
gom_wrapped_element_set_attribute_ns (GomElement *elem, 
				      const char *namespace_uri,
				      const char *qualified_name,
				      const char *value,
				      GError    **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_wrapped_element_remove_attribute_ns (GomElement *elem,
					 const char *namespace_uri,
					 const char *local_name,
					 GError    **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static GomAttr *
gom_wrapped_element_get_attribute_node_ns (GomElement *elem,
					   const char *namespace_uri,
					   const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomAttr *
gom_wrapped_element_set_attribute_node_ns (GomElement *elem,
					   GomAttr    *new_attr,
					   GError    **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNodeList *
gom_wrapped_element_get_elements_by_tag_name_ns (GomElement *elem,
						 const char *namespace_uri,
						 const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static gboolean
gom_wrapped_element_has_attribute (GomElement *elem, const char *name)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

static gboolean
gom_wrapped_element_has_attribute_ns (GomElement *elem, const char *namespace_uri, const char *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

GOM_WRAPPED_CONSTRUCTOR_INIT (xgElement, Init)

GOM_IMPLEMENT (ELEMENT, element, gom_wrapped_element);

G_DEFINE_TYPE_WITH_CODE (GomWrappedElement, gom_wrapped_element, GOM_TYPE_WRAPPED_NODE,
			 GOM_IMPLEMENT_INTERFACE (ELEMENT, element, gom_wrapped_element));

static void gom_wrapped_element_init (GomWrappedElement *element) { }

static void
gom_wrapped_element_class_init (GomWrappedElementClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    oclass->get_property = gom_wrapped_element_get_property;
    oclass->set_property = gom_wrapped_element_set_property;

    g_object_class_install_property (
        oclass,
	PROP_TAG_NAME,
        g_param_spec_string ("tag-name", NULL,
                             "The name of the element.",
                             NULL,
                             GParamFlags (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));

    gom_wrapped_register_interface (GOM_TYPE_ELEMENT, GOM_TYPE_WRAPPED_ELEMENT,
				    NS_GET_IID (nsIDOMElement), xgElementConstructor);
}
