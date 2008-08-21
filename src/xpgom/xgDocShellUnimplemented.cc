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

/* [noscript] void loadStream (in nsIInputStream aStream, in nsIURI aURI, in ACString aContentType, in ACString aContentCharset, in nsIDocShellLoadInfo aLoadInfo); */
NS_IMETHODIMP xgDocShell::LoadStream(nsIInputStream *aStream, nsIURI *aURI, const nsACString & aContentType, const nsACString & aContentCharset, nsIDocShellLoadInfo *aLoadInfo)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] void internalLoad (in nsIURI aURI, in nsIURI aReferrer, in nsISupports aOwner, in PRUint32 aFlags, in wstring aWindowTarget, in string aTypeHint, in nsIInputStream aPostDataStream, in nsIInputStream aHeadersStream, in unsigned long aLoadFlags, in nsISHEntry aSHEntry, in boolean firstParty, out nsIDocShell aDocShell, out nsIRequest aRequest); */
NS_IMETHODIMP xgDocShell::InternalLoad(nsIURI *aURI, nsIURI *aReferrer, nsISupports *aOwner, PRUint32 aFlags, const PRUnichar *aWindowTarget, const char *aTypeHint, nsIInputStream *aPostDataStream, nsIInputStream *aHeadersStream, PRUint32 aLoadFlags, nsISHEntry *aSHEntry, PRBool firstParty, nsIDocShell **aDocShell, nsIRequest **aRequest)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void createLoadInfo (out nsIDocShellLoadInfo loadInfo); */
NS_IMETHODIMP xgDocShell::CreateLoadInfo(nsIDocShellLoadInfo **loadInfo)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void prepareForNewContentModel (); */
NS_IMETHODIMP xgDocShell::PrepareForNewContentModel()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] void firePageHideNotification (in boolean isUnload); */
NS_IMETHODIMP xgDocShell::FirePageHideNotification(PRBool isUnload)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] readonly attribute nsPresContext presContext; */
NS_IMETHODIMP xgDocShell::GetPresContext(nsPresContext * *aPresContext)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] readonly attribute nsIPresShell presShell; */
NS_IMETHODIMP xgDocShell::GetPresShell(nsIPresShell * *aPresShell)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] readonly attribute nsIPresShell eldestPresShell; */
NS_IMETHODIMP xgDocShell::GetEldestPresShell(nsIPresShell * *aEldestPresShell)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIContentViewer contentViewer; */
NS_IMETHODIMP xgDocShell::GetContentViewer(nsIContentViewer * *aContentViewer)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute nsIDOMEventTarget chromeEventHandler; */
NS_IMETHODIMP xgDocShell::GetChromeEventHandler(nsIDOMEventTarget * *aChromeEventHandler)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetChromeEventHandler(nsIDOMEventTarget * aChromeEventHandler)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute nsIDocumentCharsetInfo documentCharsetInfo; */
NS_IMETHODIMP xgDocShell::GetDocumentCharsetInfo(nsIDocumentCharsetInfo * *aDocumentCharsetInfo)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetDocumentCharsetInfo(nsIDocumentCharsetInfo * aDocumentCharsetInfo)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean allowPlugins; */
NS_IMETHODIMP xgDocShell::GetAllowPlugins(PRBool *aAllowPlugins)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetAllowPlugins(PRBool aAllowPlugins)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean allowJavascript; */
NS_IMETHODIMP xgDocShell::SetAllowJavascript(PRBool aAllowJavascript)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean allowMetaRedirects; */
NS_IMETHODIMP xgDocShell::GetAllowMetaRedirects(PRBool *aAllowMetaRedirects)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetAllowMetaRedirects(PRBool aAllowMetaRedirects)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean allowSubframes; */
NS_IMETHODIMP xgDocShell::GetAllowSubframes(PRBool *aAllowSubframes)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetAllowSubframes(PRBool aAllowSubframes)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean allowImages; */
NS_IMETHODIMP xgDocShell::GetAllowImages(PRBool *aAllowImages)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetAllowImages(PRBool aAllowImages)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsISimpleEnumerator getDocShellEnumerator (in long aItemType, in long aDirection); */
NS_IMETHODIMP xgDocShell::GetDocShellEnumerator(PRInt32 aItemType, PRInt32 aDirection, nsISimpleEnumerator **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute unsigned long appType; */
NS_IMETHODIMP xgDocShell::GetAppType(PRUint32 *aAppType)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetAppType(PRUint32 aAppType)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean allowAuth; */
NS_IMETHODIMP xgDocShell::GetAllowAuth(PRBool *aAllowAuth)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetAllowAuth(PRBool aAllowAuth)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute float zoom; */
NS_IMETHODIMP xgDocShell::GetZoom(float *aZoom)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetZoom(float aZoom)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute long marginWidth; */
NS_IMETHODIMP xgDocShell::GetMarginWidth(PRInt32 *aMarginWidth)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetMarginWidth(PRInt32 aMarginWidth)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute long marginHeight; */
NS_IMETHODIMP xgDocShell::GetMarginHeight(PRInt32 *aMarginHeight)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetMarginHeight(PRInt32 aMarginHeight)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean hasFocus; */
NS_IMETHODIMP xgDocShell::GetHasFocus(PRBool *aHasFocus)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetHasFocus(PRBool aHasFocus)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean canvasHasFocus; */
NS_IMETHODIMP xgDocShell::GetCanvasHasFocus(PRBool *aCanvasHasFocus)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetCanvasHasFocus(PRBool aCanvasHasFocus)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void tabToTreeOwner (in boolean forward, out boolean tookFocus); */
NS_IMETHODIMP xgDocShell::TabToTreeOwner(PRBool forward, PRBool *tookFocus)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute unsigned long busyFlags; */
NS_IMETHODIMP xgDocShell::GetBusyFlags(PRUint32 *aBusyFlags)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute unsigned long loadType; */
NS_IMETHODIMP xgDocShell::GetLoadType(PRUint32 *aLoadType)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetLoadType(PRUint32 aLoadType)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean isBeingDestroyed (); */
NS_IMETHODIMP xgDocShell::IsBeingDestroyed(PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isExecutingOnLoadHandler; */
NS_IMETHODIMP xgDocShell::GetIsExecutingOnLoadHandler(PRBool *aIsExecutingOnLoadHandler)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute nsILayoutHistoryState layoutHistoryState; */
NS_IMETHODIMP xgDocShell::GetLayoutHistoryState(nsILayoutHistoryState * *aLayoutHistoryState)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetLayoutHistoryState(nsILayoutHistoryState * aLayoutHistoryState)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute boolean shouldSaveLayoutState; */
NS_IMETHODIMP xgDocShell::GetShouldSaveLayoutState(PRBool *aShouldSaveLayoutState)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute nsISecureBrowserUI securityUI; */
NS_IMETHODIMP xgDocShell::GetSecurityUI(nsISecureBrowserUI * *aSecurityUI)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetSecurityUI(nsISecureBrowserUI * aSecurityUI)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void suspendRefreshURIs (); */
NS_IMETHODIMP xgDocShell::SuspendRefreshURIs()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void resumeRefreshURIs (); */
NS_IMETHODIMP xgDocShell::ResumeRefreshURIs()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void beginRestore (in nsIContentViewer viewer, in boolean top); */
NS_IMETHODIMP xgDocShell::BeginRestore(nsIContentViewer *viewer, PRBool top)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void finishRestore (); */
NS_IMETHODIMP xgDocShell::FinishRestore()
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute boolean restoringDocument; */
NS_IMETHODIMP xgDocShell::GetRestoringDocument(PRBool *aRestoringDocument)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute boolean useErrorPages; */
NS_IMETHODIMP xgDocShell::GetUseErrorPages(PRBool *aUseErrorPages)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetUseErrorPages(PRBool aUseErrorPages)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute long previousTransIndex; */
NS_IMETHODIMP xgDocShell::GetPreviousTransIndex(PRInt32 *aPreviousTransIndex)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute long loadedTransIndex; */
NS_IMETHODIMP xgDocShell::GetLoadedTransIndex(PRInt32 *aLoadedTransIndex)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void historyPurged (in long numEntries); */
NS_IMETHODIMP xgDocShell::HistoryPurged(PRInt32 numEntries)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMStorage getSessionStorageForURI (in nsIURI uri); */
NS_IMETHODIMP xgDocShell::GetSessionStorageForURI(nsIURI *uri, nsIDOMStorage **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void addSessionStorage (in ACString aDomain, in nsIDOMStorage storage); */
NS_IMETHODIMP xgDocShell::AddSessionStorage(const nsACString & aDomain, nsIDOMStorage *storage)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIChannel currentDocumentChannel; */
NS_IMETHODIMP xgDocShell::GetCurrentDocumentChannel(nsIChannel * *aCurrentDocumentChannel)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] void setChildOffset (in unsigned long offset); */
NS_IMETHODIMP xgDocShell::SetChildOffset(PRUint32 offset)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute boolean isInUnload; */
NS_IMETHODIMP xgDocShell::GetIsInUnload(PRBool *aIsInUnload)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute boolean channelIsUnsafe; */
NS_IMETHODIMP xgDocShell::GetChannelIsUnsafe(PRBool *aChannelIsUnsafe)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript, notxpcom] void DetachEditorFromWindow (); */
NS_IMETHODIMP_(void) xgDocShell::DetachEditorFromWindow()
{
    GOM_NOT_IMPLEMENTED;
}

/* void alert (in wstring dialogTitle, in wstring text); */
NS_IMETHODIMP xgDocShell::Alert(const PRUnichar *dialogTitle, const PRUnichar *text)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void alertCheck (in wstring dialogTitle, in wstring text, in wstring checkMsg, inout boolean checkValue); */
NS_IMETHODIMP xgDocShell::AlertCheck(const PRUnichar *dialogTitle, const PRUnichar *text, const PRUnichar *checkMsg, PRBool *checkValue)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean confirm (in wstring dialogTitle, in wstring text); */
NS_IMETHODIMP xgDocShell::Confirm(const PRUnichar *dialogTitle, const PRUnichar *text, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean confirmCheck (in wstring dialogTitle, in wstring text, in wstring checkMsg, inout boolean checkValue); */
NS_IMETHODIMP xgDocShell::ConfirmCheck(const PRUnichar *dialogTitle, const PRUnichar *text, const PRUnichar *checkMsg, PRBool *checkValue, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* PRInt32 confirmEx (in wstring dialogTitle, in wstring text, in unsigned long buttonFlags, in wstring button0Title, in wstring button1Title, in wstring button2Title, in wstring checkMsg, inout boolean checkValue); */
NS_IMETHODIMP xgDocShell::ConfirmEx(const PRUnichar *dialogTitle, const PRUnichar *text, PRUint32 buttonFlags, const PRUnichar *button0Title, const PRUnichar *button1Title, const PRUnichar *button2Title, const PRUnichar *checkMsg, PRBool *checkValue, PRInt32 *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean prompt (in wstring dialogTitle, in wstring text, inout wstring value, in wstring checkMsg, inout boolean checkValue); */
NS_IMETHODIMP xgDocShell::Prompt(const PRUnichar *dialogTitle, const PRUnichar *text, PRUnichar **value, const PRUnichar *checkMsg, PRBool *checkValue, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean promptPassword (in wstring dialogTitle, in wstring text, inout wstring password, in wstring checkMsg, inout boolean checkValue); */
NS_IMETHODIMP xgDocShell::PromptPassword(const PRUnichar *dialogTitle, const PRUnichar *text, PRUnichar **password, const PRUnichar *checkMsg, PRBool *checkValue, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean promptUsernameAndPassword (in wstring dialogTitle, in wstring text, inout wstring username, inout wstring password, in wstring checkMsg, inout boolean checkValue); */
NS_IMETHODIMP xgDocShell::PromptUsernameAndPassword(const PRUnichar *dialogTitle, const PRUnichar *text, PRUnichar **username, PRUnichar **password, const PRUnichar *checkMsg, PRBool *checkValue, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean select (in wstring dialogTitle, in wstring text, in PRUint32 count, [array, size_is (count)] in wstring selectList, out long outSelection); */
NS_IMETHODIMP xgDocShell::Select(const PRUnichar *dialogTitle, const PRUnichar *text, PRUint32 count, const PRUnichar **selectList, PRInt32 *outSelection, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDocShellTreeItem */
/* attribute wstring name; */
NS_IMETHODIMP xgDocShell::GetName(PRUnichar * *aName)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetName(const PRUnichar * aName)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* boolean nameEquals (in wstring name); */
NS_IMETHODIMP xgDocShell::NameEquals(const PRUnichar *name, PRBool *_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* attribute long itemType; */
NS_IMETHODIMP xgDocShell::GetItemType(PRInt32 *aItemType)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xgDocShell::SetItemType(PRInt32 aItemType)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDocShellTreeItem parent; */
NS_IMETHODIMP xgDocShell::GetParent(nsIDocShellTreeItem * *aParent)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDocShellTreeItem sameTypeParent; */
NS_IMETHODIMP xgDocShell::GetSameTypeParent(nsIDocShellTreeItem * *aSameTypeParent)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDocShellTreeItem rootTreeItem; */
NS_IMETHODIMP xgDocShell::GetRootTreeItem(nsIDocShellTreeItem * *aRootTreeItem)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDocShellTreeItem sameTypeRootTreeItem; */
NS_IMETHODIMP xgDocShell::GetSameTypeRootTreeItem(nsIDocShellTreeItem * *aSameTypeRootTreeItem)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDocShellTreeItem findItemWithName (in wstring name, in nsISupports aRequestor, in nsIDocShellTreeItem aOriginalRequestor); */
NS_IMETHODIMP xgDocShell::FindItemWithName(const PRUnichar *name, nsISupports *aRequestor, nsIDocShellTreeItem *aOriginalRequestor, nsIDocShellTreeItem **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* readonly attribute nsIDocShellTreeOwner treeOwner; */
NS_IMETHODIMP xgDocShell::GetTreeOwner(nsIDocShellTreeOwner * *aTreeOwner)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* [noscript] void setTreeOwner (in nsIDocShellTreeOwner treeOwner); */
NS_IMETHODIMP xgDocShell::SetTreeOwner(nsIDocShellTreeOwner *treeOwner)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDocShellTreeNode */
/* void addChild (in nsIDocShellTreeItem child); */
NS_IMETHODIMP xgDocShell::AddChild(nsIDocShellTreeItem *child)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void removeChild (in nsIDocShellTreeItem child); */
NS_IMETHODIMP xgDocShell::RemoveChild(nsIDocShellTreeItem *child)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDocShellTreeItem getChildAt (in long index); */
NS_IMETHODIMP xgDocShell::GetChildAt(PRInt32 index, nsIDocShellTreeItem **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDocShellTreeItem findChildWithName (in wstring aName, in boolean aRecurse, in boolean aSameType, in nsIDocShellTreeItem aRequestor, in nsIDocShellTreeItem aOriginalRequestor); */
NS_IMETHODIMP xgDocShell::FindChildWithName(const PRUnichar *aName, PRBool aRecurse, PRBool aSameType, nsIDocShellTreeItem *aRequestor, nsIDocShellTreeItem *aOriginalRequestor, nsIDocShellTreeItem **_retval)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void onStateChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aStateFlags, in nsresult aStatus); */
NS_IMETHODIMP xgDocShell::OnStateChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aStateFlags, nsresult aStatus)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void onProgressChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in long aCurSelfProgress, in long aMaxSelfProgress, in long aCurTotalProgress, in long aMaxTotalProgress); */
NS_IMETHODIMP xgDocShell::OnProgressChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRInt32 aCurSelfProgress, PRInt32 aMaxSelfProgress, PRInt32 aCurTotalProgress, PRInt32 aMaxTotalProgress)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void onLocationChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsIURI aLocation); */
NS_IMETHODIMP xgDocShell::OnLocationChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsIURI *aLocation)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void onStatusChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in nsresult aStatus, in wstring aMessage); */
NS_IMETHODIMP xgDocShell::OnStatusChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, nsresult aStatus, const PRUnichar *aMessage)
{
    XG_RETURN_NOT_IMPLEMENTED;
}

/* void onSecurityChange (in nsIWebProgress aWebProgress, in nsIRequest aRequest, in unsigned long aState); */
NS_IMETHODIMP xgDocShell::OnSecurityChange(nsIWebProgress *aWebProgress, nsIRequest *aRequest, PRUint32 aState)
{
    XG_RETURN_NOT_IMPLEMENTED;
}
