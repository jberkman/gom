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
#ifndef GOM_EVT_TGT_H
#define GOM_EVT_TGT_H

#include <glib/gmacros.h>

G_BEGIN_DECLS

typedef struct _GomEvtTgt      GomEvtTgt;
typedef struct _GomEvtTgtClass GomEvtTgtClass;

G_END_DECLS

#include "gom/gomeventtargetinternal.h"

G_BEGIN_DECLS

#define GOM_TYPE_EVT_TGT         (gom_evt_tgt_get_type ())
#define GOM_EVT_TGT(i)           (G_TYPE_CHECK_INSTANCE_CAST ((i), GOM_TYPE_EVT_TGT, GomEvtTgt))
#define GOM_EVT_TGT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST    ((k), GOM_TYPE_EVT_TGT, GomEvtTgtClass))
#define GOM_IS_EVT_TGT(i)        (G_TYPE_CHECK_INSTANCE_TYPE ((i), GOM_TYPE_EVT_TGT))
#define GOM_IS_EVT_TGT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE    ((k), GOM_TYPE_EVT_TGT))
#define GOM_EVT_TGT_GET_CLASS(i) (G_TYPE_INSTANCE_GET_CLASS  ((i), GOM_TYPE_EVT_TGT, GomEvtTgtClass))

struct _GomEvtTgt {
    GObject parent;
};

struct _GomEvtTgtClass {
    GObjectClass parent_class;
};

GType gom_evt_tgt_get_type (void);

void  gom_evt_tgt_dispatch_listeners (GomEventTargetInternal *current_target,
                                      GomEvent               *evt,
                                      const char             *namespace_uri,
                                      const char             *type_name,
                                      GomPhaseType            phase);

G_END_DECLS

#endif /* GOM_EVT_TGT_H */
