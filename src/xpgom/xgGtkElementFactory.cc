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

#include "xpgom/xgGtkElement.hh"
#include "xpgom/xgGtkElementFactory.hh"
#include "xpgom/xgString.hh"

#include <nsCOMPtr.h>

#include <gtk/gtkwidget.h>

#include "gommacros.h"

/* Implementation file */
NS_IMPL_ISUPPORTS1 (xgGtkElementFactory, nsIXTFElementFactory)

xgGtkElementFactory::xgGtkElementFactory()
{
  /* member initializers and constructor code */
}

xgGtkElementFactory::~xgGtkElementFactory()
{
  /* destructor code */
}

/* nsIXTFElement createElement (in AString tagName); */
NS_IMETHODIMP
xgGtkElementFactory::CreateElement (const nsAString &tagName,
				    nsIXTFElement  **_retval)
{
    GOM_ASTRING_TO_GSTRING_RETURN (tag_name, tagName, NS_ERROR_INVALID_ARG);
    GType gtype = g_type_from_name (tag_name);
    if (!gtype || !g_type_is_a (gtype, GTK_TYPE_WIDGET)) {
	return NS_ERROR_FAILURE;
    }
    xgGtkElement *elem = new xgGtkElement ();
    if (!elem) {
	return NS_ERROR_OUT_OF_MEMORY;
    }
    *_retval = elem;
    NS_ADDREF (*_retval);
    nsresult rv = elem->Init (gtype);
    if (!NS_SUCCEEDED (rv)) {
	NS_RELEASE (*_retval);
	*_retval = NULL;
    }
    return rv;
}
