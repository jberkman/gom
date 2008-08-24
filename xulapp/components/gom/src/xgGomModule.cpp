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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xgGomElementFactory.h"
#include "xgGtkElementFactory.h"
#include "xgString.h"

#include <gtk/gtk.h>

#include <nsIClassInfoImpl.h>
#include <nsIFile.h>
#include <nsIGenericFactory.h>
#include <nsIXTFElementFactory.h>

NS_GENERIC_FACTORY_CONSTRUCTOR(xgGomElementFactory)
NS_GENERIC_FACTORY_CONSTRUCTOR(xgGtkElementFactory)

static const nsModuleComponentInfo components[] = {
    {
	"Gom Core Element Factory",
	XG_GOMELEMENTFACTORY_CID, XG_GOMELEMENTFACTORY_CONTRACTID,
	xgGomElementFactoryConstructor
    },
    {
	"Gom Gtk Element Factory",
	XG_GTKELEMENTFACTORY_CID, XG_GTKELEMENTFACTORY_CONTRACTID,
	xgGtkElementFactoryConstructor
    },
};

static nsresult
nsGomModuleConstructor (nsIModule *self)
{
    if (!gtk_init_check (NULL, NULL)) {
	g_warning ("Could not initialize Gtk; Gom module unavailable.");
	return NS_ERROR_NOT_AVAILABLE;
    }

#define WIDGET(w) g_type_qname (w);
#include "gomwidgets.h"
#undef WIDGET

    return NS_OK;
}

NS_IMPL_NSGETMODULE_WITH_CTOR(nsGomModule, components, nsGomModuleConstructor)
