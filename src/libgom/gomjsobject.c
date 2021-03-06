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

#include "gom/gomjsobject.h"

#include "gom/gomcamel.h"

#include "gom/gomgcmanager.h"
#include "gom/gomjscontext.h"
#include "gom/gomobject.h"
#include "gom/gomvalue.h"

#include "gommacros.h"

#include <string.h>

GQuark gom_js_object_closures_quark (void);
GQuark gom_js_object_g2js_quark (void);
GQuark gom_js_object_js2g_quark (void);
GQuark gom_js_object_gt2jsc_quark (void);

GOM_DEFINE_QUARK (js_object_closures);
#define CLOSURES_QUARK (gom_js_object_closures_quark ())
#define CLOSURES(o) ((GHashTable *)g_object_get_qdata (G_OBJECT (o), CLOSURES_QUARK));

GOM_DEFINE_QUARK (js_object_g2js);
#define G2JS_QUARK (gom_js_object_g2js_quark ())
#define G2JS(cx) ((GHashTable *)GOM_JS_CONTEXT_GET_QDATA (cx, G2JS_QUARK))

GOM_DEFINE_QUARK (js_object_js2g);
#define JS2G_QUARK (gom_js_object_js2g_quark ())
#define JS2G(cx) ((GHashTable *)GOM_JS_CONTEXT_GET_QDATA (cx, JS2G_QUARK))

GOM_DEFINE_QUARK (js_object_gt2jsc);
#define GT2JSC_QUARK (gom_js_object_gt2jsc_quark ())
#define GT2JSC(cx) (*(GSList **)GOM_JS_CONTEXT_GET_QDATA (cx, GT2JSC_QUARK))

typedef struct {
    JSClass *jsclass;
    GType    type;
} ClassMap;

void
gom_js_object_register_js_class (JSContext *cx, GType objtype, JSClass *jsclass)
{
    ClassMap *map = g_new (ClassMap, 1);

    if (objtype != G_TYPE_OBJECT && !G_TYPE_IS_INTERFACE (objtype)) {
        g_warning (GOM_LOC ("registering non-interface %s"), g_type_name (objtype));
    }

    map->type    = objtype;
    map->jsclass = jsclass;

    GT2JSC (cx) = g_slist_prepend (GT2JSC (cx), map);
}

JSClass *
gom_js_object_get_js_class (JSContext *cx, gpointer gobj)
{
    GType type;
    GSList *li;
    ClassMap *map;

    type = G_TYPE_FROM_INSTANCE (gobj);
    for (li = GT2JSC (cx); li; li = li->next) {
        map = li->data;
        if (g_type_is_a (type, map->type)) {
#if 0
            g_print (GOM_LOC ("%s -> %s\n"), g_type_name (type), map->jsclass->name);
#endif
            return map->jsclass;
        }
#if 0
        g_print (GOM_LOC ("%s !> %s\n"), g_type_name (type), map->jsclass->name);
#endif
    }

    return NULL;
}

typedef struct {
    GClosure    closure;
    JSContext  *cx;
    JSObject   *obj;
    JSFunction *fun;
    JSObject   *fun_obj;
} GomJSClosure;

static void
gom_js_closure_finalize (gpointer data, GClosure *closure)
{
    GomJSClosure *jsclosure = (GomJSClosure *)closure;
    g_message(GOM_LOC ("finalizing a closure"));
    JS_RemoveRoot (jsclosure->cx, &jsclosure->obj);
    JS_RemoveRoot (jsclosure->cx, &jsclosure->fun_obj);
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
    jsclosure->fun_obj = JS_GetFunctionObject (fun);

    JS_AddNamedRoot (cx, &jsclosure->obj,     "JSClosure Object");
    JS_AddNamedRoot (cx, &jsclosure->fun_obj, "JSClosure Function");

    g_closure_set_marshal (closure, gom_js_closure_marshal);

    return closure;
}

gulong
gom_js_object_connect (JSContext *cx, JSObject *jsobj,
                       guint signal_id,
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

    return g_signal_connect_closure_by_id (gobj, signal_id, 0, closure, FALSE);
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
        g_object_set_qdata_full (gobj, CLOSURES_QUARK, closures, (GDestroyNotify)g_hash_table_destroy);
    }
    g_hash_table_insert (closures, GUINT_TO_POINTER (signal_id), closure);
}

