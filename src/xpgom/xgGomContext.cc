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

#include "xpgom/xgGomContext.hh"
#include "xgIGomParser.hh"

#include <nsCOMPtr.h>
#include <nsComponentManagerUtils.h>
#include <nsIClassInfoImpl.h>
#include <nsIDOMDocument.h>
#include <nsMemory.h>

#include <gtk/gtkmain.h>

class xgGomContextParserListener : public xgIGomParserListener
{
public:
    NS_DECL_ISUPPORTS
    NS_DECL_XGIGOMPARSERLISTENER

    xgGomContextParserListener ();

    nsresult Init (xgGomContext *context);

private:
    ~xgGomContextParserListener();

protected:
    xgGomContext *mContext;
};

xgGomContext::xgGomContext()
{
}

xgGomContext::~xgGomContext()
{
}


NS_IMPL_ISUPPORTS1_CI(xgGomContext, xgIGomContext)

/* readonly attribute nsIDOMDocument document; */
NS_IMETHODIMP
xgGomContext::GetDocument (nsIDOMDocument **aDocument)
{
    *aDocument = mDocument;
    NS_IF_ADDREF (*aDocument);
    return NS_OK;
}

/* void init (in string uri); */
NS_IMETHODIMP
xgGomContext::Init (const char *uri)
{
    nsresult rv;
    xgGomContextParserListener *listener = new xgGomContextParserListener ();

    NS_ADDREF (listener);
    rv = listener->Init (this);
    if (NS_FAILED (rv)) {
	NS_RELEASE (listener);
	return rv;
    }

    nsCOMPtr<xgIGomParserListener> iListener (do_QueryInterface (listener));
    NS_RELEASE (listener);

    nsCOMPtr<xgIGomParser> parser (do_CreateInstance ("@ilovegom.org/parser;1", &rv));
    if (NS_FAILED (rv)) {
	return rv;
    }
    
    nsCOMPtr<nsIDOMDocument> document;
    return parser->ParseURI (uri, iListener, getter_AddRefs (document));
}

/* void run (); */
NS_IMETHODIMP
xgGomContext::Run()
{
    gtk_main ();
    return NS_OK;
}

xgGomContextParserListener::xgGomContextParserListener()
    : mContext (NULL)
{
}

xgGomContextParserListener::~xgGomContextParserListener()
{
    mContext = NULL;
}

NS_IMPL_ISUPPORTS1(xgGomContextParserListener, xgIGomParserListener)

nsresult
xgGomContextParserListener::Init (xgGomContext *context)
{
    mContext = context;
    return NS_OK;
}


/* void documentCreated (in nsIDOMDocument document); */
NS_IMETHODIMP
xgGomContextParserListener::DocumentCreated (nsIDOMDocument *document)
{
    mContext->mDocument = document;
    return NS_OK;
}
