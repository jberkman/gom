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

#include "xpgom/xgNode.hh"

#include "gom/gomnoodle.h"

#include <nsStringAPI.h>

#include "gommacros.h"

#define CHECK_INITIALIZED GOM_XGO_CHECK_INIALIZED (GOM_TYPE_NODE)

NS_IMPL_ISUPPORTS1 (xgNode, nsIDOMNode)

xgNode::xgNode (GomNode *aNode) : xgObject (aNode ? G_OBJECT (aNode) : NULL, GOM_TYPE_NOODLE)
{
}

xgNode::xgNode (GomNode *aNode, GType aType) : xgObject (aNode ? G_OBJECT (aNode) : NULL, aType)
{
}

xgNode::~xgNode ()
{
}

nsresult
xgNode::Init()
{
    GType ifaces[2];
    ifaces[0] = GOM_TYPE_NODE;
    ifaces[1] = 0;
    return xgObject::Init (ifaces);
}

/* readonly attribute DOMString nodeName; */
NS_IMETHODIMP xgNode::GetNodeName(nsAString & aNodeName)
{
    CHECK_INITIALIZED;
    char *s;
    g_object_get (mObject, "node-name", &s, NULL);
    nsCAutoString cstr(s);
    g_free (s);
    CopyUTF8toUTF16 (cstr, aNodeName);
    return NS_OK;
}

/* attribute DOMString nodeValue; */
NS_IMETHODIMP xgNode::GetNodeValue(nsAString & aNodeValue)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgNode::SetNodeValue(const nsAString & aNodeValue)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short nodeType; */
NS_IMETHODIMP xgNode::GetNodeType(PRUint16 *aNodeType)
{
    CHECK_INITIALIZED;
    GomNodeType t;
    g_object_get (mObject, "node-type", &t, NULL);
    *aNodeType = t;
    return NS_OK;
}

/* readonly attribute nsIDOMNode parentNode; */
NS_IMETHODIMP xgNode::GetParentNode(nsIDOMNode * *aParentNode)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMNodeList childNodes; */
NS_IMETHODIMP xgNode::GetChildNodes(nsIDOMNodeList * *aChildNodes)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMNode firstChild; */
NS_IMETHODIMP xgNode::GetFirstChild(nsIDOMNode * *aFirstChild)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMNode lastChild; */
NS_IMETHODIMP xgNode::GetLastChild(nsIDOMNode * *aLastChild)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMNode previousSibling; */
NS_IMETHODIMP xgNode::GetPreviousSibling(nsIDOMNode * *aPreviousSibling)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMNode nextSibling; */
NS_IMETHODIMP xgNode::GetNextSibling(nsIDOMNode * *aNextSibling)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMNamedNodeMap attributes; */
NS_IMETHODIMP xgNode::GetAttributes(nsIDOMNamedNodeMap * *aAttributes)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMDocument ownerDocument; */
NS_IMETHODIMP xgNode::GetOwnerDocument(nsIDOMDocument * *aOwnerDocument)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMNode insertBefore (in nsIDOMNode newChild, in nsIDOMNode refChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::InsertBefore(nsIDOMNode *newChild, nsIDOMNode *refChild, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMNode replaceChild (in nsIDOMNode newChild, in nsIDOMNode oldChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::ReplaceChild(nsIDOMNode *newChild, nsIDOMNode *oldChild, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMNode removeChild (in nsIDOMNode oldChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::RemoveChild(nsIDOMNode *oldChild, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMNode appendChild (in nsIDOMNode newChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::AppendChild(nsIDOMNode *newChild, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean hasChildNodes (); */
NS_IMETHODIMP xgNode::HasChildNodes(PRBool *_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMNode cloneNode (in boolean deep); */
NS_IMETHODIMP xgNode::CloneNode(PRBool deep, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void normalize (); */
NS_IMETHODIMP xgNode::Normalize()
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isSupported (in DOMString feature, in DOMString version); */
NS_IMETHODIMP xgNode::IsSupported(const nsAString & feature, const nsAString & version, PRBool *_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute DOMString namespaceURI; */
NS_IMETHODIMP xgNode::GetNamespaceURI(nsAString & aNamespaceURI)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute DOMString prefix; */
NS_IMETHODIMP xgNode::GetPrefix(nsAString & aPrefix)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgNode::SetPrefix(const nsAString & aPrefix)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute DOMString localName; */
NS_IMETHODIMP xgNode::GetLocalName(nsAString & aLocalName)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean hasAttributes (); */
NS_IMETHODIMP xgNode::HasAttributes(PRBool *_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}
