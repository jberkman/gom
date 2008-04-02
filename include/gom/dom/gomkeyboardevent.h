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
#ifndef GOM_KEYBOARD_EVENT_H
#define GOM_KEYBOARD_EVENT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomKeyboardEvent          GomKeyboardEvent; /*dummy object*/
typedef struct _GomKeyboardEventInterface GomKeyboardEventInterface;

typedef enum {
    GOM_DOM_KEY_LOCATION_STANDARD = 0,
    GOM_DOM_KEY_LOCATION_LEFT,
    GOM_DOM_KEY_LOCATION_RIGHT,
    GOM_DOM_KEY_LOCATION_NUMPAD
} GomKeyLocationCode;

G_END_DECLS

#include <gom/dom/gomabstractview.h>
#include <gom/dom/gomeventtarget.h>

G_BEGIN_DECLS

#define GOM_TYPE_KEYBOARD_EVENT             (gom_keyboard_event_get_type ())
#define GOM_KEYBOARD_EVENT(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_KEYBOARD_EVENT, GomKeyboardEvent))
#define GOM_IS_KEYBOARD_EVENT(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_KEYBOARD_EVENT))
#define GOM_KEYBOARD_EVENT_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_KEYBOARD_EVENT, GomKeyboardEventInterface))

#define _GOM_IMPLEMENT_KEYBOARD_EVENT(i, p, f) (((GomKeyboardEventInterface*)i)->f = p##_##f)
#define GOM_IMPLEMENT_KEYBOARD_EVENT(i, p)                              \
    G_STMT_START {                                                      \
        _GOM_IMPLEMENT_KEYBOARD_EVENT (i, p, get_modifier_state);       \
        _GOM_IMPLEMENT_KEYBOARD_EVENT (i, p, init_keyboard_event);      \
        _GOM_IMPLEMENT_KEYBOARD_EVENT (i, p, init_keyboard_event_ns);   \
    } G_STMT_END

#define GOM_KEYDOWN "keydown"
#define GOM_KEYUP   "keyup"

struct _GomKeyboardEventInterface {
    GTypeInterface parent;

    gboolean (*get_modifier_state) (GomKeyboardEvent *gom_keyboard_event,
                                    const char       *key_identifier_arg);
    
    void (*init_keyboard_event) (GomKeyboardEvent   *gom_keyboard_event,
                                 const char         *type_arg,
                                 gboolean            can_bubble_arg,
                                 gboolean            cancelable_arg,
                                 GomAbstractView    *view_arg,
                                 const char         *key_identifier_arg,
                                 GomKeyLocationCode  key_location_arg,
                                 const char         *modifiers_list);

    void (*init_keyboard_event_ns) (GomKeyboardEvent   *gom_keyboard_event,
                                    const char         *namespace_uri,
                                    const char         *type_arg,
                                    gboolean            can_bubble_arg,
                                    gboolean            cancelable_arg,
                                    GomAbstractView    *view_arg,
                                    const char         *key_identifier_arg,
                                    GomKeyLocationCode  key_location_arg,
                                    const char         *modifiers_list);
};

GType gom_keyboard_event_get_type (void);

gboolean gom_keyboard_event_get_modifier_state (GomKeyboardEvent *evt,
                                                const char       *key_identifier_arg);

void  gom_keyboard_event_init_keyboard_event (GomKeyboardEvent   *gom_keyboard_event,
                                              const char         *type_arg,
                                              gboolean            can_bubble_arg,
                                              gboolean            cancelable_arg,
                                              GomAbstractView    *view_arg,
                                              const char         *key_identifier_arg,
                                              GomKeyLocationCode  key_location_arg,
                                              const char         *modifiers_list);

void  gom_keyboard_event_init_keyboard_event_ns (GomKeyboardEvent   *evt,
                                                 const char         *namespace_uri,
                                                 const char         *type_arg,
                                                 gboolean            can_bubble_arg,
                                                 gboolean            cancelable_arg,
                                                 GomAbstractView    *view_arg,
                                                 const char         *key_identifier_arg,
                                                 GomKeyLocationCode  key_location_arg,
                                                 const char         *modifiers_list);

G_END_DECLS

#endif /* GOM_KEYBOARD_EVENT_H */
