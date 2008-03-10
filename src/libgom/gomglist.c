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

#include <gom/gomglist.h>

#include <gom/dom/gomnodelist.h>

enum {
    PROP_LENGTH = 1
};

typedef struct {
    GList *glist;
    GList *last_node;

    gulong length;
    gulong last_index;
} GomGListPrivate;

#define PRIV(i) (G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_G_LIST, GomGListPrivate))

static void
gom_g_list_get_property (GObject        *object,
                         guint           property_id,
                         GValue         *value,
                         GParamSpec     *pspec)
{
    switch (property_id) {
    case PROP_LENGTH:
        g_value_set_ulong (value, PRIV (object)->length);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static GomNode *
gom_g_list_item (GomNodeList *list, gulong index)
{
    GomGListPrivate *priv = PRIV (list);
    if (index >= priv->length) {
        return NULL;
    }

    return g_list_nth_data (priv->glist, index);
}

static void
gom_g_list_interface_init (gpointer g_iface, gpointer iface_data)
{
    GomNodeListInterface *iface = (GomNodeListInterface *)g_iface;

    iface->item = gom_g_list_item;
}

G_DEFINE_TYPE_WITH_CODE (GomGList, gom_g_list, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (GOM_TYPE_NODE_LIST, gom_g_list_interface_init));

GomNodeList *
gom_g_list_new (GList *glist)
{
    GomGList *list;
    GomGListPrivate *priv;

    list = g_object_new (GOM_TYPE_G_LIST, NULL);
    priv = PRIV (list);

    priv->glist = priv->last_node = glist;
    priv->length = g_list_length (glist);
    priv->last_index = 0;

    return GOM_NODE_LIST (list);
}

static void
gom_g_list_init (GomGList *self)
{

}

static void
gom_g_list_dispose (GObject *obj)
{
    GomGListPrivate *priv = PRIV (obj);

    if (priv->glist) {
        g_list_free (priv->glist);
        priv->glist = NULL;
    }
    priv->last_node = NULL;

    G_OBJECT_CLASS (gom_g_list_parent_class)->dispose (obj);
}

static void
gom_g_list_class_init (GomGListClass *klass)
{
    GObjectClass *g_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomGListPrivate));

    g_class->get_property = gom_g_list_get_property;
    g_class->dispose = gom_g_list_dispose;

    g_object_class_override_property (g_class, PROP_LENGTH, "length");
}
