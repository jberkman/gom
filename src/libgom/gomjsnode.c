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

#include <gom/gomjsnode.h>

#include <gommacros.h>

#include <gom/dom/gomnode.h>
#include <gom/gomjsexception.h>
#include <gom/gomjsobject.h>

struct JSClass GomJSNodeClass = {
    "Node", 0,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    JS_FinalizeStub
};

static JSPropertySpec gom_js_node_props[] = {
    { NULL }
};

static JSBool
gom_js_node_insert_before (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_replace_child (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_remove_child (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_append_child (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomNode *node;
    JSObject *js_new_child;
    GomNode *new_child;
    GError *error = NULL;

    node = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_NODE (node)) {
        return JS_FALSE;
    }
    if (!JS_ConvertArguments (cx, argc, argv, "o", &js_new_child)) {
        return JS_FALSE;
    }
    new_child = gom_js_object_get_g_object (cx, js_new_child);
    if (!GOM_IS_NODE (new_child)) {
        return JS_FALSE;
    }
    new_child = gom_node_append_child (node, new_child, &error);
    if (!new_child) {
        gom_js_exception_set_error (cx, error);
        g_error_free (error);
        return JS_FALSE;
    }
    *rval = OBJECT_TO_JSVAL (gom_js_object_get_or_create_js_object (cx, new_child));
    g_assert (*rval == OBJECT_TO_JSVAL (js_new_child));
    return JS_TRUE;
}

static JSBool
gom_js_node_has_child_nodes (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomNode *node;
    
    node = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_NODE (node)) {
        return JS_FALSE;
    }
    *rval = BOOLEAN_TO_JSVAL (gom_node_has_child_nodes (node));
    return JS_TRUE;
}

static JSBool
gom_js_node_clone_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSFunctionSpec gom_js_node_funcs[] = {
    { "insertBefore",  gom_js_node_insert_before,   2 },
    { "replaceChild",  gom_js_node_replace_child,   2 },
    { "removeChild",   gom_js_node_remove_child,    1 },
    { "appendChild",   gom_js_node_append_child,    1 },
    { "hasChildNodes", gom_js_node_has_child_nodes, 0 },
    { "cloneNode",     gom_js_node_clone_node,      1 },
    { NULL }
};

static JSBool
gom_js_node_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_TRUE;
}

JSObject *
gom_js_node_init_class (JSContext *cx, JSObject *obj)
{
    JSObject *proto = JS_ConstructObject (cx, &GomJSObjectClass, NULL, NULL);
    return JS_InitClass (cx, obj, proto, &GomJSNodeClass, gom_js_node_construct, 0,
                         gom_js_node_props, gom_js_node_funcs, NULL, NULL);
}
