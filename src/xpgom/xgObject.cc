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

#include "xpgom/xgObject.hh"

xgObject::xgObject (GObject *aObject, GType aType) 
  : mObject (aObject ? (GObject *)g_object_ref (aObject) : aObject)
  , mType(aType)
{
}

xgObject::~xgObject ()
{
    if (mObject) {
	g_object_unref (mObject);
    }
}

nsresult
xgObject::Init (GType *ifaces)
{
    if (!mObject) {
	if (mType) {
	    mObject = (GObject *)g_object_new (mType, NULL);
	    if (!mObject) {
		return NS_ERROR_OUT_OF_MEMORY;
	    }
	} else {
	    return NS_ERROR_NOT_IMPLEMENTED;
	}
    }
    for (GType t = G_OBJECT_TYPE (mObject);
	 *ifaces;
	 ++ifaces) {
	if (!g_type_is_a (t, *ifaces)) {
	    g_object_unref (mObject);
	    mObject = NULL;
	    return NS_ERROR_NO_INTERFACE;
	}
    }
    return NS_OK;
}
