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
#include "gom/gomelem.h"
#include "xpgom/gomwrapped.hh"
#include "xpgom/xgElement.hh"

#include <nsIDOMElement.h>
#include <nsStringAPI.h>

#include "gommacros.h"

#define CHECK_INITIALIZED XG_WRAPPED_CHECK_INIALIZED (GOM_TYPE_ELEMENT)

NS_IMPL_ISUPPORTS_INHERITED1(xgElement, xgNode, nsIDOMElement)

xgElement::xgElement () : xgNode (GOM_TYPE_ELEM)
{
}

xgElement::xgElement (GType aType) : xgNode (aType)
{
}

nsresult
xgElement::Init(GObject *aDoc)
{
    GType ifaces[2];
    ifaces[0] = GOM_TYPE_ELEMENT;
    ifaces[1] = 0;
    return xgWrapped::Init (ifaces, aDoc);
}

/* readonly attribute DOMString tagName; */
XG_WRAPPED_IMPL_GET_STRING (xgElement, GOM_TYPE_ELEMENT, GetTagName, "tag-name")

/* DOMString getAttribute (in DOMString name); */
NS_IMETHODIMP xgElement::GetAttribute(const nsAString & name, nsAString & _retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void setAttribute (in DOMString name, in DOMString value)  raises (DOMException); */
NS_IMETHODIMP xgElement::SetAttribute(const nsAString & name, const nsAString & value)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void removeAttribute (in DOMString name)  raises (DOMException); */
NS_IMETHODIMP xgElement::RemoveAttribute(const nsAString & name)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMAttr getAttributeNode (in DOMString name); */
NS_IMETHODIMP xgElement::GetAttributeNode(const nsAString & name, nsIDOMAttr **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMAttr setAttributeNode (in nsIDOMAttr newAttr)  raises (DOMException); */
NS_IMETHODIMP xgElement::SetAttributeNode(nsIDOMAttr *newAttr, nsIDOMAttr **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMAttr removeAttributeNode (in nsIDOMAttr oldAttr)  raises (DOMException); */
NS_IMETHODIMP xgElement::RemoveAttributeNode(nsIDOMAttr *oldAttr, nsIDOMAttr **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNodeList getElementsByTagName (in DOMString name); */
NS_IMETHODIMP xgElement::GetElementsByTagName(const nsAString & name, nsIDOMNodeList **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* DOMString getAttributeNS (in DOMString namespaceURI, in DOMString localName); */
NS_IMETHODIMP xgElement::GetAttributeNS(const nsAString & namespaceURI, const nsAString & localName, nsAString & _retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void setAttributeNS (in DOMString namespaceURI, in DOMString qualifiedName, in DOMString value)  raises (DOMException); */
NS_IMETHODIMP xgElement::SetAttributeNS(const nsAString & namespaceURI, const nsAString & qualifiedName, const nsAString & value)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void removeAttributeNS (in DOMString namespaceURI, in DOMString localName)  raises (DOMException); */
NS_IMETHODIMP xgElement::RemoveAttributeNS(const nsAString & namespaceURI, const nsAString & localName)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMAttr getAttributeNodeNS (in DOMString namespaceURI, in DOMString localName); */
NS_IMETHODIMP xgElement::GetAttributeNodeNS(const nsAString & namespaceURI, const nsAString & localName, nsIDOMAttr **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMAttr setAttributeNodeNS (in nsIDOMAttr newAttr)  raises (DOMException); */
NS_IMETHODIMP xgElement::SetAttributeNodeNS(nsIDOMAttr *newAttr, nsIDOMAttr **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNodeList getElementsByTagNameNS (in DOMString namespaceURI, in DOMString localName); */
NS_IMETHODIMP xgElement::GetElementsByTagNameNS(const nsAString & namespaceURI, const nsAString & localName, nsIDOMNodeList **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean hasAttribute (in DOMString name); */
NS_IMETHODIMP xgElement::HasAttribute(const nsAString & name, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean hasAttributeNS (in DOMString namespaceURI, in DOMString localName); */
NS_IMETHODIMP xgElement::HasAttributeNS(const nsAString & namespaceURI, const nsAString & localName, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
