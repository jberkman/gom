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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xgGtkElement.h"

#include <gtk/gtk.h>

#include "gom/gomcamel.h"
#include "gommacros.h"

#include <nsIAtom.h>
#include <nsIAtomService.h>
#include <nsIDocument.h>
#include <nsIDOMDocument.h>
#include <nsIDOMDocumentEvent.h>
#include <nsIDOMElement.h>
#include <nsIDOMEvent.h>
#include <nsIDOMEventTarget.h>
#include <nsIDOMNamedNodeMap.h>
#include <nsIDOMNode.h>
#include <nsIDOMUIEvent.h>
#include <nsIScriptContext.h>
#include <nsIScriptGlobalObject.h>
#include <nsMemory.h>
#include <nsServiceManagerUtils.h>
#include <nsStringAPI.h>

xgGtkElement::xgGtkElement () 
{
}

xgGtkElement::~xgGtkElement()
{
}

NS_IMPL_ISUPPORTS_INHERITED2 (xgGtkElement, xgGObject, nsIXTFAttributeHandler, nsIXTFElement)

nsresult
xgGtkElement::Init (GType aType)
{
    nsresult rv;
    g_print (GOM_LOC ("Creating new <%s>...\n"), g_type_name (aType));
    GObject *obj = (GObject *)g_object_new (aType, NULL);
    rv = xgGObject::Init (obj);
    g_object_unref (obj);
    return rv;
}

void
xgGtkElement::WidgetActivate (GtkWidget *w, gpointer data)
{
    xgGtkElement *self = (xgGtkElement *)data;
    nsresult rv;

    nsCOMPtr<nsIDOMElement> elem;
    rv = self->mWrapper->GetElementNode (getter_AddRefs (elem));
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to get element: %#x"), rv);
	return;
    }

    nsCOMPtr<nsIDOMDocument> doc;
    rv = elem->GetOwnerDocument (getter_AddRefs (doc));
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to get doc: %#x"), rv);
	return;
    }

    nsCOMPtr<nsIDOMDocumentEvent> docEvent (do_QueryInterface (doc, &rv));
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to get doc event: %#x"), rv);
	return;
    }

    nsCOMPtr<nsIDOMEvent> evt;
    rv = docEvent->CreateEvent (NS_LITERAL_STRING ("UIEvent"), getter_AddRefs (evt));
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to create event: %#x"), rv);
	return;
    }

    nsCOMPtr<nsIDOMUIEvent> uiEvt (do_QueryInterface (evt, &rv));
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to QI for ui event: %#x"), rv);
	return;
    }

    rv = uiEvt->InitUIEvent (NS_LITERAL_STRING ("DOMActivate"),
			     PR_TRUE, PR_TRUE, NULL, 0);
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to init UI event: %#x"), rv);
	return;
    }

    nsCOMPtr<nsIDOMEventTarget> target (do_QueryInterface (elem, &rv));
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to QI for an event target: %#x"), rv);
	return;
    }

    PRBool doDefault;
    rv = target->DispatchEvent (evt, &doDefault);
    if (NS_FAILED (rv)) {
	g_warning (GOM_LOC ("Failed to dispatch event: %#x"), rv);
	return;
    }

    g_message (GOM_LOC ("Event dispatch returned %#x"), doDefault);
}

/* void onCreated (in nsIXTFElementWrapper wrapper); */
NS_IMETHODIMP
xgGtkElement::OnCreated (nsIXTFElementWrapper *wrapper)
{
    nsresult rv;

    mWrapper = wrapper;
    mWrapper->SetNotificationMask (NOTIFY_PARENT_CHANGED |
				   NOTIFY_WILL_INSERT_CHILD |
				   NOTIFY_WILL_APPEND_CHILD |
				   NOTIFY_CHILD_REMOVED);

    nsCOMPtr<nsIDOMElement> elem;
    rv = mWrapper->GetElementNode (getter_AddRefs (elem));
    NS_ENSURE_SUCCESS (rv, rv);

    JSContext *jscx;
    rv = GetJSContext (&jscx);
    NS_ENSURE_SUCCESS (rv, rv);

    rv = DefineProperties (jscx, elem);
    NS_ENSURE_SUCCESS (rv, rv);

    if (GTK_IS_ENTRY (mObject)) {
	g_signal_connect (mObject, "activate",
			  G_CALLBACK (&xgGtkElement::WidgetActivate), this);
    }
    gtk_widget_show (GTK_WIDGET (mObject));

    return NS_OK;
}

