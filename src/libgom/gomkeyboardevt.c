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
#include "config.h"

#include <gom/gomkeyboardevt.h>

#include "gommacros.h"

#include <gom/dom/gomkeyboardevent.h>
#include <gom/dom/gomuievent.h>
#include <gom/gomkeyidentifiers.h>

#include <gdk/gdktypes.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkkeys.h>

#include <string.h>

enum {
    PROP_KEY_IDENTIFIER = 1,
    PROP_KEY_LOCATION,
    PROP_CTRL_KEY,
    PROP_SHIFT_KEY,
    PROP_ALT_KEY,
    PROP_META_KEY
};

typedef struct {
    char               *key_identifier;
    GomKeyLocationCode  key_location;
    guint               state;
} GomKeyboardEvtPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_KEYBOARD_EVT, GomKeyboardEvtPrivate)

static void
gom_keyboard_evt_get_property (GObject    *object,
                               guint       property_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
    GomKeyboardEvtPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_KEY_IDENTIFIER:
        g_value_set_string (value, priv->key_identifier);
        break;
    case PROP_KEY_LOCATION:
        g_value_set_enum (value, priv->key_location);
        break;
    case PROP_CTRL_KEY:
        g_value_set_boolean (value, priv->state & GDK_CONTROL_MASK);
        break;
    case PROP_SHIFT_KEY:
        g_value_set_boolean (value, priv->state & GDK_SHIFT_MASK);
        break;
    case PROP_ALT_KEY:
        g_value_set_boolean (value, priv->state & GDK_MOD1_MASK);
        break;
    case PROP_META_KEY:
        g_value_set_boolean (value, priv->state & GDK_META_MASK);
        break;
    default: 
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

gboolean
gom_keyboard_evt_lookup_keyval  (guint               keyval,
                                 const char        **key_identifier, 
                                 GomKeyLocationCode *key_location)
{
    *key_location = GOM_DOM_KEY_LOCATION_STANDARD;

#define ID(g) *key_identifier = GOM_KEY_##g; break
#define ID_L(g) *key_location = GOM_DOM_KEY_LOCATION_LEFT; ID(g)
#define ID_R(g) *key_location = GOM_DOM_KEY_LOCATION_RIGHT; ID(g)
#define ID_N(g) *key_location = GOM_DOM_KEY_LOCATION_NUMPAD; ID(g)

    switch (keyval) {
    case GDK_3270_Attn: ID (ATTN);
    case GDK_Alt_L: ID_L (ALT);
    case GDK_Alt_R: ID_R (ALT);
    case GDK_Caps_Lock: ID (CAPS_LOCK);
    case GDK_Clear: ID (CLEAR);
    case GDK_Codeinput: ID (CODE_INPUT);
    case GDK_Control_L: ID_L (CONTROL);
    case GDK_Control_R: ID_R (CONTROL);
    case GDK_3270_CursorSelect: ID (CRSEL);
    case GDK_3270_Copy: ID (COPY);
    case GDK_KP_Down: ID_N (DOWN);
    case GDK_Down: ID (DOWN);
    case GDK_KP_End: ID_N (END);
    case GDK_End: ID (END);
    case GDK_KP_Enter: ID_N (ENTER);
    case GDK_ISO_Enter:
    case GDK_3270_Enter:
    case GDK_Return: ID (ENTER);
    case GDK_Execute: ID (EXECUTE);

    case GDK_KP_F1: ID_N (F1);
    case GDK_F1: ID (F1);

    case GDK_KP_F2: ID_N (F2);
    case GDK_F2: ID (F2);

    case GDK_KP_F3: ID_N (F3);
    case GDK_F3: ID (F3);

    case GDK_KP_F4: ID_N (F4);
    case GDK_F4: ID (F4);

    case GDK_F5: ID (F5);
    case GDK_F6: ID (F6);
    case GDK_F7: ID (F7);
    case GDK_F8: ID (F8);
    case GDK_F9: ID (F9);
    case GDK_F10: ID (F10);
    case GDK_F11: ID (F11);
    case GDK_F12: ID (F12);
    case GDK_F13: ID (F13);
    case GDK_F14: ID (F14);
    case GDK_F15: ID (F15);
    case GDK_F16: ID (F16);
    case GDK_F17: ID (F17);
    case GDK_F18: ID (F18);
    case GDK_F19: ID (F19);
    case GDK_F20: ID (F20);
    case GDK_F21: ID (F21);
    case GDK_F22: ID (F22);
    case GDK_F23: ID (F23);
    case GDK_F24: ID (F24);
    case GDK_F25: ID (F25);
    case GDK_F26: ID (F26);
    case GDK_F27: ID (F27);
    case GDK_F28: ID (F28);
    case GDK_F29: ID (F29);
    case GDK_F30: ID (F30);
    case GDK_F31: ID (F31);
    case GDK_F32: ID (F32);
    case GDK_F33: ID (F33);
    case GDK_F34: ID (F34);
    case GDK_F35: ID (F35);

    case GDK_Find: ID (FIND);
    case GDK_Help: ID (HELP);
    case GDK_KP_Home: ID_N (HOME);
    case GDK_Home: ID (HOME);
    case GDK_Insert: ID (INSERT);
    case GDK_KP_Left: ID_N (LEFT);
    case GDK_Left: ID (LEFT);
    case GDK_Meta_L: ID_L (META);
    case GDK_Meta_R: ID_R (META);
    case GDK_Num_Lock: ID (NUM_LOCK);
    case GDK_KP_Page_Down: ID_N (PAGE_DOWN);
    case GDK_Page_Down: ID (PAGE_DOWN);
    case GDK_KP_Page_Up: ID_N (PAGE_UP);
    case GDK_Page_Up: ID (PAGE_UP);
    case GDK_Pause: ID (PAUSE);
    case GDK_Right: ID (RIGHT);
    case GDK_Scroll_Lock: ID (SCROLL);
    case GDK_Select: ID (SELECT);
    case GDK_Shift_L: ID_L (SHIFT);
    case GDK_Shift_R: ID_R (SHIFT);
    case GDK_KP_Up: ID_N (UP);
    case GDK_Up: ID (UP);
    case GDK_Undo: ID (UNDO);

    case GDK_KP_Space: ID_N (SPACE);
    case GDK_KP_Tab: ID_N (TAB);
    case GDK_KP_Delete: ID_N (DELETE);
    case GDK_KP_Equal: ID_N (EQUALS);
    case GDK_KP_Multiply: ID_N (ASTERIX);
    case GDK_KP_Add: ID_N (PLUS);
    case GDK_KP_Separator: ID_N (COMMA);
    case GDK_KP_Subtract: ID_N (HYPHEN);
    case GDK_KP_Decimal: ID_N (PERIOD);
    case GDK_KP_Divide: ID_N (BACKSLASH);
    case GDK_KP_0: ID_N (0);
    case GDK_KP_1: ID_N (1);
    case GDK_KP_2: ID_N (2);
    case GDK_KP_3: ID_N (3);
    case GDK_KP_4: ID_N (4);
    case GDK_KP_5: ID_N (5);
    case GDK_KP_6: ID_N (6);
    case GDK_KP_7: ID_N (7);
    case GDK_KP_8: ID_N (8);
    case GDK_KP_9: ID_N (9);
    default: {
        char *str;
        str = g_strdup_printf ("U+%04x", gdk_keyval_to_unicode (gdk_keyval_to_upper (keyval)));
        *key_identifier = g_intern_string (str);
        if (str != *key_identifier) {
            g_free (str);
        }
        break;
    }
    }
    return TRUE;
}

typedef struct {
    const char      *id;
    GdkModifierType  mask;
} IdMask;

static IdMask id_masks[] = {
    { GOM_KEY_ALT,     GDK_MOD1_MASK },
    { GOM_KEY_CONTROL, GDK_CONTROL_MASK },
    { GOM_KEY_META,    GDK_META_MASK },
    { GOM_KEY_SHIFT,   GDK_SHIFT_MASK }
};

guint
gom_keyboard_evt_state_from_string (const char *modifiers_list)
{
    guint state = 0;
    const char *start;
    const char *end;
    const char *charset = " \n\r\t";
    int i;

    end = modifiers_list;
    do {
        /* skip whitespace... */
        start =   end + strspn  (end, charset);
        /* skip non-whitespace... */
        end   = start + strcspn (start, charset);

        /* done! */
        if (start == end) {
            return state;
        }

        for (i = 0; i < G_N_ELEMENTS (id_masks); i++) {
            if (!strncmp (id_masks[i].id, start, end - start)) {
                state |= id_masks[i].mask;
                break;
            }
        }
    } while (1);
}

char *
gom_keyboard_evt_string_from_state (guint state)
{
    GString *str = g_string_new (NULL);
    int i;

    for (i = 0; i < G_N_ELEMENTS (id_masks); i++) {
        if (state & id_masks[i].mask) {
            if (str->len) {
                g_string_append_c (str, ' ');
            }
            g_string_append (str, id_masks[i].id);
        }
    }

    return g_string_free (str, FALSE);
}

static void
gom_keyboard_evt_init_keyboard_event_ns (GomKeyboardEvent   *evt,
                                         const char         *namespace_uri,
                                         const char         *event_type_arg,
                                         gboolean            can_bubble_arg,
                                         gboolean            cancelable_arg,
                                         GomAbstractView    *view_arg,
                                         const char         *key_identifier_arg,
                                         GomKeyLocationCode  key_location_arg,
                                         const char         *modifiers_list)
{
    GomKeyboardEvtPrivate *priv = PRIV (evt);
    GomEventTarget *target;

    g_object_get (evt, "target", &target, NULL);
    if (target) {
        g_warning ("Event %p has already begun dispatching; changes are not permitted.", evt);
        return;
    }

    gom_ui_event_init_ui_event (GOM_UI_EVENT (evt), event_type_arg, can_bubble_arg, cancelable_arg, view_arg, 0);

    priv->state = gom_keyboard_evt_state_from_string (modifiers_list);

    g_free (priv->key_identifier);
    priv->key_identifier = g_strdup (key_identifier_arg);

    priv->key_location = key_location_arg;
}

static void
gom_keyboard_evt_init_keyboard_event (GomKeyboardEvent   *evt,
                                      const char         *event_type_arg,
                                      gboolean            can_bubble_arg,
                                      gboolean            cancelable_arg,
                                      GomAbstractView    *view_arg,
                                      const char         *key_identifier_arg,
                                      GomKeyLocationCode  key_location_arg,
                                      const char         *modifiers_list)
{
    gom_keyboard_evt_init_keyboard_event_ns (evt, NULL, event_type_arg,
                                             can_bubble_arg, cancelable_arg, view_arg,
                                             key_identifier_arg, key_location_arg, modifiers_list);
}

static gboolean
gom_keyboard_evt_get_modifier_state (GomKeyboardEvent *evt,
                                     const char       *key_identifier_arg)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

GOM_IMPLEMENT (KEYBOARD_EVENT, keyboard_event, gom_keyboard_evt);

G_DEFINE_TYPE_WITH_CODE (GomKeyboardEvt, gom_keyboard_evt, GOM_TYPE_UI_EVT,
                         GOM_IMPLEMENT_INTERFACE (KEYBOARD_EVENT, keyboard_event, gom_keyboard_evt));

static void gom_keyboard_evt_init (GomKeyboardEvt *event) { }

static void
gom_keyboard_evt_class_init (GomKeyboardEvtClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomKeyboardEvtPrivate));

    oclass->get_property = gom_keyboard_evt_get_property;

    g_object_class_override_property (oclass, PROP_KEY_IDENTIFIER, "key-identifier");
    g_object_class_override_property (oclass, PROP_KEY_LOCATION,   "key-location");
    g_object_class_override_property (oclass, PROP_CTRL_KEY,       "ctrl-key");
    g_object_class_override_property (oclass, PROP_SHIFT_KEY,      "shift-key");
    g_object_class_override_property (oclass, PROP_ALT_KEY,        "alt-key");
    g_object_class_override_property (oclass, PROP_META_KEY,       "meta-key");
}
