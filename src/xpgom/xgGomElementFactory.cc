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

#include "xpgom/xgGomElementFactory.hh"
#include "xpgom/xgScriptElement.hh"
#include "xpgom/xgString.hh"

#include <nsISupportsUtils.h>

/* Implementation file */
NS_IMPL_ISUPPORTS1(xgGomElementFactory, nsIXTFElementFactory)

xgGomElementFactory::xgGomElementFactory()
{
  /* member initializers and constructor code */
}

xgGomElementFactory::~xgGomElementFactory()
{
  /* destructor code */
}

/* nsIXTFElement createElement (in AString tagName); */
NS_IMETHODIMP
xgGomElementFactory::CreateElement (const nsAString &tagName, nsIXTFElement **_retval)
{
    *_retval = NULL;
    if (tagName.EqualsLiteral ("script")) {
	*_retval = new xgScriptElement;
    }
    NS_IF_ADDREF (*_retval);
    return NS_OK;
}
