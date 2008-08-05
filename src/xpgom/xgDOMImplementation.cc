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
#include "gom/gomdom.h"

#include "xpgom/xgDocument.hh"

#include "gom/dom/gomdomexception.h"

#include <nsCOMPtr.h>
#include <nsEmbedString.h>
#include <nsIClassInfoImpl.h>
#include <nsMemory.h>

#include "gommacros.h"

#define CHECK_INITIALIZED GOM_XGO_CHECK_INIALIZED (GOM_TYPE_DOM_IMPLEMENTATION)

NS_IMPL_ISUPPORTS1_CI(xgDOMImplementation, nsIDOMDOMImplementation)

xgDOMImplementation::xgDOMImplementation (GomDOMImplementation *aDom)
    : xgObject (aDom ? G_OBJECT (aDom) : NULL, GOM_TYPE_DOM)
{
}

xgDOMImplementation::~xgDOMImplementation ()
{
}

nsresult
xgDOMImplementation::Init ()
{
    GType ifaces[2];
    ifaces[0] = GOM_TYPE_DOM_IMPLEMENTATION;
    ifaces[1] = 0;
    return xgObject::Init (ifaces);
}    

/* boolean hasFeature (in DOMString feature, in DOMString version); */
NS_IMETHODIMP
xgDOMImplementation::HasFeature (const nsAString &feature,
				 const nsAString &version,
				 PRBool          *_retval)
{
    CHECK_INITIALIZED;
    GOM_ASTRING_TO_GSTRING (feat, feature, NS_ERROR_INVALID_ARG);
    GOM_ASTRING_TO_GSTRING (ver, version, NS_ERROR_INVALID_ARG);

    *_retval = gom_dom_implementation_has_feature (GOM_DOM_IMPLEMENTATION (mObject),
						   feat, ver);

    return NS_OK;
}

/* nsIDOMDocumentType createDocumentType (in DOMString qualifiedName, in DOMString publicId, in DOMString systemId)  raises (DOMException); */
NS_IMETHODIMP
xgDOMImplementation::CreateDocumentType (const nsAString     &qualifiedName,
					 const nsAString     &publicId,
					 const nsAString     &systemId,
					 nsIDOMDocumentType **_retval)
{
    CHECK_INITIALIZED;
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* nsIDOMDocument createDocument (in DOMString namespaceURI, in DOMString qualifiedName, in nsIDOMDocumentType doctype)  raises (DOMException); */
NS_IMETHODIMP
xgDOMImplementation::CreateDocument (const nsAString    &namespaceURI,
				     const nsAString    &qualifiedName,
				     nsIDOMDocumentType *doctype,
				     nsIDOMDocument    **_retval)
{
    CHECK_INITIALIZED;
    GOM_ASTRING_TO_GSTRING (nspace, namespaceURI, NS_ERROR_INVALID_ARG);
    GOM_ASTRING_TO_GSTRING (qname, qualifiedName, NS_ERROR_INVALID_ARG);
    g_return_val_if_fail (doctype == NULL, NS_ERROR_INVALID_ARG);

    GError *error = NULL;
    GomDocument *doc = gom_dom_implementation_create_document (GOM_DOM_IMPLEMENTATION (mObject),
							       nspace, qname, NULL, &error);
    if (!doc) {
	// frees error
	GOM_RETURN_NSRESULT_FROM_GERROR (error);
    }

    xgDocument *xDoc = new xgDocument (doc);
    if (!xDoc) {
	return NS_ERROR_OUT_OF_MEMORY;
    }
    nsresult rv = xDoc->Init();
    if (NS_SUCCEEDED (rv)) {
	rv = CallQueryInterface (xDoc, _retval);
    }
    return rv;
}
