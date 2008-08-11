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

#include "gom/dom/gomdocument.h"
#include "gom/dom/gomdomexception.h"
#include "xpgom/gomwrappeddocument.hh"
#include "xpgom/xgDocument.hh"

#include <nsIDOMDocument.h>
#include <nsCOMPtr.h>
#include <nsIDOMElement.h>
#include <nsIDOMDocumentType.h>
#include <nsIDOMDOMImplementation.h>

#include "gommacros.h"

enum {
    PROP_DOCTYPE = 1,
    PROP_IMPLEMENTATION,
    PROP_DOCUMENT_ELEMENT,
    PROP_NODE_NAME,
    PROP_NODE_TYPE,
    PROP_DOCUMENT_URI
};

#define GET_DOCUMENT(i)	GOM_WRAPPED_GET (i, nsIDOMDocument, mDoc)

static void
gom_wrapped_document_get_property (GObject    *object,
				   guint       property_id,
				   GValue     *value,
				   GParamSpec *pspec)
{
    GET_DOCUMENT (object);

    switch (property_id) {
    case PROP_DOCUMENT_ELEMENT:
	GOM_WRAPPED_GET_OBJECT (mDoc, GetDocumentElement, nsIDOMElement, GOM_TYPE_ELEMENT, GomElement);
	break;
    case PROP_DOCTYPE:
	GOM_WRAPPED_GET_OBJECT (mDoc, GetDoctype, nsIDOMDocumentType, GOM_TYPE_DOCUMENT_TYPE, GomDocumentType);
	break;
    case PROP_IMPLEMENTATION:
	GOM_WRAPPED_GET_OBJECT (mDoc, GetImplementation, nsIDOMDOMImplementation, GOM_TYPE_DOM_IMPLEMENTATION, GomDOMImplementation);
	break;
#if 0
    case PROP_DOCUMENT_URI:
	GOM_WRAPPED_GET_STRING (mDoc, GetDocumentURI);
	break;
#endif
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_wrapped_document_set_property (GObject *object,
				   guint property_id,
				   const GValue *value,
				   GParamSpec *pspec)
{
    GET_DOCUMENT (object);

    switch (property_id) {
    case PROP_DOCTYPE:
    case PROP_IMPLEMENTATION:
    case PROP_DOCUMENT_URI:
	GOM_NOT_IMPLEMENTED;
	break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GomElement *
gom_wrapped_document_create_element (GomDocument *doc,
				     const char  *tag_name,
				     GError    **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomDocumentFragment *
gom_wrapped_document_create_document_fragment (GomDocument *doc)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomText *
gom_wrapped_document_create_text_node (GomDocument *doc,
				       const char  *data)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomComment *
gom_wrapped_document_create_comment (GomDocument *doc,
				     const char  *data)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomCDATASection *
gom_wrapped_document_create_cdata_section (GomDocument *doc,
					   const char  *data,
					   GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomProcessingInstruction *
gom_wrapped_document_create_processing_instruction (GomDocument *doc,
						    const char  *target,
						    const char  *data,
						    GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomAttr *
gom_wrapped_document_create_attribute (GomDocument *doc,
				       const char  *name,
				       GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomEntityReference *
gom_wrapped_document_create_entity_reference (GomDocument *doc,
					      const char  *name,
					      GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNodeList *
gom_wrapped_document_get_elements_by_tag_name (GomDocument *doc,
					       const char  *tag_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

/* Introduced in DOM Level 2: */
static GomNode *
gom_wrapped_document_import_node (GomDocument *doc,
				  GomNode     *node,
				  gboolean     deep,
				  GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomElement *
gom_wrapped_document_create_element_ns (GomDocument *doc,
					const char  *namespace_uri,
					const char  *qualified_name,
					GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomAttr *
gom_wrapped_document_create_attribute_ns (GomDocument *doc,
					  const char  *namespace_uri,
					  const char  *qualified_name,
					  GError     **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomNodeList *
gom_wrapped_document_get_elements_by_tag_name_ns (GomDocument *doc,
						  const char  *namespace_uri,
						  const char  *local_name)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

static GomElement *
gom_wrapped_document_get_element_by_id (GomDocument *doc,
					const char  *element_id)
{
    GOM_NOT_IMPLEMENTED;
    return NULL;
}

GOM_WRAPPED_CONSTRUCTOR_INIT (xgDocument, Init)

GOM_IMPLEMENT (DOCUMENT, document, gom_wrapped_document);

G_DEFINE_TYPE_WITH_CODE (GomWrappedDocument, gom_wrapped_document, GOM_TYPE_WRAPPED_NODE,
			 GOM_IMPLEMENT_INTERFACE (DOCUMENT, document, gom_wrapped_document));

static void gom_wrapped_document_init (GomWrappedDocument *document) { }

static void
gom_wrapped_document_class_init (GomWrappedDocumentClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    oclass->get_property = gom_wrapped_document_get_property;
    oclass->set_property = gom_wrapped_document_set_property;

    g_object_class_override_property (oclass, PROP_DOCTYPE,          "doctype");
    g_object_class_override_property (oclass, PROP_IMPLEMENTATION,   "implementation");
    g_object_class_override_property (oclass, PROP_DOCUMENT_ELEMENT, "document-element");
    g_object_class_override_property (oclass, PROP_NODE_NAME,        "node-name");
    g_object_class_override_property (oclass, PROP_NODE_TYPE,        "node-type");
    g_object_class_override_property (oclass, PROP_DOCUMENT_URI,     "document-u-r-i");

    gom_wrapped_register_interface (GOM_TYPE_DOCUMENT, GOM_TYPE_WRAPPED_DOCUMENT,
				    NS_GET_IID (nsIDOMDocument), xgDocumentConstructor);
}
