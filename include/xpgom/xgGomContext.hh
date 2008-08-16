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
#ifndef XG_GOM_CONTEXT_HH
#define XG_GOM_CONTEXT_HH

#include "xgIGomContext.hh"

#include <nsCOMPtr.h>

class nsIDOMDocument;

#define XG_GOMCONTEXT_CID_STR "C47A034A-600D-43CF-96A0-769642753BA0"
#define XG_GOMCONTEXT_CID \
    { 0xC47A034A, 0x600D, 0x43CF, { 0x96, 0xA0, 0x76, 0x96, 0x42, 0x75, 0x3B, 0xA0 } }
#define XG_GOMCONTEXT_CONTRACTID "@ilovegom.org/context;1"

class xgGomContext : public xgIGomContext
{
    friend class xgGomContextParserListener;

public:
    NS_DECL_ISUPPORTS
    NS_DECL_XGIGOMCONTEXT

    xgGomContext();

private:
    ~xgGomContext();

    nsCOMPtr<nsIDOMDocument> mDocument;

protected:
    void SetDocument (nsIDOMDocument *aDocument);
};

#endif /* XG_GOM_CONTEXT_HH */
