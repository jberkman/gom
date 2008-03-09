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

#include <gom/gomjselement.h>

#include <gom/dom/gomelement.h>
#include <gom/gomjsexception.h>
#include <gom/gomjsnode.h>
#include <gom/gomjsobject.h>
#include <gom/gomobject.h>
#include <gom/gomvalue.h>

#include <gtk/gtkwidget.h>

#include <string.h>

#include <gommacros.h>

#define JSVAL_CHARS(jval) (JS_GetStringBytes (JSVAL_TO_STRING (jval)))

static JSBool
gom_js_element_get_prop (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GObject *gobj;
    GParamSpec *spec;
    const char *name;
    guint signal_id;

    GValue *gval;
    GError *error = NULL;

    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

    g_print ("%s:%d:%s(): ", __FILE__, __LINE__, __FUNCTION__);
    name = JSVAL_CHARS (id);

    /* let handled properties fall through */
    if (gom_js_object_resolve (cx, obj, name, &gobj, &spec, &signal_id)) {
        return JS_TRUE;
    }

    gval = gom_object_get_attribute (gobj, name);
    if (!gval) {
        *vp = JSVAL_VOID;
        return JS_TRUE;
    }
    if (!gom_jsval (cx, vp, gval, &error)) {
        gom_js_exception_set_error (cx, error);
        g_error_free (error);
        return JS_FALSE;
    }
    return JS_TRUE;
}

static JSBool
gom_js_element_set_prop (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GObject *gobj;
    GParamSpec *spec;
    const char *name;
    guint signal_id;

    GValue gval = { 0 };
    GError *error = NULL;

    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

    g_print ("%s:%d:%s(): ", __FILE__, __LINE__, __FUNCTION__);
    name = JSVAL_CHARS (id);

    /* let resolvable properties fall through */
    if (gom_js_object_resolve (cx, obj, name, &gobj, &spec, &signal_id)) {
        return JS_TRUE;
    }

    if (!gom_g_value (cx, &gval, *vp, &error)) {
        gom_js_exception_set_error (cx, error);
        g_error_free (error);
        return JS_FALSE;
    }

    gom_object_set_attribute (gobj, name, &gval);
    g_value_unset (&gval);

    return JS_TRUE;
}

static JSBool
gom_js_element_resolve (JSContext *cx, JSObject *obj, jsval id, uintN flags, JSObject **objp)
{
    GObject *gobj;
    GParamSpec *spec;
    const char *name;
    guint signal_id;
    JSBool has_prop;
    JSObject *proto;

    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

    g_print ("%s:%d:%s(%p, %p): ", __FILE__, __LINE__, __FUNCTION__, obj, *objp);
    name = JSVAL_CHARS (id);

    /* let GomJSObject pick it up for defined properties */
    if (gom_js_object_resolve (cx, *objp, name, &gobj, &spec, &signal_id)) {
        *objp = NULL;
        return JS_TRUE;
    }

    proto = JS_GetPrototype (cx, obj);
    if (!JS_HasProperty (cx, proto, name, &has_prop)) {
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "error looking up property")));
        }
        return JS_FALSE;
    }
    if (has_prop) {
        g_print ("%s:%d:%s(): %s.%s already exists\n",
                 __FILE__, __LINE__, __FUNCTION__,
                 JS_GET_CLASS (cx, proto)->name, name);
        return JS_TRUE;
    }

    if (!JS_DefineProperty (cx, *objp, name, JSVAL_VOID,
                            gom_js_element_get_prop, gom_js_element_set_prop,
                            JSPROP_ENUMERATE)) {
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "error defining property")));
        }
        return JS_FALSE;
    }
    g_print ("%s:%d:%s(): defined new property: %s.%s (%s)\n", 
             __FILE__, __LINE__, __FUNCTION__,
             JS_GET_CLASS (cx, *objp)->name, name, JS_GET_CLASS (cx, obj)->name);
    return JS_TRUE;
}

