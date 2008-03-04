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

#include <gom/gomobject.h>
#include <gom/gomvalue.h>

#include <gommacros.h>

#define JSVAL_CHARS(jval) (JS_GetStringBytes (JSVAL_TO_STRING (jval)))

#define GOM_JS_OBJECT_CLOSURES_QUARK (closures_quark ())
#define CLOSURES(o) ((GHashTable *)g_object_get_qdata (G_OBJECT (o), GOM_JS_OBJECT_CLOSURES_QUARK));

static gpointer
closures_quark_once (gpointer data)
{
    return GUINT_TO_POINTER (g_quark_from_static_string ("gom-object-closures-quark"));
}

static GQuark
closures_quark (void)
{
    static GOnce closures_once = G_ONCE_INIT;
    return GPOINTER_TO_UINT (g_once (&closures_once, closures_quark_once, NULL));
}

static GHashTable *g2js = NULL;
static GHashTable *js2g = NULL;

typedef struct {
    GClosure    closure;
    JSContext  *cx;
    JSObject   *obj;
    JSFunction *fun;
} GomJSClosure;

static void
gom_js_closure_finalize (gpointer data, GClosure *closure)
{
    /*GomJSClosure *jsclosure = (GomJSClosure *)closure;*/
    GOM_NOT_IMPLEMENTED;
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
    GError *error = NULL;
    int i;
    jsval *argv;
    jsval  rval;
    GValue tmp_ret = { 0 };
    const char *rettype = NULL;

    if (return_value) {
        rettype = G_VALUE_TYPE_NAME (return_value);
    }
    if (!rettype) {
        rettype = "void";
    }
    g_print ("running closure; returns %s, %d arguments\n", 
             rettype, n_param_values);

    argv = g_new0 (jsval, n_param_values);
    for (i = 0; i < n_param_values; i++) {
        if (!gom_jsval (jsclosure->cx, &argv[i], &param_values[i], &error)) {
            g_printerr ("Error converting param of type '%s': %s\n",
                        G_VALUE_TYPE_NAME (&param_values[i]),
                        error->message);
            g_error_free (error);
            error = NULL;
            argv[i] = JSVAL_NULL;
        }
    }

    if (!JS_CallFunction (jsclosure->cx, jsclosure->obj,
                          jsclosure->fun,
                          n_param_values, argv,
                          &rval)) {
        g_printerr ("Error calling function!\n");
        return;
    }
    
    if (return_value && G_VALUE_TYPE (return_value) != G_TYPE_INVALID) {
        if (!gom_g_value (jsclosure->cx, &tmp_ret, rval, &error)) {
            g_printerr ("Error converting return type to g_value: %s\n", error->message);
            return;
        }
        if (!g_value_transform (&tmp_ret, return_value)) {
            g_printerr ("Could not convert js return type '%s' to desired '%s'\n",
                        G_VALUE_TYPE_NAME (&tmp_ret),
                        G_VALUE_TYPE_NAME (return_value));
            g_value_unset (&tmp_ret);
            return;
        }
        g_value_unset (&tmp_ret);
    }
}

static GClosure *
gom_js_closure_new (JSContext *cx, JSObject *obj, JSFunction *fun)
{
    GClosure *closure;
    GomJSClosure *jsclosure;

    closure = g_closure_new_simple (sizeof (GomJSClosure), NULL);
    jsclosure = (GomJSClosure *)closure;

    g_closure_add_finalize_notifier (closure, NULL, gom_js_closure_finalize);

    jsclosure->cx  = cx;
    jsclosure->obj = obj;
    jsclosure->fun = fun;

    g_closure_set_marshal (closure, gom_js_closure_marshal);

    return closure;
}

