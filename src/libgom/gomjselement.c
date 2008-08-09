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

#include "gom/gomjselement.h"

#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomelement.h"
#include "gom/gomelem.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsnode.h"
#include "gom/gomjsobject.h"
#include "gom/gomobject.h"
#include "gom/gomvalue.h"

#include "gommacros.h"

#include <gtk/gtkwidget.h>

#include <string.h>

static JSBool
gom_js_element_get_prop (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GObject *gobj;
    GParamSpec *spec;
    const char *name;
    guint signal_id;

    const GValue *gval;
    GError *error = NULL;

    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

#if 0
    g_print (GOM_LOC ("");
#endif
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
        return gom_js_exception_set_error (cx, &error);
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

#if 0
    g_print (GOM_LOC());
#endif
    name = JSVAL_CHARS (id);

    /* let resolvable properties fall through */
    if (gom_js_object_resolve (cx, obj, name, &gobj, &spec, &signal_id)) {
        return JS_TRUE;
    }

    if (!gom_g_value (cx, &gval, *vp, &error)) {
        return gom_js_exception_set_error (cx, &error);
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
        *objp = NULL;
        return JS_TRUE;
    }

#if 0
    g_print ("%s:%d:%s(%p, %p): ", obj, *objp);
#endif
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
#if 0
        g_print (GOM_LOC ("%s.%s already exists\n",
                  JS_GET_CLASS (cx, proto)->name, name);
#endif
        *objp = NULL;
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
#if 0
    g_print (GOM_LOC ("defined new property: %s.%s (%s)\n", 
              JS_GET_CLASS (cx, *objp)->name, name, JS_GET_CLASS (cx, obj)->name);
#endif
    return JS_TRUE;
}

static void
gom_js_element_finalize (JSContext *cx, JSObject *obj)
{
    GomJSNodeClass.finalize (cx, obj);
}

JSClass GomJSElementClass = {
    "Element",
    JSCLASS_NEW_RESOLVE | JSCLASS_NEW_RESOLVE_GETS_START | JSCLASS_NEW_ENUMERATE,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,

    (JSEnumerateOp)gom_js_object_enumerate,
    (JSResolveOp)gom_js_element_resolve,
    JS_ConvertStub, gom_js_element_finalize
};

static JSPropertySpec gom_js_element_props[] = {
    { NULL }
};

static JSBool
gom_js_element_get_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomElement *elem;
    char *attr, *name;
    GParamSpec *spec;
    guint signal_id;

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

    gom_object_resolve (G_OBJECT (elem), name, &spec, &signal_id);
    if (signal_id) {
        attr = g_strdup ("<signal handler>");
    } else {
        attr = gom_element_get_attribute (elem, spec ? spec->name : name);
    }
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
    GParamSpec *spec;
    guint signal_id;
    JSFunction *fun;

    elem = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_ELEMENT (elem)) {
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "ss", &name, &value)) {
        return JS_FALSE;
    }

    gom_object_resolve (G_OBJECT (elem), name, &spec, &signal_id);
    if (signal_id) {
        fun = JS_CompileFunction (cx, obj, NULL, 0, NULL,
                                  value, strlen (value), NULL, 0);
        if (!fun) {
            if (!JS_IsExceptionPending (cx)) {
                JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Unknown error while compiling function")));
            }
            return JS_FALSE;
        }

        if (!gom_js_object_connect (cx, obj, signal_id, fun)) {
            if (!JS_IsExceptionPending (cx)) {
                JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Unknown error while connecting event handler")));
            }
            return JS_FALSE;
        }
    } else {
        gom_element_set_attribute (elem, spec ? spec->name : name, value, &error);
    }
    return error ? gom_js_exception_set_error (cx, &error) : JS_TRUE;
}

static JSBool
gom_js_element_remove_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_get_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_set_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_remove_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_get_elements_by_tag_name (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_get_attribute_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_set_attribute_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_remove_attribute_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_get_attribute_node_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_set_attribute_node_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_get_elements_by_tag_name_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_has_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_element_has_attribute_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
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

    { "getAttributeNS",         gom_js_element_get_attribute_ns, 2 },
    { "setAttributeNS",         gom_js_element_set_attribute_ns, 3 },
    { "removeAttributeNS",      gom_js_element_remove_attribute_ns, 2 },
    { "getAttributeNodeNS",     gom_js_element_get_attribute_node_ns, 2 },
    { "setAttributeNodeNS",     gom_js_element_set_attribute_node_ns, 1 },
    { "getElementsByTagNameNS", gom_js_element_get_elements_by_tag_name_ns, 2 },
    { "hasAttribute",           gom_js_element_has_attribute, 1 },
    { "hasAttributeNS",         gom_js_element_has_attribute_ns, 2, },

    { NULL }
};

static JSBool
gom_js_element_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_element_init_class (JSContext *cx, JSObject *obj)
{
    gom_js_object_register_js_class (cx, GOM_TYPE_ELEMENT, &GomJSElementClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, &GomJSNodeClass, NULL, NULL),
                         &GomJSElementClass, gom_js_element_construct, 0,
                         gom_js_element_props, gom_js_element_funcs, NULL, NULL);
}
