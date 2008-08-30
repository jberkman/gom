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

#include "xgGObjectUtils.h"

#include "xgIGObjectHolder.h"

#include "gommacros.h"
#include "gom/gomcamel.h"
#include "gom/gomvalue.h"

#include <jsapi.h>

#include <nsCOMPtr.h>
//#include <nsDOMClassInfo.h>
#include <nsIDOMDocument.h>
#include <nsIDOMNode.h>
#include <nsIDocument.h>
#include <nsIScriptContext.h>
#include <nsIScriptGlobalObject.h>
#include <nsIXPConnect.h>
#include <nsServiceManagerUtils.h>

// static
nsresult
xgGObjectUtils::DefineProperties (nsIDOMNode *aNode, GType aType)
{
    nsresult rv;

    nsCOMPtr<nsIDOMDocument> doc;
    rv = aNode->GetOwnerDocument (getter_AddRefs (doc));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIDocument> nsdoc (do_QueryInterface (doc, &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIScriptGlobalObject> global (nsdoc->GetScriptGlobalObject ());
    NS_ENSURE_TRUE (global, NS_ERROR_UNEXPECTED);

    rv = global->EnsureScriptEnvironment (nsIProgrammingLanguage::JAVASCRIPT);
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIScriptContext> cx (global->GetScriptContext (nsIProgrammingLanguage::JAVASCRIPT));
    NS_ENSURE_TRUE (cx, NS_ERROR_UNEXPECTED);

    JSContext *jscx = (JSContext *)cx->GetNativeContext ();
    NS_ENSURE_TRUE (jscx, NS_ERROR_UNEXPECTED);

    JSObject *jsglobal = (JSObject *)cx->GetNativeGlobal ();
    NS_ENSURE_TRUE (jsglobal, NS_ERROR_UNEXPECTED);

    nsCOMPtr<nsIXPConnect> xpc (do_GetService ("@mozilla.org/js/xpc/XPConnect;1", &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIXPConnectJSObjectHolder> jswrapper;
    rv = xpc->WrapNative (jscx, jsglobal, aNode, NS_GET_IID (nsISupports), getter_AddRefs (jswrapper));
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
    GParamSpec **props = g_object_class_list_properties ((GObjectClass *)g_type_class_peek (aType), &n_properties);
    g_message (GOM_LOC ("Adding %d properties from %s"), n_properties, g_type_name (aType));

    const char *camelName;
    for (guint i = 0; i < n_properties; i++) {
	camelName = gom_camel_case (props[i]->name);
	if (!JS_DefineProperty (jscx, jsobj, camelName, JSVAL_VOID,
				xgGObjectUtils::GetProperty,
				xgGObjectUtils::SetProperty,
				JSPROP_ENUMERATE | JSPROP_PERMANENT)) {
	    g_printerr ("Could not define a property for %s\n", camelName);
	} else {
	    g_print (GOM_LOC ("Defined property: %s.%s\n"),
		     g_type_name (aType), camelName);
	}
	GOM_CAMEL_FREE (camelName, props[i]->name);
    }
    return NS_OK;
}

// static
JSBool
xgGObjectUtils::GetProperty (JSContext *cx,
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
xgGObjectUtils::SetProperty (JSContext *cx,
			     JSObject  *obj,
			     jsval      id,
			     jsval     *vp)
{
    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

#if 0
    g_print (GOM_LOC (""));
#endif
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

PRBool
xgGObjectUtils::Resolve (GType        aType,
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
xgGObjectUtils::GetNative (JSContext    *cx,
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
