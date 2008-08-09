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

#include "gom/dom/gomcdatasection.h"

#include "gom/gomcdata.h"

#include "gom/dom/gomdomexception.h"

#include "gommacros.h"

#include <string.h>

GOM_IMPLEMENT (CDATA_SECTION, cdata_section, gom_cdata);

G_DEFINE_TYPE_WITH_CODE (GomCData, gom_cdata, GOM_TYPE_TXT,
                         GOM_IMPLEMENT_INTERFACE (CDATA_SECTION, cdata_section, gom_cdata));

static void gom_cdata_init       (GomCData      *cdata)   { }
static void gom_cdata_class_init (GomCDataClass *klass) { }
