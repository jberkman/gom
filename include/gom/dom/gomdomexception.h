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
    GOM_DOM_EXCEPTION_ERROR_INDEX_SIZE_ERR = 1,
    GOM_DOM_EXCEPTION_ERROR_DOMSTRING_SIZE_ERR,
    GOM_DOM_EXCEPTION_ERROR_HIERCHY_REQUEST_ERR,
    GOM_DOM_EXCEPTION_ERROR_WRONG_DOCUMENT_ERR,
    GOM_DOM_EXCEPTION_ERROR_INVALID_CHARACTER_ERR,
    GOM_DOM_EXCEPTION_ERROR_NO_DATA_ALLOWED_ERR,
    GOM_DOM_EXCEPTION_ERROR_NO_MODIFICATION_ALLOWED_ERR,
    GOM_DOM_EXCEPTION_ERROR_NOT_FOUND_ERR,
    GOM_DOM_EXCEPTION_ERROR_NOT_SUPPORTED_ERR,
    GOM_DOM_EXCEPTION_ERROR_INUSE_ATTRIBUTE_ERR,

    /* Gom-specific exceptions */
    GOM_DOM_EXCEPTION_ERROR_UNKNOWN_TAG_NAME = 101,
} GomDOMExceptionError;

GQuark gom_dom_exception_error_quark (void);

G_END_DECLS

#endif /* GOM_DOM_EXCEPTION_H */
