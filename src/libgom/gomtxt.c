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

#include "gom/gomtxt.h"

#include "gom/dom/gomtext.h"
#include "gom/dom/gomdomexception.h"

#include "gommacros.h"

#include <string.h>

static GomText *
gom_txt_split_text (GomText *text,
                    gulong   offset,
                    GError **error)
{
    GOM_NOT_IMPLEMENTED_ERROR (error);
    return NULL;
}

GOM_IMPLEMENT (TEXT, text, gom_txt);

G_DEFINE_TYPE_WITH_CODE (GomTxt, gom_txt, GOM_TYPE_CHAR_DATA,
                         GOM_IMPLEMENT_INTERFACE (TEXT, text, gom_txt));

static void gom_txt_init       (GomTxt      *txt)   { }
static void gom_txt_class_init (GomTxtClass *klass) { }
