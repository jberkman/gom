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

#include <gom/gomjsnavigator.h>

#include <glib/gmessages.h>

#include <gommacros.h>

#include <locale.h>

JSClass GomJSNavigatorClass = {
    "Navigator", 0,

    JS_PropertyStub, JS_PropertyStub,
    JS_PropertyStub, JS_PropertyStub,
    JS_EnumerateStub, JS_ResolveStub,
    JS_ConvertStub, JS_FinalizeStub
};

typedef enum {
    PROP_APP_CODE_NAME = 1,
    PROP_APP_NAME,
    PROP_APP_VERSION,
    PROP_COOKIE_ENABLED,
    PROP_LANGUAGE,
    PROP_MIME_TYPES,
    PROP_PLATFORM,
    /* PROP_PLUGINS, */
    PROP_USER_AGENT
} GomJSWindowProps;

#ifdef LINUX
#define NAV_PLATORM "Linux"
#elif defined (__APPLE__)
#define NAV_PLATFORM "Mac OS X"
#elif defined (G_OS_WIN32)
#define NAV_PLATFORM "Win32"
#else
#define NAV_PLATFORM "Other"
#endif

    
static JSBool
gom_js_navigator_get_prop (JSContext *cx, JSObject *obj, jsval id, jsval *vp)
{
    switch (JSVAL_TO_INT (id)) {
    case PROP_APP_CODE_NAME:
        *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Mozilla"));
        break;
    case PROP_APP_NAME:
        *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Gom"));
        break;
    case PROP_APP_VERSION:
        *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, VERSION));
        break;
    case PROP_COOKIE_ENABLED:
        *vp = BOOLEAN_TO_JSVAL (FALSE);
        break;
    case PROP_LANGUAGE:
        *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, setlocale (LC_ALL, NULL)));
        break;
    case PROP_MIME_TYPES:
        *vp = JSVAL_NULL;
        break;
    case PROP_PLATFORM:
        *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, NAV_PLATFORM));
        break;
    case PROP_USER_AGENT:
        *vp = STRING_TO_JSVAL (JS_NewStringCopyZ (cx, "Mozilla/5.0 (compatible; Gom/"VERSION"; "NAV_PLATFORM")"));
        break;
    default:
        return JS_FALSE;
    }
    g_print ("%s:%d:%s: %d = %s\n", __FILE__, __LINE__, __FUNCTION__, JSVAL_TO_INT (id), JSVAL_CHARS (*vp));
    return JS_TRUE;
}

static JSPropertySpec gom_js_navigator_props[] = {
    { "appCodeName",   PROP_APP_CODE_NAME,  JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { "appName",       PROP_APP_NAME,       JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { "appVersion",    PROP_APP_VERSION,    JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { "cookieEnabled", PROP_COOKIE_ENABLED, JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { "language",      PROP_LANGUAGE,       JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { "mimeTypes",     PROP_MIME_TYPES,     JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { "platform",      PROP_PLATFORM,       JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { "userAgent",     PROP_USER_AGENT,     JSPROP_PERMANENT | JSPROP_READONLY, gom_js_navigator_get_prop },
    { NULL }
};

static JSBool
gom_js_navigator_java_enabled (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    *rval = BOOLEAN_TO_JSVAL (FALSE);
    return JS_TRUE;
}

static JSFunctionSpec gom_js_navigator_funcs[] = {
    { "javaEnabled", gom_js_navigator_java_enabled, 0 },
    { NULL }
};

static JSBool
gom_js_navigator_construct (JSContext *cx, JSObject *obj, uintN argc, jsval *argv, jsval *rval)
{
    return JS_TRUE;
}

JSObject *
gom_js_navigator_init_class (JSContext *cx, JSObject *obj)
{
    return JS_InitClass (cx, obj,
                         JS_ConstructObject (cx, NULL, NULL, NULL),
                         &GomJSNavigatorClass, gom_js_navigator_construct, 0,
                         gom_js_navigator_props, gom_js_navigator_funcs, NULL, NULL);
}
