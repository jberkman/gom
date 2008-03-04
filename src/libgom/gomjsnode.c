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
#include <gom/gomjsobject.h>

struct JSClass GomJSNodeClass = {
    "GomNode", 0,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    JS_FinalizeStub
};

static JSBool
gom_js_node_get_node_name (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_node_value (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_set_node_value (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_node_type (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_parent_node (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_child_nodes (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_first_child (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GomNode *node;
    GomNode *child;
    JSObject *jschild;

    node = gom_js_object_get_g_object (cx, obj);
    child = gom_node_get_first_child (node);
    if (child) {
        jschild = gom_js_object_get_or_create_js_object (cx, child);
        *vp = OBJECT_TO_JSVAL (jschild);
    }

    return JS_TRUE;
}

static JSBool
gom_js_node_get_last_child (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_previous_sibling (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_next_sibling (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_attributes (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_get_owner_document (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSPropertySpec gom_js_node_props[] = {
    { "nodeName",        -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_node_name },
    { "nodeValue",       -1, JSPROP_PERMANENT,                   gom_js_node_get_node_value, gom_js_node_set_node_value },
    { "nodeType",        -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_node_type },
    { "parentNode",      -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_parent_node },
    { "childNodes",      -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_child_nodes },
    { "firstChild",      -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_first_child },
    { "lastChild",       -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_last_child },
    { "previousSibling", -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_previous_sibling },
    { "nextSibling",     -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_next_sibling },
    { "attributes",      -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_attributes },
    { "ownerDocument",   -1, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_node_get_owner_document },
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
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
}

static JSBool
gom_js_node_has_child_nodes (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_FALSE;
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
