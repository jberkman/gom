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
#ifndef XG_G_OBJECT_UTILS_H
#define XG_G_OBJECT_UTILS_H

#include "xgIGObjectHolder.h"

#include <jspubtd.h>
#include <nscore.h>
#include <nsIXPCScriptable.h>

class xgGObject : public xgIGObjectHolder,
		  public nsIXPCScriptable
{
public:
    NS_DECL_ISUPPORTS
    NS_DECL_XGIGOBJECTHOLDER
    NS_DECL_NSIXPCSCRIPTABLE

    xgGObject();
    nsresult Init (GObject *aObject);

    JSBool Resolve (const char *name, GParamSpec **spec, guint *signal_id);
    JSBool Resolve (jsval id, GParamSpec **spec, guint *signal_id);

    static JSObject *GetNewOrUsed (JSContext *cx, GObject *gobj);
    static nsresult  GetNative (JSContext *cx, JSObject *obj, nsISupports **_retval);

protected:
    static JSBool Resolve (GType aType, jsval id, GParamSpec **spec, guint *signal_id);
    static JSBool Resolve (GType aType, const char *name, GParamSpec **spec, guint *signal_id);

    static JSBool GetProperty (JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    static JSBool GetProperty (JSContext *cx, GObject *obj, jsval id, jsval *vp);

    static JSBool SetProperty (JSContext *cx, JSObject *obj, jsval id, jsval *vp);
    static JSBool SetProperty (JSContext *cx, GObject *obj, jsval id, jsval *vp);

    GObject *mObject;

    ~xgGObject ();
};

#endif // XG_G_OBJECT_UTILS_H
