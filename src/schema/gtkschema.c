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

#include <gtk/gtk.h>

#include <string.h>

static char *
camel_case (const char *s)
{
    int si, ri;
    char *r = g_malloc (strlen (s) + 1);
    gboolean upper = FALSE;
    for (ri = si = 0; s[si]; si++) {
        switch (s[si]) {
        case '-':
        case '_':
            upper = TRUE;
            break;
        default:
            if (upper) {
                r[ri++] = g_ascii_toupper (s[si]);
                upper = FALSE;
            } else {
                r[ri++] = s[si];
            }
            break;
        }
    }
    r[ri] = s[si];
    /*g_print ("%s:%d:%s: %s -> %s\n", __FILE__, __LINE__, __FUNCTION__, s, r);*/
    return r;
}

static gboolean comma = FALSE;

#define COMMA (comma ? ",\n        " : "")
#define ATTR "%sattribute %s { "
#define ATTR_BOOLEAN (g_print (ATTR"xsd:boolean }?", COMMA, camel))

#if 1
#define ATTR_NUMERIC(name, pf, t)                                       \
    (g_print (ATTR"xsd:"name" { minInclusive = \""pf"\" maxInclusive = \""pf"\" } }?", \
              COMMA, camel, ((t*)spec)->minimum, ((t*)spec)->maximum))
#else
#define ATTR_NUMERIC(name, pf, t)                                       \
    (g_print (ATTR"xsd:"name" }?", COMMA, camel))
#endif

#define ATTR_UNICHAR (g_print (ATTR"xsd:string { maxLength = \"1\" } }?", COMMA, camel))

#define ATTR_TEXT (g_print (ATTR"text }?", COMMA, camel))

static gboolean
process_attribute (GParamSpec *spec)
{
    char *camel = camel_case (spec->name);
    gboolean ret = TRUE;
    guint i;
    GEnumClass *eclass;

    if (G_IS_PARAM_SPEC_UNICHAR (spec)) {
        ATTR_UNICHAR;
    } else {
        switch (G_TYPE_FUNDAMENTAL (G_PARAM_SPEC_VALUE_TYPE (spec))) {
        case G_TYPE_CHAR:
            ATTR_NUMERIC ("byte", "%d", GParamSpecChar);
            break;
        case G_TYPE_UCHAR:
            ATTR_NUMERIC ("unsignedByte", "%u", GParamSpecUChar);
            break;
        case G_TYPE_BOOLEAN:
            ATTR_BOOLEAN;
            break;
        case G_TYPE_INT:
            ATTR_NUMERIC ("int", "%d", GParamSpecInt);
            break;
        case G_TYPE_UINT:
            ATTR_NUMERIC ("unsignedInt", "%u", GParamSpecUInt);
            break;
        case G_TYPE_LONG:
            ATTR_NUMERIC (
#if GLIB_SIZEOF_LONG == 8
                "long",
#else
                "int",
#endif
                "%ld", GParamSpecLong);
            break;
        case G_TYPE_ULONG:
            ATTR_NUMERIC (
#if GLIB_SIZEOF_LONG == 8
                "unsignedLong",
#else
                "unsignedInt",
#endif
                "%lu", GParamSpecLong);
            break;
        case G_TYPE_INT64:
            ATTR_NUMERIC ("long", "%"G_GINT64_FORMAT, GParamSpecInt64);
            break;
        case G_TYPE_UINT64:
            ATTR_NUMERIC ("unsignedLong", "%"G_GUINT64_FORMAT, GParamSpecUInt64);
            break;
        case G_TYPE_FLOAT:
            ATTR_NUMERIC ("float", "%f", GParamSpecFloat);
            break;
        case G_TYPE_DOUBLE:
            ATTR_NUMERIC ("double", "%f", GParamSpecDouble);
            break;
        case G_TYPE_FLAGS:
        case G_TYPE_STRING:
            ATTR_TEXT;
            break;
        case G_TYPE_ENUM:
            eclass = g_type_class_peek (G_PARAM_SPEC_VALUE_TYPE (spec));
            g_print (ATTR, COMMA, camel);
            for (i = 0; i < eclass->n_values; i++) {
                g_print ("%s\"%s\" ", (i == 0 ? "" : "| "), eclass->values[i].value_name);
            }
            g_print ("}?");
            break;
        default:
            ret = FALSE;
#if 0
            g_warning ("Skipping %s property %s.%s\n",
                       g_type_name (spec->value_type),
                       g_type_name (spec->owner_type),
                       camel);
#endif
            break;
        }
    }
    g_free (camel);
    return ret;
}

