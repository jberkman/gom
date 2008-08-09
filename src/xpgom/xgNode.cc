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
#include "gom/dom/gomdomexception.h"
#include "xpgom/gomwrapped.hh"

#include <nsStringAPI.h>

#include "gommacros.h"

#define CHECK_INITIALIZED GOM_XG_WRAPPED_CHECK_INIALIZED (GOM_TYPE_NODE)

NS_IMPL_ISUPPORTS_INHERITED1 (xgNode, xgWrapped, nsIDOMNode)

xgNode::xgNode () : xgWrapped (GOM_TYPE_NOODLE)
{
}

xgNode::xgNode (GType aType) : xgWrapped (aType)
{
}

xgNode::~xgNode ()
{
}

nsresult
xgNode::Init (GObject *aNode)
{
    GType ifaces[2];
    ifaces[0] = GOM_TYPE_NODE;
    ifaces[1] = 0;
    return xgWrapped::Init (ifaces, aNode);
}

/* readonly attribute DOMString nodeName; */
NS_IMETHODIMP xgNode::GetNodeName(nsAString & aNodeName)
{
    CHECK_INITIALIZED;
    char *s;
    g_object_get (mWrapped, "node-name", &s, NULL);
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
    g_object_get (mWrapped, "node-type", &t, NULL);
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
NS_IMETHODIMP xgNode::ReplaceChild (nsIDOMNode *newChild, nsIDOMNode *oldChild, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    GError *error = NULL;
    GomNode *new_child = (GomNode *)gom_wrap_xpcom (newChild, GOM_TYPE_NODE, &error);
    if (!new_child) {
	GOM_RETURN_NSRESULT_FROM_GERROR (error);
    }
    GomNode *old_child = (GomNode *)gom_wrap_xpcom (oldChild, GOM_TYPE_NODE, &error);
    if (!old_child) {
	g_object_unref (new_child);
	GOM_RETURN_NSRESULT_FROM_GERROR (error);
    }
    GomNode *ret = gom_node_replace_child (GOM_NODE (mWrapped), new_child, old_child, &error);
    g_object_unref (new_child);
    g_object_unref (old_child);
    if (!ret) {
	GOM_RETURN_NSRESULT_FROM_GERROR (error);
    }
    nsresult rv = gom_wrap_g_object (ret, NS_GET_IID (nsIDOMNode), (gpointer *)_retval);
    g_object_unref (ret);
    return rv;
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
