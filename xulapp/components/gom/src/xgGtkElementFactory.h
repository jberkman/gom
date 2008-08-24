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
#ifndef XG_GTK_ELEMENT_FACTORY_HH
#define XG_GTK_ELEMENT_FACTORY_HH

#include <nsIXTFElementFactory.h>

#define XG_NAMESPACE_GTK "http://ilovegom.org/Gtk"

#define XG_GTKELEMENTFACTORY_CID_STR "D950C1C6-BEE4-44D1-AF0A-5323C09048A3"
#define XG_GTKELEMENTFACTORY_CID \
    { 0xD950C1C6, 0xBEE4, 0x44D1, { 0xAF, 0x0A, 0x53, 0x23, 0xC0, 0x90, 0x48, 0xA3 } }
#define XG_GTKELEMENTFACTORY_CONTRACTID NS_XTF_ELEMENT_FACTORY_CONTRACTID_PREFIX XG_NAMESPACE_GTK

class xgGtkElementFactory : public nsIXTFElementFactory
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXTFELEMENTFACTORY

  xgGtkElementFactory();

private:
  ~xgGtkElementFactory();

protected:
  /* additional members */
};

#endif /* XG_GTK_ELEMENT_FACTORY_HH */

