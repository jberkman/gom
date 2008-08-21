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

#include "xpgom/xgScriptElement.hh"

#include <nsDOMError.h>
#include <nsIAtom.h>
#include <nsIChannel.h>
#include <nsIDOM3Document.h>
#include <nsIDOMDocument.h>
#include <nsIDOMElement.h>
#include <nsIDOMScriptObjectFactory.h>
#include <nsIDocument.h>
#include <nsIIOService.h>
#include <nsIInputStream.h>
#include <nsIProgrammingLanguage.h>
#include <nsIScriptContext.h>
#include <nsIScriptGlobalObject.h>
#include <nsISupportsImpl.h>
#include <nsServiceManagerUtils.h>
#include <nsIXTFElementWrapper.h>

#include <glib/gmessages.h>

#include <stdio.h>

#include "gommacros.h"

static NS_DEFINE_CID (kDOMScriptObjectFactoryCID, NS_DOM_SCRIPT_OBJECT_FACTORY_CID);

xgScriptElement::xgScriptElement()
    : mLangId (nsIProgrammingLanguage::UNKNOWN),
      mActive (true)
{
}

xgScriptElement::~xgScriptElement()
{
}

NS_IMPL_ISUPPORTS1 (xgScriptElement, nsIXTFElement)

nsresult
xgScriptElement::DownloadScript ()
{
    NS_ENSURE_TRUE (mActive, NS_ERROR_FAILURE);
    
    nsresult rv;
    nsCOMPtr<nsIIOService> io (do_GetService ("@mozilla.org/network/io-service;1", &rv));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIChannel> chan;
    rv = io->NewChannel (mSrc, "UTF-8", mDocumentURI, getter_AddRefs (chan));
    mDocumentURI = NULL;
    mSrc.Truncate ();
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIURI> uri;
    rv = chan->GetOriginalURI (getter_AddRefs (uri));
    if (NS_SUCCEEDED (rv)) {
	nsCAutoString curi;
	if (NS_SUCCEEDED (uri->GetSpec (curi))) {
	    g_message (GOM_LOC ("<script> downloading %s..."), curi.get());
	}
    }

    nsCOMPtr<nsIInputStream> stream;
    rv = chan->Open (getter_AddRefs (stream));
    NS_ENSURE_SUCCESS (rv, rv);

    nsCAutoString script;
    PRUint32 bytes_read;
    char buf[BUFSIZ];
    while (1) {
	rv = stream->Read (buf, BUFSIZ, &bytes_read);
	if (rv == NS_BASE_STREAM_WOULD_BLOCK) {
	    continue;
	}
	NS_ENSURE_SUCCESS (rv, rv);
	if (bytes_read == 0) {
	    stream->Close();
	    break;
	}
	script.Append (buf, bytes_read);
    }
    mScript = NS_ConvertUTF8toUTF16 (script);

    return NS_OK;
}

nsresult
xgScriptElement::EvaluateScript ()
{
    NS_ENSURE_TRUE (mActive, NS_ERROR_FAILURE);
    mActive = false;

    NS_ENSURE_FALSE (mScript.IsEmpty(), NS_OK);
    
    nsCOMPtr<nsIScriptGlobalObject> global = do_QueryReferent (mGlobal);
    NS_ENSURE_TRUE (global, NS_ERROR_FAILURE);

    nsresult rv = global->EnsureScriptEnvironment (mLangId);
    NS_ENSURE_SUCCESS (rv, rv);

    nsCOMPtr<nsIScriptContext> cx (global->GetScriptContext (mLangId));
    NS_ENSURE_TRUE (cx, NS_ERROR_FAILURE);

    nsAutoString aRet;
    PRBool isUndefined;
    rv = cx->EvaluateString (mScript, NULL, NULL, NULL, 0, 0, &aRet, &isUndefined);
    mScript.Truncate ();
    mGlobal = NULL;
    NS_ENSURE_SUCCESS (rv, rv);

    g_message (GOM_LOC ("<script> evaluated to %s (%d)"),
	       isUndefined ? "<undefined>" : NS_ConvertUTF16toUTF8 (aRet).get(),
	       isUndefined);

    return NS_OK;
}

