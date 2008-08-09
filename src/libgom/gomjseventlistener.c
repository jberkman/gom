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

#include "gom/dom/gomeventlistener.h"

#include "gom/gomjseventlistener.h"

#include "gom/dom/gomeventtarget.h"
#include "gom/gomgcmanaged.h"
#include "gom/gomjsobject.h"

#include "gommacros.h"

#include <jsapi.h>

enum {
    PROP_JS_CONTEXT = 1,
    PROP_JS_OBJECT
};

typedef struct {
    JSContext *cx;
    JSObject  *obj;
} GomJSEventListenerPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_JS_EVENT_LISTENER, GomJSEventListenerPrivate)

static void
gom_js_event_listener_set_property (GObject      *object,
                                    guint         property_id,
                                    const GValue *value,
                                    GParamSpec   *pspec)
{
    GomJSEventListenerPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_JS_CONTEXT:
        if (priv->cx) {
            g_warning (GOM_LOC ("Context is already set on %p"), object);
        } else {
            priv->cx = g_value_get_pointer (value);
        }
        break;
    case PROP_JS_OBJECT:
        if (priv->obj) {
            g_warning (GOM_LOC ("Object is already set on %p"), object);
        } else if (!priv->cx) {
            g_warning (GOM_LOC ("Context needs to be set on %p before object"), object);
        }
	priv->obj = g_value_get_pointer (value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_js_event_listener_handle_event (GomEventListener *listener,
                                    GomEvent         *evt)
{
    GomJSEventListenerPrivate *priv = PRIV (listener);
    GomEventTarget *target;
    JSObject *obj;
    jsval argv, rval, fval;

    if (!priv->cx) {
        g_warning (GOM_LOC ("%p.cx is NULL\n"), listener);
        return;
    }
    if (!priv->obj) {
        g_warning (GOM_LOC ("%p.obj is NULL\n"), listener);
        return;
    }

    /* this reference we are "giving" to JS below */
    argv = OBJECT_TO_JSVAL (gom_js_object_get_or_create_js_object (priv->cx, evt));

    if (JS_ObjectIsFunction (priv->cx, priv->obj)) {
#if 0
        JSString *str;
#endif

        fval = OBJECT_TO_JSVAL (priv->obj);
#if 0
        str = JS_ValueToString (priv->cx, fval);

        g_print ("handleEvent(): %s\n", JS_GetStringBytes (str));
#endif

        g_object_get (evt, "target", &target, NULL);
        obj = gom_js_object_get_or_create_js_object (priv->cx, target);
        g_object_unref (target);

        JS_CallFunctionValue (priv->cx, obj, fval, 1, &argv, &rval);
    } else {
        JS_CallFunctionName (priv->cx, priv->obj, "handleEvent", 1, &argv, &rval);
    }
}

static void
gom_js_event_listener_mark_live_objects (GomGCManaged *managed,
					 JSContext    *cx,
					 JSGCStatus    status)
{
    GomJSEventListenerPrivate *priv = PRIV (managed);

    g_assert (priv->cx == cx);

    if (!JS_IsAboutToBeFinalized (cx, priv->obj)) {
        g_message (GOM_LOC ("Marking a %s live..."), JS_GET_CLASS (cx, priv->obj)->name);
        JS_MarkGCThing (cx, priv->obj, g_type_name (G_TYPE_FROM_INSTANCE (managed)), NULL);
    }
}

GOM_IMPLEMENT (EVENT_LISTENER, event_listener, gom_js_event_listener);
GOM_IMPLEMENT (GC_MANAGED, gc_managed, gom_js_event_listener);

G_DEFINE_TYPE_WITH_CODE (GomJSEventListener, gom_js_event_listener, G_TYPE_OBJECT,
                         {
			     GOM_IMPLEMENT_INTERFACE (EVENT_LISTENER, event_listener, gom_js_event_listener);
			     GOM_IMPLEMENT_INTERFACE (GC_MANAGED, gc_managed, gom_js_event_listener);
			 });

static void gom_js_event_listener_init (GomJSEventListener *listener) { }

static void
gom_js_event_listener_class_init (GomJSEventListenerClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomJSEventListenerPrivate));
    
    oclass->set_property = gom_js_event_listener_set_property;

    g_object_class_install_property (
        oclass, PROP_JS_CONTEXT,
        g_param_spec_pointer ("js-context", NULL,
                              "The JSContext associated with the JSObject for this object",
                              G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_property (
        oclass, PROP_JS_OBJECT,
        g_param_spec_pointer ("js-object", NULL,
                              "The JSObject associated with this object",
                              G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_STATIC_STRINGS));
}
