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
#ifndef GOM_EVENT_TARGET_INTERNAL_H
#define GOM_EVENT_TARGET_INTERNAL_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomEventTargetInternal          GomEventTargetInternal; /* dummy object */
typedef struct _GomEventTargetInternalInterface GomEventTargetInternalInterface;

G_END_DECLS

#include <gom/dom/gomevent.h>
#include <gom/dom/gomeventtarget.h>

G_BEGIN_DECLS

#define GOM_TYPE_EVENT_TARGET_INTERNAL             (gom_event_target_internal_get_type ())
#define GOM_EVENT_TARGET_INTERNAL(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_EVENT_TARGET_INTERNAL, GomEventTargetInternal))
#define GOM_IS_EVENT_TARGET_INTERNAL(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_EVENT_TARGET_INTERNAL))
#define GOM_EVENT_TARGET_INTERNAL_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_EVENT_TARGET_INTERNAL, GomEventTargetInternalInterface))

#define _GOM_IMPLEMENT_EVENT_TARGET_INTERNAL(i, p, f) (((GomEventTargetInternalInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_EVENT_TARGET_INTERNAL(i, p)                       \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_EVENT_TARGET_INTERNAL (i, p, get_parent_target); \
        _GOM_IMPLEMENT_EVENT_TARGET_INTERNAL (i, p, dispatch_listeners); \
    } G_STMT_END

struct _GomEventTargetInternalInterface {
    GTypeInterface parent;

    GomEventTarget *(*get_parent_target)  (GomEventTargetInternal *target);
    
    void            (*dispatch_listeners) (GomEventTargetInternal *current_target,
                                           GomEvent               *evt,
                                           const char             *namespace_uri,
                                           const char             *type_name,
                                           GomPhaseType            phase);
};

GType           gom_event_target_internal_get_type (void);

GomEventTarget *gom_event_target_internal_get_parent_target (GomEventTargetInternal *target);

void            gom_event_target_internal_dispatch_listeners (GomEventTargetInternal *current_target,
                                                              GomEvent               *evt,
                                                              const char             *namespace_uri,
                                                              const char             *type_name,
                                                              GomPhaseType            phase);

G_END_DECLS

#endif /* GOM_EVENT_TARGET_INTERNAL_H */