nsresult
xgScriptElement::MaybeEvaluateScript ()
{
    NS_ENSURE_TRUE (mActive, NS_ERROR_FAILURE);
    nsresult rv;
    if (mLangId != nsIProgrammingLanguage::UNKNOWN) {
	if (!mSrc.IsEmpty ()){
	    if (!mDocumentURI) {
		return NS_OK;
	    }
	    rv = DownloadScript ();
	    mActive = NS_SUCCEEDED (rv);
	    NS_ENSURE_SUCCESS (rv, rv);
	}
	if (!mScript.IsEmpty ()) {
	    rv = EvaluateScript ();
	    NS_ENSURE_FALSE (mActive, NS_ERROR_FAILURE);
	    NS_ENSURE_SUCCESS (rv, rv);
	}
    }
    return NS_OK;
}

/* void onCreated (in nsIXTFElementWrapper wrapper); */
NS_IMETHODIMP
xgScriptElement::OnCreated (nsIXTFElementWrapper *wrapper)
{
    nsCOMPtr<nsIDOMElement> elem;
    wrapper->GetElementNode (getter_AddRefs (elem));

    // unset in OnDestroyed
    mElement = getter_AddRefs (NS_GetWeakReference (elem));

    wrapper->SetNotificationMask (NOTIFY_DOCUMENT_CHANGED |
				  NOTIFY_WILL_INSERT_CHILD |
				  NOTIFY_WILL_APPEND_CHILD |
				  NOTIFY_ATTRIBUTE_SET |
				  NOTIFY_DONE_ADDING_CHILDREN);

    nsCOMPtr<nsIDOMDocument> doc;
    nsresult rv = elem->GetOwnerDocument (getter_AddRefs (doc));
    NS_ENSURE_SUCCESS (rv, rv);

    return NS_OK;
}

/* void onDestroyed (); */
NS_IMETHODIMP xgScriptElement::OnDestroyed()
{
    return NS_OK;
}

/* readonly attribute boolean isAttributeHandler; */
NS_IMETHODIMP xgScriptElement::GetIsAttributeHandler(PRBool *aIsAttributeHandler)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void getScriptingInterfaces (out unsigned long count, [array, size_is (count), retval] out nsIIDPtr array); */
NS_IMETHODIMP xgScriptElement::GetScriptingInterfaces(PRUint32 *count, nsIID * **array)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willChangeDocument (in nsIDOMDocument newDoc); */
NS_IMETHODIMP xgScriptElement::WillChangeDocument(nsIDOMDocument *newDoc)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void documentChanged (in nsIDOMDocument newDoc); */
NS_IMETHODIMP
xgScriptElement::DocumentChanged (nsIDOMDocument *newDoc)
{
    if (!mActive) {
	return NS_OK;
    }

    nsCOMPtr<nsIScriptGlobalObject> global = do_QueryReferent (mGlobal);
    NS_ENSURE_FALSE (global, NS_ERROR_FAILURE);

    nsCOMPtr<nsIDocument> doc (do_QueryInterface (newDoc));
    if (doc) {
	global = doc->GetScriptGlobalObject ();
    }
    mGlobal = getter_AddRefs (NS_GetWeakReference (global));
    if (!global) {
	g_warning (GOM_LOC ("Could not find a ScriptedGlobalObject; disabling this script"));
	mActive = false;
    }

    mDocumentURI = doc->GetDocumentURI ();
    if (!mDocumentURI) {
	nsCOMPtr<nsIDOM3Document> doc3 (do_QueryInterface (newDoc));
	NS_ENSURE_TRUE (doc3, NS_OK);

	nsCOMPtr<nsIIOService> io (do_GetService ("@mozilla.org/network/io-service;1"));
	NS_ENSURE_TRUE (io, NS_OK);

	nsAutoString uriStr;
	nsresult rv = doc3->GetDocumentURI (uriStr);
	NS_ENSURE_SUCCESS (rv, NS_OK);
	
	rv = io->NewURI (NS_ConvertUTF16toUTF8 (uriStr), "UTF-8", NULL, getter_AddRefs (mDocumentURI));
	NS_ENSURE_SUCCESS (rv, NS_OK);
    }

    nsCAutoString spec;
    mDocumentURI->GetSpec (spec);
    g_message (GOM_LOC ("Have documentURI: %s"), spec.get());
    return MaybeEvaluateScript ();
}

