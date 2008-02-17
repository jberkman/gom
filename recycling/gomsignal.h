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
#ifndef GOM_SIGNAL_H
#define GOM_SIGNAL_H

#include <glib.h>
#include <jsapi.h>

G_BEGIN_DECLS

#define GOM_SIGNAL_ERROR (gom_signal_error_quark ())
typedef enum {
    GOM_SIGNAL_ERROR_MISSING_CNAME_ATTRIBUTE = 1,
    GOM_SIGNAL_ERROR_MISSING_TYPE_ATTRIBUTE,
    GOM_SIGNAL_ERROR_SIGNAL_NOT_FOUND,
    GOM_SIGNAL_ERROR_POINTERS_TO_POINTERS_UNSUPPORTED
} GomSignalError;

GQuark gom_signal_error_quark (void);

gboolean gom_signal_init (GError **error);

gulong gom_signal_connect_script (JSContext *cx, JSObject *obj,
                                  const char *signal,
                                  const char *script, gsize scriptlen,
                                  GError **error);

G_END_DECLS

#endif /* GOM_SIGNAL_H */
