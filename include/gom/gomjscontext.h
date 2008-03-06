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
#ifndef GOM_JS_CONTEXT_H
#define GOM_JS_CONTEXT_H

#include <glib/gmacros.h>
#include <glib/gquark.h>
#include <jsapi.h>

G_BEGIN_DECLS

JSObject *gom_js_context_init (JSContext *cx);

#define GOM_JS_CONTEXT_PRIV(cx) (G_OBJECT (JS_GetContextPrivate ((cx))))
#define GOM_JS_CONTEXT_GET_QDATA(cx, quark) \
    (g_object_get_qdata (GOM_JS_CONTEXT_PRIV (cx), (quark)))
#define GOM_JS_CONTEXT_SET_QDATA_FULL(cx, quark, data, destroy) \
    (g_object_set_qdata_full (GOM_JS_CONTEXT_PRIV (cx), (quark), (data), (destroy)))

G_END_DECLS

#endif /* GOM_JS_CONTEXT_H */
