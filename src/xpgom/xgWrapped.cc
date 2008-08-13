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

#include "xpgom/gomwrapped.hh"
#include "xpgom/xgWrapped.hh"
#include "gom/dom/gomdomexception.h"

#include "gommacros.h"

NS_IMPL_ADDREF (xgWrapped)
NS_IMPL_RELEASE (xgWrapped)

NS_INTERFACE_TABLE_HEAD(xgWrapped)
NS_INTERFACE_TABLE1(xgWrapped, xgPIWrapped)
if (NS_SUCCEEDED (gom_wrap_g_object (mWrapped, aIID, aInstancePtr))) {
    char nsid[NSID_LENGTH];
    aIID.ToProvidedString (nsid);
    g_print (GOM_LOC ("%s -> %s\n"), mTypeName, nsid);
    return NS_OK;
 } else
NS_INTERFACE_TABLE_TAIL

xgWrapped::xgWrapped (GType aType) 
    : mWrapped (NULL),
      mType (aType)
{
}

xgWrapped::~xgWrapped ()
{
    if (mWrapped) {
	g_object_remove_weak_pointer (mWrapped, (gpointer *)&mWrapped);
	g_object_unref (mWrapped);
	mWrapped = NULL;
    } else if (mTypeName) {
	g_warning (GOM_LOC ("We had a %s, but now it's gone..."), mTypeName);
    }
}

nsresult
xgWrapped::Init (GType *ifaces, GObject *aObject)
{
    if (mWrapped) {
	return NS_ERROR_ALREADY_INITIALIZED;
    }
    if (!aObject) {
	if (mType) {
	    aObject = (GObject *)g_object_new (mType, NULL);
	    if (!aObject) {
		return NS_ERROR_OUT_OF_MEMORY;
	    }
	} else {
	    XG_RETURN_NOT_IMPLEMENTED;
	}
    }
    for (GType t = G_OBJECT_TYPE (aObject);
	 *ifaces;
	 ++ifaces) {
	if (!g_type_is_a (t, *ifaces)) {
	    return NS_ERROR_NO_INTERFACE;
	}
    }
    mWrapped = G_OBJECT (g_object_ref (aObject));
    g_object_add_weak_pointer (mWrapped, (gpointer *)&mWrapped);
    mTypeName = G_OBJECT_TYPE_NAME (mWrapped);
    return NS_OK;
}

/* xgNativeGObject getWrappedGObject (); */
NS_IMETHODIMP
xgWrapped::GetWrappedGObject (GObject **_retval)
{
    *_retval = mWrapped ? G_OBJECT (g_object_ref (mWrapped)) : NULL;
    return NS_OK;
}
