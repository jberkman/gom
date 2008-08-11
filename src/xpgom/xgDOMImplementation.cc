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

#include "gom/dom/gomdomexception.h"
#include "gom/gomdom.h"
#include "xpgom/gomwrapped.hh"
#include "xpgom/xgDOMImplementation.hh"
#include "xpgom/xgDocument.hh"

#include <nsCOMPtr.h>
#include <nsEmbedString.h>
#include <nsIClassInfoImpl.h>
#include <nsMemory.h>

#include "gommacros.h"

#define CHECK_INITIALIZED XG_WRAPPED_CHECK_INIALIZED (GOM_TYPE_DOM_IMPLEMENTATION)

NS_IMPL_ADDREF_INHERITED(xgDOMImplementation, xgWrapped)
NS_IMPL_RELEASE_INHERITED(xgDOMImplementation, xgWrapped)

NS_INTERFACE_MAP_BEGIN(xgDOMImplementation)
NS_INTERFACE_MAP_ENTRY(nsIDOMDOMImplementation)
NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIDOMDOMImplementation)
NS_IMPL_QUERY_CLASSINFO(xgDOMImplementation)
NS_INTERFACE_MAP_END_INHERITING(xgWrapped)

NS_IMPL_CI_INTERFACE_GETTER1(xgDOMImplementation, nsIDOMDOMImplementation)

xgDOMImplementation::xgDOMImplementation () : xgWrapped (GOM_TYPE_DOM)
{
}

xgDOMImplementation::~xgDOMImplementation ()
{
}

nsresult
xgDOMImplementation::Init (GObject *aDom)
{
    GType ifaces[2];
    ifaces[0] = GOM_TYPE_DOM_IMPLEMENTATION;
    ifaces[1] = 0;
    return xgWrapped::Init (ifaces, aDom);
}    

/* boolean hasFeature (in DOMString feature, in DOMString version); */
NS_IMETHODIMP
xgDOMImplementation::HasFeature (const nsAString &feature,
				 const nsAString &version,
				 PRBool          *_retval)
{
    CHECK_INITIALIZED;
    GOM_ASTRING_TO_GSTRING_RETURN (feat, feature, NS_ERROR_INVALID_ARG);
    GOM_ASTRING_TO_GSTRING_RETURN (ver, version, NS_ERROR_INVALID_ARG);

    *_retval = gom_dom_implementation_has_feature (GOM_DOM_IMPLEMENTATION (mWrapped),
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
    XG_RETURN_NOT_IMPLEMENTED;
}

/* nsIDOMDocument createDocument (in DOMString namespaceURI, in DOMString qualifiedName, in nsIDOMDocumentType doctype)  raises (DOMException); */
NS_IMETHODIMP
xgDOMImplementation::CreateDocument (const nsAString    &namespaceURI,
				     const nsAString    &qualifiedName,
				     nsIDOMDocumentType *doctype,
				     nsIDOMDocument    **_retval)
{
    CHECK_INITIALIZED;
    GOM_ASTRING_TO_GSTRING_RETURN (nspace, namespaceURI, NS_ERROR_INVALID_ARG);
    GOM_ASTRING_TO_GSTRING_RETURN (qname, qualifiedName, NS_ERROR_INVALID_ARG);
    g_return_val_if_fail (doctype == NULL, NS_ERROR_INVALID_ARG);

    GError *error = NULL;
    GomDocument *doc = gom_dom_implementation_create_document (GOM_DOM_IMPLEMENTATION (mWrapped),
							       nspace, qname, NULL, &error);
    GOM_RETURN_NSRESULT_FROM_GERROR (error);

    nsresult rv = gom_wrap_g_object (doc, NS_GET_IID (nsIDOMDocument), (gpointer *)_retval);
    if (doc) {
	g_object_unref (doc);
    }
    return rv;
}
