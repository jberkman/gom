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

#include "xpgom/xgDOMImplementation.hh"

#include <nsIClassInfoImpl.h>

#include <nsMemory.h>
#include <nsEmbedString.h>

xgDOMImplementation::xgDOMImplementation()
{
}

xgDOMImplementation::~xgDOMImplementation()
{
}

NS_IMPL_ISUPPORTS1_CI(xgDOMImplementation, nsIDOMDOMImplementation)

/* boolean hasFeature (in DOMString feature, in DOMString version); */
NS_IMETHODIMP
xgDOMImplementation::HasFeature (const nsAString &feature,
				 const nsAString &version,
				 PRBool          *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMDocumentType createDocumentType (in DOMString qualifiedName, in DOMString publicId, in DOMString systemId)  raises (DOMException); */
NS_IMETHODIMP
xgDOMImplementation::CreateDocumentType (const nsAString     &qualifiedName,
					 const nsAString     &publicId,
					 const nsAString     &systemId,
					 nsIDOMDocumentType **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMDocument createDocument (in DOMString namespaceURI, in DOMString qualifiedName, in nsIDOMDocumentType doctype)  raises (DOMException); */
NS_IMETHODIMP
xgDOMImplementation::CreateDocument (const nsAString    &namespaceURI,
				     const nsAString    &qualifiedName,
				     nsIDOMDocumentType *doctype,
				     nsIDOMDocument    **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
