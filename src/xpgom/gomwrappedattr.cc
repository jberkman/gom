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

#include "xpgom/gomwrappedattr.hh"
#include "gom/dom/gomattr.h"
#include "gom/dom/gomelement.h"
#if 0
#include "xpgom/xgAttr.hh"
#endif
#include "gom/dom/gomdomexception.h"

#include <nsCOMPtr.h>
#include <nsIDOMAttr.h>
#include <nsIDOMElement.h>
#include <nsStringAPI.h>

#include "gommacros.h"

enum {
    PROP_NAME = 1,
    PROP_SPECIFIED,
    PROP_VALUE,
    PROP_OWNER_ELEMENT
};

#define GET_ATTR(i) GOM_WRAPPED_GET (i, nsIDOMAttr, mAttr)

static void
gom_wrapped_attr_get_property (GObject    *object,
			       guint       property_id,
			       GValue     *value,
			       GParamSpec *pspec)
{
    GET_ATTR (object);
    switch (property_id) {
    case PROP_NAME:
	GOM_WRAPPED_GET_STRING (mAttr, GetName);
	break;
    case PROP_SPECIFIED:
	GOM_WRAPPED_GET_BOOL (mAttr, GetSpecified);
	break;
    case PROP_VALUE:
	GOM_WRAPPED_GET_STRING (mAttr, GetValue);
	break;
    case PROP_OWNER_ELEMENT:
	GOM_WRAPPED_GET_OBJECT (mAttr, GetOwnerElement, nsIDOMElement, GOM_TYPE_ELEMENT, GomElement);
	break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_wrapped_attr_set_property (GObject      *object,
			       guint         property_id,
			       const GValue *value,
			       GParamSpec   *pspec)
{
    GET_ATTR (object);
    switch (property_id) {
    case PROP_NAME:
    case PROP_SPECIFIED:
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	break;
    }
}

// GOM_WRAPPED_CONSTRUCTOR_INIT (xgNode, Init)

GOM_IMPLEMENT (ATTR, attr, gom_wrapped_attr)

G_DEFINE_TYPE_WITH_CODE (GomWrappedAttr, gom_wrapped_attr, GOM_TYPE_WRAPPED_NODE,
			 GOM_IMPLEMENT_INTERFACE (ATTR, attr, gom_wrapped_attr))

static void gom_wrapped_attr_init (GomWrappedAttr *node) { }

static void
gom_wrapped_attr_class_init (GomWrappedAttrClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    oclass->get_property = gom_wrapped_attr_get_property;
    oclass->set_property = gom_wrapped_attr_set_property;

    g_object_class_override_property (oclass, PROP_NAME, "name");
    g_object_class_override_property (oclass, PROP_SPECIFIED, "specified");
    g_object_class_override_property (oclass, PROP_VALUE, "value");
    g_object_class_override_property (oclass, PROP_OWNER_ELEMENT, "owner-element");

    gom_wrapped_register_interface (GOM_TYPE_ATTR, GOM_TYPE_WRAPPED_ATTR,
				    NS_GET_IID (nsIDOMAttr), NULL /*xgAttrConstructor*/);
}
