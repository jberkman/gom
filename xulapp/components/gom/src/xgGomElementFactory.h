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
#ifndef XG_GOM_ELEMENT_FACTORY_HH
#define XG_GOM_ELEMENT_FACTORY_HH

#include <nsIXTFElementFactory.h>

#define XG_NAMESPACE_GOM "http://ilovegom.org/Gom"

#define XG_GOMELEMENTFACTORY_CID_STR "DAD332E0-0067-4F10-B1F5-DFFB6F1188BA"
#define XG_GOMELEMENTFACTORY_CID \
    { 0xDAD332E0, 0x0067, 0x4F10, { 0xB1, 0xF5, 0xDF, 0xFB, 0x6F, 0x11, 0x88, 0xBA } }
#define XG_GOMELEMENTFACTORY_CONTRACTID NS_XTF_ELEMENT_FACTORY_CONTRACTID_PREFIX XG_NAMESPACE_GOM

class xgGomElementFactory : public nsIXTFElementFactory
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIXTFELEMENTFACTORY

  xgGomElementFactory();

private:
  ~xgGomElementFactory();

protected:
  /* additional members */
};

#endif /* XG_GOM_ELEMENT_FACTORY_HH */

