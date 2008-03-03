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
#include <jsapi.h>

#define GOM_OBJECT_ATTRS_QUARK (attrs_quark ())
#define ATTRS(o) ((GHashTable *)g_object_get_qdata (G_OBJECT (o), GOM_OBJECT_ATTRS_QUARK));

static gpointer
attrs_quark_once (gpointer data)
{
    return GUINT_TO_POINTER (g_quark_from_static_string ("gom-object-attrs-quark"));
}

static GQuark
attrs_quark (void)
{
    static GOnce attrs_once = G_ONCE_INIT;
    return GPOINTER_TO_UINT (g_once (&attrs_once, attrs_quark_once, NULL));
}

gboolean
gom_object_resolve (GObject *gobj, const char *name, GParamSpec **spec, guint *signal_id)
{
    if (name[0] == 'o' && name[1] == 'n' &&
        (*signal_id = g_signal_lookup (&name[2], G_TYPE_FROM_INSTANCE (gobj)))) {
        g_print ("resolve %s.%s -> signal %u\n", g_type_name (G_TYPE_FROM_INSTANCE (gobj)), name, *signal_id);
        *spec = NULL;
        return TRUE;
    }

    *spec = g_object_class_find_property (G_OBJECT_GET_CLASS (gobj), name);
    *signal_id = 0;
    g_print ("resolve %s.%s -> %s\n", 
             g_type_name (G_TYPE_FROM_INSTANCE (gobj)), name,
             *spec ? g_type_name (G_PARAM_SPEC_VALUE_TYPE (*spec)) : "FAIL");

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
        g_object_set_qdata_full (obj, GOM_OBJECT_ATTRS_QUARK, attrs, (GDestroyNotify)g_hash_table_destroy);
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