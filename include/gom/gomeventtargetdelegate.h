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
#ifndef GOM_EVENT_TARGET_DELEGATE_H
#define GOM_EVENT_TARGET_DELEGATE_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomEventTargetHelper          GomEventTargetHelper; /* dummy object */
typedef struct _GomEventTargetHelperInterface GomEventTargetHelperInterface;

typedef struct _GomEventTargetDelegate      GomEventTargetDelegate;
typedef struct _GomEventTargetDelegateClass GomEventTargetDelegateClass;

G_END_DECLS

#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_EVENT_TARGET_HELPER             (gom_event_target_helper_get_type ())
#define GOM_EVENT_TARGET_HELPER(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_EVENT_TARGET_HELPER, GomEventTargetHelper))
#define GOM_IS_EVENT_TARGET_HELPER(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_EVENT_TARGET_HELPER))
#define GOM_EVENT_TARGET_HELPER_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_EVENT_TARGET_HELPER, GomEventTargetHelperInterface))

#define GOM_HAS_EVENT_TARGET_DELEGATE(i)         (GOM_IS_EVENT_TARGET_HELPER (i))

#define _GOM_IMPLEMENT_EVENT_TARGET_HELPER(i, p, f) (((GomEventTargetHelperInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_EVENT_TARGET_HELPER(i, p)                         \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_EVENT_TARGET_HELPER (i, p, get_event_target_delegate); \
    } G_STMT_END

#define GOM_TYPE_EVENT_TARGET_DELEGATE           (gom_event_target_delegate_get_type ())
#define GOM_EVENT_TARGET_DELEGATE(i)             (G_TYPE_CHECK_INSTANCE_CAST ((i), GOM_TYPE_EVENT_TARGET_DELEGATE, GomEventTargetDelegate))
#define GOM_EVENT_TARGET_DELEGATE_CLASS(k)       (G_TYPE_CHECK_CLASS_CAST    ((k), GOM_TYPE_EVENT_TARGET_DELEGATE, GomEventTargetDelegateClass))
#define GOM_IS_EVENT_TARGET_DELEGATE(i)          (G_TYPE_CHECK_INSTANCE_TYPE ((i), GOM_TYPE_EVENT_TARGET_DELEGATE))
#define GOM_IS_EVENT_TARGET_DELEGATE_CLASS(k)    (G_TYPE_CHECK_CLASS_TYPE    ((k), GOM_TYPE_EVENT_TARGET_DELEGATE))
#define GOM_EVENT_TARGET_DELEGATE_GET_CLASS(i)   (G_TYPE_INSTANCE_GET_CLASS  ((i), GOM_TYPE_EVENT_TARGET_DELEGATE, GomEventTargetDelegateClass))

struct _GomEventTargetHelperInterface {
    GTypeInterface parent;

    GomEventTargetDelegate *(*get_event_target_delegate) (GomEventTargetHelper *gom_event_target_helper);
};

struct _GomEventTargetDelegate {
    GObject parent;
};

struct _GomEventTargetDelegateClass {
    GObjectClass parent_class;
};

GType           gom_event_target_helper_get_type     (void);
GType           gom_event_target_delegate_get_type   (void);

GomEventTargetDelegate *gom_event_target_helper_get_event_target_delegate (GomEventTargetHelper *gom_event_target_helper);

G_END_DECLS

#endif /* GOM_EVENT_TARGET_DELEGATE_H */
