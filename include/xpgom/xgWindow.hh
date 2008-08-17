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
#ifndef XG_WINDOW_HH
#define XG_WINDOW_HH

#include <xgIWindow.hh>

#include <nsIScriptGlobalObject.h>
#include <nsCOMPtr.h>
#include <jsapi.h>

#define XG_WINDOW_CID_STR "86BACE85-5414-4A67-A5E2-825EAE4169CB"
#define XG_WINDOW_CID \
    { 0x86BACE85, 0x5414, 0x4A67, { 0xA5, 0xE2, 0x82, 0x5E, 0xAE, 0x41, 0x69, 0xCB } }
#define XG_WINDOW_CONTRACTID "@ilovegom.org/window;1"

class xgWindow : public nsIScriptGlobalObject,
		 public xgIWindow
{
public:
    NS_DECL_ISUPPORTS
    NS_DECL_XGIWINDOW

    // nsIScriptGlobalObject
    nsresult EnsureScriptEnvironment (PRUint32 aLangID);
    nsIScriptContext *GetScriptContext (PRUint32 lang);
    void *GetScriptGlobal (PRUint32 lang);
    nsresult SetScriptContext (PRUint32 lang, nsIScriptContext *aContext);
    void OnFinalize (PRUint32 aLangID, void *aScriptGlobal);
    void SetScriptsEnabled (PRBool aEnabled, PRBool aFireTimeouts);
    nsresult SetNewArguments(nsIArray *aArguments);
    nsresult HandleScriptError(nsScriptErrorEvent *aErrorEvent,
			       nsEventStatus *aEventStatus);

    xgWindow();
    nsresult Init ();

    // oh well
    nsresult SetDocument (nsIDOMDocument *aDocument);

private:
    ~xgWindow();

protected:

    // xgIWindow
    nsCOMPtr<nsIDOMDocument> mDocument;
    nsCOMPtr<nsIDOMNavigator> mNavigator;
    nsCOMPtr<nsIDOMLocation> mLocation;

    // nsIScriptGlobalObject
    nsCOMPtr<nsIScriptContext> mContext[NS_STID_ARRAY_UBOUND];
    void *mGlobal[NS_STID_ARRAY_UBOUND];
};


#endif // XG_WINDOW_HH
