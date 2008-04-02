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
#ifndef GOM_MUTATION_NAME_EVENT_H
#define GOM_MUTATION_NAME_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomMutationNameEvent          GomMutationNameEvent; /*dummy object*/
typedef struct _GomMutationNameEventInterface GomMutationNameEventInterface;

G_END_DECLS

#include <glib-object.h>
#include <gom/dom/gomnode.h>

G_BEGIN_DECLS

#define GOM_TYPE_MUTATION_NAME_EVENT             (gom_mutation_name_event_get_type ())
#define GOM_MUTATION_NAME_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_MUTATION_NAME_EVENT, GomMutationNameEvent))
#define GOM_IS_MUTATION_NAME_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_MUTATION_NAME_EVENT))
#define GOM_MUTATION_NAME_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_MUTATION_NAME_EVENT, GomMutationNameEventInterface))

#define _GOM_IMPLEMENT_MUTATION_NAME_EVENT(i, p, f) (((GomMutationNameEventInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_MUTATION_NAME_EVENT(i, p)                         \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_MUTATION_NAME_EVENT (i, p, init_mutation_name_event); \
        _GOM_IMPLEMENT_MUTATION_NAME_EVENT (i, p, init_mutation_name_event_ns); \
    } G_STMT_END

#define GOM_DOM_ELEMENT_NAME_CHANGED   "DOMElementNameChanged"
#define GOM_DOM_ATTRIBUTE_NAME_CHANGED "DOMAttributeNameChanged"

struct _GomMutationNameEventInterface {
    GTypeInterface parent;

    void (*init_mutation_name_event) (GomMutationNameEvent *gom_mutation_name_event,
                                      const char           *event_type_arg,
                                      gboolean              can_bubble_arg,
                                      gboolean              cancelable_arg,
                                      GomNode              *related_node_arg,
                                      const char           *prev_namespace_uri_arg,
                                      const char           *prev_node_name_arg);

    void (*init_mutation_name_event_ns) (GomMutationNameEvent *gom_mutation_name_event,
                                         const char           *namespace_uri,
                                         const char           *event_type_arg,
                                         gboolean              can_bubble_arg,
                                         gboolean              cancelable_arg,
                                         GomNode              *related_node_arg,
                                         const char           *prev_namespace_uri_arg,
                                         const char           *prev_node_name_arg);
};

GType gom_mutation_name_event_get_type (void);
void  gom_mutation_name_event_init_mutation_name_event (GomMutationNameEvent *evt,
                                                        const char           *event_type_arg,
                                                        gboolean              can_bubble_arg,
                                                        gboolean              cancelable_arg,
                                                        GomNode              *related_node_arg,
                                                        const char           *prev_namespace_uri_arg,
                                                        const char           *prev_node_name_arg);

void  gom_mutation_name_event_init_mutation_name_event_ns (GomMutationNameEvent *evt,
                                                           const char           *namespace_uri,
                                                           const char           *event_type_arg,
                                                           gboolean              can_bubble_arg,
                                                           gboolean              cancelable_arg,
                                                           GomNode              *related_node_arg,
                                                           const char           *prev_namespace_uri_arg,
                                                           const char           *prev_node_name_arg);

G_END_DECLS

#endif /* GOM_MUTATION_NAME_EVENT_H */
