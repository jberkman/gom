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

#include <gom/gomdoc.h>
#include <gom/gomjsdocument.h>
#include <gom/gomjsobject.h>
#include <gom/gomjscontext.h>
#include <gom/gomwidget.h>

#include <gtk/gtk.h>

static void
gom_error_reporter (JSContext *cx, const char *message, JSErrorReport *report)
{
    g_printerr ("JS Error: %s\n", message);
}

typedef struct {
    JSContext   *cx;
    JSObject    *window;
    const char  *filename;
} MainData;

static gboolean
parse_idle (gpointer data)
{
    MainData *d = data;
    GError *error = NULL;
    GomDocument *doc;
    JSObject *jsdoc;

    doc   = g_object_new (GOM_TYPE_DOC, NULL);
    jsdoc = gom_js_object_get_or_create_js_object (d->cx, doc);
    
    JS_DefineProperty (d->cx, d->window, "document",
                       OBJECT_TO_JSVAL (jsdoc), NULL, NULL,
                       JSPROP_PERMANENT | JSPROP_READONLY);

    if (!gom_doc_parse_file (doc, d->cx, d->window, d->filename, &error)) {
        g_printerr ("Error parsing %s: %s\n", d->filename, error->message);
        gtk_main_quit ();
    }

    return FALSE;
}

int
main (int argc, char *argv[])
{
    MainData d = { NULL };
    GObject *cxpriv;
    JSRuntime *rt; 

    gtk_init (&argc, &argv);

    if (argc < 2) {
        g_printerr ("Usage: gom <file.gom>\n");
        return 1;
    }

    d.filename = argv[1];

    gom_widget_init ();

    rt = JS_NewRuntime(0x100000); 
    d.cx = JS_NewContext(rt, 0x1000); 

    JS_SetErrorReporter (d.cx, gom_error_reporter);

    d.window = gom_js_context_init (d.cx);

    if (!d.window) {
        g_printerr ("Could not initialize the window object.\n");
        return 1;
    }

    /* parse the file in an idle so that scripts can quit() */
    g_idle_add (parse_idle, &d);
    gtk_main ();

    cxpriv = GOM_JS_CONTEXT_PRIV (d.cx);
    JS_DestroyContext (d.cx);
    g_object_unref (cxpriv);
    JS_DestroyRuntime (rt);
    JS_ShutDown ();

    return 0;
}
