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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xgGObject.h"

#include "gommacros.h"
#include "gom/gomcamel.h"
#include "gom/gomvalue.h"

#include <jsapi.h>

#include <nsCOMPtr.h>
#include <nsCRTGlue.h>
#include <nsIXPConnect.h>
#include <nsServiceManagerUtils.h>

xgGObject::xgGObject ()
{
}

xgGObject::~xgGObject ()
{
    if (mObject) {
	g_object_unref (mObject);
	mObject = NULL;
    }
}

NS_IMPL_ISUPPORTS2 (xgGObject, xgIGObjectHolder, nsIXPCScriptable)

nsresult
xgGObject::Init (GObject *object)
{
    gpointer obj = g_object_get_data (object, "XG_GOBJECT");
    if (obj) {
	return NS_ERROR_ALREADY_INITIALIZED;
    }
    mObject = object;
    g_object_set_data (object, "XG_GOBJECT", this);
    g_object_ref (object);
    return NS_OK;
}

/* GObjectPtr getGObject (); */
NS_IMETHODIMP
xgGObject::GetGObject (GObject **_retval)
{
    *_retval = mObject;
    return NS_OK;
}


nsresult
xgGObject::DefineProperties (JSContext *jscx, nsISupports *aCOMObj)
{
    nsresult rv;

    nsCOMPtr<nsIXPConnect> xpc (do_GetService ("@mozilla.org/js/xpc/XPConnect;1", &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIXPConnectJSObjectHolder> jswrapper;
    rv = xpc->WrapNative (jscx, JS_GetGlobalObject (jscx), aCOMObj, NS_GET_IID (nsISupports), getter_AddRefs (jswrapper));
    NS_ENSURE_SUCCESS (rv, rv);

#if 0
    nsCOMPtr<nsIXPConnectWrappedNative> wrapper (do_QueryInterface (jswrapper, &rv));
    NS_ENSURE_SUCCESS (rv, rv);
    nsDOMClassInfo::PreserveNodeWrapper(wrapper);
#endif

    JSObject *jsobj;
    rv = jswrapper->GetJSObject (&jsobj);
    NS_ENSURE_SUCCESS (rv, rv);

    g_message (GOM_LOC ("Got JSObject: %p"), (void *)jsobj);

    guint n_properties;
    GParamSpec **props = g_object_class_list_properties (G_OBJECT_GET_CLASS (mObject), &n_properties);
    g_message (GOM_LOC ("Adding %d properties from %s"), n_properties, G_OBJECT_TYPE_NAME (mObject));

    JS_BeginRequest (jscx);

    const char *camelName;
    for (guint i = 0; i < n_properties; i++) {
	camelName = gom_camel_case (props[i]->name);
	if (!JS_DefineProperty (jscx, jsobj, camelName, JSVAL_VOID,
				xgGObject::GetProperty,
				xgGObject::SetProperty,
				JSPROP_ENUMERATE | JSPROP_PERMANENT)) {
	    g_printerr ("Could not define a property for %s\n", camelName);
	} else {
	    g_print (GOM_LOC ("Defined property: %s.%s\n"),
		     G_OBJECT_TYPE_NAME (mObject), camelName);
	}
	GOM_CAMEL_FREE (camelName, props[i]->name);
    }

    JS_EndRequest (jscx);

    return NS_OK;
}

// static
JSBool
xgGObject::GetProperty (JSContext *cx,
			JSObject  *obj,
			jsval      id,
			jsval     *vp)
{
    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

    nsresult rv;

    nsCOMPtr<nsISupports> native;
    rv = GetNative (cx, obj, getter_AddRefs (native));
    NS_ENSURE_SUCCESS (rv, JS_FALSE);

    nsCOMPtr<xgIGObjectHolder> holder (do_QueryInterface (native, &rv));
    NS_ENSURE_SUCCESS (rv, JS_FALSE);

    GObject *gobj;
    rv = holder->GetGObject (&gobj);
    NS_ENSURE_SUCCESS (rv, rv);
    NS_ENSURE_TRUE (G_IS_OBJECT (gobj), NS_ERROR_UNEXPECTED);

    const char *name = JSVAL_CHARS (id);
    GParamSpec *spec;
    guint signal_id;

    g_print (GOM_LOC (""));
    if (!Resolve (G_TYPE_FROM_INSTANCE (gobj), name, &spec, &signal_id)) {
	return JS_TRUE;
    }

#if 0
    if (signal_id) {
        closure = gom_js_object_get_closure_prop (gobj, signal_id);
        g_print ("closure: %p\n", closure);
        *vp = closure ? OBJECT_TO_JSVAL (JS_GetFunctionObject (closure->fun)) : JSVAL_VOID;
        return JS_TRUE;
    }
#endif

    g_assert (spec);
    
    GValue gval = { 0 };
    g_value_init (&gval, G_PARAM_SPEC_VALUE_TYPE (spec));
    g_object_get_property (gobj, spec->name, &gval);

    GError *error = NULL;
    if (!gom_jsval (cx, vp, &gval, &error)) {
        g_printerr ("Could not get jsval: %s\n", error->message);
        g_value_unset (&gval);
        g_error_free (error);
        return JS_FALSE;
    }
    g_value_unset (&gval);

    return JS_TRUE;
}

// static
JSBool
xgGObject::SetProperty (JSContext *cx,
			     JSObject  *obj,
			     jsval      id,
			     jsval     *vp)
{
    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

    nsresult rv;

    nsCOMPtr<nsISupports> native;
    rv = GetNative (cx, obj, getter_AddRefs (native));
    NS_ENSURE_SUCCESS (rv, JS_FALSE);

    nsCOMPtr<xgIGObjectHolder> holder (do_QueryInterface (native, &rv));
    NS_ENSURE_SUCCESS (rv, JS_FALSE);

    GObject *gobj;
    rv = holder->GetGObject (&gobj);
    NS_ENSURE_SUCCESS (rv, rv);
    NS_ENSURE_TRUE (G_IS_OBJECT (gobj), NS_ERROR_UNEXPECTED);

    const char *name = JSVAL_CHARS (id);
    GParamSpec *spec;
    guint signal_id;

    g_print (GOM_LOC (""));
    if (!Resolve (G_TYPE_FROM_INSTANCE (gobj), name, &spec, &signal_id)) {
	return JS_TRUE;
    }

#if 0
    GomJSClosure *closure;
    if (signal_id) {
        JSFunction *fun;
        if (!JSVAL_IS_OBJECT (*vp) ||
            !JS_ObjectIsFunction (cx, JSVAL_TO_OBJECT (*vp))) {
            g_printerr ("jsval is not a function.  sucka.\n");
            return JS_FALSE;
        }
        fun = JS_ValueToFunction (cx, *vp);
        if (!fun) {
            g_printerr ("could not get function from value\n");
            return JS_FALSE;
        }
        closure = gom_js_object_get_closure_prop (gobj, signal_id);
        if (closure) {
            closure->fun = fun;
        } else {
            closure = (GomJSClosure *)gom_js_closure_new (cx, obj, fun);
            gom_js_object_set_closure_prop (gobj, signal_id, closure);
            g_signal_connect_closure_by_id (gobj, signal_id, 0, &closure->closure, FALSE);
        }
        return JS_TRUE;
    }
#endif
    g_assert (spec);

    GValue gval = { 0 };
    GError *error = NULL;
    if (!gom_g_value (cx, &gval, *vp, &error)) {
        g_printerr ("Could not get GValue: %s\n", error->message);
        g_error_free (error);
        return JS_FALSE;
    }

    g_object_set_property (gobj, spec->name, &gval);
    g_value_unset (&gval);

    return JS_TRUE;
}

gboolean
xgGObject::Resolve (const char  *name,
		    GParamSpec **spec,
		    guint       *signal_id)
{
    return Resolve (G_TYPE_FROM_INSTANCE (mObject), name, spec, signal_id);
}

// static
gboolean
xgGObject::Resolve (GType        aType,
		    const char  *name,
		    GParamSpec **spec,
		    guint       *signal_id)
{
    const char *n;

    n = gom_camel_uncase (name);

    if (n[0] == 'o' && n[1] == 'n' &&
        (*signal_id = g_signal_lookup (&n[2], aType))) {
#if 1
	g_print ("resolve %s.%s -> signal %u\n", g_type_name (aType), n, *signal_id);
#endif
        *spec = NULL;
        GOM_CAMEL_FREE (n, name);
        return PR_TRUE;
    }

    *spec = g_object_class_find_property ((GObjectClass *)g_type_class_peek (aType), n);
    *signal_id = 0;
#if 1
    g_print ("resolve %s.%s -> %s: %s\n", 
             g_type_name (aType), name, n,
             *spec ? g_type_name (G_PARAM_SPEC_VALUE_TYPE (*spec)) : "FAIL");
#endif
    GOM_CAMEL_FREE (n, name);

    return *spec != NULL;
}

nsresult
xgGObject::GetNative (JSContext    *cx,
		      JSObject     *obj,
		      nsISupports **_retval)
{
    nsresult rv;
    nsCOMPtr<nsIXPConnect> xpc (do_GetService ("@mozilla.org/js/xpc/XPConnect;1", &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIXPConnectWrappedNative> wrappedNative;
    rv = xpc->GetWrappedNativeOfJSObject (cx, obj, getter_AddRefs (wrappedNative));
    NS_ENSURE_SUCCESS (rv, rv);

    return wrappedNative->GetNative (_retval);
}

// static
JSObject *
xgGObject::GetNewOrUsed (JSContext *cx, GObject *gobj)
{
    nsCOMPtr<xgIGObjectHolder> holder (static_cast<xgIGObjectHolder *>(g_object_get_data (gobj, "XG_GOBJECT")));
    if (!holder) {
	xgGObject *obj = new xgGObject ();
	holder = obj;
	if (NS_FAILED (obj->Init (gobj))) {
	    return NULL;
	}
	obj->DefineProperties (cx, holder);
    }

    nsresult rv;
    nsCOMPtr<nsIXPConnect> xpc (do_GetService ("@mozilla.org/js/xpc/XPConnect;1", &rv));
    NS_ENSURE_SUCCESS (rv, NULL);

    nsCOMPtr<nsIXPConnectJSObjectHolder> jswrapper;
    rv = xpc->WrapNative (cx, JS_GetGlobalObject (cx), holder, NS_GET_IID (nsISupports), getter_AddRefs (jswrapper));
    NS_ENSURE_SUCCESS (rv, NULL);

    JSObject *jsobj;
    rv = jswrapper->GetJSObject (&jsobj);
    NS_ENSURE_SUCCESS (rv, NULL);

    return jsobj;
}

/* readonly attribute string className; */
NS_IMETHODIMP
xgGObject::GetClassName(char **aClassName)
{
    *aClassName = NS_strdup (G_OBJECT_TYPE_NAME (mObject));
    return NS_OK;
}

/* readonly attribute PRUint32 scriptableFlags; */
NS_IMETHODIMP
xgGObject::GetScriptableFlags (PRUint32 *aScriptableFlags)
{
    *aScriptableFlags = WANT_ADDPROPERTY | WANT_GETPROPERTY | WANT_SETPROPERTY | WANT_NEWRESOLVE | WANT_NEWENUMERATE;
    return NS_OK;
}

/* void preCreate (in nsISupports nativeObj, in JSContextPtr cx, in JSObjectPtr globalObj, out JSObjectPtr parentObj); */
NS_IMETHODIMP xgGObject::PreCreate(nsISupports *nativeObj, JSContext * cx, JSObject * globalObj, JSObject * *parentObj)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void create (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj); */
NS_IMETHODIMP xgGObject::Create(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void postCreate (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj); */
NS_IMETHODIMP xgGObject::PostCreate(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool addProperty (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal id, in JSValPtr vp); */
NS_IMETHODIMP
xgGObject::AddProperty (nsIXPConnectWrappedNative *wrapper,
			JSContext *cx, JSObject *obj, jsval id, jsval *vp, PRBool *_retval)
{
#if 0
    if (JSVAL_IS_INT (id)) {
	*_retval = PR_TRUE;
	return NS_OK;
    }

    const char *name = JSVAL_CHARS (id);
    GParamSpec *spec;
    guint signal_id;

#if 1
    g_print (GOM_LOC (""));
#endif
    if (!Resolve (name, &spec, &signal_id)) {
	*_retval = PR_TRUE;
	return NS_OK;
    }
#endif
    *_retval = PR_TRUE;
    return NS_OK;
}

/* PRBool delProperty (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal id, in JSValPtr vp); */
NS_IMETHODIMP xgGObject::DelProperty(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, jsval id, jsval * vp, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool getProperty (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal id, in JSValPtr vp); */
NS_IMETHODIMP
xgGObject::GetProperty (nsIXPConnectWrappedNative *wrapper,
			JSContext * cx, JSObject * obj, jsval id, jsval * vp, PRBool *_retval)
{
    if (JSVAL_IS_INT (id)) {
	*_retval = PR_TRUE;
	return NS_OK;
    }

    const char *name = JSVAL_CHARS (id);
    GParamSpec *spec;
    guint signal_id;

#if 1
    g_print (GOM_LOC (""));
#endif
    if (!Resolve (name, &spec, &signal_id)) {
	*_retval = PR_TRUE;
	return NS_OK;
    }

#if 0
    if (signal_id) {
        closure = gom_js_object_get_closure_prop (gobj, signal_id);
        g_print ("closure: %p\n", closure);
        *vp = closure ? OBJECT_TO_JSVAL (JS_GetFunctionObject (closure->fun)) : JSVAL_VOID;
        return JS_TRUE;
    }
#endif

    g_assert (spec);
    
    GValue gval = { 0 };
    g_value_init (&gval, G_PARAM_SPEC_VALUE_TYPE (spec));
    g_object_get_property (mObject, spec->name, &gval);

    GError *error = NULL;
    if (!gom_jsval (cx, vp, &gval, &error)) {
        g_printerr ("Could not get jsval: %s\n", error->message);
        g_value_unset (&gval);
        g_error_free (error);
	*_retval = PR_FALSE;
	return NS_OK;
    }
    g_value_unset (&gval);

    *_retval = PR_TRUE;
    return NS_OK;
}

/* PRBool setProperty (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal id, in JSValPtr vp); */
NS_IMETHODIMP xgGObject::SetProperty(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, jsval id, jsval * vp, PRBool *_retval)
{
    g_message (GOM_LOC ("well, got here?"));
    if (JSVAL_IS_INT (id)) {
	*_retval = PR_TRUE;
	return NS_OK;
    }

    const char *name = JSVAL_CHARS (id);
    GParamSpec *spec;
    guint signal_id;

    g_print (GOM_LOC (""));
    if (!Resolve (name, &spec, &signal_id)) {
	*_retval = PR_TRUE;
	return NS_OK;
    }

#if 0
    GomJSClosure *closure;
    if (signal_id) {
        JSFunction *fun;
        if (!JSVAL_IS_OBJECT (*vp) ||
            !JS_ObjectIsFunction (cx, JSVAL_TO_OBJECT (*vp))) {
            g_printerr ("jsval is not a function.  sucka.\n");
            return JS_FALSE;
        }
        fun = JS_ValueToFunction (cx, *vp);
        if (!fun) {
            g_printerr ("could not get function from value\n");
            return JS_FALSE;
        }
        closure = gom_js_object_get_closure_prop (gobj, signal_id);
        if (closure) {
            closure->fun = fun;
        } else {
            closure = (GomJSClosure *)gom_js_closure_new (cx, obj, fun);
            gom_js_object_set_closure_prop (gobj, signal_id, closure);
            g_signal_connect_closure_by_id (gobj, signal_id, 0, &closure->closure, FALSE);
        }
        return JS_TRUE;
    }
#endif
    g_assert (spec);

    GValue gval = { 0 };
    GError *error = NULL;
    if (!gom_g_value (cx, &gval, *vp, &error)) {
        g_printerr ("Could not get GValue: %s\n", error->message);
        g_error_free (error);
	*_retval = PR_FALSE;
	return NS_OK;
    }

    g_object_set_property (mObject, spec->name, &gval);
    g_value_unset (&gval);

    *_retval = PR_TRUE;
    return NS_OK;
}

/* PRBool enumerate (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj); */
NS_IMETHODIMP xgGObject::Enumerate(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool newEnumerate (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in PRUint32 enum_op, in JSValPtr statep, out JSID idp); */
NS_IMETHODIMP xgGObject::NewEnumerate(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, PRUint32 enum_op, jsval * statep, jsid *idp, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool newResolve (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal id, in PRUint32 flags, out JSObjectPtr objp); */
NS_IMETHODIMP
xgGObject::NewResolve (nsIXPConnectWrappedNative *wrapper,
		       JSContext *cx, JSObject *obj, jsval id, PRUint32 flags, JSObject **objp, PRBool *_retval)
{
    if (JSVAL_IS_INT (id)) {
	*objp = NULL;
        *_retval = PR_TRUE;
	return NS_OK;
    }
    const char *name = JSVAL_CHARS (id);
    GParamSpec *spec;
    guint signal_id;

    g_print (GOM_LOC (""));
    if (!Resolve (name, &spec, &signal_id)) {
	*objp = NULL;
	*_retval = PR_TRUE;
	return NS_OK;
    }

    *_retval = PR_TRUE;
    return NS_OK;
}

/* PRBool convert (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in PRUint32 type, in JSValPtr vp); */
NS_IMETHODIMP xgGObject::Convert(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, PRUint32 type, jsval * vp, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void finalize (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj); */
NS_IMETHODIMP xgGObject::Finalize(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool checkAccess (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal id, in PRUint32 mode, in JSValPtr vp); */
NS_IMETHODIMP xgGObject::CheckAccess(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, jsval id, PRUint32 mode, jsval * vp, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool call (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in PRUint32 argc, in JSValPtr argv, in JSValPtr vp); */
NS_IMETHODIMP xgGObject::Call(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, PRUint32 argc, jsval * argv, jsval * vp, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool construct (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in PRUint32 argc, in JSValPtr argv, in JSValPtr vp); */
NS_IMETHODIMP xgGObject::Construct(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, PRUint32 argc, jsval * argv, jsval * vp, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool hasInstance (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal val, out PRBool bp); */
NS_IMETHODIMP xgGObject::HasInstance(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, jsval val, PRBool *bp, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void trace (in nsIXPConnectWrappedNative wrapper, in JSTracerPtr trc, in JSObjectPtr obj); */
NS_IMETHODIMP xgGObject::Trace(nsIXPConnectWrappedNative *wrapper, JSTracer * trc, JSObject * obj)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRBool equality (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj, in JSVal val); */
NS_IMETHODIMP xgGObject::Equality(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, jsval val, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* JSObjectPtr outerObject (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj); */
NS_IMETHODIMP xgGObject::OuterObject(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, JSObject * *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* JSObjectPtr innerObject (in nsIXPConnectWrappedNative wrapper, in JSContextPtr cx, in JSObjectPtr obj); */
NS_IMETHODIMP xgGObject::InnerObject(nsIXPConnectWrappedNative *wrapper, JSContext * cx, JSObject * obj, JSObject * *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
