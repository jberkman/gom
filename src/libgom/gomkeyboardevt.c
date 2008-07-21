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

#include "gom/dom/gomkeyboardevent.h"

#include "gom/gomkeyboardevt.h"

#include "gom/dom/gomuievent.h"
#include "gom/gomkeyidentifiers.h"

#include "gommacros.h"

#include <gdk/gdktypes.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkkeys.h>

#include <string.h>
#include <stdlib.h>
#include <limits.h>

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

typedef struct {
    const char *id;
    guint standard;
    guint left;
    guint right;
    guint numpad;
} KeyIdentifierCodes;

static KeyIdentifierCodes key_identifier_codes[] = {
    { GOM_KEY_ATTN, GDK_3270_Attn },
    { GOM_KEY_ALT, 0, GDK_Alt_L, GDK_Alt_R },
    { GOM_KEY_CAPS_LOCK, GDK_Caps_Lock },
    { GOM_KEY_CLEAR, GDK_Clear },
    { GOM_KEY_CODE_INPUT, GDK_Codeinput },
    { GOM_KEY_CONTROL, 0, GDK_Control_L, GDK_Control_R },
    { GOM_KEY_CRSEL, GDK_3270_CursorSelect },
    { GOM_KEY_COPY, GDK_3270_Copy },
    { GOM_KEY_DOWN, GDK_Down, 0, 0, GDK_KP_Down },
    { GOM_KEY_END, GDK_End, 0, 0,  GDK_KP_End },
    { GOM_KEY_ENTER, GDK_Return, 0, 0, GDK_KP_Enter },
    { GOM_KEY_EXECUTE, GDK_Execute },

    { GOM_KEY_F1,  GDK_F1, 0, 0, GDK_KP_F1 },
    { GOM_KEY_F2,  GDK_F2, 0, 0, GDK_KP_F2 },
    { GOM_KEY_F3,  GDK_F3, 0, 0, GDK_KP_F3 },
    { GOM_KEY_F4,  GDK_F4, 0, 0, GDK_KP_F4 },
    { GOM_KEY_F5,  GDK_F5  },
    { GOM_KEY_F6,  GDK_F6  },
    { GOM_KEY_F7,  GDK_F7  },
    { GOM_KEY_F8,  GDK_F8  },
    { GOM_KEY_F9,  GDK_F9  },
    { GOM_KEY_F10, GDK_F10 },
    { GOM_KEY_F11, GDK_F11 },
    { GOM_KEY_F12, GDK_F12 },
    { GOM_KEY_F13, GDK_F13 },
    { GOM_KEY_F14, GDK_F14 },
    { GOM_KEY_F15, GDK_F15 },
    { GOM_KEY_F16, GDK_F16 },
    { GOM_KEY_F17, GDK_F17 },
    { GOM_KEY_F18, GDK_F18 },
    { GOM_KEY_F19, GDK_F19 },
    { GOM_KEY_F20, GDK_F20 },
    { GOM_KEY_F21, GDK_F21 },
    { GOM_KEY_F22, GDK_F22 },
    { GOM_KEY_F23, GDK_F23 },
    { GOM_KEY_F24, GDK_F24 },
    { GOM_KEY_F25, GDK_F25 },
    { GOM_KEY_F26, GDK_F26 },
    { GOM_KEY_F27, GDK_F27 },
    { GOM_KEY_F28, GDK_F28 },
    { GOM_KEY_F29, GDK_F29 },
    { GOM_KEY_F30, GDK_F30 },
    { GOM_KEY_F31, GDK_F31 },
    { GOM_KEY_F32, GDK_F32 },
    { GOM_KEY_F33, GDK_F33 },
    { GOM_KEY_F34, GDK_F34 },
    { GOM_KEY_F35, GDK_F35 },

    { GOM_KEY_FIND, GDK_Find },
    { GOM_KEY_HELP, GDK_Help },
    { GOM_KEY_HOME, GDK_Home, 0, 0, GDK_KP_Home },
    { GOM_KEY_INSERT, GDK_Insert },
    { GOM_KEY_LEFT, GDK_Left, 0, 0,  GDK_KP_Left },
    { GOM_KEY_LEFT, GDK_Left },
    { GOM_KEY_META, 0, GDK_Meta_L, GDK_Meta_R },
    { GOM_KEY_NUM_LOCK, GDK_Num_Lock },
    { GOM_KEY_PAGE_DOWN, GDK_Page_Down, 0, 0, GDK_KP_Page_Down },
    { GOM_KEY_PAGE_UP, GDK_Page_Up, 0, 0, GDK_KP_Page_Up },
    { GOM_KEY_PAUSE, GDK_Pause },
    { GOM_KEY_RIGHT, GDK_Right },
    { GOM_KEY_SCROLL, GDK_Scroll_Lock },
    { GOM_KEY_SELECT, GDK_Select },
    { GOM_KEY_SHIFT, 0, GDK_Shift_L, GDK_Shift_R },
    { GOM_KEY_UP, GDK_Up, 0, 0, GDK_KP_Up },
    { GOM_KEY_UNDO, GDK_Undo },

    { GOM_KEY_SPACE, 0, 0, 0, GDK_KP_Space },
    { GOM_KEY_TAB, 0, 0, 0, GDK_KP_Tab },
    { GOM_KEY_DELETE, 0, 0, 0, GDK_KP_Delete },
    { GOM_KEY_EQUALS, 0, 0, 0, GDK_KP_Equal },
    { GOM_KEY_ASTERIX, 0, 0, 0, GDK_KP_Multiply },
    { GOM_KEY_PLUS, 0, 0, 0, GDK_KP_Add },
    { GOM_KEY_COMMA, 0, 0, 0, GDK_KP_Separator },
    { GOM_KEY_HYPHEN, 0, 0, 0, GDK_KP_Subtract },
    { GOM_KEY_PERIOD, 0, 0, 0, GDK_KP_Decimal },
    { GOM_KEY_BACKSLASH, 0, 0, 0, GDK_KP_Divide },
    { GOM_KEY_0, 0, 0, 0, GDK_KP_0 },
    { GOM_KEY_1, 0, 0, 0, GDK_KP_1 },
    { GOM_KEY_2, 0, 0, 0, GDK_KP_2 },
    { GOM_KEY_3, 0, 0, 0, GDK_KP_3 },
    { GOM_KEY_4, 0, 0, 0, GDK_KP_4 },
    { GOM_KEY_5, 0, 0, 0, GDK_KP_5 },
    { GOM_KEY_6, 0, 0, 0, GDK_KP_6 },
    { GOM_KEY_7, 0, 0, 0, GDK_KP_7 },
    { GOM_KEY_8, 0, 0, 0, GDK_KP_8 },
    { GOM_KEY_9, 0, 0, 0, GDK_KP_9 },

    { NULL }
};

