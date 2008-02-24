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

#include <gommacros.h>

#include <gom/dom/gomelement.h>
#include <gom/gomjsnode.h>
#include <gom/gomjsobject.h>

#include <string.h>

struct JSClass GomJSElementClass = {
    "GomElement", 0,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub,
    JS_ConvertStub, JS_FinalizeStub
};

static JSBool
element_get_tag_name (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GomElement *elem;
    const char *tag_name;

    elem = gom_js_object_get_g_object (cx, obj);
    tag_name = gom_element_get_tag_name (elem);
    
    *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, tag_name));

    return TRUE;
}

static JSPropertySpec gom_js_element_props[] = {
    { "tagName", -1, JSPROP_PERMANENT | JSPROP_READONLY, element_get_tag_name },
    { NULL }
};

static JSBool
element_get_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomElement *elem;
    char *attr, *name;

    elem = gom_js_object_get_g_object (cx, obj);
    if (!elem) {
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "s", &name)) {
        return JS_FALSE;
    }

    attr = gom_element_get_attribute (elem, name);
    *rval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, attr));
    g_free (attr);

    return TRUE;
}

static JSBool
element_set_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomElement *elem;
    char *name;
    char *value;

    elem = gom_js_object_get_g_object (cx, obj);
    if (!elem) {
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "ss", &name, &value)) {
        return JS_FALSE;
    }

    if (name[0] == 'o' && name[1] == 'n' && g_signal_lookup (&name[2], G_TYPE_FROM_INSTANCE (elem))) {
        JSFunction *fun  = NULL;

        fun = JS_CompileFunction (cx, obj, NULL, 0, NULL,
                                  value, strlen (value), NULL, 0);
        if (!fun) {
            g_printerr ("Error compiling script '%s'\n", value);
            return JS_FALSE;
        }

        if (!gom_js_object_connect (cx, obj, &name[2], fun)) {
            g_printerr ("couldn't connect function for signal %s\n", name);
        }
    } else {
        gom_element_set_attribute (elem, name, value, NULL);
    }

    return JS_TRUE;
}

static JSBool
element_remove_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
element_get_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
element_set_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
element_remove_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
element_get_elements_by_tag_name (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
element_normalize (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSFunctionSpec gom_js_element_funcs[] = {
    { "getAttribute",         element_get_attribute, 1 },
    { "setAttribute",         element_set_attribute, 2 },
    { "removeAttribute",      element_remove_attribute, 1 },
    { "getAttributeNode",     element_get_attribute_node, 1 },
    { "setAttributeNode",     element_set_attribute_node, 1 },
    { "removeAttributeNode",  element_remove_attribute_node, 1 },
    { "getElementsByTagName", element_get_elements_by_tag_name, 1 },
    { "normalize",            element_normalize, 0 },
    { NULL }
};

static JSBool
element_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_TRUE;
}

JSObject *
gom_js_element_init_class (JSContext *cx, JSObject *obj)
{
    JSObject *proto = JS_ConstructObject (cx, &GomJSNodeClass, NULL, NULL);
    return JS_InitClass (cx, obj, proto, &GomJSElementClass, element_construct, 0,
                         gom_js_element_props, gom_js_element_funcs, NULL, NULL);
}
