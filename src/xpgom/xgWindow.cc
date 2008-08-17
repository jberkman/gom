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

#include "xpgom/xgWindow.hh"
#include "xgIParser.hh"

#include <nsComponentManagerUtils.h>
#include <nsDOMCID.h>
#include <nsIClassInfoImpl.h>
#include <nsIDOMDocument.h>
#include <nsIDOMLocation.h>
#include <nsIDOMNavigator.h>
#include <nsIDOMScriptObjectFactory.h>
#include <nsIScriptContext.h>
#include <nsIScriptRuntime.h>
#include <nsMemory.h>
#include <nsServiceManagerUtils.h>
#include <nsStringAPI.h>

#include <glib/gmacros.h>
#include <glib/gmessages.h>

#include <gtk/gtkmain.h>

#include "gommacros.h"

static NS_DEFINE_CID (kDOMScriptObjectFactoryCID, NS_DOM_SCRIPT_OBJECT_FACTORY_CID);
class xgLocation : public nsIDOMLocation
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIDOMLOCATION

  xgLocation (xgWindow *aWindow);

private:
  ~xgLocation();

protected:
    xgWindow *mWindow;
};

class xgWindowParserListener : public xgIParserListener
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_XGIPARSERLISTENER

  xgWindowParserListener(xgWindow *aWindow);

private:
  ~xgWindowParserListener();

protected:
    xgWindow *mWindow;
};

xgWindow::xgWindow ()
{
    memset (mGlobal, 0, sizeof (mGlobal));
}

xgWindow::~xgWindow ()
{
    /* destructor code */
}

NS_IMPL_ISUPPORTS2_CI (xgWindow, xgIWindow, nsIScriptGlobalObject)

nsresult
xgWindow::Init ()
{
    nsresult rv;
    mNavigator = do_CreateInstance ("@ilovegom.org/navigator;1", &rv);
    NS_ENSURE_SUCCESS (rv, rv);

    mLocation = new xgLocation (this);
    NS_ENSURE_TRUE (mLocation, NS_ERROR_OUT_OF_MEMORY);
    return NS_OK;
}

/* readonly attribute xgIWindow window; */
NS_IMETHODIMP
xgWindow::GetWindow (xgIWindow **aWindow)
{
    NS_ADDREF (*aWindow = this);
    return NS_OK;
}

/* readonly attribute nsIDOMDocument document; */
NS_IMETHODIMP
xgWindow::GetDocument (nsIDOMDocument **aDocument)
{
    NS_IF_ADDREF (*aDocument = mDocument);
    return NS_OK;
}

nsresult
xgWindow::SetDocument (nsIDOMDocument *aDocument)
{
    NS_ENSURE_TRUE (aDocument, NS_ERROR_INVALID_ARG);

    nsresult rv = Init ();
    NS_ENSURE_SUCCESS (rv, rv);

    mDocument = aDocument;

    PRUint32 lang;
    NS_STID_FOR_INDEX (lang) {
	nsIScriptContext *cx = mContext[lang];
	if (cx) {
	    cx->WillInitializeContext ();
	    cx->InitClasses (mGlobal[lang]);
	    cx->DidInitializeContext ();
	    cx->DidSetDocument (aDocument, mGlobal[lang]);
	}
    }

    return NS_OK;
}

/* readonly attribute nsIDOMNavigator navigator; */
NS_IMETHODIMP
xgWindow::GetNavigator (nsIDOMNavigator **aNavigator)
{
    NS_IF_ADDREF (*aNavigator = mNavigator);
    return NS_OK;
}

/* readonly attribute nsIDOMLocation location; */
NS_IMETHODIMP
xgWindow::GetLocation (nsIDOMLocation **aLocation)
{
    NS_IF_ADDREF (*aLocation = mLocation);
    return NS_OK;
}

/* void alert (in DOMString text); */
NS_IMETHODIMP
xgWindow::Alert (const nsAString &text)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void run (); */
NS_IMETHODIMP
xgWindow::Run ()
{
    gtk_main ();
    return NS_OK;
}

/* void close (); */
NS_IMETHODIMP
xgWindow::Close ()
{
    gtk_main_quit ();
    return NS_OK;
}

/* void dump (in DOMString str); */
NS_IMETHODIMP
xgWindow::Dump (const nsAString &str)
{
    GOM_ASTRING_TO_GSTRING_RETURN (gstr, str, NS_ERROR_INVALID_ARG);
    g_message ("JAVASCRIPT: %s\n", gstr);
    return NS_OK;
}