static gpointer
keyval_to_key_identifier_once (gpointer data)
{
    KeyIdentifierCodes *codes;
    GHashTable *ht;
    guint i;

    ht = g_hash_table_new (g_int_hash, g_int_equal);

    for (i = 0; key_identifier_codes[i].id; i++) {
        codes = &key_identifier_codes[i];
        if (codes->standard) {
            g_hash_table_insert (ht, &codes->standard, codes);
        }
        if (codes->left) {
            g_hash_table_insert (ht, &codes->left, codes);
        }
        if (codes->right) {
            g_hash_table_insert (ht, &codes->right, codes);
        }
        if (codes->numpad) {
            g_hash_table_insert (ht, &codes->numpad, codes);
        }
    }

    return ht;
}

gboolean
gom_keyboard_evt_keyval_to_key_identifier (guint               keyval,
                                           const char        **key_identifier, 
                                           GomKeyLocationCode *key_location)
{
    static GOnce just_this = G_ONCE_INIT;
    KeyIdentifierCodes *codes;
    GHashTable *ht;

    ht = g_once (&just_this, keyval_to_key_identifier_once, NULL);

    codes = g_hash_table_lookup (ht, &keyval);

    if (!codes) {
        keyval = gdk_keyval_to_upper (keyval);
        codes = g_hash_table_lookup (ht, &keyval);
        if (!codes) {
            codes = g_new0 (KeyIdentifierCodes, 1);
            codes->standard = keyval;
            codes->id = g_strdup_printf ("U+%04x", gdk_keyval_to_unicode (keyval));
            g_hash_table_insert (ht, &codes->standard, codes);
        }
    }
    if (keyval == codes->left) {
        *key_location = GOM_DOM_KEY_LOCATION_LEFT;
    } else if (keyval == codes->right) {
        *key_location = GOM_DOM_KEY_LOCATION_RIGHT;
    } else if (keyval == codes->numpad) {
        *key_location = GOM_DOM_KEY_LOCATION_NUMPAD;
    } else {
        *key_location = GOM_DOM_KEY_LOCATION_STANDARD;
    }
    *key_identifier = codes->id;

    return TRUE;
}

