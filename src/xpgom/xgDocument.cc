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

#include "gom/dom/gomdomexception.h"
#include "gom/gomdoc.h"
#include "xpgom/gomwrapped.hh"
#include "xpgom/xgDocument.hh"

#include <nsIDOMElement.h>
#include <nsIDOMDOMImplementation.h>
#include <nsIDOMDocumentType.h>
#include <nsStringAPI.h>

#include "gommacros.h"

#define CHECK_INITIALIZED XG_WRAPPED_CHECK_INIALIZED (GOM_TYPE_DOCUMENT)
#define IMPL_GET_OBJECT(_func, _iface, _prop_name, _prop_gtype, _prop_class) \
    XG_WRAPPED_IMPL_GET_OBJECT (xgDocument, GOM_TYPE_DOCUMENT, _func, _iface, _prop_name, _prop_gtype, _prop_class);

NS_IMPL_ISUPPORTS_INHERITED1(xgDocument, xgNode, nsIDOMDocument)

xgDocument::xgDocument () : xgNode (GOM_TYPE_DOC)
{
}

xgDocument::xgDocument (GType aType) : xgNode (aType)
{
}

nsresult
xgDocument::Init (GObject *aDoc)
{
    GType ifaces[2];
    ifaces[0] = GOM_TYPE_DOCUMENT;
    ifaces[1] = 0;
    return xgWrapped::Init (ifaces, aDoc);
}

/* readonly attribute nsIDOMDocumentType doctype; */
IMPL_GET_OBJECT(GetDoctype, nsIDOMDocumentType, "doctype", GOM_TYPE_DOCUMENT_TYPE, GomDocumentType)

/* readonly attribute nsIDOMDOMImplementation implementation; */
IMPL_GET_OBJECT (GetImplementation, nsIDOMDOMImplementation, "implementation",
		 GOM_TYPE_DOM_IMPLEMENTATION, GomDOMImplementation)
		 

/* readonly attribute nsIDOMElement documentElement; */
IMPL_GET_OBJECT (GetDocumentElement, nsIDOMElement, "document-element", GOM_TYPE_ELEMENT, GomElement)

/* nsIDOMElement createElement (in DOMString tagName)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateElement(const nsAString & tagName, nsIDOMElement **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMDocumentFragment createDocumentFragment (); */
NS_IMETHODIMP xgDocument::CreateDocumentFragment(nsIDOMDocumentFragment **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMText createTextNode (in DOMString data); */
NS_IMETHODIMP xgDocument::CreateTextNode(const nsAString & data, nsIDOMText **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMComment createComment (in DOMString data); */
NS_IMETHODIMP xgDocument::CreateComment(const nsAString & data, nsIDOMComment **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMCDATASection createCDATASection (in DOMString data)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateCDATASection(const nsAString & data, nsIDOMCDATASection **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMProcessingInstruction createProcessingInstruction (in DOMString target, in DOMString data)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateProcessingInstruction(const nsAString & target, const nsAString & data, nsIDOMProcessingInstruction **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMAttr createAttribute (in DOMString name)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateAttribute(const nsAString & name, nsIDOMAttr **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMEntityReference createEntityReference (in DOMString name)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateEntityReference(const nsAString & name, nsIDOMEntityReference **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNodeList getElementsByTagName (in DOMString tagname); */
NS_IMETHODIMP xgDocument::GetElementsByTagName(const nsAString & tagname, nsIDOMNodeList **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNode importNode (in nsIDOMNode importedNode, in boolean deep)  raises (DOMException); */
NS_IMETHODIMP
xgDocument::ImportNode (nsIDOMNode  *importedNode,
			PRBool       deep,
			nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    GError *error = NULL;
    GomNode *node = (GomNode *)gom_wrap_xpcom (importedNode, GOM_TYPE_NODE, &error);
    GOM_RETURN_NSRESULT_FROM_GERROR (error);

    GomNode *ret = gom_document_import_node (GOM_DOCUMENT (mWrapped), node, deep, &error);
    if (node) {
	g_object_unref (node);
    }
    GOM_RETURN_NSRESULT_FROM_GERROR (error);

    nsresult rv = gom_wrap_g_object (ret, NS_GET_IID (nsIDOMNode), (gpointer *)_retval);
    if (ret) {
	g_object_unref (ret);
    }
    return rv;
}

/* nsIDOMElement createElementNS (in DOMString namespaceURI, in DOMString qualifiedName)  raises (DOMException); */
NS_IMETHODIMP
xgDocument::CreateElementNS (const nsAString &namespaceURI,
			     const nsAString &qualifiedName,
			     nsIDOMElement **_retval)
{
    CHECK_INITIALIZED;
    GOM_ASTRING_TO_GSTRING_RETURN (nspace, namespaceURI, NS_ERROR_INVALID_ARG);
    GOM_ASTRING_TO_GSTRING_RETURN (qname, qualifiedName, NS_ERROR_INVALID_ARG);

    GError *error = NULL;
    GomElement *elem = gom_document_create_element_ns (GOM_DOCUMENT (mWrapped),
						       nspace, qname, &error);
    GOM_RETURN_NSRESULT_FROM_GERROR (error);

    nsresult rv = gom_wrap_g_object (elem, NS_GET_IID (nsIDOMElement), (gpointer *)_retval);
    if (elem)  {
	g_object_unref (elem);
    }
    return rv;
}

/* nsIDOMAttr createAttributeNS (in DOMString namespaceURI, in DOMString qualifiedName)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateAttributeNS(const nsAString & namespaceURI, const nsAString & qualifiedName, nsIDOMAttr **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNodeList getElementsByTagNameNS (in DOMString namespaceURI, in DOMString localName); */
NS_IMETHODIMP xgDocument::GetElementsByTagNameNS(const nsAString & namespaceURI, const nsAString & localName, nsIDOMNodeList **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMElement getElementById (in DOMString elementId); */
NS_IMETHODIMP xgDocument::GetElementById(const nsAString & elementId, nsIDOMElement **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
