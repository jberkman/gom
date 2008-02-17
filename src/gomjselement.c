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
    char *attr;

    elem = gom_js_object_get_g_object (cx, obj);
    attr = gom_element_get_attribute (elem, JS_GetStringBytes (JSVAL_TO_STRING (argv[0])));
    *rval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, attr));

    return TRUE;
}

typedef struct {
    JSContext *cx;
    JSScript  *script;
    JSObject  *scriptobj;
} SignalData;



static JSBool
element_set_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomElement *elem;
    char *name;
    char *value;

    if (argc != 2) {
        return JS_FALSE;
    }

    elem = gom_js_object_get_g_object (cx, obj);
    if (!elem) {
        return JS_FALSE;
    }

    name = JS_GetStringBytes (JSVAL_TO_STRING (argv[0]));
    value = JS_GetStringBytes (JSVAL_TO_STRING (argv[1]));

    if (!name || !value) {
        return JS_FALSE;
    }

    if (name[0] == 'o' && name[1] == 'n' && g_signal_lookup (&name[2], G_TYPE_FROM_INSTANCE (elem))) {
        SignalData *data = NULL;
        JSScript *script = NULL;

        script = JS_CompileScript (cx, obj, value, strlen (value), "<script>", 0);
        if (!script) {
            g_printerr ("Error compiling script '%s'\n", value);
            return JS_FALSE;
        }

        data = g_new0 (SignalData, 1);
        data->cx     = cx;
        data->script = script;
        data->scriptobj = JS_NewScriptObject (cx, script);
        if (!data->scriptobj || !JS_AddRoot (cx, &data->scriptobj)) {
            /*goto script_fail*/;
        }
#if 0
        g_signal_connect_data (elem, &name[2], 
            
        mod = g_module_open (NULL, 0);
        g_print ("found signal '%s' on '%s'\n", &name[2], g_type_name _from_instance ((GTypeInstance *)elem));
        if (g_module_symbol (mod, value, &c_handler)) {
            g_print ("found handler for signal '%s' -> '%s' '%p'\n",
                     &name[2], value, c_handler);
            g_signal_connect (elem, &name[2], c_handler, NULL);
        } else {
            g_print ("could not find handler for signal '%s' -> '%s'\n",
                     &name[2], value);
        }
        g_module_close (mod);
#endif
    } else {
        gom_element_set_attribute (elem, name, value, NULL);
    }

    return JS_TRUE;
}

static JSBool
element_remove_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
element_get_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
element_set_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
element_remove_attribute_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
element_get_elements_by_tag_name (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
element_normalize (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
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

JSObject *
gom_js_element_init_class (JSContext *cx, JSObject *obj)
{
    JSObject *proto = JS_NewObject (cx, &GomJSNodeClass, NULL, obj);
    return JS_InitClass (cx, obj, proto, &GomJSElementClass, NULL, 0,
                         gom_js_element_props, gom_js_element_funcs, NULL, NULL);
}
