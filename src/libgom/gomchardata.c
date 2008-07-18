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

#include "gom/gomchardata.h"

#include "gom/dom/gomcharacterdata.h"
#include "gom/dom/gomdomexception.h"

#include "gommacros.h"

#include <string.h>

enum {
    PROP_DATA = 1,
    PROP_LENGTH
};

typedef struct {
    char   *data;
    gulong  length;
} GomCharDataPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_CHAR_DATA, GomCharDataPrivate)

static void
gom_char_data_get_property (GObject    *object,
                         guint       property_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
    GomCharDataPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_DATA:
        g_value_set_string (value, priv->data);
        break;
    case PROP_LENGTH:
        g_value_set_ulong (value, priv->length);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gom_char_data_set_property (GObject *object,
                         guint property_id,
                         const GValue *value,
                         GParamSpec *pspec)
{
    GomCharDataPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_DATA:
        g_free (priv->data);
        priv->data = g_value_dup_string (value);
        priv->length = strlen (priv->data);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static const char *
gom_char_data_substring_data (GomCharacterData *char_data,
                              gulong            offset,
                              gulong            count,
                              GError          **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static void
gom_char_data_append_data (GomCharacterData *char_data,
                           const char       *arg,
                           GError          **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_char_data_insert_data (GomCharacterData *char_data,
                           gulong            offset,
                           const char       *arg,
                           GError          **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_char_data_delete_data (GomCharacterData *char_data,
                           gulong            offset,
                           gulong            count,
                           GError          **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

static void
gom_char_data_replace_data (GomCharacterData *char_data,
                            gulong            offset,
                            gulong            count,
                            const char       *arg,
                            GError          **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
}

GOM_IMPLEMENT (CHARACTER_DATA, character_data, gom_char_data);

G_DEFINE_TYPE_WITH_CODE (GomCharData, gom_char_data, GOM_TYPE_NOODLE,
                         GOM_IMPLEMENT_INTERFACE (CHARACTER_DATA, character_data, gom_char_data));

static void gom_char_data_init (GomCharData *char_data) { }

static void
gom_char_data_dispose (GObject *object)
{
    GomCharDataPrivate *priv = PRIV (object);
#if 0
    g_print (G_STRLOC": %s %p\n",
             g_type_name (G_TYPE_FROM_INSTANCE (object)), object);
#endif
    g_free (priv->data);
    priv->data = NULL;

    G_OBJECT_CLASS (gom_char_data_parent_class)->dispose (object);
}

static void
gom_char_data_class_init (GomCharDataClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomCharDataPrivate));

    oclass->dispose      = gom_char_data_dispose;
    oclass->get_property = gom_char_data_get_property;
    oclass->set_property = gom_char_data_set_property;

    g_object_class_override_property (oclass, PROP_DATA,   "node-value");
    g_object_class_override_property (oclass, PROP_DATA,   "data");
    g_object_class_override_property (oclass, PROP_LENGTH, "length");
}
