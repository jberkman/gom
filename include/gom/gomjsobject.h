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

#include <jsapi.h>
#include <glib-object.h>

G_BEGIN_DECLS

void     gom_js_object_register_js_class (JSContext *cx, GType objtype, JSClass *jsclass);
JSClass *gom_js_object_get_js_class      (JSContext *cx, gpointer gobj);

extern JSClass GomJSObjectClass;

JSObject *gom_js_object_init_class (JSContext *cx, JSObject *obj);

void      gom_js_object_set_g_object  (JSContext *cx, JSObject *jsobj, gpointer gobj);
gpointer  gom_js_object_get_g_object  (JSContext *cx, JSObject *jsobj);
JSObject *gom_js_object_get_js_object (JSContext *cx, gpointer  gobj);
JSObject *gom_js_object_get_or_create_js_object (JSContext *cx, gpointer gobj);

gboolean  gom_js_object_resolve (JSContext *cx, JSObject *obj, const char *name, 
                                 GObject **gobj, GParamSpec **spec, guint *signal);

gulong    gom_js_object_connect (JSContext *cx,   JSObject   *jsobj,
                                 guint signal_id, JSFunction *func);

JSBool    gom_js_object_enumerate (JSContext   *cx, 
                                   JSObject    *obj,
                                   JSIterateOp  enum_op,
                                   jsval       *statep,
                                   jsid        *idp);

JSObject *gom_js_object_define_static_enums (JSContext *cx, JSObject *proto, GType enum_type);

G_END_DECLS

#endif /* GOM_JS_H */
