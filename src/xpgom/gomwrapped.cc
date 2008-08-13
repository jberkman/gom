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

#include "xpgom/gomwrapped.hh"
#include "gom/gomunknown.h"
#include "gom/dom/gomdomexception.h"

#include <nsCOMPtr.h>
#include "idl/xgPIWrapped.hh"

#include "gommacros.h"

enum {
    PROP_WRAPPED_OBJECT = 1
};

typedef struct {
    nsISupports *wrappedObject;
} GomWrappedPrivate;

#define PRIV(i) G_TYPE_INSTANCE_GET_PRIVATE ((i), GOM_TYPE_WRAPPED, GomWrappedPrivate)

static void
gom_wrapped_get_property (GObject    *object,
			  guint       property_id,
			  GValue     *value,
			  GParamSpec *pspec)
{
    GomWrappedPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_WRAPPED_OBJECT:
	NS_IF_ADDREF (priv->wrappedObject);
	g_value_set_pointer (value, priv->wrappedObject);
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	break;
    }
}

static void
gom_wrapped_set_property (GObject *object,
			  guint property_id,
			  const GValue *value,
			  GParamSpec *pspec)
{
    GomWrappedPrivate *priv = PRIV (object);

    switch (property_id) {
    case PROP_WRAPPED_OBJECT:
	if (priv->wrappedObject) {
	    g_warning (GOM_LOC ("Wrapped object already exists: %p"),
		       priv->wrappedObject);
	    return;
	}
	priv->wrappedObject = (nsISupports *)g_value_get_pointer (value);
	NS_IF_ADDREF (priv->wrappedObject);
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
	break;
    }
}

static gpointer
gom_wrapped_query_interface (gpointer unknown, GType requested_interface, GError **error)
{
    GomWrappedPrivate *priv = PRIV (unknown);
    if (g_type_is_a (G_OBJECT_TYPE (unknown), requested_interface)) {
	return g_object_ref (unknown);
    }
    return gom_wrap_xpcom (priv->wrappedObject, requested_interface, error);
}

GOM_IMPLEMENT (UNKNOWN, unknown, gom_wrapped);

G_DEFINE_TYPE_WITH_CODE (GomWrapped, gom_wrapped, G_TYPE_OBJECT, GOM_IMPLEMENT_INTERFACE (UNKNOWN, unknown, gom_wrapped));

static void gom_wrapped_init (GomWrapped *wrapped) { }

static void
gom_wrapped_dispose (GObject *object)
{
    GomWrappedPrivate *priv = PRIV (object);

    NS_IF_RELEASE (priv->wrappedObject);
    priv->wrappedObject = NULL;

    G_OBJECT_CLASS (gom_wrapped_parent_class)->dispose (object);
}

static void
gom_wrapped_class_init (GomWrappedClass *klass)
{
    GObjectClass *oclass = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (GomWrappedPrivate));

    oclass->dispose      = gom_wrapped_dispose;
    oclass->get_property = gom_wrapped_get_property;
    oclass->set_property = gom_wrapped_set_property;

    g_object_class_install_property (
        oclass,
	PROP_WRAPPED_OBJECT,
	g_param_spec_pointer ("wrapped-object", NULL,
			      "The nsISupports * this object is wrapping",
			      GParamFlags (G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS)));
}

struct WrapMapEntry {
    GType                 mInterfaceType;
    GType                 mWrapperType;
    nsIID                 mIid;
    GomWrappedConstructor mConstructor;
};
static GSList *wrap_map = NULL;

void
gom_wrapped_register_interface (GType                 g_interface,
				GType                 g_wrapper,
				REFNSIID              iid,
				GomWrappedConstructor ctor)
{
    WrapMapEntry *entry = new WrapMapEntry();
    char nsid[NSID_LENGTH];

    iid.ToProvidedString (nsid);

    g_message (GOM_LOC ("Adding mapping for %s -> %s (%s -> %p)"),
	       g_type_name (g_interface), g_type_name (g_wrapper),
	       nsid, ctor);

    entry->mInterfaceType = g_interface;
    entry->mWrapperType = g_wrapper;
    entry->mIid = iid;
    entry->mConstructor = ctor;

    wrap_map = g_slist_prepend (wrap_map, entry);
}

gpointer
gom_wrap_xpcom (nsISupports *object,
		GType        requested_interface,
		GError     **error)
{
    if (!object) {
	return NULL;
    }
    {
	nsCOMPtr<xgPIWrapped> wrapped = do_QueryInterface (object);
	if (wrapped) {
	    GObject *nugget;
	    wrapped->GetWrappedGObject (&nugget);
	    GObject *ret = G_OBJECT (gom_unknown_query_interface ((gpointer)nugget, requested_interface, error));
	    g_object_unref (nugget);
	    g_message ("Returning a GObject nugget %p %s for %s",
		       ret, G_OBJECT_TYPE_NAME (ret),
		       g_type_name (requested_interface));
	    return ret;
	}
    }
    WrapMapEntry *ent;
    for (GSList *li = wrap_map; li; li = li->next) {
	ent = (WrapMapEntry *)li->data;
	//g_print (GOM_LOC ("%s?\n"), g_type_name (ent->mInterfaceType));
	if (g_type_is_a (ent->mInterfaceType, requested_interface)) {
	    //g_print (GOM_LOC ("maybe %s...\n"), g_type_name (ent->mInterfaceType));
	    nsCOMPtr<nsISupports> queried;
	    if (NS_SUCCEEDED (object->QueryInterface (ent->mIid, getter_AddRefs (queried)))) {
		char nsid[NSID_LENGTH];
		ent->mIid.ToProvidedString (nsid);
		g_print (GOM_LOC ("%s %p is_a %s (%s)\n"),
			 nsid, object,
			 g_type_name (requested_interface),
			 g_type_name (ent->mWrapperType));
		return g_object_new (ent->mWrapperType, "wrapped-object", object, NULL);
	    }
	}
    }
    g_set_error (error, GOM_DOM_EXCEPTION_ERROR, GOM_NO_INTERFACE_ERR,
		 "Could not find a registered wrapper for %s",
		 g_type_name (requested_interface));
    return NULL;
}

nsresult
gom_wrap_g_object (gpointer      object,
		   REFNSIID      iid,
		   void        **retval)
{
    GSList *li;
    WrapMapEntry *ent;
    if (!object) {
	*retval = NULL;
	return NS_OK;
    }
    if (GOM_IS_WRAPPED (object)) {
	nsISupports *wrappedp;
	g_object_get (object, "wrapped-object", &wrappedp, NULL);
	nsCOMPtr<nsISupports> wrapped = dont_AddRef (wrappedp);
	return wrapped->QueryInterface (iid, retval);
    }
    for (li = wrap_map; li; li = li->next) {
	ent = (WrapMapEntry *)li->data;
	//g_print (GOM_LOC ("%s?\n"), g_type_name (ent->mInterfaceType));
	if (g_type_is_a (G_OBJECT_TYPE (object), ent->mInterfaceType)) {
	    //g_print (GOM_LOC ("maybe %s...\n"), g_type_name (ent->mInterfaceType));
	    if (iid.Equals (ent->mIid)) {
		char nsid[NSID_LENGTH];
		iid.ToProvidedString (nsid);
		g_print (GOM_LOC ("%s %p is_a %s (%s)\n"),
			 G_OBJECT_TYPE_NAME (object),
			 object,
			 nsid,
			 g_type_name (ent->mInterfaceType));
		return ent->mConstructor (G_OBJECT (object), iid, retval);
	    }
	}
    }
    return NS_ERROR_NO_INTERFACE;
}

