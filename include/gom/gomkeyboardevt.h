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
#ifndef GOM_KEYBOARD_EVT_H
#define GOM_KEYBOARD_EVT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomKeyboardEvt      GomKeyboardEvt;
typedef struct _GomKeyboardEvtClass GomKeyboardEvtClass;

G_END_DECLS

#include <gom/gomuievt.h>
#include <gom/dom/gomkeyboardevent.h>

G_BEGIN_DECLS

#define GOM_TYPE_KEYBOARD_EVT         (gom_keyboard_evt_get_type ())
#define GOM_KEYBOARD_EVT(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GOM_TYPE_KEYBOARD_EVT, GomKeyboardEvt))
#define GOM_KEYBOARD_EVT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST    ((k), GOM_TYPE_KEYBOARD_EVT, GomKeyboardEvtClass))
#define GOM_IS_KEYBOARD_EVT(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GOM_TYPE_KEYBOARD_EVT))
#define GOM_IS_KEYBOARD_EVT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE    ((k), GOM_TYPE_KEYBOARD_EVT))
#define GOM_KEYBOARD_EVT_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS  ((i), GOM_TYPE_KEYBOARD_EVT, GomKeyboardEvtClass))

struct _GomKeyboardEvt {
    GomUIEvt parent;
};

struct _GomKeyboardEvtClass {
    GomUIEvtClass parent;
};

GType gom_keyboard_evt_get_type (void);

guint gom_keyboard_evt_state_from_string (const char *modifiers_list);
char *gom_keyboard_evt_string_from_state (guint       state);

gboolean gom_keyboard_evt_keyval_to_key_identifier (guint               keyval,
                                                    const char        **key_identifier, 
                                                    GomKeyLocationCode *key_location);

gboolean gom_keyboard_evt_key_identifier_to_keyval (const char         *key_identifier, 
                                                    GomKeyLocationCode  key_location,
                                                    guint              *keyval);

G_END_DECLS

#endif /* GOM_KEYBOARD_EVT_H */