gboolean
gom_js_object_resolve (JSContext *cx, JSObject *obj, const char *name, 
                       GObject **gobj, GParamSpec **spec, guint *signal_id)
{
    *gobj = gom_js_object_get_g_object (cx, obj);
    if (!*gobj) {
#if 0
        g_print ("resolve %s.%s -> FAIL (no GObject)\n", JS_GET_CLASS (cx, obj)->name, name);
#endif
        return FALSE;
    }
    g_assert (G_IS_OBJECT (*gobj));
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

#if 0
    g_print (GOM_LOC (""));
#endif
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
    g_object_get_property (gobj, spec->name, &gval);
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

#if 0
    g_print (GOM_LOC (""));
#endif
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

    g_object_set_property (gobj, spec->name, &gval);
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

#if 0
    g_print (GOM_LOC (""));
#endif
    if (!gom_js_object_resolve (cx, *objp, name, &gobj, &spec, &signal_id)) {
        *objp = NULL;
        return JS_TRUE;
    }

    if (!JS_DefineProperty (cx, *objp, name, JSVAL_VOID,
                            gom_js_object_get_prop, gom_js_object_set_prop, 
                            spec ? JSPROP_ENUMERATE | JSPROP_PERMANENT : 0)) {
        g_printerr ("Could not define a property for %s\n", name);
        return JS_FALSE;
    }

#if 0
    g_print (GOM_LOC ("defined new property: %s.%s (%s)\n"), 
              JS_GET_CLASS (cx, *objp)->name, name, JS_GET_CLASS (cx, obj)->name);
#endif
    
    return JS_TRUE;
}

static void
gom_js_object_finalize (JSContext *cx, JSObject *obj)
{
    GObject *gobj = gom_js_object_get_g_object (cx, obj);
    g_print ("Finalizing JSObject %s %p: unref %s %p -> refcnt %d\n",
             JS_GET_CLASS (cx, obj)->name, obj,
             gobj ? g_type_name (G_TYPE_FROM_INSTANCE (gobj)) : "<NULL>",
             gobj,
#if 0
             gobj ? gobj->ref_count :
#endif
             0);
    if (gobj) {
        g_assert (G_IS_OBJECT (gobj));
        g_hash_table_remove (G2JS (cx), gobj);
        g_hash_table_remove (JS2G (cx), obj);
        g_object_unref (gobj);
    }
}

typedef struct {
    GParamSpec  **specs;
    guint         n_items;
    guint         i;
} EnumerateData;

JSBool
gom_js_object_enumerate (JSContext *cx, JSObject *obj, JSIterateOp enum_op, jsval *statep, jsid *idp)
{
    GObject *gobj;
    EnumerateData *data;
    GParamSpec *spec;
    const char *name;
    JSString *jsname;
    
    gobj = gom_js_object_get_g_object (cx, obj);
#if 0
    g_print (GOM_LOC ("%s %d -> %p\n"),
              JS_GET_CLASS (cx, obj)->name, enum_op, gobj);
#endif
    switch (enum_op) {
    case JSENUMERATE_INIT:
        if (!gobj) {
            *statep = JSVAL_NULL;
            return JS_TRUE;
        }
        data = g_new0 (EnumerateData, 1);
        data->specs = g_object_class_list_properties (G_OBJECT_GET_CLASS (gobj), &data->n_items);

        *statep = PRIVATE_TO_JSVAL (data);
        if (idp) {
            *idp = JSVAL_ZERO;
        }
        break;
    case JSENUMERATE_NEXT:
        g_assert (G_IS_OBJECT (gobj));
        data = JSVAL_TO_PRIVATE (*statep);

        if (data->i < data->n_items) {
            spec = data->specs[data->i++];
            name = gom_camel_case (spec->name);
#if 0
            g_print ("enumerating %s.%s\n", g_type_name (G_TYPE_FROM_INSTANCE (gobj)), name);
#endif
            jsname = JS_NewStringCopyZ (cx, name);
            GOM_CAMEL_FREE (name, spec->name);
            if (!JS_ValueToId (cx, STRING_TO_JSVAL (jsname), idp)) {
                return JS_FALSE;
            }
            break;
        }
        *statep = JSVAL_NULL;
        break;
    case JSENUMERATE_DESTROY:
        data = JSVAL_TO_PRIVATE (*statep);
        g_free (data->specs);
        g_free (data);
        break;
    }
    return JS_TRUE;
}
                         

JSClass GomJSObjectClass = {
    "GomObject",
    JSCLASS_NEW_RESOLVE | JSCLASS_NEW_RESOLVE_GETS_START | JSCLASS_NEW_ENUMERATE,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    (JSEnumerateOp)gom_js_object_enumerate,
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
    return JS_TRUE;
}

