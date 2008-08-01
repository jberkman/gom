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

#include <nsIGenericFactory.h>
#include <nsIClassInfoImpl.h>

NS_GENERIC_FACTORY_CONSTRUCTOR(xgDOMImplementation)

#if 0
static NS_METHOD
xgGomRegistrationProc (nsIComponentManager         *aCompMgr,
		       nsIFile                     *aPath,
		       const char                  *registryLocation,
		       const char                  *componentType,
		       const nsModuleComponentInfo *info)
{
    return NS_OK;
}

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
	NULL, NULL, NULL,
	NS_CI_INTERFACE_GETTER_NAME (xgDOMImplementation),
	NULL,
	&NS_CLASSINFO_NAME(xgDOMImplementation)
    }
};

NS_IMPL_NSGETMODULE(nsGomModule, components)


