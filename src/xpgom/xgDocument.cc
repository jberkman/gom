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

#include "gommacros.h"

#define CHECK_INITIALIZED GOM_XG_WRAPPED_CHECK_INIALIZED (GOM_TYPE_DOCUMENT)

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
NS_IMETHODIMP xgDocument::GetDoctype(nsIDOMDocumentType * *aDoctype)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMDOMImplementation implementation; */
NS_IMETHODIMP xgDocument::GetImplementation(nsIDOMDOMImplementation * *aImplementation)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMElement documentElement; */
NS_IMETHODIMP
xgDocument::GetDocumentElement (nsIDOMElement **aDocumentElement)
{
    CHECK_INITIALIZED;
    GomElement *elem;
    g_object_get (mWrapped, "document-element", &elem, NULL);
    if (!GOM_IS_ELEMENT (elem)) {
	g_message (GOM_LOC ("got document element: %s"),
		   elem ? G_OBJECT_TYPE_NAME (elem) : "NULL");
	return NS_ERROR_UNEXPECTED;
    }
    nsresult rv = gom_wrap_g_object (elem, NS_GET_IID (nsIDOMElement), (gpointer *)aDocumentElement);
    g_object_unref (elem);
    return rv;
}

/* nsIDOMElement createElement (in DOMString tagName)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateElement(const nsAString & tagName, nsIDOMElement **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMDocumentFragment createDocumentFragment (); */
NS_IMETHODIMP xgDocument::CreateDocumentFragment(nsIDOMDocumentFragment **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMText createTextNode (in DOMString data); */
NS_IMETHODIMP xgDocument::CreateTextNode(const nsAString & data, nsIDOMText **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMComment createComment (in DOMString data); */
NS_IMETHODIMP xgDocument::CreateComment(const nsAString & data, nsIDOMComment **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMCDATASection createCDATASection (in DOMString data)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateCDATASection(const nsAString & data, nsIDOMCDATASection **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMProcessingInstruction createProcessingInstruction (in DOMString target, in DOMString data)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateProcessingInstruction(const nsAString & target, const nsAString & data, nsIDOMProcessingInstruction **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMAttr createAttribute (in DOMString name)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateAttribute(const nsAString & name, nsIDOMAttr **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMEntityReference createEntityReference (in DOMString name)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateEntityReference(const nsAString & name, nsIDOMEntityReference **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMNodeList getElementsByTagName (in DOMString tagname); */
NS_IMETHODIMP xgDocument::GetElementsByTagName(const nsAString & tagname, nsIDOMNodeList **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
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
    if (!node) {
	GOM_RETURN_NSRESULT_FROM_GERROR (error);
    }
    GomNode *ret = gom_document_import_node (GOM_DOCUMENT (mWrapped), node, deep, &error);
    g_object_unref (node);
    if (!ret) {
	GOM_RETURN_NSRESULT_FROM_GERROR (error);
    }
    nsresult rv = gom_wrap_g_object (ret, NS_GET_IID (nsIDOMNode), (gpointer *)_retval);
    g_object_unref (ret);
    return rv;
}

/* nsIDOMElement createElementNS (in DOMString namespaceURI, in DOMString qualifiedName)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateElementNS(const nsAString & namespaceURI, const nsAString & qualifiedName, nsIDOMElement **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMAttr createAttributeNS (in DOMString namespaceURI, in DOMString qualifiedName)  raises (DOMException); */
NS_IMETHODIMP xgDocument::CreateAttributeNS(const nsAString & namespaceURI, const nsAString & qualifiedName, nsIDOMAttr **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMNodeList getElementsByTagNameNS (in DOMString namespaceURI, in DOMString localName); */
NS_IMETHODIMP xgDocument::GetElementsByTagNameNS(const nsAString & namespaceURI, const nsAString & localName, nsIDOMNodeList **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMElement getElementById (in DOMString elementId); */
NS_IMETHODIMP xgDocument::GetElementById(const nsAString & elementId, nsIDOMElement **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}
