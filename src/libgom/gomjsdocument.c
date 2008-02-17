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

#include <gom/gomjsdocument.h>

#include <gom/gomdoc.h>
#include <gom/gomjsnode.h>

struct JSClass GomJSDocumentClass = {
    "GomDocument", 0,
    
    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub,
    JS_ConvertStub, JS_FinalizeStub
};

static JSBool
document_get_doctype (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    return JS_FALSE;
}

static JSBool
document_get_implementation (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    return JS_FALSE;
}

static JSBool
document_get_document_element (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    return JS_FALSE;
}

static JSPropertySpec document_props[] = {
    { "docType",         -1, JSPROP_READONLY | JSPROP_PERMANENT, document_get_doctype },
    { "implementation",  -1, JSPROP_READONLY | JSPROP_PERMANENT, document_get_implementation },
    { "documentElement", -1, JSPROP_READONLY | JSPROP_PERMANENT, document_get_document_element },
    { NULL }
};

static JSBool
document_create_element(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_create_document_fragment (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_create_text_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_create_comment (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_create_cdata_section (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_create_processing_instruction (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_create_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_create_entity_reference (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSBool
document_get_elements_by_tag_name (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_FALSE;
}

static JSFunctionSpec document_funcs[] = {
    { "createElement",               document_create_element, 1 },
    { "createDocumentFragment",      document_create_document_fragment, 0 },
    { "createTextNode",              document_create_text_node, 1 },
    { "createComment",               document_create_comment, 1 },
    { "createCDataSection",          document_create_cdata_section, 1 },
    { "createProcessingInstruction", document_create_processing_instruction, 2 },
    { "createAttribute",             document_create_attribute, 1 },
    { "createEntityReference",       document_create_entity_reference, 1 },
    { "getElementsByTagName",        document_get_elements_by_tag_name, 1 },
    { NULL }
};

JSObject *
gom_js_document_init_class (JSContext *cx, JSObject *obj)
{
    JSObject *proto = JS_NewObject (cx, &GomJSNodeClass, NULL, obj);
    return JS_InitClass (cx, obj, proto, &GomJSDocumentClass, NULL, 0,
                         document_props, document_funcs, NULL, NULL);
}
