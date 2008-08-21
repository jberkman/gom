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

#include "xpgom/xgDocShell.hh"

#include <nsComponentManagerUtils.h>
#include <nsDOMCID.h>
#include <nsIChannel.h>
#include <nsIDOMScriptObjectFactory.h>
#include <nsIDOMXMLDocument.h>
#include <nsIDocument.h>
#include <nsIIOService.h>
#include <nsIPrincipal.h>
#include <nsIScriptGlobalObject.h>
#include <nsIStreamListener.h>
#include <nsIThread.h>
#include <nsIThreadManager.h>
#include <nsMemory.h>
#include <nsPIDOMWindow.h>
#include <nsServiceManagerUtils.h>
#include <nsThreadUtils.h>

#include <glib/gmessages.h>

#include "gommacros.h"

static NS_DEFINE_CID(kDOMScriptObjectFactoryCID, NS_DOM_SCRIPT_OBJECT_FACTORY_CID);

xgDocShell::xgDocShell()
{
}

xgDocShell::~xgDocShell()
{
}

NS_IMPL_ADDREF_INHERITED(xgDocShell, nsDocLoader)
NS_IMPL_RELEASE_INHERITED(xgDocShell, nsDocLoader)

NS_INTERFACE_MAP_BEGIN(xgDocShell)
NS_INTERFACE_MAP_ENTRY(nsIDocShell)
NS_INTERFACE_MAP_ENTRY(nsIDocShellTreeItem)
NS_INTERFACE_MAP_ENTRY(nsIDocShellTreeNode)
NS_INTERFACE_MAP_ENTRY(nsIPrompt)
NS_INTERFACE_MAP_ENTRY(nsIScriptGlobalObjectOwner)
NS_INTERFACE_MAP_ENTRY(nsIWebProgressListener)
#if 0
NS_IMPL_QUERY_CLASSINFO(xgDocShell)
#endif
NS_INTERFACE_MAP_END_INHERITING(nsDocLoader)

#if 0
NS_CLASSINFO_HELPER_BEGIN
NS_CLASSINFO_HELPER_ENTRY(nsIRequestObserver)
NS_CLASSINFO_HELPER_ENTRY(nsIDocumentLoader)
NS_CLASSINFO_HELPER_ENTRY(nsIWebProgress)
NS_CLASSINFO_HELPER_ENTRY(nsIProgressEventSink)   
NS_CLASSINFO_HELPER_ENTRY(nsIInterfaceRequestor)
NS_CLASSINFO_HELPER_ENTRY(nsIChannelEventSink)
NS_CLASSINFO_HELPER_ENTRY(nsISecurityEventSink)
NS_CLASSINFO_HELPER_ENTRY(nsISupportsPriority)



NS_IMPL_CI_INTERFACE_GETTER6(xgDocShell, nsIDocShell, nsIDocShellTreeNode, nsIDocShellTreeItem, nsIScriptGlobalObjectOwner, nsIPrompt)
#endif

// nsIScriptGlobalObjectOwner methods
nsIScriptGlobalObject *
xgDocShell::GetScriptGlobalObject ()
{
    if (!mGlobal) {
	nsresult rv;
	nsCOMPtr<nsIDOMScriptObjectFactory> factory (do_GetService (kDOMScriptObjectFactoryCID, &rv));
	NS_ENSURE_SUCCESS (rv, NULL);

	nsCOMPtr<nsIScriptGlobalObject> global;
	rv = factory->NewScriptGlobalObject (true, false, getter_AddRefs (global));
	NS_ENSURE_SUCCESS (rv, NULL);

	// you'd think it'd be able to figure this one out on its own...
	rv = global->EnsureScriptEnvironment (nsIProgrammingLanguage::JAVASCRIPT);
	NS_ENSURE_SUCCESS (rv, NULL);

	nsCOMPtr<nsPIDOMWindow> win (do_QueryInterface (global, &rv));
	NS_ENSURE_SUCCESS (rv, NULL);

	win->SetDocShell (this);
	
	mGlobal = global;
    }
    return mGlobal;
}

/* [noscript] void loadURI (in nsIURI uri, in nsIDocShellLoadInfo loadInfo, in unsigned long aLoadFlags, in boolean firstParty); */
NS_IMETHODIMP
xgDocShell::LoadURI (nsIURI *uri, nsIDocShellLoadInfo *loadInfo, PRUint32 aLoadFlags, PRBool firstParty)
{
    nsresult rv;

    rv = uri->Clone (getter_AddRefs (mUri));
    NS_ENSURE_SUCCESS (rv, rv);

    FireOnLocationChange (this, NULL, mUri);

    nsCOMPtr<nsIThread> thread = do_GetCurrentThread();
    while (true) {
	if (!NS_ProcessNextEvent(thread))
	    break;
    }

#if 0
    // We set return to true unless there was a parsing error
    nsCOMPtr<nsIDOMNode> node = do_QueryInterface(GetRootContent());
    if (node) {
	nsAutoString name, ns;      
	if (NS_SUCCEEDED(node->GetLocalName(name)) &&
	    name.EqualsLiteral("parsererror") &&
	    NS_SUCCEEDED(node->GetNamespaceURI(ns)) &&
	    ns.EqualsLiteral("http://www.mozilla.org/newlayout/xml/parsererror.xml")) {
	    //return is already false
	} else {
	    *aReturn = PR_TRUE;
	}
    }
#endif
    return NS_OK;
}

/* void setCurrentURI (in nsIURI aURI); */
NS_IMETHODIMP
xgDocShell::SetCurrentURI (nsIURI *aURI)
{
    return LoadURI (aURI, NULL, 0, false);
}

/* attribute boolean allowJavascript; */
NS_IMETHODIMP
xgDocShell::GetAllowJavascript (PRBool *aAllowJavascript)
{
    *aAllowJavascript = true;
    return NS_OK;
}

/* readonly attribute long childCount; */
NS_IMETHODIMP
xgDocShell::GetChildCount (PRInt32 *aChildCount)
{
    *aChildCount = 0;
    return NS_OK;
}

#include "xgDocShellUnimplemented.cc"