static void
process_properties (GType type, GObjectClass *oclass)
{
    guint i, n_properties;
    GParamSpec **specs;

    comma = TRUE;
    specs = g_object_class_list_properties (oclass, &n_properties);
    if (type == GTK_TYPE_WIDGET) {
        g_print ("%sAttrs = id?, GtkWidgetChildAttrs", g_type_name (type));
    } else {
        g_print ("%sAttrs = %sAttrs", g_type_name (type), g_type_name (g_type_parent (type)));
    }
    for (i = 0; i < n_properties; i++) {
        if (specs[i]->owner_type == type) {
            if (process_attribute (specs[i])) {
                comma = TRUE;
            }
        }
    }
    g_print ("\n\n");
    g_free (specs);
}

static GList *child_attrs = NULL;;

static void
process_child_properties (GType type, GObjectClass *oclass)
{
    guint i, n_properties;
    GParamSpec **specs;

    specs = gtk_container_class_list_child_properties (oclass, &n_properties);
    if (!specs || n_properties == 0) {
        return;
    }

    /* skip types that have parents who have child properties, but
     * that don't actually have any themselves */
    for (i = 0; i < n_properties; i++) {
        if (specs[i]->owner_type == type) {
            goto process_child_properties;
        }
    }
    goto free_specs;

process_child_properties:
    g_print ("%sChildAttrs = ", g_type_name (type));
    comma = FALSE;
    for (i = 0; i < n_properties; i++) {
        if (specs[i]->owner_type == type) {
            if (process_attribute (specs[i])) {
                comma = TRUE;
            }
        }
    }
    child_attrs = g_list_prepend (child_attrs, GSIZE_TO_POINTER (type));

free_specs:
    g_free (specs);
}

static void
process_signals (GType type)
{
    guint i, n_signals, *signal_ids;
    char *camel;

    comma = TRUE;
    if (type == GTK_TYPE_WIDGET) {
        g_print ("%sSignals = ", g_type_name (type));
        comma = FALSE;
    } else {
        g_print ("%sSignals = %sSignals", g_type_name (type), g_type_name (g_type_parent (type)));
    }
    signal_ids = g_signal_list_ids (type, &n_signals);
    for (i = 0; i < n_signals; i++) {
        camel = camel_case (g_signal_name (signal_ids[i]));
        g_print ("%sattribute on%s { text }?", COMMA, camel);
        g_free (camel);
        comma = TRUE;
    }
    g_free (signal_ids);
}

static void
process_widget (GType type)
{
    GObjectClass *oclass;

    if (!type ||
        !G_TYPE_IS_CLASSED (type) ||
        !g_type_is_a (type, GTK_TYPE_WIDGET)) {
        return;
    }

    oclass = g_type_class_ref (type);

    process_properties (type, oclass);

    if (g_type_is_a (type, GTK_TYPE_CONTAINER) && type != GTK_TYPE_CONTAINER) {
        process_child_properties (type, oclass);
    }

    process_signals (type);

    g_print ("\n\n%1$s = element %1$s { %1$sAttrs, %1$sSignals %2$s}\n",
             g_type_name (type),
             g_type_is_a (type, GTK_TYPE_CONTAINER) 
             ? ", (misc | widget)*"
             : ", misc*");
    if (type != GTK_TYPE_WIDGET) {
        g_print ("widget |= %s\n", g_type_name (type));
    }
    g_print ("\n");
}

int
main (int argc, char *argv[])
{
    GList *li;

    gtk_init (&argc, &argv);

    g_print ("default namespace = \"http://ilovegom.org/Gtk\"\n\n"
             "include \"gom.rnc\"\n\n"
             "toplevel   |= GtkWindow\n\n"
             "widget      = GtkWidget\n\n");

#define WIDGET(w) process_widget (w);
#include "gomwidgets.h"
#undef WIDGET

    if (child_attrs) {
        comma = FALSE;
        g_print ("GtkWidgetChildAttrs = ");
        for (li = child_attrs; li; li = li->next) {
            g_print ("%s%sChildAttrs", COMMA, g_type_name (GPOINTER_TO_SIZE (li->data)));
            comma = TRUE;
        }
        g_print ("\n");
    }

    return 0;
}
