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
#include <nsIDOMNodeList.h>
#include <nsIDOMNamedNodeMap.h>
#include <nsIDOMDocument.h>

#include "gommacros.h"

#define CHECK_INITIALIZED XG_WRAPPED_CHECK_INIALIZED (GOM_TYPE_NODE)

#define IMPL_GET_OBJECT(_func, _iface, _prop_name, _prop_gtype, _prop_class) \
    XG_WRAPPED_IMPL_GET_OBJECT (xgNode, GOM_TYPE_NODE, _func, _iface, _prop_name, _prop_gtype, _prop_class);

#define IMPL_GET_STRING(_func, _prop_name)				\
    XG_WRAPPED_IMPL_GET_STRING (xgNode, GOM_TYPE_NODE, _func, _prop_name);

#define IMPL_GET_ENUM(_func, _prop_name, _prop_class)			\
    XG_WRAPPED_IMPL_GET_ENUM (xgNode, GOM_TYPE_NODE, _func, _prop_name, _prop_class);

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
IMPL_GET_STRING (GetNodeName, "node-name")

/* attribute DOMString nodeValue; */
IMPL_GET_STRING (GetNodeValue, "node-value")
NS_IMETHODIMP xgNode::SetNodeValue(const nsAString & aNodeValue)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned short nodeType; */
IMPL_GET_ENUM (GetNodeType, "node-type", GomNodeType)

/* readonly attribute nsIDOMNode parentNode; */
IMPL_GET_OBJECT (GetParentNode, nsIDOMNode, "parent-node", GOM_TYPE_NODE, GomNode)

/* readonly attribute nsIDOMNodeList childNodes; */
IMPL_GET_OBJECT (GetChildNodes, nsIDOMNodeList, "child-nodes", GOM_TYPE_NODE_LIST, GomNodeList)

/* readonly attribute nsIDOMNode firstChild; */
IMPL_GET_OBJECT (GetFirstChild, nsIDOMNode, "first-child", GOM_TYPE_NODE, GomNode)

/* readonly attribute nsIDOMNode lastChild; */
IMPL_GET_OBJECT (GetLastChild, nsIDOMNode, "last-child", GOM_TYPE_NODE, GomNode)

/* readonly attribute nsIDOMNode previousSibling; */
IMPL_GET_OBJECT (GetPreviousSibling, nsIDOMNode, "previous-sibling", GOM_TYPE_NODE, GomNode)

/* readonly attribute nsIDOMNode nextSibling; */
IMPL_GET_OBJECT (GetNextSibling, nsIDOMNode, "next-sibling", GOM_TYPE_NODE, GomNode)

/* readonly attribute nsIDOMNamedNodeMap attributes; */
IMPL_GET_OBJECT (GetAttributes, nsIDOMNamedNodeMap, "attributes", GOM_TYPE_NAMED_NODE_MAP, GomNamedNodeMap)

/* readonly attribute nsIDOMDocument ownerDocument; */
IMPL_GET_OBJECT (GetOwnerDocument, nsIDOMDocument, "owner-document", GOM_TYPE_DOCUMENT, GomDocument)

/* nsIDOMNode insertBefore (in nsIDOMNode newChild, in nsIDOMNode refChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::InsertBefore(nsIDOMNode *newChild, nsIDOMNode *refChild, nsIDOMNode **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNode replaceChild (in nsIDOMNode newChild, in nsIDOMNode oldChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::ReplaceChild (nsIDOMNode *newChild, nsIDOMNode *oldChild, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    GError *error = NULL;
    GomNode *new_child = (GomNode *)gom_wrap_xpcom (newChild, GOM_TYPE_NODE, &error);
    GOM_RETURN_NSRESULT_FROM_GERROR (error);

    GomNode *old_child = (GomNode *)gom_wrap_xpcom (oldChild, GOM_TYPE_NODE, &error);
    if (error) {
	if (newChild) {
	    g_object_unref (new_child);
	}
	GOM_RETURN_NSRESULT_FROM_GERROR (error);
    }
    GomNode *ret = gom_node_replace_child (GOM_NODE (mWrapped), new_child, old_child, &error);
    if (new_child) {
	g_object_unref (new_child);
    }
    if (old_child) {
	g_object_unref (old_child);
    }
    GOM_RETURN_NSRESULT_FROM_GERROR (error);

    nsresult rv = gom_wrap_g_object (ret, NS_GET_IID (nsIDOMNode), (gpointer *)_retval);
    if (ret) {
	g_object_unref (ret);
    }
    return rv;
}

/* nsIDOMNode removeChild (in nsIDOMNode oldChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::RemoveChild(nsIDOMNode *oldChild, nsIDOMNode **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNode appendChild (in nsIDOMNode newChild)  raises (DOMException); */
NS_IMETHODIMP xgNode::AppendChild(nsIDOMNode *newChild, nsIDOMNode **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean hasChildNodes (); */
NS_IMETHODIMP xgNode::HasChildNodes(PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMNode cloneNode (in boolean deep); */
NS_IMETHODIMP xgNode::CloneNode(PRBool deep, nsIDOMNode **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void normalize (); */
NS_IMETHODIMP xgNode::Normalize()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean isSupported (in DOMString feature, in DOMString version); */
NS_IMETHODIMP xgNode::IsSupported(const nsAString & feature, const nsAString & version, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute DOMString namespaceURI; */
IMPL_GET_STRING (GetNamespaceURI, "namespace-u-r-i")

/* attribute DOMString prefix; */
IMPL_GET_STRING (GetPrefix, "prefix")
NS_IMETHODIMP xgNode::SetPrefix(const nsAString & aPrefix)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute DOMString localName; */
IMPL_GET_STRING (GetLocalName, "local-name")

/* boolean hasAttributes (); */
NS_IMETHODIMP xgNode::HasAttributes(PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