static void
g2js_warn (gpointer key, gpointer value, gpointer user_data)
{
    g_print ("\tGObject %p %s -> JSObject %p %s\n",
	     key, g_type_name (G_TYPE_FROM_INSTANCE (key)),
	     value, JS_GET_CLASS (NULL, value)->name);
}

static void
g2js_free (gpointer data)
{
    GHashTable *g2js = data;
    g_message (GOM_LOC ("GObject to JSObject table leaked %d objects"),
	        g_hash_table_size (g2js));
    g_hash_table_foreach (g2js, g2js_warn, NULL);
    g_hash_table_destroy (g2js);
}


static void
js2g_warn (gpointer key, gpointer value, gpointer user_data)
{
    g_printerr ("\tJSObject %p %s -> GObject %p %s\n",
		key, JS_GET_CLASS (NULL, key)->name,
		value, g_type_name (G_TYPE_FROM_INSTANCE (value)));
}

static void
js2g_free (gpointer data)
{
    GHashTable *js2g = data;
    g_message (GOM_LOC ("JSObject to GObject table leaked %d objects"),
	        g_hash_table_size (js2g));
    g_hash_table_foreach (js2g, js2g_warn, NULL);
    g_hash_table_destroy (js2g);
}

static void
g2jsc_free (gpointer data)
{
    GSList *slist = *(GSList **)data;
    g_slist_foreach (slist, (GFunc)g_free, NULL);
    g_slist_free (slist);
    g_free (data);
}

JSObject *
gom_js_object_init_class (JSContext *cx, JSObject *obj)
{
    GOM_JS_CONTEXT_SET_QDATA_FULL (cx, G2JS_QUARK,   g_hash_table_new (NULL, NULL), g2js_free); // (GDestroyNotify)g_hash_table_destroy);
    GOM_JS_CONTEXT_SET_QDATA_FULL (cx, JS2G_QUARK,   g_hash_table_new (NULL, NULL), js2g_free); // (GDestroyNotify)g_hash_table_destroy);
    GOM_JS_CONTEXT_SET_QDATA_FULL (cx, GT2JSC_QUARK, g_new0 (GSList *, 1), g2jsc_free);

    gom_js_object_register_js_class (cx, G_TYPE_OBJECT, &GomJSObjectClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, NULL, NULL, NULL),
                         &GomJSObjectClass, gom_js_object_construct, 0,
                         gom_js_object_props, gom_js_object_funcs, 
                         NULL, NULL);
}

void
gom_js_object_set_g_object (JSContext *cx, JSObject *jsobj, gpointer gobject)
{
#if 0
    g_print ("Binding %s %p to JSObject %p\n",
             g_type_name (G_TYPE_FROM_INSTANCE (gobject)),
             gobject, jsobj);
#endif
    g_hash_table_insert (JS2G (cx), jsobj, g_object_ref (gobject));
    g_hash_table_insert (G2JS (cx), gobject, jsobj);
    if (GOM_IS_GC_MANAGED (gobject)) {
        gom_gc_manager_manage_object (cx, gobject);
    }
}

gpointer
gom_js_object_get_g_object (JSContext *cx, JSObject *jsobj)
{
    return g_hash_table_lookup (JS2G (cx), jsobj);
}

JSObject *
gom_js_object_get_js_object (JSContext *cx, gpointer gobj)
{
    return g_hash_table_lookup (G2JS (cx), gobj);
}

JSObject *
gom_js_object_get_or_create_js_object (JSContext *cx, gpointer gobj)
{
    JSObject *jsobj;
    jsobj = gom_js_object_get_js_object (cx, gobj);

    if (!jsobj) {
        jsobj = JS_ConstructObject (cx, gom_js_object_get_js_class (cx, gobj), NULL, NULL);
        if (jsobj) {
            gom_js_object_set_g_object (cx, jsobj, gobj);
        }
    }

    return jsobj;
}

static JSBool
gom_js_object_get_enum (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    *vp = id;
    return JS_TRUE;
}

JSObject *
gom_js_object_define_static_enums (JSContext *cx, JSObject *proto, GType enum_type)
{
    GEnumClass *enums;
    JSObject *ctor;
    guint i;

    ctor = JS_GetConstructor (cx, proto);

    enums = g_type_class_ref (enum_type);
    for (i = 0; i < enums->n_values; i++) {
        JS_DefinePropertyWithTinyId (cx, ctor,
                                     &enums->values[i].value_name[4],
                                     enums->values[i].value,
                                     INT_TO_JSVAL(enums->values[i].value),
                                     gom_js_object_get_enum, NULL,
                                     JSPROP_READONLY | JSPROP_PERMANENT);
    }
    return proto;
}
