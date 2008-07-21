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
#ifndef GOM_DOM_EXCEPTION_H
#define GOM_DOM_EXCEPTION_H

#include <glib/gquark.h>

G_BEGIN_DECLS

#define GOM_DOM_EXCEPTION_ERROR (gom_dom_exception_error_quark ())

typedef enum {
    GOM_INDEX_SIZE_ERR              = 1,
    GOM_DOMSTRING_SIZE_ERR          = 2,
    GOM_HIERCHY_REQUEST_ERR         = 3,
    GOM_WRONG_DOCUMENT_ERR          = 4,
    GOM_INVALID_CHARACTER_ERR       = 5,
    GOM_NO_DATA_ALLOWED_ERR         = 6,
    GOM_NO_MODIFICATION_ALLOWED_ERR = 7,
    GOM_NOT_FOUND_ERR               = 8,
    GOM_NOT_SUPPORTED_ERR           = 9,
    GOM_INUSE_ATTRIBUTE_ERR         = 10,

    /* DOM Level 2 */
    GOM_INVALID_STATE_ERR           = 11,
    GOM_SYNTAX_ERR                  = 12,
    GOM_INVALID_MODIFICATION_ERR    = 13,
    GOM_NAMESPACE_ERR               = 14,
    GOM_INVALID_ACCESS_ERR          = 15,
    GOM_VALIDATION_ERR              = 16,

    /* DOM Level 3 */
    GOM_TYPE_MISMATCH_ERR           = 17,
    GOM_SECURITY_ERR                = 18,

    /* XMLHttpRequest */
    GOM_NETWORK_ERR                 = 101,
    GOM_ABORT_ERR                   = 102,

    /* Gom-specific exceptions */
    GOM_UNKNOWN_TAG_NAME_ERR        = 87001,
    GOM_INVALID_ATTRIBUTE_TYPE_ERR  = 87002,
    GOM_NOT_IMPLEMENTED_ERR         = 87003
} GomExceptionCode;

GQuark gom_dom_exception_error_quark (void);

G_END_DECLS

#endif /* GOM_DOM_EXCEPTION_H */
