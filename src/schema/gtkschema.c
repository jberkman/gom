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

#define ATTR ",\n        attribute %s { "
#define ATTR_BOOLEAN (g_print (ATTR"xsd:boolean }?", camel))

#if 1
#define ATTR_NUMERIC(name, pf, t)                                       \
    (g_print (ATTR"xsd:"name" { minInclusive = \""pf"\" maxInclusive = \""pf"\" } }?", \
              camel, ((t*)spec)->minimum, ((t*)spec)->maximum))
#else
#define ATTR_NUMERIC(name, pf, t)                                       \
    (g_print (ATTR"xsd:"name" }", camel))
#endif

#define ATTR_UNICHAR (g_print (ATTR"xsd:string { maxLength = \"1\" } }?", camel))

#define ATTR_TEXT (g_print (ATTR"text }?", camel))

static void
process_attribute (GParamSpec *spec)
{
    char *camel = camel_case (spec->name);

    if (G_IS_PARAM_SPEC_CHAR (spec)) {
        ATTR_NUMERIC ("byte", "%d", GParamSpecChar);
    } else if (G_IS_PARAM_SPEC_UCHAR (spec)) {
        ATTR_NUMERIC ("unsignedByte", "%u", GParamSpecUChar);
    } else if (G_IS_PARAM_SPEC_BOOLEAN (spec)) {
        ATTR_BOOLEAN;
    } else if (G_IS_PARAM_SPEC_INT (spec)) {
        ATTR_NUMERIC ("int", "%d", GParamSpecInt);
    } else if (G_IS_PARAM_SPEC_UINT (spec)) {
        ATTR_NUMERIC ("unsignedInt", "%u", GParamSpecUInt);
    } else if (G_IS_PARAM_SPEC_LONG (spec)) {
        ATTR_NUMERIC (
#if GLIB_SIZEOF_LONG == 8
            "long",
#else
            "int",
#endif
            "%ld", GParamSpecLong);
    } else if (G_IS_PARAM_SPEC_ULONG (spec)) {
        ATTR_NUMERIC (
#if GLIB_SIZEOF_LONG == 8
            "unsignedLong",
#else
            "unsignedInt",
#endif
            "%lu", GParamSpecLong);
    } else if (G_IS_PARAM_SPEC_INT64 (spec)) {
        ATTR_NUMERIC ("long", "%"G_GINT64_FORMAT, GParamSpecInt64);
    } else if (G_IS_PARAM_SPEC_UINT64 (spec)) {
        ATTR_NUMERIC ("unsignedLong", "%"G_GUINT64_FORMAT, GParamSpecUInt64);
    } else if (G_IS_PARAM_SPEC_FLOAT (spec)) {
        ATTR_NUMERIC ("float", "%f", GParamSpecFloat);
    } else if (G_IS_PARAM_SPEC_DOUBLE (spec)) {
        ATTR_NUMERIC ("double", "%f", GParamSpecDouble);
    } else if (G_IS_PARAM_SPEC_STRING (spec)) {
        ATTR_TEXT;
    } else if (G_IS_PARAM_SPEC_UNICHAR (spec)) {
        ATTR_UNICHAR;
#if 0
    } else {
        g_warning ("Skipping %s property %s.%s\n",
                   g_type_name (spec->value_type),
                   g_type_name (spec->owner_type),
                   camel);
#endif
    }
    g_free (camel);
}

static void
process_widget (GType type)
{
    guint i, n_items, *signal_ids;
    GParamSpec **specs;
    GObjectClass *oclass;
    char *camel;
    gboolean comma = TRUE;

    if (!type ||
        !G_TYPE_IS_CLASSED (type) ||
        !g_type_is_a (type, GTK_TYPE_WIDGET)) {
        return;
    }

    oclass = g_type_class_ref (type);

    specs = g_object_class_list_properties (oclass, &n_items);
    if (type == GTK_TYPE_WIDGET) {
        g_print ("%sAttrs = id?", g_type_name (type));
        comma = FALSE;
    } else {
        g_print ("%sAttrs = %sAttrs", g_type_name (type), g_type_name (g_type_parent (type)));
    }
    for (i = 0; i < n_items; i++) {
        if (specs[i]->owner_type == type) {
            process_attribute (specs[i]);
        }
    }

    g_print ("\n\n");

    if (type == GTK_TYPE_WIDGET) {
        g_print ("%sSignals = ", g_type_name (type));
    } else {
        g_print ("%sSignals = %sSignals", g_type_name (type), g_type_name (g_type_parent (type)));
    }
    signal_ids = g_signal_list_ids (type, &n_items);
    for (i = 0; i < n_items; i++) {
        camel = camel_case (g_signal_name (signal_ids[i]));
        g_print ("%s\n        attribute on%s { text }?", comma ? "," : "", camel);
        g_free (camel);
        comma = TRUE;
    }

    g_print ("\n\n%1$s = element %1$s { %1$sAttrs, %1$sSignals%2$s }\n\n",
             g_type_name (type),
             g_type_is_a (type, GTK_TYPE_CONTAINER) 
             ? ", widget*"
             : "");
}

int
main (int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    g_print ("default namespace = \"http://ilovegom.org/Gtk\"\n\n"
             "include \"gom.rnc\"\n\n"
             "toplevel   |= GtkWindow\n\n"
             "child       = element * { attribute * { text }*, mixed { script } }\n"
             "container   = element * { attribute * { text }*, widget* }\n"
             "widget      = child | container | script\n\n");

#define WIDGET(w) process_widget (w);
#include "gomwidgets.c"
#undef WIDGET

    return 0;
}
