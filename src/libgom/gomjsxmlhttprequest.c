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
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_xml_http_request_set_request_header (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_xml_http_request_send (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_xml_http_request_abort (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_xml_http_request_get_all_response_headers (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_xml_http_request_get_response_header (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
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
    GObject *gobj = g_object_new (GOM_TYPE_XHR, NULL);
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