/* long setTimeout (); */
NS_IMETHODIMP
xgWindow::SetTimeout (PRInt32 *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* long setInterval (); */
NS_IMETHODIMP
xgWindow::SetInterval (PRInt32 *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void clearTimeout (); */
NS_IMETHODIMP
xgWindow::ClearTimeout ()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void clearInterval (); */
NS_IMETHODIMP
xgWindow::ClearInterval ()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

nsresult
xgWindow::EnsureScriptEnvironment (PRUint32 aLangID)
{
    NS_ENSURE_TRUE (NS_STID_VALID (aLangID), NS_ERROR_INVALID_ARG);
    
    if (mGlobal[aLangID]) {
	return NS_OK;
    }

    nsresult rv;
    nsCOMPtr<nsIDOMScriptObjectFactory> sof (do_GetService (kDOMScriptObjectFactoryCID, &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIScriptRuntime> rt;
    rv = sof->GetScriptRuntimeByID (aLangID, getter_AddRefs (rt));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIScriptContext> cx;
    rv = rt->CreateContext (getter_AddRefs (cx));
    NS_ENSURE_SUCCESS (rv, rv);

    return SetScriptContext (aLangID, cx);
}

nsIScriptContext *
xgWindow::GetScriptContext (PRUint32 lang)
{
    NS_ENSURE_TRUE (NS_STID_VALID (lang), NULL);
    return mContext[NS_STID_INDEX (lang)];
}

void *
xgWindow::GetScriptGlobal (PRUint32 lang)
{
    NS_ENSURE_TRUE (NS_STID_VALID (lang), NULL);
    return mGlobal[NS_STID_INDEX (lang)];
}

nsresult
xgWindow::SetScriptContext (PRUint32 lang, nsIScriptContext *aContext)
{
    NS_ENSURE_TRUE (NS_STID_VALID (lang), NULL);

    mContext[NS_STID_INDEX (lang)] = aContext;
    if (aContext) {
	aContext->DidInitializeContext ();
    }
    mGlobal[NS_STID_INDEX (lang)] = aContext ? aContext->GetNativeGlobal () : NULL;

    return NS_OK;
}

void
xgWindow::OnFinalize (PRUint32 aLangID, void *aScriptGlobal)
{
    if (!NS_STID_VALID (aLangID)) {
	return;
    }
    mGlobal[NS_STID_INDEX (aLangID)] = NULL;
}

void
xgWindow::SetScriptsEnabled (PRBool aEnabled, PRBool aFireTimeouts)
{
    GOM_NOT_IMPLEMENTED;
}

nsresult
xgWindow::SetNewArguments (nsIArray *aArguments)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

nsresult
xgWindow::HandleScriptError (nsScriptErrorEvent *aErrorEvent,
			     nsEventStatus *aEventStatus)
{
    XG_RETURN_NOT_IMPLEMENTED;
}


/*
 * nsILocation implementation
 */

xgLocation::xgLocation (xgWindow *aWindow)
    : mWindow (aWindow)
{
}

xgLocation::~xgLocation ()
{
    mWindow = NULL;
}

NS_IMPL_ISUPPORTS1 (xgLocation, nsIDOMLocation)

/* attribute DOMString hash; */
NS_IMETHODIMP
xgLocation::GetHash (nsAString &aHash)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetHash (const nsAString &aHash)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute DOMString host; */
NS_IMETHODIMP
xgLocation::GetHost (nsAString &aHost)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetHost (const nsAString &aHost)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute DOMString hostname; */
NS_IMETHODIMP
xgLocation::GetHostname (nsAString &aHostname)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetHostname (const nsAString &aHostname)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute DOMString href; */
NS_IMETHODIMP
xgLocation::GetHref (nsAString &aHref)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetHref (const nsAString &aHref)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute DOMString pathname; */
NS_IMETHODIMP
xgLocation::GetPathname (nsAString &aPathname)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetPathname (const nsAString &aPathname)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute DOMString port; */
NS_IMETHODIMP
xgLocation::GetPort (nsAString &aPort)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetPort (const nsAString &aPort)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute DOMString protocol; */
NS_IMETHODIMP
xgLocation::GetProtocol (nsAString &aProtocol)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetProtocol (const nsAString &aProtocol)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute DOMString search; */
NS_IMETHODIMP
xgLocation::GetSearch (nsAString &aSearch)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP
xgLocation::SetSearch (const nsAString &aSearch)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] void reload (in boolean forceget); */
NS_IMETHODIMP
xgLocation::Reload (PRBool forceget)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void replace (in DOMString url); */
NS_IMETHODIMP
xgLocation::Replace (const nsAString &url)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void assign (in DOMString url); */
NS_IMETHODIMP
xgLocation::Assign (const nsAString &url)
{
    nsCOMPtr<xgIParserListener> listener (new xgWindowParserListener (mWindow));
    NS_ENSURE_TRUE (listener, NS_ERROR_OUT_OF_MEMORY);

    nsresult rv;
    nsCOMPtr<xgIParser> parser (do_CreateInstance ("@ilovegom.org/parser;1", &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIDOMDocument> doc;
    return parser->ParseURI (url, listener, getter_AddRefs (doc));
}

/* DOMString toString (); */
NS_IMETHODIMP
xgLocation::ToString (nsAString &_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/*
 * an xgParserListener
 */

xgWindowParserListener::xgWindowParserListener(xgWindow *aWindow)
    : mWindow (aWindow)
{
    NS_IF_ADDREF (mWindow);
}

xgWindowParserListener::~xgWindowParserListener()
{
    NS_IF_RELEASE (mWindow);
}

/* Implementation file */
NS_IMPL_ISUPPORTS1(xgWindowParserListener, xgIParserListener)

/* void documentCreated (in nsIDOMDocument document); */
NS_IMETHODIMP
xgWindowParserListener::DocumentCreated (nsIDOMDocument *aDocument)
{
    // friend access!
    return mWindow->SetDocument (aDocument);
}
