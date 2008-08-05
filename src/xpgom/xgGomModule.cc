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

#include <gtk/gtkmain.h>

#include <nsIGenericFactory.h>
#include <nsIClassInfoImpl.h>
#include <nsIFile.h>
#include <nsStringAPI.h>

NS_GENERIC_FACTORY_CONSTRUCTOR_INIT(xgDOMImplementation, Init);

static NS_METHOD
xgGomRegistrationProc (nsIComponentManager         *aCompMgr,
		       nsIFile                     *aPath,
		       const char                  *registryLocation,
		       const char                  *componentType,
		       const nsModuleComponentInfo *info)
{
    if (!gtk_init_check (NULL, NULL)) {
	g_warning ("Could not initialize Gtk; Gom module unavailable.");
	return NS_ERROR_NOT_AVAILABLE;
    }
    return NS_OK;
}

#if 0
static NS_METHOD
xgGomUnregistrationProc (nsIComponentManager         *aCompMgr,
			 nsIFile                     *aPath,
			 const char                  *registryLocation,
			 const nsModuleComponentInfo *info)
{
    return NS_OK;
}
#endif

NS_DECL_CLASSINFO(xgDOMImplementation)

static const nsModuleComponentInfo components[] = {
    {
	"DOM Implementation",
	XG_DOMIMPLEMENTATION_CID, XG_DOMIMPLEMENTATION_CONTRACTID,
	xgDOMImplementationConstructor,
	xgGomRegistrationProc, NULL, NULL,
	NS_CI_INTERFACE_GETTER_NAME (xgDOMImplementation),
	NULL,
	&NS_CLASSINFO_NAME(xgDOMImplementation)
    }
};

NS_IMPL_NSGETMODULE(nsGomModule, components)


