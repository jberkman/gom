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
#ifndef GOM_ELEMENT_H
#define GOM_ELEMENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomElement GomElement; /* dummy object */
typedef struct _GomElementInterface GomElementInterface;

G_END_DECLS

#include <gom/dom/gomnode.h>
#include <gom/dom/gomattr.h>

G_BEGIN_DECLS

#define GOM_TYPE_ELEMENT             (gom_element_get_type ())
#define GOM_ELEMENT(i)               (G_TYPE_CHECK_INSTANCE_CAST ((i), GOM_TYPE_ELEMENT, GomElement))
#define GOM_IS_ELEMENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE ((i), GOM_TYPE_ELEMENT))
#define GOM_ELEMENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_ELEMENT, GomElementInterface))

struct _GomElementInterface {
    GTypeInterface parent;

    char  *(*get_attribute)    (GomElement *elem, const char *name);
    void   (*set_attribute)    (GomElement *elem, const char *name, const char *value, GError **error);
    void   (*remove_attribute) (GomElement *elem, const char *name, GError **error);

    GomAttr *(*get_attribute_node)    (GomElement *elem, const char *name);
    GomAttr *(*set_attribute_node)    (GomElement *elem, GomAttr *new_attr, GError **error);
    GomAttr *(*remove_attribute_node) (GomElement *elem, GomAttr *old_attr, GError **error);

    GomNodeList *(*get_elements_by_tag_name) (GomElement *elem, const char *name);

    char *(*get_attribute_ns) (GomElement *elem, const char *namespace_uri, const char *local_name);
    void  (*set_attribute_ns) (GomElement *elem, 
                               const char *namespace_uri,
                               const char *qualified_name,
                               const char *value,
                               GError    **error);

    void  (*remove_attribute_ns) (GomElement *elem,
                                  const char *namespace_uri,
                                  const char *local_name,
                                  GError    **error);

    GomAttr *(*get_attribute_node_ns) (GomElement *elem,
                                       const char *namespace_uri,
                                       const char *local_name);

    GomAttr *(*set_attribute_node_ns) (GomElement *elem,
                                       GomAttr    *new_attr,
                                       GError    **error);

    GomNodeList *(*get_elements_by_tag_name_ns) (GomElement *elem,
                                                 const char *namespace_uri,
                                                 const char *local_name);

    gboolean (*has_attribute)    (GomElement *elem, const char *name);

    gboolean (*has_attribute_ns) (GomElement *elem, const char *namespace_uri, const char *local_name);
};

GType gom_element_get_type (void);

char  *gom_element_get_attribute    (GomElement *elem, const char *name);
void   gom_element_set_attribute    (GomElement *elem, const char *name, const char *value, GError **error);
void   gom_element_remove_attribute (GomElement *elem, const char *name, GError **error);

GomAttr *gom_element_get_attribute_node    (GomElement *elem, const char *name);
GomAttr *gom_element_set_attribute_node    (GomElement *elem, GomAttr *new_attr, GError **error);
GomAttr *gom_element_remove_attribute_node (GomElement *elem, GomAttr *old_attr, GError **error);

GomNodeList *gom_element_get_elements_by_tag_name (GomElement *elem, const char *name);

char *gom_element_get_attribute_ns (GomElement *elem, const char *namespace_uri, const char *local_name);
void  gom_element_set_attribute_ns (GomElement *elem, 
                                    const char *namespace_uri,
                                    const char *qualified_name,
                                    const char *value,
                                    GError    **error);

void  gom_element_remove_attribute_ns (GomElement *elem,
                                       const char *namespace_uri,
                                       const char *local_name,
                                       GError    **error);

GomAttr *gom_element_get_attribute_node_ns (GomElement *elem,
                                            const char *namespace_uri,
                                            const char *local_name);

GomAttr *gom_element_set_attribute_node_ns (GomElement *elem,
                                            GomAttr    *new_attr,
                                            GError    **error);

GomNodeList *gom_element_get_elements_by_tag_name_ns (GomElement *elem,
                                                      const char *namespace_uri,
                                                      const char *local_name);

gboolean gom_element_has_attribute    (GomElement *elem, const char *name);

gboolean gom_element_has_attribute_ns (GomElement *elem, const char *namespace_uri, const char *local_name);

G_END_DECLS

#endif /* GOM_ELEMENT_H */