struct JSClass GomJSElementClass = {
    "Element",
    JSCLASS_NEW_RESOLVE | JSCLASS_NEW_RESOLVE_GETS_START,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,

    JS_EnumerateStub,
    (JSResolveOp)gom_js_element_resolve,
    JS_ConvertStub, JS_FinalizeStub
};

static JSBool
gom_js_element_get_tag_name (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GomElement *elem;
    const char *tag_name;

    elem = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_ELEMENT (elem)) {
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "this is not a GomElement")));
        }
        return JS_FALSE;
    }
    tag_name = gom_element_get_tag_name (elem);
    
    *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, tag_name));

    return TRUE;
}

static JSPropertySpec gom_js_element_props[] = {
    { "tagName", -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_element_get_tag_name },
    { NULL }
};

static JSBool
gom_js_element_get_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomElement *elem;
    char *attr, *name;

    elem = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_ELEMENT (elem)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "this is not a GomElement")));
        }
#endif
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "s", &name)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "invalid arguments")));
        }
#endif
        return JS_FALSE;
    }

    attr = gom_element_get_attribute (elem, name);
    *rval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, attr));
    g_free (attr);

    return TRUE;
}

static JSBool
gom_js_element_set_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomElement *elem;
    char *name;
    char *value;
    GError *error = NULL;

    elem = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_ELEMENT (elem)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "this is not a GomElement")));
        }
#endif
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "ss", &name, &value)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "invalid arguments")));
        }
#endif
        return JS_FALSE;
    }

    if (name[0] == 'o' && name[1] == 'n' && g_signal_lookup (&name[2], G_TYPE_FROM_INSTANCE (elem))) {
        JSFunction *fun  = NULL;

        fun = JS_CompileFunction (cx, obj, NULL, 0, NULL,
                                  value, strlen (value), NULL, 0);
        if (!fun) {
            if (!JS_IsExceptionPending (cx)) {
                JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Unknown error while compiling function")));
            }
            return JS_FALSE;
        }

        if (!gom_js_object_connect (cx, obj, &name[2], fun)) {
            if (!JS_IsExceptionPending (cx)) {
                JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Unknown error while connecting event handler")));
            }
            return JS_FALSE;
        }
    } else {
        gom_element_set_attribute (elem, name, value, &error);
        if (error) {
            gom_js_exception_set_error (cx, error);
            return JS_FALSE;
        }
    }

    return JS_TRUE;
}

static JSBool
gom_js_element_remove_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_element_get_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_element_set_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_element_remove_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_element_get_elements_by_tag_name (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_element_normalize (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSFunctionSpec gom_js_element_funcs[] = {
    { "getAttribute",         gom_js_element_get_attribute, 1 },
    { "setAttribute",         gom_js_element_set_attribute, 2 },
    { "removeAttribute",      gom_js_element_remove_attribute, 1 },
    { "getAttributeNode",     gom_js_element_get_attribute_node, 1 },
    { "setAttributeNode",     gom_js_element_set_attribute_node, 1 },
    { "removeAttributeNode",  gom_js_element_remove_attribute_node, 1 },
    { "getElementsByTagName", gom_js_element_get_elements_by_tag_name, 1 },
    { "normalize",            gom_js_element_normalize, 0 },
    { NULL }
};

static JSBool
gom_js_element_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_TRUE;
}

JSObject *
gom_js_element_init_class (JSContext *cx, JSObject *obj)
{
    JSObject *proto;

    gom_js_object_register_js_class (cx, GTK_TYPE_WIDGET, &GomJSElementClass);

    proto = JS_ConstructObject (cx, &GomJSNodeClass, NULL, NULL);
    return JS_InitClass (cx, obj, proto, &GomJSElementClass, gom_js_element_construct, 0,
                         gom_js_element_props, gom_js_element_funcs, NULL, NULL);
}
