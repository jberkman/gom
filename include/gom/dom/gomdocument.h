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
#ifndef GOM_DOCUMENT_H
#define GOM_DOCUMENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomDocument GomDocument; /* dummy object */
typedef struct _GomDocumentInterface GomDocumentInterface;

G_END_DECLS

#include <gom/dom/gomattr.h>
#include <gom/dom/gomcdatasection.h>
#include <gom/dom/gomcomment.h>
#include <gom/dom/gomdocumentfragment.h>
#include <gom/dom/gomdocumenttype.h>
#include <gom/dom/gomdomimplementation.h>
#include <gom/dom/gomelement.h>
#include <gom/dom/gomentityreference.h>
#include <gom/dom/gomprocessinginstruction.h>
#include <gom/dom/gomtext.h>

G_BEGIN_DECLS

#define GOM_TYPE_DOCUMENT             (gom_document_get_type ())
#define GOM_DOCUMENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_DOCUMENT, GomDocument))
#define GOM_IS_DOCUMENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_DOCUMENT))
#define GOM_DOCUMENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_DOCUMENT, GomDocumentInterface))

#define _GOM_IMPLEMENT_DOCUMENT(i, p, f) (((GomDocumentInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_DOCUMENT(i, p)                                    \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_element);                 \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_document_fragment);       \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_text_node);               \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_comment);                 \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_cdata_section);           \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_processing_instruction);  \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_attribute);               \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_entity_reference);        \
        _GOM_IMPLEMENT_DOCUMENT (i, p, get_elements_by_tag_name);       \
        _GOM_IMPLEMENT_DOCUMENT (i, p, import_node);                    \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_element_ns);              \
        _GOM_IMPLEMENT_DOCUMENT (i, p, create_attribute_ns);            \
        _GOM_IMPLEMENT_DOCUMENT (i, p, get_elements_by_tag_name_ns);    \
        _GOM_IMPLEMENT_DOCUMENT (i, p, get_element_by_id);              \
    } G_STMT_END

struct _GomDocumentInterface {
    GTypeInterface parent;

    GomElement               *(*create_element)                (GomDocument *doc,
                                                                const char  *tag_name,
                                                                GError    **error);
    GomDocumentFragment      *(*create_document_fragment)      (GomDocument *doc);
    GomText                  *(*create_text_node)              (GomDocument *doc,
                                                                const char  *data);
    GomComment               *(*create_comment)                (GomDocument *doc,
                                                                const char  *data);
    GomCDATASection          *(*create_cdata_section)          (GomDocument *doc,
                                                                const char  *data,
                                                                GError     **error);
    GomProcessingInstruction *(*create_processing_instruction) (GomDocument *doc,
                                                                const char  *target,
                                                                const char  *data,
                                                                GError     **error);
    GomAttr                  *(*create_attribute)              (GomDocument *doc,
                                                                const char  *name,
                                                                GError     **error);
    GomEntityReference       *(*create_entity_reference)       (GomDocument *doc,
                                                                const char  *name,
                                                                GError     **error);
    GomNodeList              *(*get_elements_by_tag_name)      (GomDocument *doc,
                                                                const char  *tag_name);

    /* introduced in DOM level 2: */
    GomNode                  *(*import_node)                   (GomDocument *doc,
                                                                GomNode     *node,
                                                                gboolean     deep,
                                                                GError     **error);

    GomElement               *(*create_element_ns)             (GomDocument *doc,
                                                                const char  *namespace_uri,
                                                                const char  *qualified_name,
                                                                GError     **error);

    GomAttr                  *(*create_attribute_ns)           (GomDocument *doc,
                                                                const char  *namespace_uri,
                                                                const char  *qualified_name,
                                                                GError     **error);

    GomNodeList              *(*get_elements_by_tag_name_ns)   (GomDocument *doc,
                                                                const char  *namespace_uri,
                                                                const char  *local_name);

    GomElement               *(*get_element_by_id)             (GomDocument *doc,
                                                                const char  *element_id);
};

GType gom_document_get_type (void);

GomElement               *gom_document_create_element                (GomDocument *doc,
                                                                      const char  *tag_name,
                                                                      GError    **error);
GomDocumentFragment      *gom_document_create_document_fragment      (GomDocument *doc);
GomText                  *gom_document_create_text_node              (GomDocument *doc,
                                                                      const char  *data);
GomComment               *gom_document_create_comment                (GomDocument *doc,
                                                                      const char  *data);
GomCDATASection          *gom_document_create_cdata_section          (GomDocument *doc,
                                                                      const char  *data,
                                                                      GError     **error);
GomProcessingInstruction *gom_document_create_processing_instruction (GomDocument *doc,
                                                                      const char  *target,
                                                                      const char  *data,
                                                                      GError     **error);
GomAttr                  *gom_document_create_attribute              (GomDocument *doc,
                                                                      const char  *name,
                                                                      GError     **error);
GomEntityReference       *gom_document_create_entity_reference       (GomDocument *doc,
                                                                      const char  *name,
                                                                      GError     **error);
GomNodeList              *gom_document_get_elements_by_tag_name      (GomDocument *doc,
                                                                      const char  *tag_name);

/* Introduced in DOM Level 2: */
GomNode                  *gom_document_import_node                   (GomDocument *doc,
                                                                      GomNode     *node,
                                                                      gboolean     deep,
                                                                      GError     **error);

GomElement               *gom_document_create_element_ns             (GomDocument *doc,
                                                                      const char  *namespace_uri,
                                                                      const char  *qualified_name,
                                                                      GError     **error);

GomAttr                  *gom_document_create_attribute_ns           (GomDocument *doc,
                                                                      const char  *namespace_uri,
                                                                      const char  *qualified_name,
                                                                      GError     **error);

GomNodeList              *gom_document_get_elements_by_tag_name_ns   (GomDocument *doc,
                                                                      const char  *namespace_uri,
                                                                      const char  *local_name);

GomElement               *gom_document_get_element_by_id             (GomDocument *doc,
                                                                      const char  *element_id);
G_END_DECLS

#endif /* GOM_DOCUMENT_H */
