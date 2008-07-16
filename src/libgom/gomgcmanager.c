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

#ifdef GOM_USE_GC_MANAGER
#include "gom/gomgcmanager.h"

#include "gom/gomjscontext.h"
#include "gom/gomjsobject.h"

#include "gommacros.h"

GQuark gom_gc_manager_objects_quark (void);

GOM_DEFINE_QUARK (gc_manager_objects)
#define OBJECTS_QUARK (gom_gc_manager_objects_quark ())
#define OBJECTS(cx) (*(GSList **)GOM_JS_CONTEXT_GET_QDATA(cx, OBJECTS_QUARK))

static JSBool
gom_gc_manager_run (JSContext *cx, JSGCStatus status)
{
    GSList *li;
    if (status != JSGC_MARK_END) {
        return JS_TRUE;
    }

    if (!GOM_JS_CONTEXT_GET_QDATA (cx, OBJECTS_QUARK)) {
        return JS_TRUE;
    }

    g_message (G_STRLOC": Marking live objects...");
    for (li = OBJECTS(cx); li; li = li->next) {
        g_assert (GOM_IS_GC_MANAGED (li->data));
        gom_gc_managed_mark_live_objects (
            GOM_GC_MANAGED (li->data), cx, status);
    }
    g_message (G_STRLOC": done.");
    return JS_TRUE;
}

void
gom_gc_manager_manage_runtime (JSRuntime *runtime)
{
    JS_SetGCCallbackRT (runtime, gom_gc_manager_run);
}

static void
gom_gc_manager_unregister (gpointer cx, GObject *obj)
{
    OBJECTS (cx) = g_slist_remove (OBJECTS (cx), obj);
}

static void
free_objects (gpointer data)
{
    GSList *slist = *(GSList **)data;
    g_slist_free (slist);
    g_free (data);
}

void
gom_gc_manager_manage_object (JSContext *cx, GomGCManaged *obj)
{
    g_return_if_fail (GOM_IS_GC_MANAGED (obj));
    if (!GOM_JS_CONTEXT_GET_QDATA (cx, OBJECTS_QUARK)) {
        GOM_JS_CONTEXT_SET_QDATA_FULL (cx, OBJECTS_QUARK, g_new0 (GSList *, 1), free_objects);
    }
    OBJECTS (cx) = g_slist_prepend (OBJECTS (cx), obj);
    g_object_weak_ref (G_OBJECT (obj), gom_gc_manager_unregister, cx);
}

void
gom_gc_manager_mark_live_objects (GomGCManaged *managed,
                                  JSContext    *cx,
                                  JSGCStatus    status)
{
    JSObject *jsobj;

    jsobj = gom_js_object_get_js_object (cx, managed);
    if (!jsobj) {
        g_warning(G_STRLOC": could not get a js object while marking live objects for %p",
                  managed);
    } else if (!JS_IsAboutToBeFinalized (cx, jsobj)) {
        g_message (G_STRLOC": Marking a %s live...",
                   g_type_name (G_TYPE_FROM_INSTANCE (managed)));
        JS_MarkGCThing (cx, jsobj, g_type_name (G_TYPE_FROM_INSTANCE (managed)), NULL);
    }
}

GOM_DEFINE_INTERFACE (GomGCManaged, gom_gc_managed, {});

GOM_STUB_VOID(GOM_GC_MANAGED, gom_gc_managed, mark_live_objects,
              (GomGCManaged *gom_gc_managed, JSContext *cx, JSGCStatus status),
              (gom_gc_managed, cx, status));
#endif /* GOM_USE_GC_MANAGER */
