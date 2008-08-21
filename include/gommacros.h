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
#ifndef GOM_MACROS_H
#define GOM_MACROS_H

#define JSVAL_CHARS(jval) (JS_GetStringBytes (JSVAL_TO_STRING (jval)))

#ifdef __cplusplus
#define GOM_LOC(s) G_STRLOC":%s: "s, G_STRFUNC
#else
#define GOM_LOC(s) G_STRLOC":%s(): "s, G_STRFUNC
#endif

#define GOM_NOT_IMPLEMENTED (g_message (GOM_LOC("Not implemented yet.")))

#define GOM_NOT_IMPLEMENTED_ERROR(error)                                \
    G_STMT_START {                                                      \
        GOM_NOT_IMPLEMENTED;                                            \
        g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_NOT_IMPLEMENTED_ERR, \
                     GOM_LOC ("Not implemented yet"));                  \
    } G_STMT_END

#define GOM_JS_NOT_IMPLEMENTED(cx)                                      \
    G_STMT_START {                                                      \
        if (!JS_IsExceptionPending (cx)) {                              \
            GError *error = NULL;                                       \
            GOM_NOT_IMPLEMENTED_ERROR (&error);                         \
            gom_js_exception_set_error (cx, &error);                    \
        }                                                               \
    } G_STMT_END

#define GOM_PROPERTY_NOT_IMPLEMENTED(pspec)                     \
    (g_message (GOM_LOC ("%s.%s not implemented"),              \
                g_type_name (pspec->owner_type), pspec->name))

#define XG_RETURN_NOT_IMPLEMENTED               \
    G_STMT_START {                              \
        GOM_NOT_IMPLEMENTED;                    \
        return NS_ERROR_NOT_IMPLEMENTED;        \
    } G_STMT_END

