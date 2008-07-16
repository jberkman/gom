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

#include "gom/gomjsdocument.h"

#include "gommacros.h"

#include "gom/dom/gomdocument.h"
#include "gom/dom/gomdocumentevent.h"
#include "gom/dom/gomdomexception.h"
#include "gom/gomdoc.h"
#include "gom/gomjsexception.h"
#include "gom/gomjsnode.h"
#include "gom/gomjsobject.h"

static void
gom_js_document_finalize (JSContext *cx, JSObject *obj)
{
    GomJSNodeClass.finalize (cx, obj);
}

JSClass GomJSDocumentClass = {
    "Document",
    JSCLASS_NEW_ENUMERATE,
    
    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    (JSEnumerateOp)gom_js_object_enumerate,
    JS_ResolveStub,
    JS_ConvertStub,
    gom_js_document_finalize
};

static JSPropertySpec document_props[] = {
    { NULL }
};

static JSBool
gom_js_document_create_element(JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomDocument *doc;
    GomElement *elem;
    const char *tag_name;
    GError *error = NULL;

    doc = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_DOCUMENT (doc)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "this is not a GomDocument")));
        }
#endif
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "s", &tag_name)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "invalid arguments")));
        }
#endif
        return JS_FALSE;
    }

    elem = gom_document_create_element (doc, tag_name, &error);
    if (!elem) {
        return gom_js_exception_set_error (cx, &error);
    }
    *rval = OBJECT_TO_JSVAL (gom_js_object_get_or_create_js_object (cx, elem));

    return JS_TRUE;
}

static JSBool
gom_js_document_create_document_fragment (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_text_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_comment (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_cdata_section (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_processing_instruction (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_attribute (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_entity_reference (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_get_elements_by_tag_name (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_import_node (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_element_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_create_attribute_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_get_elements_by_tag_name_ns (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSBool
gom_js_document_get_element_by_id (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomDocument *doc;
    GomElement *elem;
    const char *element_id;

    doc = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_DOCUMENT (doc)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "this is not a GomDocument")));
        }
#endif
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "s", &element_id)) {
#if 0
        if (!JS_IsExceptionPending (cx)) {
            JS_SetPendingException (cx, STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "invalid arguments")));
        }
#endif
        return JS_FALSE;
    }

    elem = gom_document_get_element_by_id (doc, element_id);
    *rval = elem ? OBJECT_TO_JSVAL (gom_js_object_get_or_create_js_object (cx, elem)) : JSVAL_NULL;

    return JS_TRUE;
}

static JSBool
gom_js_document_create_event (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GomDocumentEvent *doc;
    GomEvent *evt;
    char *type;
    GError *error = NULL;

    doc = gom_js_object_get_g_object (cx, obj);
    if (!GOM_IS_DOCUMENT_EVENT (doc)) {
        return JS_FALSE;
    }

    if (!JS_ConvertArguments (cx, argc, argv, "s", &type)) {
        return JS_FALSE;
    }

    evt = gom_document_event_create_event (doc, type, &error);
    if (!evt) {
        return gom_js_exception_set_error (cx, &error);
    }

    *rval = OBJECT_TO_JSVAL (gom_js_object_get_or_create_js_object (cx, evt));

    return JS_TRUE;
}

static JSBool
gom_js_document_can_dispatch (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    GOM_JS_NOT_IMPLEMENTED (cx);
    return JS_FALSE;
}

static JSFunctionSpec document_funcs[] = {
    { "createElement",               gom_js_document_create_element, 1 },
    { "createDocumentFragment",      gom_js_document_create_document_fragment, 0 },
    { "createTextNode",              gom_js_document_create_text_node, 1 },
    { "createComment",               gom_js_document_create_comment, 1 },
    { "createCDataSection",          gom_js_document_create_cdata_section, 1 },
    { "createProcessingInstruction", gom_js_document_create_processing_instruction, 2 },
    { "createAttribute",             gom_js_document_create_attribute, 1 },
    { "createEntityReference",       gom_js_document_create_entity_reference, 1 },
    { "getElementsByTagName",        gom_js_document_get_elements_by_tag_name, 1 },

    /* Introduced in DOM Level 2: */
    { "importNode",                  gom_js_document_import_node, 2 },
    { "createElementNS",             gom_js_document_create_element_ns, 2 },
    { "createAttributeNS",           gom_js_document_create_attribute_ns, 2 },
    { "getElementsByTagNameNS",      gom_js_document_get_elements_by_tag_name_ns, 2 },
    { "getElementById",              gom_js_document_get_element_by_id, 1 },

    /* Events level 3: */
    { "createEvent",                 gom_js_document_create_event, 1 },
    { "canDispatch",                 gom_js_document_can_dispatch, 1 },

    { NULL }
};

static JSBool
gom_js_document_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_document_init_class (JSContext *cx, JSObject *obj)
{
    gom_js_object_register_js_class (cx, GOM_TYPE_DOCUMENT, &GomJSDocumentClass);
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, &GomJSNodeClass, NULL, NULL),
                         &GomJSDocumentClass, gom_js_document_construct, 0,
                         document_props, document_funcs, NULL, NULL);
}
