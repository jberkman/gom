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
#ifndef GOM_MUTATION_EVENT_H
#define GOM_MUTATION_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomMutationEvent          GomMutationEvent; /*dummy object*/
typedef struct _GomMutationEventInterface GomMutationEventInterface;

typedef enum {
    GOM_MUTATION_EVENT_MODIFICATION = 1,
    GOM_MUTATION_EVENT_ADDITION,
    GOM_MUTATION_EVENT_REMOVAL
} GomMutationEventAttrChangeType;

G_END_DECLS

#include <glib-object.h>
#include <gom/dom/gomnode.h>

G_BEGIN_DECLS

#define GOM_TYPE_MUTATION_EVENT             (gom_mutation_event_get_type ())
#define GOM_MUTATION_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_MUTATION_EVENT, GomMutationEvent))
#define GOM_IS_MUTATION_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_MUTATION_EVENT))
#define GOM_MUTATION_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_MUTATION_EVENT, GomMutationEventInterface))

struct _GomMutationEventInterface {
    GTypeInterface parent;

    void (*init_mutation_event) (GomMutationEvent *evt,
                                 const char       *event_type_arg,
                                 gboolean          can_bubble_arg,
                                 gboolean          cancelable_arg,
                                 GomNode          *related_node_arg,
                                 const char       *prev_value_arg,
                                 const char       *new_value_arg,
                                 const char       *attr_name_arg,
                                 GomMutationEventAttrChangeType attr_change_arg);
};

GType gom_mutation_event_get_type (void);
void  gom_mutation_event_init_mutation_event (GomMutationEvent *evt,
                                              const char       *event_type_arg,
                                              gboolean          can_bubble_arg,
                                              gboolean          cancelable_arg,
                                              GomNode          *related_node_arg,
                                              const char       *prev_value_arg,
                                              const char       *new_value_arg,
                                              const char       *attr_name_arg,
                                              GomMutationEventAttrChangeType attr_change_arg);

G_END_DECLS

#endif /* GOM_MUTATION_EVENT_H */