/* void onDestroyed (); */
NS_IMETHODIMP
xgGtkElement::OnDestroyed ()
{
    mWrapper = NULL;
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isAttributeHandler; */
NS_IMETHODIMP
xgGtkElement::GetIsAttributeHandler (PRBool *aIsAttributeHandler)
{
    *aIsAttributeHandler = PR_TRUE;
    return NS_OK;
}

/* void getScriptingInterfaces (out unsigned long count, [array, size_is (count), retval] out nsIIDPtr array); */
NS_IMETHODIMP
xgGtkElement::GetScriptingInterfaces (PRUint32 *count, nsIID ***array)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willChangeDocument (in nsIDOMDocument newDoc); */
NS_IMETHODIMP
xgGtkElement::WillChangeDocument (nsIDOMDocument *newDoc)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void documentChanged (in nsIDOMDocument newDoc); */
NS_IMETHODIMP
xgGtkElement::DocumentChanged (nsIDOMDocument *newDoc)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willChangeParent (in nsIDOMElement newParent); */
NS_IMETHODIMP
xgGtkElement::WillChangeParent (nsIDOMElement *newParent)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

static void
append_child_attrs_foreach (gpointer key, gpointer value, gpointer user_data)
{
    if (!value) {
	return;
    }

    GParamSpec *spec;
    GError *error = NULL;
    GValue gval = { 0 };

    GtkWidget *child = GTK_WIDGET (user_data);
    GtkWidget *parent = gtk_widget_get_parent (child);

    spec = gtk_container_class_find_child_property (G_OBJECT_GET_CLASS (parent), (char *)key);
    if (!spec ||
        G_TYPE_FUNDAMENTAL (G_PARAM_SPEC_VALUE_TYPE (spec)) == G_TYPE_OBJECT) {
        return;
    }
#if 1
    g_print ("found child property %s.%s on %s\n",
             g_type_name (G_TYPE_FROM_INSTANCE (parent)),
             spec->name,
             g_type_name (G_TYPE_FROM_INSTANCE (child)));
#endif
    if (gtk_builder_value_from_string (NULL, spec,
				       g_value_get_string ((const GValue *)value),
				       &gval, &error)) {
        gtk_container_child_set_property (GTK_CONTAINER (parent), child,
                                          spec->name, &gval);
        g_value_unset (&gval);
    } else {
        g_print ("Error getting value_from_string: %s\n",
                 error->message);
	g_error_free (error);
    }
}

/* void parentChanged (in nsIDOMElement newParent); */
NS_IMETHODIMP
xgGtkElement::ParentChanged (nsIDOMElement *newParent)
{
    /* no need for this rigamarole... */
    if (GTK_IS_WINDOW (mObject)) {
	return NS_OK;
    }

    /* just as a sanity check */
    nsCOMPtr<xgIGObjectHolder> wrappedParent (do_QueryInterface (newParent));
    GObject *parent = NULL;
    if (!wrappedParent || NS_FAILED (wrappedParent->GetGObject (&parent)) ||
	!GTK_IS_CONTAINER (parent)) {
	g_warning (GOM_LOC ("Could not get parent widget for node: <%s>"),
		   G_OBJECT_TYPE_NAME (mObject));
	return NS_ERROR_FAILURE;
    }
    g_assert (GTK_WIDGET (parent) == gtk_widget_get_parent (GTK_WIDGET (mObject)));

    nsresult rv;
    nsCOMPtr<nsIDOMElement> elem;
    rv = mWrapper->GetElementNode (getter_AddRefs (elem));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIDOMNamedNodeMap> attrs;
    rv = elem->GetAttributes (getter_AddRefs (attrs));
    NS_ENSURE_SUCCESS (rv, rv);

    PRUint32 len;
    rv = attrs->GetLength (&len);
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIDOMNode> attr;
    nsAutoString attrName;
    nsAutoString attrValue;
    const char *camel_prop;
    GValue gval = { 0 };

    for (PRUint32 i = 0; i < len; i++) {
	rv = attrs->Item (i, getter_AddRefs (attr));
	NS_ENSURE_SUCCESS (rv, rv);

	rv = attr->GetNodeName (attrName);
	NS_ENSURE_SUCCESS (rv, rv);

	NS_ConvertUTF16toUTF8 attrCName (attrName);
	camel_prop = gom_camel_uncase (attrCName.get ());

	rv = attr->GetNodeValue (attrValue);
	NS_ENSURE_SUCCESS (rv, rv);

	g_value_init (&gval, G_TYPE_STRING);
	g_value_set_string (&gval, NS_ConvertUTF16toUTF8 (attrValue).get ());
	append_child_attrs_foreach ((gpointer)camel_prop,
				    (gpointer)&gval,
				    (gpointer)mObject);

	GOM_CAMEL_FREE (camel_prop, attrCName.get ());
	g_value_unset (&gval);
    }

    return NS_OK;
}

/* void willInsertChild (in nsIDOMNode child, in unsigned long index); */
NS_IMETHODIMP
xgGtkElement::WillInsertChild (nsIDOMNode *child, PRUint32 index)
{
    return WillAppendChild (child);
}

/* void childInserted (in nsIDOMNode child, in unsigned long index); */
NS_IMETHODIMP
xgGtkElement::ChildInserted (nsIDOMNode *child, PRUint32 index)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willAppendChild (in nsIDOMNode child); */
NS_IMETHODIMP
xgGtkElement::WillAppendChild (nsIDOMNode *child)
{
    if (!GTK_IS_CONTAINER (mObject)) {
	return NS_ERROR_FAILURE;
    }

    nsCOMPtr<xgIGObjectHolder> wrappedChild (do_QueryInterface (child));
    GObject *widget = NULL;
    if (!wrappedChild || NS_FAILED (wrappedChild->GetGObject (&widget)) ||
	!GTK_IS_WIDGET (widget)) {
	return NS_ERROR_FAILURE;
    }

    gtk_container_add (GTK_CONTAINER (mObject), GTK_WIDGET (widget));
    return NS_OK;
}

/* void childAppended (in nsIDOMNode child); */
NS_IMETHODIMP
xgGtkElement::ChildAppended (nsIDOMNode *child)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willRemoveChild (in unsigned long index); */
NS_IMETHODIMP
xgGtkElement::WillRemoveChild (PRUint32 index)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void childRemoved (in unsigned long index); */
NS_IMETHODIMP
xgGtkElement::ChildRemoved (PRUint32 index)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willSetAttribute (in nsIAtom name, in AString newValue); */
NS_IMETHODIMP
xgGtkElement::WillSetAttribute (nsIAtom *name, const nsAString & newValue)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void attributeSet (in nsIAtom name, in AString newValue); */
NS_IMETHODIMP
xgGtkElement::AttributeSet (nsIAtom *name, const nsAString & newValue)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willRemoveAttribute (in nsIAtom name); */
NS_IMETHODIMP
xgGtkElement::WillRemoveAttribute (nsIAtom *name)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void attributeRemoved (in nsIAtom name); */
NS_IMETHODIMP
xgGtkElement::AttributeRemoved (nsIAtom *name)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void beginAddingChildren (); */
NS_IMETHODIMP
xgGtkElement::BeginAddingChildren ()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void doneAddingChildren (); */
NS_IMETHODIMP
xgGtkElement::DoneAddingChildren ()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean handleDefault (in nsIDOMEvent aEvent); */
NS_IMETHODIMP
xgGtkElement::HandleDefault (nsIDOMEvent *aEvent, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void cloneState (in nsIDOMElement aElement); */
NS_IMETHODIMP
xgGtkElement::CloneState (nsIDOMElement *aElement)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMAttr accesskeyNode; */
NS_IMETHODIMP
xgGtkElement::GetAccesskeyNode (nsIDOMAttr **aAccesskeyNode)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void performAccesskey (); */
NS_IMETHODIMP
xgGtkElement::PerformAccesskey ()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean handlesAttribute (in nsIAtom name); */
NS_IMETHODIMP
xgGtkElement::HandlesAttribute (nsIAtom *name, PRBool *_retval)
{
    GOM_ATOM_TO_GSTRING_RETURN (prop, name, NS_ERROR_INVALID_ARG);
    GParamSpec *spec;
    guint signal_id;
    *_retval = Resolve (prop, &spec, &signal_id) ? PR_TRUE : PR_FALSE;
    g_print (GOM_LOC ("%s -> %d\n"), prop, *_retval);
    return NS_OK;
}

/* void setAttribute (in nsIAtom name, in AString newValue); */
NS_IMETHODIMP
xgGtkElement::SetAttribute (nsIAtom *name, const nsAString &newValue)
{
    GOM_ATOM_TO_GSTRING_RETURN (prop, name, NS_ERROR_INVALID_ARG);
    GOM_ASTRING_TO_GSTRING_RETURN (value, newValue, NS_ERROR_INVALID_ARG);

    GParamSpec *spec;
    guint signal_id;
    if (!Resolve (prop, &spec, &signal_id)) {
	return NS_ERROR_FAILURE;
    }
    if (!spec) {
	return NS_ERROR_FAILURE;
    }

    GError *error = NULL;
    GValue gval = { 0 };
    if (G_TYPE_FUNDAMENTAL (G_PARAM_SPEC_VALUE_TYPE (spec)) == G_TYPE_OBJECT) {
	g_warning (GOM_LOC ("Attribute %s.%s is a %s, which a string cannot be converted to"),
		   G_OBJECT_TYPE_NAME (mObject), prop,
		   g_type_name (G_PARAM_SPEC_VALUE_TYPE (spec)));
	return NS_ERROR_FAILURE;
    } else if (gtk_builder_value_from_string (NULL, spec, value, &gval, &error)) {
	g_object_set_property (mObject, spec->name, &gval);
	g_value_unset (&gval);
    } else {
	g_warning (GOM_LOC ("Could not get value from string: '%s'"), value);
	return NS_ERROR_FAILURE;
    }
    return NS_OK;
}

/* void removeAttribute (in nsIAtom name); */
NS_IMETHODIMP
xgGtkElement::RemoveAttribute (nsIAtom *name)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* AString getAttribute (in nsIAtom name); */
NS_IMETHODIMP
xgGtkElement::GetAttribute (nsIAtom *name, nsAString &_retval)
{
    GOM_ATOM_TO_GSTRING_RETURN (prop, name, NS_ERROR_INVALID_ARG);

    GParamSpec *spec;
    guint signal_id;
    if (!Resolve (prop, &spec, &signal_id)) {
	return NS_ERROR_FAILURE;
    }
    if (!spec) {
	return NS_ERROR_FAILURE;
    }

    GValue gval = { 0 };
    g_value_init (&gval, G_TYPE_STRING);
    g_object_get_property (G_OBJECT (mObject), spec->name, &gval);
    if (G_VALUE_HOLDS (&gval, G_TYPE_STRING)) {
	_retval = NS_ConvertUTF8toUTF16 (nsCAutoString (g_value_get_string (&gval)));	
	g_value_unset (&gval);
	return NS_OK;
    }
    g_value_unset (&gval);
    return NS_ERROR_FAILURE;
}

/* boolean hasAttribute (in nsIAtom name); */
NS_IMETHODIMP
xgGtkElement::HasAttribute (nsIAtom *name, PRBool *_retval)
{
    GOM_ATOM_TO_GSTRING_RETURN (prop, name, NS_ERROR_INVALID_ARG);

    GParamSpec *spec;
    guint signal_id;
    *_retval = Resolve (prop, &spec, &signal_id);

    return NS_OK;
}

/* unsigned long getAttributeCount (); */
NS_IMETHODIMP
xgGtkElement::GetAttributeCount (PRUint32 *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIAtom getAttributeNameAt (in unsigned long index); */
NS_IMETHODIMP
xgGtkElement::GetAttributeNameAt (PRUint32 index, nsIAtom **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

nsresult
xgGtkElement::GetJSContext (JSContext **jscx)
{
    nsresult rv;

    nsCOMPtr<nsIDOMElement> elem;
    rv = mWrapper->GetElementNode (getter_AddRefs (elem));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIDOMDocument> doc;
    rv = elem->GetOwnerDocument (getter_AddRefs (doc));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIDocument> nsdoc (do_QueryInterface (doc, &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIScriptGlobalObject> global (nsdoc->GetScriptGlobalObject ());
    NS_ENSURE_TRUE (global, NS_ERROR_UNEXPECTED);

    rv = global->EnsureScriptEnvironment (nsIProgrammingLanguage::JAVASCRIPT);
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIScriptContext> cx (global->GetScriptContext (nsIProgrammingLanguage::JAVASCRIPT));
    NS_ENSURE_TRUE (cx, NS_ERROR_UNEXPECTED);

    *jscx = (JSContext *)cx->GetNativeContext ();
    NS_ENSURE_TRUE (*jscx, NS_ERROR_UNEXPECTED);

    return NS_OK;
}
