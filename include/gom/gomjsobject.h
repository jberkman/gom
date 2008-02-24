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
#ifndef GOM_JS_OBJECT_H
#define GOM_JS_OBJECT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomJSObject GomJSObject; /* dummy object */
typedef struct _GomJSObjectInterface GomJSObjectInterface;

G_END_DECLS

#include <jsapi.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_JS_OBJECT             (gom_js_object_get_type ())
#define GOM_JS_OBJECT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_JS_OBJECT, GomJSObject))
#define GOM_IS_JS_OBJECT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_JS_OBJECT))
#define GOM_JS_OBJECT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_JS_OBJECT, GomJSObjectInterface))

struct _GomJSObjectInterface {
    GTypeInterface parent;

    JSClass *(*get_js_class) (GomJSObject *obj);
};

GType gom_js_object_get_type (void);

#define GOM_DEFINE_JS_OBJECT(prefix, klass)                             \
    static JSClass *                                                    \
    prefix##_js_object_get_js_class (GomJSObject *obj)                  \
    {                                                                   \
        return &klass;                                                   \
    }                                                                   \
                                                                        \
    static void                                                         \
    prefix##_js_object_init (gpointer g_iface, gpointer iface_data)     \
    {                                                                   \
        GomJSObjectInterface *jsobj = (GomJSObjectInterface *)g_iface;  \
                                                                        \
        jsobj->get_js_class = prefix##_js_object_get_js_class;          \
    }

JSClass *gom_js_object_get_js_class (GomJSObject *obj);

extern JSClass GomJSObjectClass;

JSObject *gom_js_object_init_class (JSContext *cx, JSObject *obj);

void      gom_js_object_set_g_object  (JSContext *cx, JSObject *jsobj, gpointer gobject);
gpointer  gom_js_object_get_g_object  (JSContext *cx, JSObject *jsobj);
JSObject *gom_js_object_get_js_object (gpointer   gobj);
JSObject *gom_js_object_get_or_create_js_object (JSContext *cx, gpointer gobj);

gboolean  gom_js_object_resolve (JSContext *cx, JSObject *obj, const char *name, 
                                 GObject **gobj, GParamSpec **spec, guint *signal);

gulong    gom_js_object_connect (JSContext *cx, JSObject *jsobj,
                                 const char *signal, JSFunction *func);
                                 

G_END_DECLS

#endif /* GOM_JS_H */
