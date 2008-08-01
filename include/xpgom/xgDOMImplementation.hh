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
#ifndef XG_DOM_IMPLEMENTATION_HH
#define XG_DOM_IMPLEMENTATION_HH

#include <nsIDOMDOMImplementation.h>
#include "gom/dom/gomdomimplementation.h"

#define XG_DOMIMPLEMENTATION_CID_STR "4138BAA2-29BD-4D1C-9193-2D2254D4CA28"
#define XG_DOMIMPLEMENTATION_CID \
{ 0x4138BAA2, 0x29BD, 0x4D1C, { 0x91, 0x93, 0x2D, 0x22, 0x54, 0xD4, 0xCA, 0x28 } }
#define XG_DOMIMPLEMENTATION_CONTRACTID "@ilovegom.org/dom-implementation;1"

class xgDOMImplementation : public nsIDOMDOMImplementation
{
public:
    NS_DECL_ISUPPORTS
    NS_DECL_NSIDOMDOMIMPLEMENTATION

    xgDOMImplementation();

private:
    ~xgDOMImplementation();

    GomDOMImplementation *gdom;
protected:
};

#endif /* XG_DOM_IMPLEMENTATION_HH */