#define GOM_DEFINE_QUARK(n)                                             \
    static gpointer                                                     \
    gom_##n##_quark_once (gpointer data)                                \
    {                                                                   \
        return GUINT_TO_POINTER (g_quark_from_static_string ("gom-"#n"-quark")); \
    }                                                                   \
                                                                        \
    GQuark                                                              \
    gom_##n##_quark (void)                                              \
    {                                                                   \
        static GOnce once = G_ONCE_INIT;                                \
        return GPOINTER_TO_UINT (g_once (&once, gom_##n##_quark_once, NULL)); \
    }

#define GOM_DEFINE_INTERFACE_WITH_CODE(IN, i_n, b_i, _C_)      \
    _GOM_DEFINE_INTERFACE_BEGIN(IN, i_n, b_i)                  \
    { _C_; }                                                   \
    _GOM_DEFINE_INTERFACE_END

#define GOM_DEFINE_INTERFACE_WITH_PREREQUISITE(IN, i_n, b_i, P_N)       \
    GOM_DEFINE_INTERFACE_WITH_CODE (IN, i_n, b_i, g_type_interface_add_prerequisite (type, P_N))

#define GOM_DEFINE_INTERFACE(IN, i_n, b_i)                              \
    GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (IN, i_n, b_i, G_TYPE_OBJECT)

#define _GOM_DEFINE_BASE_INIT_BEGIN(type_name) \
    static gpointer type_name##_base_init_once (gpointer g_iface);      \
                                                                        \
    static void                                                         \
    type_name##_base_init (gpointer g_iface)                            \
    {                                                                   \
        static GOnce base_once = G_ONCE_INIT;                           \
        g_once (&base_once, type_name##_base_init_once, g_iface);       \
    }                                                                   \
                                                                        \
    static gpointer                                                     \
    type_name##_base_init_once (gpointer g_iface)                       \
    {
#define _GOM_DEFINE_BASE_INIT_END               \
        return NULL;                            \
    }

#define _GOM_DEFINE_INTERFACE_BEGIN(TypeName, type_name, base_init)     \
    _GOM_DEFINE_BASE_INIT_BEGIN(type_name)                              \
    base_init;                                                          \
    _GOM_DEFINE_BASE_INIT_END                                           \
                                                                        \
    static gpointer type_name##_get_type_once (gpointer data);          \
                                                                        \
    GType                                                               \
    type_name##_get_type (void)                                         \
    {                                                                   \
        static GOnce type_once = G_ONCE_INIT;                           \
        g_once (&type_once, type_name##_get_type_once, NULL);           \
        return GPOINTER_TO_SIZE (type_once.retval);                     \
    } /* closes type_name##_get_type() */                               \
                                                                        \
    static gpointer                                                     \
    type_name##_get_type_once (gpointer data)                           \
    {                                                                   \
        static const GTypeInfo info = {                                 \
            sizeof (TypeName##Interface),                               \
            type_name##_base_init, /* base_init */                      \
            NULL, /* base_finalize */                                   \
            NULL, /* class_init */                                      \
            NULL, /* class_finalize */                                  \
            NULL, /* class_data */                                      \
            0,                                                          \
            0,    /* n_preallocs */                                     \
            NULL  /* instance_init */                                   \
        };                                                              \
        GType type = g_type_register_static (G_TYPE_INTERFACE, #TypeName, &info, 0); \
        { /* custom code follows */
#define _GOM_DEFINE_INTERFACE_END                                       \
        } /* following custom code */                                   \
        return GSIZE_TO_POINTER (type);                                 \
    }

#define GOM_STUB_VOID(I_N, i_n, func, args_in, args_passed)             \
    void                                                                \
    i_n##_##func args_in                                                \
    {                                                                   \
        g_return_if_fail (I_N##_GET_INTERFACE (i_n)->func != NULL);     \
        /* g_print (G_STRLOC":"#i_n"_"#func"()\n"); */                  \
        I_N##_GET_INTERFACE (i_n)->func args_passed;                    \
    }

#define GOM_STUB_FUNC(I_N, i_n, func, args_in, args_passed, ret)        \
    ret                                                                 \
    i_n##_##func args_in                                                \
    {                                                                   \
        g_return_val_if_fail (I_N##_GET_INTERFACE (i_n)->func != NULL, (ret)0); \
        /* g_print (G_STRLOC":"#i_n"_"#func"()\n"); */                  \
        return I_N##_GET_INTERFACE (i_n)->func args_passed;             \
    }

#define GOM_IMPLEMENT(I_N, i_n, p)                              \
    static void                                                 \
    p##_implement_##i_n (gpointer g_iface, gpointer iface_data) \
    {                                                           \
        GOM_IMPLEMENT_##I_N (g_iface, p);                       \
    }

#define GOM_IMPLEMENT_INTERFACE(I_N, i_n, p) (G_IMPLEMENT_INTERFACE (GOM_TYPE_##I_N, p##_implement_##i_n))

#define GOM_SET_WEAK(p, v)                                              \
    G_STMT_START {                                                      \
        if (p) {                                                        \
            g_object_remove_weak_pointer (G_OBJECT (p), (gpointer *)&p); \
        }                                                               \
        p = v;                                                          \
        if (p) {                                                        \
            g_object_add_weak_pointer (G_OBJECT (p), (gpointer *)&p);   \
        }                                                               \
    } G_STMT_END

#define GOM_UNSET_WEAK(p) GOM_SET_WEAK(p, NULL)

#define GOM_ASTRING_TO_GSTRING_RETURN(_aCString, _aString, _errval)     \
    GOM_ASTRING_TO_GSTRING(_aCString, _aString)

#define GOM_ASTRING_TO_GSTRING(_aCString, _aString)                     \
    NS_ConvertUTF16toUTF8 _aCString##String (_aString);                 \
    const char *_aCString = _aCString##String.get();

#define GOM_ATOM_TO_GSTRING_RETURN(_aCString, _aAtom, _errval) \
    const char *_aCString;                                     \
    if (NS_FAILED (_aAtom->GetUTF8String (&_aCString))) {      \
        return _errval;                                        \
    }

#define GOM_GSTRING_TO_ASTRING_RETURN(_aString, _aCString, _errval)     \
    NS_ConvertUTF8toUTF16 _aString (_aCString)

#define GOM_RETURN_NSRESULT_FROM_GERROR(_err)                           \
    G_STMT_START {                                                      \
        if (_err) {                                                     \
            nsresult _rv = NS_ERROR_UNEXPECTED;                         \
            if (_err->domain == GOM_DOM_EXCEPTION_ERROR) {              \
                if (_err->code < 87000) {                               \
                    _rv = NS_ERROR_GENERATE_FAILURE (NS_ERROR_MODULE_DOM, _err->code); \
                } else {                                                \
                    switch (_err->code) {                               \
                    case GOM_NO_INTERFACE_ERR:                          \
                        _rv = NS_ERROR_NOT_INITIALIZED;                 \
                    break;                                              \
                    case GOM_NOT_IMPLEMENTED_ERR:                       \
                        _rv = NS_ERROR_NOT_IMPLEMENTED;                 \
                        break;                                          \
                    }                                                   \
                }                                                       \
            }                                                           \
            g_error_free (_err);                                        \
            return _rv;                                                 \
        }                                                               \
    } G_STMT_END

#define XG_WRAPPED_CHECK_INIALIZED(t)                                   \
    G_STMT_START {                                                      \
        if (!mWrapped || !g_type_is_a (G_OBJECT_TYPE (mWrapped), t)) {  \
            return NS_ERROR_NOT_INITIALIZED;                            \
        }                                                               \
    } G_STMT_END

#define XG_WRAPPED_IMPL_GET_OBJECT(_class, _gtype, _func, _iface, _prop_name, _prop_gtype, _prop_class) \
    NS_IMETHODIMP                                                       \
    _class::_func (_iface **_retval)                                    \
    {                                                                   \
        XG_WRAPPED_CHECK_INIALIZED (_gtype);                            \
        _prop_class *prop = NULL;                                       \
        g_object_get (mWrapped, _prop_name, &prop, NULL);               \
        nsresult rv;                                                    \
        rv = gom_wrap_g_object (prop, NS_GET_IID (_iface), (gpointer *)_retval); \
        if (prop) {                                                     \
            g_object_unref (prop);                                      \
        }                                                               \
        return rv;                                                      \
    }

#define XG_WRAPPED_IMPL_GET_STRING(_class, _gtype, _func, _prop_name)   \
    NS_IMETHODIMP                                                       \
    _class::_func (nsAString &aProp)                                    \
    {                                                                   \
        XG_WRAPPED_CHECK_INIALIZED (_gtype);                            \
        char *prop;                                                     \
        g_object_get (mWrapped, _prop_name, &prop, NULL);               \
        GOM_GSTRING_TO_ASTRING_RETURN (aProp, prop, NS_ERROR_UNEXPECTED); \
        return NS_OK;                                                   \
    }

#define XG_WRAPPED_IMPL_GET_SIMPLE(_class, _gtype, _func, _prop_name, _prop_prtype, _prop_gtype) \
    NS_IMETHODIMP                                                       \
    _class::_func (_prop_prtype *aProp)                                 \
    {                                                                   \
        XG_WRAPPED_CHECK_INIALIZED (_gtype);                            \
        _prop_gtype prop;                                               \
        g_object_get (mWrapped, _prop_name, &prop, NULL);               \
        *aProp = prop;                                                  \
        return NS_OK;                                                   \
    }

#define XG_WRAPPED_IMPL_GET_ENUM(_class, _gtype, _func, _prop_name, _prop_class) \
    XG_WRAPPED_IMPL_GET_SIMPLE(_class, _gtype, _func, _prop_name, PRUint16, _prop_class)

#define XG_WRAPPED_IMPL_GET_BOOL(_class, _gtype, _func, _prop_name)     \
    XG_WRAPPED_IMPL_GET_SIMPLE(_class, _gtype, _func, _prop_name, PRBool, gboolean)

#define XG_WRAPPED_IMPL_GET_ULONG(_class, _gtype, _func, _prop_name)    \
    XG_WRAPPED_IMPL_GET_SIMPLE(_class, _gtype, _func, _prop_name, PRUint32, gulong)

#define GOM_WRAPPED_GET(_obj, _iface, _var)                     \
    nsCOMPtr<_iface> _var;                                      \
    {								\
	nsISupports *raw;					\
	g_object_get (_obj, "wrapped-object", &raw, NULL);      \
	nsCOMPtr<nsISupports> tmp = dont_AddRef (raw);          \
	_var = do_QueryInterface (tmp);                         \
    }

#define GOM_WRAPPED_GET_OBJECT(_wrapped, _func, _iface, _gtype, _klass) \
    G_STMT_START {                                                      \
        nsCOMPtr<_iface> aProp;                                         \
        _klass *prop = NULL;                                            \
        if (NS_SUCCEEDED (_wrapped->_func (getter_AddRefs (aProp))) && aProp) { \
            prop = (_klass *)gom_wrap_xpcom (aProp, _gtype, NULL);      \
        }                                                               \
        if (prop && g_type_is_a (G_OBJECT_TYPE (prop), _gtype)) {       \
            g_value_set_object (value, prop);                           \
            g_object_unref (prop);                                      \
        }                                                               \
    } G_STMT_END

#define GOM_WRAPPED_GET_ENUM(_wrapped, _func)           \
    G_STMT_START {                                      \
        PRUint16 aProp;                                 \
        if (NS_SUCCEEDED (_wrapped->_func (&aProp))) {  \
            g_value_set_enum (value, aProp);            \
        }                                               \
    } G_STMT_END
    
#define GOM_WRAPPED_GET_STRING(_wrapped, _func)         \
    G_STMT_START {                                      \
        nsAutoString aProp;                             \
        if (NS_SUCCEEDED (_wrapped->_func (aProp))) {   \
            GOM_ASTRING_TO_GSTRING (prop, aProp);       \
            g_value_set_string (value, prop);           \
        }                                               \
    } G_STMT_END

#define GOM_WRAPPED_GET_BOOL(_wrapped, _func)           \
    G_STMT_START {                                      \
        PRBool aProp;                                   \
        if (NS_SUCCEEDED (_wrapped->_func (&aProp))) {  \
            g_value_set_boolean (value, aProp);         \
        }                                               \
    } G_STMT_END

#define GOM_WRAPPED_GET_ULONG(_wrapped, _func)          \
    G_STMT_START {                                      \
        PRUint32 aProp;                                 \
        if (NS_SUCCEEDED (_wrapped->_func (&aProp))) {  \
            g_value_set_ulong (value, aProp);           \
        }                                               \
    } G_STMT_END

#endif /* GOM_MACROS_H */
