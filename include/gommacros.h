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

#include <glib/gmacros.h>

G_BEGIN_DECLS

#define JSVAL_CHARS(jval) (JS_GetStringBytes (JSVAL_TO_STRING (jval)))

#define GOM_NOT_IMPLEMENTED (g_printerr ("\n%s:%d:%s(): Not implemented yet.\n", __FILE__, __LINE__, __FUNCTION__))

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
        I_N##_GET_INTERFACE (i_n)->func args_passed;                    \
    }

#define GOM_STUB_FUNC(I_N, i_n, func, args_in, args_passed, ret)        \
    ret                                                                 \
    i_n##_##func args_in                                                \
    {                                                                   \
        g_return_val_if_fail (I_N##_GET_INTERFACE (i_n)->func != NULL, (ret)0); \
        return I_N##_GET_INTERFACE (i_n)->func args_passed;             \
    }

G_END_DECLS

#endif /* GOM_MACROS_H */