gulong
gom_js_object_connect (JSContext *cx, JSObject *jsobj,
                       const char *signal_name,
                       JSFunction *fun)
{
    GClosure *closure;
    GObject *gobj;

    gobj = gom_js_object_get_g_object (cx, jsobj);
    if (!gobj) {
        g_printerr ("couldn't get g_object for jsobject %p\n", jsobj);
        return 0;
    }

    closure = gom_js_closure_new (cx, jsobj, fun);
    if (!closure) {
        return 0;
    }

    return g_signal_connect_closure (gobj, signal_name, closure, FALSE);
}

static GomJSClosure *
gom_js_object_get_closure_prop (GObject *gobj, guint signal_id)
{
    GHashTable *closures = CLOSURES (gobj);
    return closures ? (GomJSClosure *)g_hash_table_lookup (closures, GUINT_TO_POINTER (signal_id)) : NULL;
}

static void
gom_js_object_set_closure_prop (GObject *gobj, guint signal_id, GomJSClosure *closure)
{
    GHashTable *closures = CLOSURES (gobj);
    if (!closures) {
        closures = g_hash_table_new (NULL, NULL);
        g_object_set_qdata_full (gobj, GOM_JS_OBJECT_CLOSURES_QUARK, closures, (GDestroyNotify)g_hash_table_destroy);
    }
    g_hash_table_insert (closures, GUINT_TO_POINTER (signal_id), closure);
}

gboolean
gom_js_object_resolve (JSContext *cx, JSObject *obj, const char *name, 
                       GObject **gobj, GParamSpec **spec, guint *signal_id)
{
    *gobj = gom_js_object_get_g_object (cx, obj);
    if (!*gobj) {
        g_print ("resolve %s.%s -> FAIL (no GObject)\n", JS_GET_CLASS (cx, obj)->name, name);
        return FALSE;
    }
    g_assert (G_IS_OBJECT (*gobj));
#if 0
    if (!GOM_IS_JS_OBJECT (*gobj)) {
        g_warning ("JSObject %p (%s) is not a GomJSObject (%s)",
                   obj, JS_GET_CLASS (cx, obj)->name, g_type_name (G_TYPE_FROM_INSTANCE (*gobj)));
    }
#endif
    return gom_object_resolve (*gobj, name, spec, signal_id);
}

static JSBool
gom_js_object_get_prop (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GObject *gobj;
    GParamSpec *spec;
    const char *name;
    guint signal_id;

    GValue gval = { 0 };
    GError *error = NULL;
    GomJSClosure *closure;

    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

    g_print ("%s:%d:%s(): ", __FILE__, __LINE__, __FUNCTION__);
    name = JSVAL_CHARS (id);
    if (!gom_js_object_resolve (cx, obj, name, &gobj, &spec, &signal_id)) {
        return JS_TRUE;
    }

    if (signal_id) {
        closure = gom_js_object_get_closure_prop (gobj, signal_id);
        g_print ("closure: %p\n", closure);
        *vp = closure ? OBJECT_TO_JSVAL (JS_GetFunctionObject (closure->fun)) : JSVAL_VOID;
        return JS_TRUE;
    }

    g_assert (spec);
    
    g_value_init (&gval, G_PARAM_SPEC_VALUE_TYPE (spec));
    g_object_get_property (gobj, name, &gval);
    if (!gom_jsval (cx, vp, &gval, &error)) {
        g_printerr ("Could not get jsval: %s\n", error->message);
        g_value_unset (&gval);
        g_error_free (error);
        return JS_FALSE;
    }
    g_value_unset (&gval);

    return JS_TRUE;
}

static JSBool
gom_js_object_set_prop (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    GObject *gobj;
    GParamSpec *spec;
    const char *name;
    guint signal_id;

    GValue gval = { 0 };
    GError *error = NULL;
    GomJSClosure *closure;

    if (JSVAL_IS_INT (id)) {
        return JS_TRUE;
    }

    g_print ("%s:%d:%s(): ", __FILE__, __LINE__, __FUNCTION__);
    name = JSVAL_CHARS (id);
    if (!gom_js_object_resolve (cx, obj, name, &gobj, &spec, &signal_id)) {
        return JS_TRUE;
    }

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

    g_assert (spec);

    if (!gom_g_value (cx, &gval, *vp, &error)) {
        g_printerr ("Could not get GValue: %s\n", error->message);
        g_error_free (error);
        return JS_FALSE;
    }

    g_object_set_property (gobj, name, &gval);
    g_value_unset (&gval);

    return JS_TRUE;
}

