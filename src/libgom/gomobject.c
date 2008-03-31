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

#include <gom/gomobject.h>

#include <gommacros.h>

#include <string.h>

GQuark gom_object_attrs_quark (void);

GOM_DEFINE_QUARK (object_attrs);
#define ATTRS_QUARK (gom_object_attrs_quark ())
#define ATTRS(o) ((GHashTable *)g_object_get_qdata (G_OBJECT (o), ATTRS_QUARK));

static char *
camel_uncase (const char *s)
{
    int si, ri;
    char *r = g_malloc (2 * strlen (s) + 1);
    for (ri = si = 0; s[si]; si++) {
        if (g_ascii_isupper (s[si])) {
            r[ri++] = '_';
            r[ri++] = g_ascii_tolower (s[si]);
        } else {
            r[ri++] = s[si];
        }
    }
    r[ri] = s[si];
#if 0
    g_print ("%s:%d:%s: %s -> %s\n", __FILE__, __LINE__, __FUNCTION__, s, r);
#endif
    return r;
}

gboolean
gom_object_resolve (GObject *gobj, const char *name, GParamSpec **spec, guint *signal_id)
{
    char *n;
    n = (char *)name;

resolve_again:
    if (n[0] == 'o' && n[1] == 'n' &&
        (*signal_id = g_signal_lookup (&n[2], G_TYPE_FROM_INSTANCE (gobj)))) {
        g_print ("resolve %s.%s -> signal %u\n", g_type_name (G_TYPE_FROM_INSTANCE (gobj)), n, *signal_id);
        *spec = NULL;
        if (n != name) {
            g_free (n);
        }
        return TRUE;
    }

    *spec = g_object_class_find_property (G_OBJECT_GET_CLASS (gobj), n);
    if (!*spec && n == name) {
        n = camel_uncase (name);
        goto resolve_again;
    }
    *signal_id = 0;
    g_print ("resolve %s.%s -> %s\n", 
             g_type_name (G_TYPE_FROM_INSTANCE (gobj)), n,
             *spec ? g_type_name (G_PARAM_SPEC_VALUE_TYPE (*spec)) : "FAIL");

    if (n != name) {
        g_free (n);
    }

    return *spec != NULL;
}

GValue *
gom_object_get_attribute (GObject *obj, const char *name)
{
    GHashTable *attrs = ATTRS (obj);
    return attrs ? (GValue *)g_hash_table_lookup (attrs, name) : NULL;
}

static void
free_value (gpointer data)
{
    GValue *value = data;
    g_value_unset (value);
    g_free (value);
}

void
gom_object_set_attribute (GObject *obj, const char *name, GValue *value)
{
    GHashTable *attrs = ATTRS (obj);
    GValue *newval;
    if (!attrs) {
        attrs = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, free_value);
        g_object_set_qdata_full (obj, ATTRS_QUARK, attrs, (GDestroyNotify)g_hash_table_destroy);
    }
    newval = g_new0 (GValue, 1);
    g_value_init (newval, G_VALUE_TYPE (value));
    g_value_copy (value, newval);
    g_hash_table_insert (attrs, g_strdup (name), newval);
}

void
gom_object_attributes_foreach (GObject *obj, GHFunc func, gpointer user_data)
{
    GHashTable *attrs = ATTRS (obj);
    if (attrs) {
        g_hash_table_foreach (attrs, func, user_data);
    }
}
