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

#include <gom/gomjsobject.h>

static GHashTable *g2js = NULL;
static GHashTable *js2g = NULL;

static void
gom_js_object_finalize (JSContext *cx, JSObject *obj)
{
    GObject *gobj = JS_GetInstancePrivate (cx, obj, &GomJSObjectClass, NULL);
    g_print ("Finalizing JSObject %p: unref %s %p\n",
             obj, gobj ? g_type_name (G_TYPE_FROM_INSTANCE (gobj)) : "<NULL>",
             gobj);
    if (gobj) {
        g_assert (G_IS_OBJECT (gobj));
        g_hash_table_remove (g2js, gobj);
        g_hash_table_remove (js2g, obj);
        g_object_unref (gobj);
    }
}

struct JSClass GomJSObjectClass = {
    "GomObject", 0,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub,
    gom_js_object_finalize
};

static JSPropertySpec gom_js_object_props[] = { { NULL } };
static JSFunctionSpec gom_js_object_funcs[] = { { NULL } };

JSObject *
gom_js_object_init_class (JSContext *cx, JSObject *obj)
{
    g2js = g_hash_table_new (NULL, NULL);
    js2g = g_hash_table_new (NULL, NULL);
    return JS_InitClass (cx, obj, NULL, &GomJSObjectClass, NULL, 0,
                         gom_js_object_props, gom_js_object_funcs, 
                         NULL, NULL);
}

void
gom_js_object_set_g_object (JSContext *cx, JSObject *jsobj, gpointer gobject)
{
    g_print ("Binding %s %p to JSObject %p\n",
             g_type_name (G_TYPE_FROM_INSTANCE (gobject)),
             gobject, jsobj);
    g_hash_table_insert (js2g, jsobj, g_object_ref (gobject));
    g_hash_table_insert (g2js, gobject, jsobj);
}

gpointer
gom_js_object_get_g_object (JSContext *cx, JSObject *jsobj)
{
    return g_hash_table_lookup (js2g, jsobj);
}

JSObject *
gom_js_object_get_js_object (gpointer gobj)
{
    return g_hash_table_lookup (g2js, gobj);
}

JSObject *
gom_js_object_get_or_create_js_object (JSContext *cx, gpointer gobj)
{
    JSObject *jsobj;

    jsobj = gom_js_object_get_js_object (gobj);
    if (!jsobj) {
        jsobj = JS_NewObject (cx, gom_js_object_get_js_class (GOM_JS_OBJECT (gobj)), NULL, NULL);
        gom_js_object_set_g_object (cx, jsobj, gobj);
    }

    return jsobj;
}

typedef struct {
    GClosure    closure;
    JSContext  *cx;
    JSObject   *obj;
    JSFunction *fun;
} GomJSClosure;

static void
gom_js_closure_finalize (gpointer data, GClosure *closure)
{
    GomJSClosure *jsclosure = (GomJSClosure *)closure;
}

static void
gom_js_closure_marshal (GClosure *closure,
                        GValue *return_value,
                        guint n_param_values,
                        const GValue *param_values,
                        gpointer invocation_hint,
                        gpointer marshal_data)
{
    GomJSClosure *jsclosure = (GomJSClosure *)closure;
    int i;
    jsval *argv;
    jsval  rval;
    g_print ("running closure; returns %s, %d arguments:\n", 
             return_value ? g_type_name (G_VALUE_TYPE (return_value)) : "void",
             n_param_values);
    argv = g_new0 (jsval, n_param_values);
    for (i = 0; i < n_param_values; i++) {
        if (g_type_is_a (G_VALUE_TYPE (&param_values[i]), GOM_TYPE_JS_OBJECT)) {
            JSObject *jsobj = gom_js_object_get_or_create_js_object (jsclosure->cx,
                                                                     g_value_get_object (&param_values[i]));
            argv[i] = OBJECT_TO_JSVAL (jsobj);
        } else {
            argv[i] = JSVAL_NULL;
        }
        g_print ("\t%s: %#lx\n", g_type_name (G_VALUE_TYPE (&param_values[i])), argv[i]);
    }

    if (!JS_CallFunction (jsclosure->cx, jsclosure->obj,
                          jsclosure->fun,
                          n_param_values, argv,
                          &rval)) {
        g_printerr ("Error calling function!\n");
    }
}

static GClosure *
gom_js_closure_new_script (JSContext *cx, JSObject *obj,
                           const char *script, gsize scriptlen,
                           const char *filename, int lineno)
{
    GClosure *closure;
    GomJSClosure *jsclosure;

    closure = g_closure_new_simple (sizeof (GomJSClosure), NULL);
    jsclosure = (GomJSClosure *)closure;

    g_closure_add_finalize_notifier (closure, NULL, gom_js_closure_finalize);

    jsclosure->cx  = cx;
    jsclosure->obj = obj;

    jsclosure->fun = JS_CompileFunction (cx, obj, NULL,
                                         0, NULL,
                                         script, scriptlen,
                                         filename, lineno);

    g_closure_set_marshal (closure, gom_js_closure_marshal);

    return closure;
}

gulong
gom_js_object_connect_script (JSContext *cx, JSObject *jsobj,
                              const char *signal,
                              const char *script, gsize scriptlen,
                              const char *filename, int lineno)
{
    GClosure *closure;
    GObject *gobj;

    gobj = gom_js_object_get_g_object (cx, jsobj);
    if (!gobj) {
        g_printerr ("couldn't get g_object for jsobject %p\n", jsobj);
        return 0;
    }

    closure = gom_js_closure_new_script (cx, jsobj, script, scriptlen, filename, lineno);
    if (!closure) {
        return 0;
    }

    return g_signal_connect_closure (gobj, signal, closure, FALSE);
}
