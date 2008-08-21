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
#ifndef XG_DOC_SHELL_HH
#define XG_DOC_SHELL_HH

#include <nsCOMPtr.h>
#include <nsDocLoader.h>
#include <nsIDOMDocument.h>
#include <nsIDocShell.h>
#include <nsIDocShellTreeItem.h>
#include <nsIPrompt.h>
#include <nsIScriptGlobalObjectOwner.h>
#include <nsIWebProgressListener.h>
#include <nsIURI.h>

#define XG_DOCSHELL_CID_STR "4B7979EF-8B9D-4A69-84A7-ADE48EE510DD"
#define XG_DOCSHELL_CID \
    { 0x4B7979EF, 0x8B9D, 0x4A69, { 0x84, 0xA7, 0xAD, 0xE4, 0x8E, 0xE5, 0x10, 0xDD } }
#define XG_DOCSHELL_CONTRACTID "@ilovegom.org/shell;1"

class xgDocShell : public nsDocLoader,
		   public nsIDocShell,
		   public nsIDocShellTreeItem,
		   public nsIScriptGlobalObjectOwner,
		   public nsIPrompt,
		   public nsIWebProgressListener
{
public:
    NS_DECL_ISUPPORTS_INHERITED
    NS_DECL_NSIDOCSHELL
    NS_DECL_NSIDOCSHELLTREENODE
    NS_DECL_NSIDOCSHELLTREEITEM
    // nsIScriptGlobalObjectOwner methods
    virtual nsIScriptGlobalObject* GetScriptGlobalObject();
    NS_DECL_NSIPROMPT
    NS_DECL_NSIWEBPROGRESSLISTENER

    xgDocShell();

private:
  ~xgDocShell();

protected:
    nsCOMPtr<nsIURI> mUri;
    nsCOMPtr<nsIScriptGlobalObject> mGlobal;
    nsCOMPtr<nsIDOMDocument> mDocument;
};

#endif // XG_DOC_SHELL_HH
