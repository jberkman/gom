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
#ifndef GOM_VALUE_H
#define GOM_VALUE_H

#include <glib-object.h>
#include <jsapi.h>

G_BEGIN_DECLS

#define GOM_VALUE_ERROR (gom_value_error_quark ())

typedef enum {
    GOM_VALUE_ERROR_UNKNOWN_JSVAL_TAG = 1,
    GOM_VALUE_ERROR_G_OBJECT_NOT_FOUND,
    GOM_VALUE_ERROR_UNHANDLED_G_TYPE,
    GOM_VALUE_ERROR_JS_OBJECT_NOT_FOUND,
} GomValueError;

GQuark gom_value_error_quark (void);

gboolean gom_g_value (JSContext *cx, GValue *gval, jsval   jval, GError **error);
gboolean gom_jsval   (JSContext *cx, jsval  *jval, GValue *gval, GError **error);

G_END_DECLS

#endif /* GOM_VALUE_H */
