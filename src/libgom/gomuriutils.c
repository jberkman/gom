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

#include "gom/gomuriutils.h"
#include "gommacros.h"

#include <gio/gio.h>

#include <string.h>

static GFile *
get_relative (const char *uri, const char *base_uri)
{
    GFile *base;
    GFile *rel;

    if (!base_uri) {
        return g_file_new_for_uri (uri);
    }

    /* g_file_resolve_relative_path() is totally daft; it treats
     * base_uri as a directory, but g_file_new_for_uri() forgets the
     * trailing '/' so we can't always use _get_parent().
     */
    base = g_file_new_for_uri (base_uri);
    if (base_uri[strlen (base_uri) - 1] != '/') {
        GFile *parent;
        parent = g_file_get_parent (base);
        g_object_unref (base);
        base = parent;
    }
    rel = g_file_resolve_relative_path (base, uri);
    g_object_unref (base);

    return rel;
}    

char *
gom_uri_join (const char *uri, const char *base_uri, GError **error)
{
    GFile *rel;
    char *ret;

    rel = get_relative (uri, base_uri);
    ret = g_file_get_uri (rel);
    g_object_unref (rel);

    return ret;
}

gboolean
gom_uri_get_contents (const char  *uri,
                      const char  *base_uri,
                      char       **contents,
                      gsize       *length,
                      GError     **error)
{
    GFile *rel;
    gboolean ret;

    rel = get_relative (uri, base_uri);
    {
        char *s = g_file_get_uri (rel);
        g_print (GOM_LOC ("URI: %s\n"), s);
        g_free (s);
    }

    ret = g_file_load_contents (rel, NULL, contents, length, NULL, error);
    g_object_unref (rel);

    return ret;
}

