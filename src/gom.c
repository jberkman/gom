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
#include <gom/gomjselement.h>
#include <gom/gomjsnode.h>
#include <gom/gomjsobject.h>
#include <gom/gomjswindow.h>
#include <gom/gomwidget.h>

#include <gtk/gtk.h>

static void
gom_error_reporter (JSContext *cx, const char *message, JSErrorReport *report)
{
    g_printerr ("JS Error: %s\n", message);
}

int
main (int argc, char *argv[])
{
    char *script;
    gsize scriptlen;
    GError *error = NULL;

    JSObject *jsdoc;
    GomDocument *doc;

    JSRuntime *rt; 
    JSContext *cx; 
    JSObject *window;

    gtk_init (&argc, &argv);

    if (argc < 2) {
        g_printerr ("Usage: gom <file>\n");
        return 1;
    }

    if (!g_file_get_contents (argv[1], &script, &scriptlen, &error)) {
        g_printerr ("Could not read %s: %s\n", argv[1], error->message);
        return 1;
    }

    gom_widget_init ();

    /* 
     * You always need: 
     *        a runtime per process, 
     *        a context per thread, 
     *        a global object per context, 
     *        standard classes (e.g. Date). 
     */ 
    rt = JS_NewRuntime(0x100000); 
    cx = JS_NewContext(rt, 0x1000); 

    window = gom_js_window_new_global (cx);

    JS_SetErrorReporter (cx, gom_error_reporter);

    gom_js_object_init_class (cx, window);
    gom_js_node_init_class (cx, window);
    gom_js_element_init_class (cx, window);
    gom_js_document_init_class (cx, window);

    jsdoc = JS_DefineObject (cx, window, "document", &GomJSDocumentClass, NULL,
                             JSPROP_PERMANENT | JSPROP_READONLY);
    doc = g_object_new (GOM_TYPE_DOC, NULL);
    gom_js_object_set_g_object (cx, jsdoc, doc);
    
    if (gom_doc_parse (doc, cx, jsdoc, argv[1], script, NULL)) {
        GomNode *node = gom_node_get_first_child (GOM_NODE (doc));
        if (node && GTK_IS_WIDGET (node)) {
            gtk_widget_show_all (GTK_WIDGET (node));
        }
        gtk_main ();
    }

    return 0;
}
