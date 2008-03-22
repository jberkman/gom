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

#ifndef GOM_DOM_IMPLEMENTATION_H
#define GOM_DOM_IMPLEMENTATION_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomDOMImplementation GomDOMImplementation; /* dummy object */
typedef struct _GomDOMImplmentationInterface GomDOMImplementationInterface;

G_END_DECLS

#include <gom/dom/gomdocument.h>

G_BEGIN_DECLS

#define GOM_TYPE_DOM_IMPLEMENTATION             (gom_dom_implementation_get_type ())
#define GOM_DOM_IMPLEMENTATION(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_DOM_IMPLEMENTATION, GomDOMImplementation))
#define GOM_IS_DOM_IMPLEMENTATION(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_DOM_IMPLEMENTATION))
#define GOM_DOM_IMPLEMENTATION_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_DOM_IMPLEMENTATION, GomDOMImplementationInterface))

struct _GomDOMImplmentationInterface {
    GTypeInterface parent;

    gboolean (*has_feature) (GomDOMImplementation *dom, const char *feature, const char *version);

    GomDocumentType *(*create_document_type) (GomDOMImplementation *dom,
                                              const char           *qualified_name,
                                              const char           *public_id,
                                              const char           *system_id,
                                              GError              **error);

    GomDocument     *(*create_document)      (GomDOMImplementation *dom,
                                              const char           *namespace_uri,
                                              const char           *qualified_name,
                                              GomDocumentType      *doctype,
                                              GError              **error);
};

GType gom_dom_implementation_get_type (void);

gboolean gom_dom_implementation_has_feature (GomDOMImplementation *dom, const char *feature, const char *version);

GomDocumentType *gom_dom_implementation_create_document_type (GomDOMImplementation *dom,
                                                              const char           *qualified_name,
                                                              const char           *public_id,
                                                              const char           *system_id,
                                                              GError              **error);

GomDocument     *gom_dom_implementation_create_document      (GomDOMImplementation *dom,
                                                              const char           *namespace_uri,
                                                              const char           *qualified_name,
                                                              GomDocumentType      *doctype,
                                                              GError              **error);

G_END_DECLS

#endif /* GOM_DOM_IMPLEMENTATION_H */
