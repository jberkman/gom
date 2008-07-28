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
#ifndef GOM_URI_UTILS_H
#define GOM_URI_UTILS_H

#include <glib/gerror.h>

G_BEGIN_DECLS

#define GOM_NSRESULT_EXCEPTION_ERROR (gom_nsresult_exception_error_quark ())

char *gom_uri_join (const char *uri, const char *base_uri, GError **error);

gboolean gom_uri_get_contents (const char  *uri,
                               const char  *base_uri,
                               char       **contents,
                               gsize       *length,
                               GError     **error);

GQuark gom_nsresult_exception_error_quark (void);

G_END_DECLS

#endif /* GOM_URI_UTILS_H */
