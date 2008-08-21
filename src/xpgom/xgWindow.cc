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

#define NS_HandleScriptError XG_HandleScriptError

#include "xpgom/xgWindow.hh"
#include "xgIParser.hh"

#include <nsComponentManagerUtils.h>
#include <nsDOMCID.h>
#include <nsIClassInfoImpl.h>
#include <nsIDocument.h>
#include <nsIDOMDocument.h>
#include <nsIDOMElement.h>
#include <nsIDOMLocation.h>
#include <nsIDOMNavigator.h>
#include <nsIDOMScriptObjectFactory.h>
#include <nsIScriptContext.h>
#include <nsIScriptRuntime.h>
#include <nsMemory.h>
#include <nsServiceManagerUtils.h>
#include <nsStringAPI.h>
#include <nsIURL.h>
#include <nsIIOService.h>

#include <glib/gmacros.h>
#include <glib/gmessages.h>

#include <gtk/gtkmain.h>

#include "gommacros.h"

static NS_DEFINE_CID (kDOMScriptObjectFactoryCID, NS_DOM_SCRIPT_OBJECT_FACTORY_CID);

class xgLocation : public nsIDOMLocation
{
    friend class xgWindow;
public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSIDOMLOCATION

    xgLocation (xgWindow *aWindow);

private:
    ~xgLocation();

protected:
    xgWindow *mWindow;
    nsCOMPtr<nsIURI> mUri;
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

NS_IMPL_ISUPPORTS7_CI (xgWindow,
		       nsIScriptGlobalObject,
		       nsIDOMWindow,
		       nsIDOMWindow2,
		       nsIDOMJSWindow,
		       nsIDOMWindowInternal,
		       nsISupportsWeakReference,
		       xgIWindow)

nsresult
xgWindow::Init ()
{
    nsresult rv;
    mNavigator = do_CreateInstance ("@ilovegom.org/navigator;1", &rv);
    NS_ENSURE_SUCCESS (rv, rv);

    mLocation = new xgLocation (this);
    NS_ENSURE_TRUE (mLocation, NS_ERROR_OUT_OF_MEMORY);

    mParser = do_CreateInstance ("@ilovegom.org/parser;1", &rv);
    NS_ENSURE_SUCCESS (rv, rv);

    mListener = new xgWindowParserListener (this);
    NS_ENSURE_TRUE (mListener, NS_ERROR_OUT_OF_MEMORY);

    return mLocation->Assign (NS_LITERAL_STRING ("about:blank"));
}

// nsIDOMWindowInternal
/* readonly attribute nsIDOMWindowInternal window; */
NS_IMETHODIMP
xgWindow::GetWindow (nsIDOMWindowInternal **aWindow)
{
    NS_ADDREF (*aWindow = this);
    return NS_OK;
}

/* readonly attribute nsIDOMWindowInternal self; */
NS_IMETHODIMP
xgWindow::GetSelf (nsIDOMWindowInternal **aSelf)
{
    NS_ADDREF (*aSelf = this);
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

/* readonly attribute nsIDOMDocument document; */
NS_IMETHODIMP
xgWindow::GetDocument (nsIDOMDocument **aDocument)
{
    NS_IF_ADDREF (*aDocument = mDocument);
    return NS_OK;
}

/* void close (); */
NS_IMETHODIMP
xgWindow::Close()
{
    gtk_main_quit ();
    mDocument = NULL;
    return NS_OK;
}

// nsIDOMJSWindow
/* void dump (in DOMString str); */
NS_IMETHODIMP xgWindow::Dump(const nsAString &str)
{
    GOM_ASTRING_TO_GSTRING_RETURN (gstr, str, NS_ERROR_INVALID_ARG);
    g_message ("JAVASCRIPT: %s\n", gstr);
    return NS_OK;
}

/* xgIWindow */
/* void main (); */
NS_IMETHODIMP
xgWindow::Main ()
{
    gtk_main ();
    return NS_OK;
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
    nsresult rv;
    void * global = NULL;
    if (aContext) {
	aContext->WillInitializeContext ();
	rv = aContext->InitContext (this);
	NS_ENSURE_TRUE (rv, rv);

	aContext->DidInitializeContext ();	

	global = aContext->GetNativeGlobal ();
	NS_ENSURE_TRUE (global, NS_ERROR_FAILURE);

	rv = aContext->InitClasses (global);
	NS_ENSURE_TRUE (rv, rv);
    }
    PRUint32 lang_idx = NS_STID_INDEX (lang);
    mContext[lang_idx] = aContext;
    mGlobal[lang_idx] = global;

    if (aContext && mDocument) {
	aContext->DidSetDocument (mDocument, global);
    }

    return NS_OK;
}

void
xgWindow::OnFinalize (PRUint32 aLangID, void *aScriptGlobal)
{
    if (!NS_STID_VALID (aLangID) ||
	mGlobal[NS_STID_INDEX (aLangID)] != aScriptGlobal) {
	return;
    }
    mGlobal[NS_STID_INDEX (aLangID)] = NULL;
}

nsresult
xgWindow::DidSetDocument ()
{
    NS_ENSURE_TRUE (mDocument, NS_ERROR_INVALID_ARG);

    nsCOMPtr<nsIDocument> doc = do_QueryInterface (mDocument);
    if (doc) {
	doc->SetScriptGlobalObject (this);
	doc->SetDocumentURI (mLocation->mUri);
    } else {
	g_warning (GOM_LOC ("mDocument does not support nsIDocument; things will likely go awry"));
    }

    PRUint32 lang;
    nsresult rv;
    NS_STID_FOR_INDEX (lang) {
	nsIScriptContext *cx = mContext[lang];
	if (cx) {
	    rv = cx->InitClasses (mGlobal[lang]);
	    if (NS_SUCCEEDED (rv)) {
		cx->DidSetDocument (mDocument, mGlobal[lang]);
	    }
	}
    }

    return NS_OK;
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

/* void assign (in DOMString url); */
NS_IMETHODIMP
xgLocation::Assign (const nsAString &url)
{
    nsresult rv;
    nsCOMPtr<nsIIOService> io (do_GetService ("@mozilla.org/network/io-service;1", &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    g_message (GOM_LOC ("Setting URI to: %s"), NS_ConvertUTF16toUTF8 (url).get ());

    nsCOMPtr<nsIURI> uri;
    rv = io->NewURI (NS_ConvertUTF16toUTF8 (url), "UTF-8", mUri, getter_AddRefs (uri));
    NS_ENSURE_SUCCESS (rv, rv);

    mUri = uri;

    g_message (GOM_LOC ("mUri is now: %p"), mUri.get());

    nsCOMPtr<nsIDOMDocument> doc;
    return mWindow->mParser->ParseURI (url, mWindow->mListener, getter_AddRefs (doc));
}

/* DOMString toString (); */
NS_IMETHODIMP
xgLocation::ToString (nsAString &_retval)
{
    NS_ENSURE_TRUE (mUri, NS_ERROR_FAILURE);

    g_message (GOM_LOC ("mUri is: %p"), mUri.get());

    nsCAutoString s;
    nsresult rv = mUri->GetSpec (s);
    NS_ENSURE_SUCCESS (rv, rv);

    return NS_CStringToUTF16 (s, NS_CSTRING_ENCODING_UTF8, _retval);
}

/*
 * an xgParserListener
 */

xgWindowParserListener::xgWindowParserListener (xgWindow *aWindow)
    : mWindow (aWindow)
{
}

xgWindowParserListener::~xgWindowParserListener ()
{
}

/* Implementation file */
NS_IMPL_ISUPPORTS1 (xgWindowParserListener, xgIParserListener)

/* void documentCreated (in nsIDOMDocument document); */
NS_IMETHODIMP
xgWindowParserListener::DocumentCreated (nsIDOMDocument *aDocument)
{
    // friend access!
    mWindow->mDocument = aDocument;
    return mWindow->DidSetDocument ();
}

PRBool
XG_HandleScriptError (nsIScriptGlobalObject *aScriptGlobal,
		      nsScriptErrorEvent *aErrorEvent,
		      nsEventStatus *aEventStatus)
{
    return PR_FALSE;
}

#include "xgWindowUnimplemented.cc"
