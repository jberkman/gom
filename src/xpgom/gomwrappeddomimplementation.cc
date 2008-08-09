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
#include "gom/dom/gomdomimplementation.h"
#include "xpgom/gomwrappeddomimplementation.hh"
#include "xpgom/xgDOMImplementation.hh"

#include <nsIDOMDOMImplementation.h>
#include <nsCOMPtr.h>

#include "gommacros.h"

#define GET_DOM_IMPLEMENTATION(i) GOM_WRAPPED_GET (i, nsIDOMDOMImplementation, dom)

static gboolean
gom_wrapped_dom_implementation_has_feature (GomDOMImplementation *dom,
					    const char *feature,
					    const char *version)
{
    GOM_NOT_IMPLEMENTED;
    return FALSE;
}

static GomDocumentType *
gom_wrapped_dom_implementation_create_document_type (GomDOMImplementation *dom,
						     const char           *qualified_name,
						     const char           *public_id,
						     const char           *system_id,
						     GError              **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

static GomDocument *
gom_wrapped_dom_implementation_create_document (GomDOMImplementation *dom,
						const char           *namespace_uri,
						const char           *qualified_name,
						GomDocumentType      *doctype,
						GError              **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

GOM_WRAPPED_CONSTRUCTOR_INIT (xgDOMImplementation, Init)

GOM_IMPLEMENT (DOM_IMPLEMENTATION, dom_implementation, gom_wrapped_dom_implementation);

G_DEFINE_TYPE_WITH_CODE (GomWrappedDOMImplementation, gom_wrapped_dom_implementation, GOM_TYPE_WRAPPED,
			 GOM_IMPLEMENT_INTERFACE (DOM_IMPLEMENTATION, dom_implementation, gom_wrapped_dom_implementation));

static void gom_wrapped_dom_implementation_init (GomWrappedDOMImplementation *node) { }

static void
gom_wrapped_dom_implementation_class_init (GomWrappedDOMImplementationClass *klass)
{
    gom_wrapped_register_interface (GOM_TYPE_DOM_IMPLEMENTATION, GOM_TYPE_WRAPPED_DOM_IMPLEMENTATION,
				    NS_GET_IID (nsIDOMDOMImplementation), xgDOMImplementationConstructor);
}
