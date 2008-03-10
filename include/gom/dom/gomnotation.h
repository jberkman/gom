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
#ifndef GOM_NOTATION_H
#define GOM_NOTATION_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomNotation GomNotation; /* dummy object */
typedef struct _GomNotationInterface GomNotationInterface;

G_END_DECLS

#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_NOTATION             (gom_notation_get_type ())
#define GOM_NOTATION(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_NOTATION, GomNotation))
#define GOM_IS_NOTATION(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_NOTATION))
#define GOM_NOTATION_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_NOTATION, GomNotationInterface))

struct _GomNotationInterface {
    GTypeInterface parent;
};

GType gom_notation_get_type (void);

#endif /* GOM_NOTATION_H */
