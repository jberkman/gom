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

#include "gom/gomjsxmlhttprequest.h"

#include "gom/dom/gomdomexception.h"
#include "gom/dom/gomxmlhttprequest.h"
#include "gom/gomjseventtarget.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsobject.h"
#include "gom/gomxhr.h"

#include "gommacros.h"

#include <glib.h>

static void
gom_js_xml_http_request_finalize (JSContext *cx, JSObject *obj)
{
    GomJSEventTargetClass.finalize (cx, obj);
}

JSClass GomJSGomXMLHttpRequestClass = {
    "XMLHttpRequest", JSCLASS_NEW_ENUMERATE,

    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    JS_PropertyStub,
    (JSEnumerateOp)gom_js_object_enumerate,
    JS_ResolveStub,
    JS_ConvertStub,
    gom_js_xml_http_request_finalize
};

static JSPropertySpec gom_js_xml_http_request_props[] = { { NULL } };

static JSBool
gom_js_xml_http_request_open (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomXMLHttpRequest *req;
    char *method;
    char *url;
    JSBool async;
    char *user;
    char *password;
    GError *error = NULL;

    req = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_XML_HTTP_REQUEST (req)) {
        return JS_FALSE;
    }
    if (!JS_ConvertArguments (cx, argc, argv, "ss/bss",
                              &method, &url, &async,
                              &user, &password)) {
        return JS_FALSE;
    }

    gom_xml_http_request_open (req, method, url,
                               argc >= 3 ? async ? TRUE : FALSE : TRUE,
                               argc >= 4 ? user : GOM_XML_HTTP_REQUEST_OMITTED,
                               argc >= 5 ? password : GOM_XML_HTTP_REQUEST_OMITTED,
                               &error);
    if (error) {
        return gom_js_exception_set_error (cx, &error);
    }

    return JS_TRUE;
}

static JSBool
gom_js_xml_http_request_set_request_header (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomXMLHttpRequest *req;
    char *name;
    char *value;
    GError *error = NULL;

    req = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_XML_HTTP_REQUEST (req)) {
        return JS_FALSE;
    }
    if (!JS_ConvertArguments (cx, argc, argv, "ss", &name, &value)) {
        return JS_FALSE;
    }

    gom_xml_http_request_set_request_header (req, name, value, &error);
    if (error) {
        return gom_js_exception_set_error (cx, &error);
    }

    return JS_TRUE;
}

static JSBool
gom_js_xml_http_request_send (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomXMLHttpRequest *req;
    GError *error = NULL;
    char *data;
    JSObject *jsdoc;
    GomDocument *doc;

    req = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_XML_HTTP_REQUEST (req)) {
        return JS_FALSE;
    }

    if (argc == 0) {
        gom_xml_http_request_send (req, &error);
    } else if (JSVAL_IS_STRING (argv[0]) && 
               JS_ConvertArguments (cx, argc, argv, "s", &data)) {
        gom_xml_http_request_send_string (req, data, &error);
    } else if (JSVAL_IS_OBJECT (argv[0]) &&
               JS_ConvertArguments (cx, argc, argv, "o", &jsdoc)) {
        doc = gom_js_object_get_g_object (cx, obj);
        if (!GOM_IS_DOCUMENT (doc)) {
            return JS_FALSE;
        }
        gom_xml_http_request_send_document (req, doc, &error);
    }
    if (error) {
        return gom_js_exception_set_error (cx, &error);
    }
    return JS_TRUE;
}

static JSBool
gom_js_xml_http_request_abort (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomXMLHttpRequest *req;

    req = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_XML_HTTP_REQUEST (req)) {
        return JS_FALSE;
    }
    if (argc != 0) {
        return JS_FALSE;
    }

    gom_xml_http_request_abort (req);

    return JS_TRUE;
}

static JSBool
gom_js_xml_http_request_get_all_response_headers (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomXMLHttpRequest *req;
    GError *error = NULL;
    char *headers;

    req = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_XML_HTTP_REQUEST (req)) {
        return JS_FALSE;
    }
    if (argc != 0) {
        return JS_FALSE;
    }

    headers = gom_xml_http_request_get_all_response_headers (req, &error);
    if (error) {
        return gom_js_exception_set_error (cx, &error);
    }

    *rval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, headers));

    return JS_TRUE;
}

static JSBool
gom_js_xml_http_request_get_response_header (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomXMLHttpRequest *req;
    char *header;
    GError *error = NULL;

    req = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_XML_HTTP_REQUEST (req)) {
        return JS_FALSE;
    }
    if (!JS_ConvertArguments (cx, argc, argv, "s", &header)) {
        return JS_FALSE;
    }

    /* the passed-in header belongs to the JS runtime; no need to free */
    header = gom_xml_http_request_get_response_header (req, header, &error);
    if (error) {
        return gom_js_exception_set_error (cx, &error);
    }

    *rval = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, header));

    return JS_TRUE;
}

static JSFunctionSpec gom_js_xml_http_request_funcs[] = {
    { "open",                  gom_js_xml_http_request_open,                     2 },
    { "setRequestHeader",      gom_js_xml_http_request_set_request_header,       2 },
    { "send",                  gom_js_xml_http_request_send,                     0 },
    { "abort",                 gom_js_xml_http_request_abort,                    0 },
    { "getAllResponseHeaders", gom_js_xml_http_request_get_all_response_headers, 0 },
    { "getResponseHeader",     gom_js_xml_http_request_get_response_header,      1 },
    { NULL }
};

static JSBool
gom_js_xml_http_request_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    JSObject *win;
    GObject *gobj;
    GomDocument *doc;
    jsval jsdoc;

    win = JS_GetPrototype (cx, obj);
    if (win) {
        win = JS_GetParent (cx, win);
    }
    if (!win ||
        !JS_LookupProperty (cx, win, "document", &jsdoc) ||
        !JSVAL_IS_OBJECT (jsdoc)) {
        return JS_FALSE;
    }

    doc = gom_js_object_get_g_object (cx, JSVAL_TO_OBJECT (jsdoc));

    gobj = g_object_new (GOM_TYPE_XHR, "document", doc, NULL);
    gom_js_object_set_g_object (cx, obj, gobj);
    g_object_unref (gobj);

    return JS_TRUE;
}

JSObject *
gom_js_xml_http_request_init_class (JSContext *cx, JSObject *obj)
{
    gom_js_object_register_js_class (cx, GOM_TYPE_XML_HTTP_REQUEST, &GomJSGomXMLHttpRequestClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, &GomJSEventTargetClass, NULL, NULL),
                         &GomJSGomXMLHttpRequestClass, gom_js_xml_http_request_construct, 0,
                         gom_js_xml_http_request_props, gom_js_xml_http_request_funcs, NULL, NULL);
}