static gpointer
key_identifier_to_keyval_once (gpointer data)
{
    GHashTable *ht;
    guint i;

    ht = g_hash_table_new (g_str_hash, g_str_equal);

    for (i = 0; key_identifier_codes[i].id; i++) {
        g_hash_table_insert (ht, (gpointer)key_identifier_codes[i].id, &key_identifier_codes[i]);
    }

    return ht;
}

gboolean
gom_keyboard_evt_key_identifier_to_keyval (const char        *key_identifier, 
                                           GomKeyLocationCode key_location,
                                           guint             *keyval)
{
    static GOnce in_a_lifetime = G_ONCE_INIT;
    KeyIdentifierCodes *codes;
    GHashTable *ht;

    if (key_location < GOM_DOM_KEY_LOCATION_STANDARD || key_location > GOM_DOM_KEY_LOCATION_NUMPAD) {
        return FALSE;
    }
    
    ht = g_once (&in_a_lifetime, key_identifier_to_keyval_once, NULL);

    codes = g_hash_table_lookup (ht, key_identifier);
    if (codes) {
        switch (key_location) {
        case GOM_DOM_KEY_LOCATION_STANDARD:
            *keyval = codes->standard;
            break;
        case GOM_DOM_KEY_LOCATION_LEFT:
            *keyval = codes->left;
            break;
        case GOM_DOM_KEY_LOCATION_RIGHT:
            *keyval = codes->right;
            break;
        case GOM_DOM_KEY_LOCATION_NUMPAD:
            *keyval = codes->numpad;
            break;
        }
        if (*keyval) {
            return TRUE;
        }
    }

    if (key_identifier[0] != 'U' || key_identifier[1] != '+') {
        return FALSE;
    }

    *keyval = strtol (&key_identifier[2], NULL, 16);
    if (!*keyval) {
        return FALSE;
    }
    *keyval = gdk_unicode_to_keyval (*keyval);
        
    codes = g_new0 (KeyIdentifierCodes, 1);
    codes->id = g_strdup (key_identifier);
    
    switch (key_location) {
    case GOM_DOM_KEY_LOCATION_STANDARD:
        codes->standard = *keyval;
        break;
    case GOM_DOM_KEY_LOCATION_LEFT:
        codes->left = *keyval;
        break;
    case GOM_DOM_KEY_LOCATION_RIGHT:
        codes->right = *keyval;
        break;
    case GOM_DOM_KEY_LOCATION_NUMPAD:
        codes->numpad = *keyval;
        break;
    }

    g_hash_table_insert (ht, (gpointer)codes->id, codes);

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
        g_object_unref (target);
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