/* void willChangeParent (in nsIDOMElement newParent); */
NS_IMETHODIMP xgScriptElement::WillChangeParent(nsIDOMElement *newParent)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void parentChanged (in nsIDOMElement newParent); */
NS_IMETHODIMP xgScriptElement::ParentChanged(nsIDOMElement *newParent)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willInsertChild (in nsIDOMNode child, in unsigned long index); */
NS_IMETHODIMP
xgScriptElement::WillInsertChild (nsIDOMNode *child, PRUint32 index)
{
    return WillAppendChild (child);
}

/* void childInserted (in nsIDOMNode child, in unsigned long index); */
NS_IMETHODIMP
xgScriptElement::ChildInserted(nsIDOMNode *child, PRUint32 index)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willAppendChild (in nsIDOMNode child); */
NS_IMETHODIMP xgScriptElement::WillAppendChild (nsIDOMNode *child)
{
    PRUint16 childType;
    nsresult rv = child->GetNodeType (&childType);
    NS_ENSURE_SUCCESS (rv, rv);

    NS_ENSURE_TRUE (childType == nsIDOMNode::TEXT_NODE ||
		    childType == nsIDOMNode::CDATA_SECTION_NODE,
		    NS_ERROR_DOM_HIERARCHY_REQUEST_ERR);

    if (!mActive) {
	return NS_OK;
    }

    nsAutoString text;
    rv = child->GetNodeValue (text);
    NS_ENSURE_SUCCESS (rv, rv);

    mScript.Append (text);
    return NS_OK;
}

/* void childAppended (in nsIDOMNode child); */
NS_IMETHODIMP xgScriptElement::ChildAppended(nsIDOMNode *child)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willRemoveChild (in unsigned long index); */
NS_IMETHODIMP xgScriptElement::WillRemoveChild(PRUint32 index)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void childRemoved (in unsigned long index); */
NS_IMETHODIMP xgScriptElement::ChildRemoved(PRUint32 index)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void willSetAttribute (in nsIAtom name, in AString newValue); */
NS_IMETHODIMP xgScriptElement::WillSetAttribute(nsIAtom *name, const nsAString &newValue)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void attributeSet (in nsIAtom name, in AString newValue); */
NS_IMETHODIMP
xgScriptElement::AttributeSet (nsIAtom *name, const nsAString &newValue)
{
    if (!mActive) {
	return NS_OK;
    }

    nsresult rv = NS_OK;
    if (name->Equals (NS_LITERAL_STRING ("src"))) {
	NS_ENSURE_TRUE (mSrc.IsEmpty(), NS_OK);
	mSrc = NS_ConvertUTF16toUTF8 (newValue);
	g_message (GOM_LOC ("got src=\"%s\""), mSrc.get());
    } else if (name->Equals (NS_LITERAL_STRING ("type"))) {
	nsCOMPtr<nsIDOMScriptObjectFactory> sof (do_GetService (kDOMScriptObjectFactoryCID, &rv));
	NS_ENSURE_SUCCESS (rv, rv);

	rv = sof->GetIDForScriptType (newValue, &mLangId);
	NS_ENSURE_SUCCESS (rv, rv);
    }

    return MaybeEvaluateScript ();
}

/* void willRemoveAttribute (in nsIAtom name); */
NS_IMETHODIMP xgScriptElement::WillRemoveAttribute(nsIAtom *name)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void attributeRemoved (in nsIAtom name); */
NS_IMETHODIMP xgScriptElement::AttributeRemoved(nsIAtom *name)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void beginAddingChildren (); */
NS_IMETHODIMP xgScriptElement::BeginAddingChildren()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void doneAddingChildren (); */
NS_IMETHODIMP
xgScriptElement::DoneAddingChildren ()
{
    if (!mActive) {
	return NS_OK;
    }
    return MaybeEvaluateScript ();
}

/* boolean handleDefault (in nsIDOMEvent aEvent); */
NS_IMETHODIMP xgScriptElement::HandleDefault(nsIDOMEvent *aEvent, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void cloneState (in nsIDOMElement aElement); */
NS_IMETHODIMP xgScriptElement::CloneState(nsIDOMElement *aElement)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDOMAttr accesskeyNode; */
NS_IMETHODIMP xgScriptElement::GetAccesskeyNode(nsIDOMAttr * *aAccesskeyNode)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void performAccesskey (); */
NS_IMETHODIMP xgScriptElement::PerformAccesskey()
{
    XG_RETURN_NOT_IMPLEMENTED;
}
