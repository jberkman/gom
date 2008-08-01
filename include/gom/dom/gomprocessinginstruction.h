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
#ifndef GOM_PROCESSING_INSTRUCTION_H
#define GOM_PROCESSING_INSTRUCTION_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomProcessingInstruction GomProcessingInstruction; /* dummy object */
typedef struct _GomProcessingInstructionInterface GomProcessingInstructionInterface;

G_END_DECLS

#include <glib-object.h>

G_BEGIN_DECLS

#define GOM_TYPE_PROCESSING_INSTRUCTION             (gom_processing_instruction_get_type ())
#define GOM_PROCESSING_INSTRUCTION(i)               (G_TYPE_CHECK_INSTANCE_CAST    ((i), GOM_TYPE_PROCESSING_INSTRUCTION, GomProcessingInstruction))
#define GOM_IS_PROCESSING_INSTRUCTION(i)            (G_TYPE_CHECK_INSTANCE_TYPE    ((i), GOM_TYPE_PROCESSING_INSTRUCTION))
#define GOM_PROCESSING_INSTRUCTION_GET_INTERFACE(i) (G_TYPE_INSTANCE_GET_INTERFACE ((i), GOM_TYPE_PROCESSING_INSTRUCTION, GomProcessingInstructionInterface))

#define GOM_IMPLEMENT_PROCESSING_INSTRUCTION(i, p)

struct _GomProcessingInstructionInterface {
    GTypeInterface parent;
};

GType gom_processing_instruction_get_type (void);

G_END_DECLS

#endif /* GOM_PROCESSING_INSTRUCTION_H */
