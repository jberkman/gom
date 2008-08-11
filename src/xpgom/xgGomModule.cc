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
#include "xpgom/gomwrappeddomimplementation.hh"
#include "xpgom/gomwrappeddocument.hh"
#include "xpgom/gomwrappedelement.hh"
#include "xpgom/gomwrappednamednodemap.hh"
#include "xpgom/gomwrappedattr.hh"

#include "gom/gomwidget.h"

#include <gtk/gtkmain.h>

#include <nsIGenericFactory.h>
#include <nsIClassInfoImpl.h>
#include <nsIFile.h>
#include <nsStringAPI.h>

NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(xgDOMImplementation, Init);

NS_DECL_CLASSINFO(xgDOMImplementation)

static const nsModuleComponentInfo components[] = {
    {
	"DOM Implementation",
	XG_DOMIMPLEMENTATION_CID, XG_DOMIMPLEMENTATION_CONTRACTID,
	xgDOMImplementationConstructor,
	/* xgGomRegistrationProc */ NULL, NULL, NULL,
	NS_CI_INTERFACE_GETTER_NAME (xgDOMImplementation),
	NULL,
	&NS_CLASSINFO_NAME(xgDOMImplementation)
    }
};

static nsresult
nsGomModuleConstructor (nsIModule *self)
{
    if (!gtk_init_check (NULL, NULL)) {
	g_warning ("Could not initialize Gtk; Gom module unavailable.");
	return NS_ERROR_NOT_AVAILABLE;
    }
    gom_widget_init ();
    g_type_class_ref (GOM_TYPE_WRAPPED_DOM_IMPLEMENTATION);
    g_type_class_ref (GOM_TYPE_WRAPPED_NAMED_NODE_MAP);
    g_type_class_ref (GOM_TYPE_WRAPPED_ATTR);
    g_type_class_ref (GOM_TYPE_WRAPPED_ELEMENT);
    g_type_class_ref (GOM_TYPE_WRAPPED_DOCUMENT);
    return NS_OK;
}

NS_IMPL_NSGETMODULE_WITH_CTOR(nsGomModule, components, nsGomModuleConstructor)
