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

#include "xpgom/xgNodeList.hh"
#include "gom/gomglist.h"
#include "gom/dom/gomdomexception.h"
#include "xpgom/gomwrapped.hh"

#include <nsStringAPI.h>
#include <nsIDOMNodeList.h>

#include "gommacros.h"

#define CHECK_INITIALIZED XG_WRAPPED_CHECK_INIALIZED (GOM_TYPE_NODE_LIST)

NS_IMPL_ISUPPORTS_INHERITED1 (xgNodeList, xgWrapped, nsIDOMNodeList)

xgNodeList::xgNodeList () : xgWrapped (GOM_TYPE_NOODLE)
{
}

xgNodeList::xgNodeList (GType aType) : xgWrapped (aType)
{
}

xgNodeList::~xgNodeList ()
{
}

nsresult
xgNodeList::Init (GObject *aNodeList)
{
    GType ifaces[2];
    ifaces[0] = GOM_TYPE_NODE_LIST;
    ifaces[1] = 0;
    return xgWrapped::Init (ifaces, aNodeList);
}

/* nsIDOMNode item (in unsigned long index); */
NS_IMETHODIMP
xgNodeList::Item (PRUint32 index, nsIDOMNode **_retval)
{
    CHECK_INITIALIZED;
    GomNode *node = gom_node_list_item (GOM_NODE_LIST (mWrapped), index);
    nsresult rv = gom_wrap_g_object (node, NS_GET_IID (nsIDOMNode), (gpointer *)_retval);
    if (node) {
	g_object_unref (node);
    }
    return rv;
}

/* readonly attribute unsigned long length; */
XG_WRAPPED_IMPL_GET_ULONG (xgNodeList, GOM_TYPE_NODE_LIST, GetLength, "length")