static JSBool
gom_js_object_resolve_priv (JSContext *cx, JSObject *obj, jsval id, uintN flags, JSObject **objp)
{
    GObject *gobj;
    GParamSpec *spec;
    const char *name;
    guint signal_id;

    if (JSVAL_IS_INT (id)) {
        *objp = NULL;
        return JS_TRUE;
    }
    g_assert (JSVAL_IS_STRING (id));
    name = JSVAL_CHARS (id);

    g_print ("%s:%d:%s(): ", __FILE__, __LINE__, __FUNCTION__);
    if (!gom_js_object_resolve (cx, *objp, name, &gobj, &spec, &signal_id)) {
        *objp = NULL;
        return JS_TRUE;
    }

    if (!JS_DefineProperty (cx, *objp, name, JSVAL_VOID,
                            gom_js_object_get_prop, gom_js_object_set_prop, 
                            spec ? JSPROP_ENUMERATE|JSPROP_PERMANENT : 0)) {
        g_printerr ("Could not define a property for %s\n", name);
        return JS_FALSE;
    }

    g_print ("%s:%d:%s(): defined new property: %s.%s (%s)\n", 
             __FILE__, __LINE__, __FUNCTION__,
             JS_GET_CLASS (cx, *objp)->name, name, JS_GET_CLASS (cx, obj)->name);
    
    return JS_TRUE;
}

static void
gom_js_object_finalize (JSContext *cx, JSObject *obj)
{
    GObject *gobj = gom_js_object_get_g_object (cx, obj);
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
    "GomObject",
    JSCLASS_NEW_RESOLVE | JSCLASS_NEW_RESOLVE_GETS_START,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub,
    (JSResolveOp)gom_js_object_resolve_priv,
    JS_ConvertStub,
    gom_js_object_finalize
};

static JSPropertySpec gom_js_object_props[] = { { NULL } };

static JSBool
gom_js_object_to_string (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GObject *gobj;
    char *str;
    
    gobj = gom_js_object_get_g_object (cx, obj);

    str = g_strdup_printf ("[gomobject %s %s]", 
                           JS_GET_CLASS (cx, obj)->name,
                           gobj
                           ? g_type_name (G_TYPE_FROM_INSTANCE (gobj)) 
                           : "<NULL GObject>");

    *rval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, str));
    g_free (str);

    return JS_TRUE;
}

static JSFunctionSpec gom_js_object_funcs[] = { 
    { "toString", gom_js_object_to_string, 0 },
    { NULL } 
};

static JSBool
gom_js_object_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_NOT_IMPLEMENTED;
    return JS_TRUE;
}

static gpointer
gom_js_object_init_once (gpointer data)
{
    g2js = g_hash_table_new (NULL, NULL);
    js2g = g_hash_table_new (NULL, NULL);
    return NULL;
}

JSObject *
gom_js_object_init_class (JSContext *cx, JSObject *obj)
{
    static GOnce init_once = G_ONCE_INIT;
    JSObject *proto;

    g_once (&init_once, gom_js_object_init_once, NULL);
    
    proto = JS_ConstructObject (cx, NULL, NULL, NULL);
    return JS_InitClass (cx, obj, proto, &GomJSObjectClass, gom_js_object_construct, 0,
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
        jsobj = JS_ConstructObject (cx, GOM_IS_JS_OBJECT (gobj) 
                                    ? gom_js_object_get_js_class (GOM_JS_OBJECT (gobj))
                                    : &GomJSObjectClass,
                                    NULL, NULL);
        gom_js_object_set_g_object (cx, jsobj, gobj);
    }

    return jsobj;
}
