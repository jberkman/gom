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

#include "gommacros.h"

#include "gom/gomeventtargetinternal.h"

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomEventTargetInternal, gom_event_target_internal, {}, GOM_TYPE_EVENT_TARGET);

GOM_STUB_FUNC (GOM_EVENT_TARGET_INTERNAL, gom_event_target_internal, get_parent_target,
               (GomEventTargetInternal *gom_event_target_internal),
               (gom_event_target_internal), GomEventTarget *);

GOM_STUB_VOID (GOM_EVENT_TARGET_INTERNAL, gom_event_target_internal, dispatch_listeners,
               (GomEventTargetInternal *gom_event_target_internal,
                GomEvent               *evt,
                const char             *namespace_uri,
                const char             *type_name,
                GomPhaseType            phase),
               (gom_event_target_internal, evt, namespace_uri, type_name, phase));

#include "gom/gomgcmanaged.h"

GOM_DEFINE_INTERFACE (GomGCManaged, gom_gc_managed, {});

GOM_STUB_VOID(GOM_GC_MANAGED, gom_gc_managed, mark_live_objects,
              (GomGCManaged *gom_gc_managed, JSContext *cx, JSGCStatus status),
              (gom_gc_managed, cx, status));

#include "gom/gomnodeinternal.h"

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomNodeInternal, gom_node_internal, {}, GOM_TYPE_NODE);

GOM_STUB_VOID (GOM_NODE_INTERNAL, gom_node_internal, set_parent,
               (GomNodeInternal *gom_node_internal, GomNode *parent), (gom_node_internal, parent));

GOM_STUB_VOID (GOM_NODE_INTERNAL, gom_node_internal, set_next_sibling,
               (GomNodeInternal *gom_node_internal, GomNode *sibling), (gom_node_internal, sibling));

GOM_STUB_VOID (GOM_NODE_INTERNAL, gom_node_internal, set_prev_sibling,
               (GomNodeInternal *gom_node_internal, GomNode *sibling), (gom_node_internal, sibling));

GOM_STUB_VOID (GOM_NODE_INTERNAL, gom_node_internal, sibling_requested,
               (GomNodeInternal *gom_node_internal, GomNode *child), (gom_node_internal, child));

