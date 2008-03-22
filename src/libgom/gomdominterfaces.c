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

#include <gom/dom/gomdombuiltins.h>

#include <gom/dom/gomabstractview.h>

#include <gom/dom/gomdocumentview.h>

GOM_DEFINE_INTERFACE(GomAbstractView, gom_abstract_view,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("document", NULL,
                             "The source DocumentView of which this is an AbstractView.",
                             GOM_TYPE_DOCUMENT_VIEW,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
});

#include <gom/dom/gomattr.h>

#include <gom/dom/gomnode.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE(GomAttr, gom_attr, 
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("name", NULL,
                             "The name of this attribute",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_boolean ("specified", NULL,
                              "Whether this attribute was explicitly given a value in the original document.",
                              FALSE,
                              G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("value", NULL,
                             "On retrieval, the value of the attribute is returned as a "
                             "string. Character and general entity references are replaced with "
                             "their values. "
                             "On setting, this creates a Text node with the unparsed contents of the string.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
    
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("owner-element", NULL,
                             "The Element node this attribute is attached to or null if this attribute is not in use.",
                             GOM_TYPE_ELEMENT,
                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_NODE);

#include <gom/dom/gomcdatasection.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomCDATASection, gom_cdata_section, GOM_NOT_IMPLEMENTED, GOM_TYPE_TEXT);

#include <gom/dom/gomcharacterdata.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE(GomCharacterData, gom_character_data,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("data", NULL,
                             "The character data of the node that implements this interface",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
    g_object_interface_install_property (
        g_iface,
        g_param_spec_ulong ("length", NULL,
                            "The number of characters that are available through data and the substringData method below",
                            0, G_MAXULONG, 0,
                            G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_CHARACTER_DATA, gom_character_data, substring_data,
               (GomCharacterData *gom_character_data, gulong offset, gulong count, GError **error),
               (gom_character_data, offset, count, error), const char *);

GOM_STUB_VOID (GOM_CHARACTER_DATA, gom_character_data, append_data,
               (GomCharacterData *gom_character_data, const char *arg, GError **error),
               (gom_character_data, arg, error));

GOM_STUB_VOID (GOM_CHARACTER_DATA, gom_character_data, insert_data,
               (GomCharacterData *gom_character_data, gulong offset, const char *arg, GError **error),
               (gom_character_data, offset, arg, error));

GOM_STUB_VOID (GOM_CHARACTER_DATA, gom_character_data, delete_data,
               (GomCharacterData *gom_character_data, gulong offset, gulong count, GError **error),
               (gom_character_data, offset, count, error));

GOM_STUB_VOID (GOM_CHARACTER_DATA, gom_character_data, replace_data,
               (GomCharacterData *gom_character_data, gulong offset, gulong count, const char *arg, GError **error),
               (gom_character_data, offset, count, arg, error));

#include <gom/dom/gomcomment.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomComment, gom_comment, GOM_NOT_IMPLEMENTED, GOM_TYPE_CHARACTER_DATA);

#include <gom/dom/gomdocument.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomDocument, gom_document,
{                                        
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("doctype", NULL,
                             "The Document Type Declaration (see DocumentType) associated with this document.",
                             GOM_TYPE_DOCUMENT_TYPE,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("implementation", NULL,
                             "The DOMImplementation object that handles this document.",
                             GOM_TYPE_DOM_IMPLEMENTATION,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("document-element", NULL,
                             "This is a convenience attribute that allows direct access to the child node that is the root element of the document.",
                             GOM_TYPE_ELEMENT,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_element,
               (GomDocument *gom_document, const char *tag_name, GError **error),
               (gom_document, tag_name, error), GomElement *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_document_fragment,
               (GomDocument *gom_document), (gom_document), GomDocumentFragment *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_text_node,
               (GomDocument *gom_document, const char *data), (gom_document, data), GomText *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_comment,
               (GomDocument *gom_document, const char *data), (gom_document, data), GomComment *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_cdata_section,
               (GomDocument *gom_document, const char *data, GError **error),
               (gom_document, data, error), GomCDATASection *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_processing_instruction,
               (GomDocument *gom_document, const char *target, const char *data, GError **error),
               (gom_document, target, data, error), GomProcessingInstruction *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_attribute,
               (GomDocument *gom_document, const char *name, GError **error),
               (gom_document, name, error), GomAttr *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_entity_reference,
               (GomDocument *gom_document, const char *name, GError **error),
               (gom_document, name, error), GomEntityReference *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, get_elements_by_tag_name,
               (GomDocument *gom_document, const char *tag_name),
               (gom_document, tag_name), GomNodeList *);

/* Introduced in DOM Level 2: */
GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, import_node,
               (GomDocument *gom_document, GomNode *node, gboolean deep, GError **error),
               (gom_document, node, deep, error), GomNode *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_element_ns,
               (GomDocument *gom_document, const char *namespace_uri, const char *qualified_name, GError **error),
               (gom_document, namespace_uri, qualified_name, error), GomElement *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, create_attribute_ns,
               (GomDocument *gom_document, const char *namespace_uri, const char *qualified_name, GError **error),
               (gom_document, namespace_uri, qualified_name, error), GomAttr *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, get_elements_by_tag_name_ns,
               (GomDocument *gom_document, const char *namespace_uri, const char *local_name),
               (gom_document, namespace_uri, local_name), GomNodeList *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, get_element_by_id,
               (GomDocument *gom_document, const char *element_id),
               (gom_document, element_id), GomElement *);

#include <gom/dom/gomdocumentevent.h>

GOM_DEFINE_INTERFACE (GomDocumentEvent, gom_document_event, GOM_NOT_IMPLEMENTED);

GOM_STUB_FUNC (GOM_DOCUMENT_EVENT, gom_document_event, create_event,
               (GomDocumentEvent *gom_document_event, const char *event_type, GError **error),
               (gom_document_event, event_type, error), GomEvent *);

#include <gom/dom/gomdocumentfragment.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomDocumentFragment, gom_document_fragment, GOM_NOT_IMPLEMENTED, GOM_TYPE_NODE);

#include <gom/dom/gomdocumenttype.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomDocumentType, gom_document_type,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("name", NULL,
                             "The name of DTD; i.e., the name immediately following the DOCTYPE keyword.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("entities", NULL,
                             "A NamedNodeMap containing the general entities, both external and internal, declared in the DTD.",
                             GOM_TYPE_NAMED_NODE_MAP,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("notations", NULL,
                             "A NamedNodeMap containing the notations declared in the DTD.",
                             GOM_TYPE_NAMED_NODE_MAP,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("public-id", NULL,
                             "The public identifier of the external subset.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("system-id", NULL,
                             "The system identifier of the external subset.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("internal-subset", NULL,
                             "The internal subset as a string.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_NODE);

#include <gom/dom/gomdocumentview.h>

GOM_DEFINE_INTERFACE(GomDocumentView, gom_document_view,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("default-view", NULL,
                             "The default AbstractView for this Document, or null if none available.",
                             GOM_TYPE_ABSTRACT_VIEW,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
});

#include <gom/dom/gomdomexception.h>

GOM_DEFINE_QUARK (dom_exception_error);

#include <gom/dom/gomdomimplementation.h>

GOM_DEFINE_INTERFACE (GomDOMImplementation, gom_dom_implementation, GOM_NOT_IMPLEMENTED);

GOM_STUB_FUNC (GOM_DOM_IMPLEMENTATION, gom_dom_implementation, has_feature,
               (GomDOMImplementation *gom_dom_implementation, const char *feature, const char *version),
               (gom_dom_implementation, feature, version),
               gboolean);

GOM_STUB_FUNC (GOM_DOM_IMPLEMENTATION, gom_dom_implementation, create_document_type,
               (GomDOMImplementation *gom_dom_implementation,
                const char           *qualified_name,
                const char           *public_id,
                const char           *system_id,
                GError              **error),
               (gom_dom_implementation, qualified_name, public_id, system_id, error), GomDocumentType *);

GOM_STUB_FUNC (GOM_DOM_IMPLEMENTATION, gom_dom_implementation, create_document,
               (GomDOMImplementation *gom_dom_implementation,
                const char           *namespace_uri,
                const char           *qualified_name,
                GomDocumentType      *doctype,
                GError              **error),
               (gom_dom_implementation, namespace_uri, qualified_name, doctype, error), GomDocument *);

#include <gom/dom/gomelement.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomElement, gom_element,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("tag-name", NULL,
                             "The name of the element.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_attribute,
               (GomElement *gom_element, const char *name),
               (gom_element, name), char *);

GOM_STUB_VOID (GOM_ELEMENT, gom_element, set_attribute,
               (GomElement *gom_element, const char *name, const char *value, GError **error),
               (gom_element, name, value, error));

GOM_STUB_VOID (GOM_ELEMENT, gom_element, remove_attribute,
               (GomElement *gom_element, const char *name, GError **error),
               (gom_element, name, error));

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_attribute_node,
               (GomElement *gom_element, const char *name),
               (gom_element, name), GomAttr *);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, set_attribute_node,
               (GomElement *gom_element, GomAttr *new_attr, GError **error),
               (gom_element, new_attr, error), GomAttr *);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, remove_attribute_node,
               (GomElement *gom_element, GomAttr *old_attr, GError **error),
               (gom_element, old_attr, error), GomAttr *);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_elements_by_tag_name,
               (GomElement *gom_element, const char *name),
               (gom_element, name), GomNodeList *);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_attribute_ns,
               (GomElement *gom_element, const char *namespace_uri, const char *local_name),
               (gom_element, namespace_uri, local_name), char *);

GOM_STUB_VOID (GOM_ELEMENT, gom_element, set_attribute_ns,
               (GomElement *gom_element, 
                const char *namespace_uri,
                const char *qualified_name,
                const char *value,
                GError    **error),
               (gom_element, namespace_uri, qualified_name, value, error));

GOM_STUB_VOID (GOM_ELEMENT, gom_element, remove_attribute_ns,
               (GomElement *gom_element,
                const char *namespace_uri,
                const char *local_name,
                GError    **error),
               (gom_element, namespace_uri, local_name, error));

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_attribute_node_ns,
               (GomElement *gom_element,
                const char *namespace_uri,
                const char *local_name),
               (gom_element, namespace_uri, local_name), GomAttr *);
               
GOM_STUB_FUNC (GOM_ELEMENT, gom_element, set_attribute_node_ns,
               (GomElement *gom_element,
                GomAttr    *new_attr,
                GError    **error),
               (gom_element, new_attr, error), GomAttr *);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_elements_by_tag_name_ns,
               (GomElement *gom_element,
                const char *namespace_uri,
                const char *local_name),
               (gom_element, namespace_uri, local_name), GomNodeList *);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, has_attribute,
               (GomElement *gom_element, const char *name),
               (gom_element, name), gboolean);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, has_attribute_ns,
               (GomElement *gom_element, const char *namespace_uri, const char *local_name),
               (gom_element, namespace_uri, local_name), gboolean);

#include <gom/dom/gomentityreference.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomEntityReference, gom_entity_reference, GOM_NOT_IMPLEMENTED, GOM_TYPE_NODE);

#include <gom/dom/gomevent.h>

#include <gom/dom/gomeventtarget.h>

GOM_DEFINE_INTERFACE (GomEvent, gom_event,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("type", NULL,
                             "The name of the event",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("target", NULL,
                             "Used to indicate the EventTarget to which the event was originally dispatched.",
                             GOM_TYPE_EVENT_TARGET,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("current-target", NULL,
                             "Used to indicate the EventTarget whose EventListeners are currently being processed.",
                             GOM_TYPE_EVENT_TARGET,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_enum ("event-phase", NULL,
                           "Used to indicate which phase of event flow is currently being evaluated.",
                           GOM_TYPE_EVENT_PHASE_TYPE,
                           GOM_EVENT_CAPTURING_PHASE,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_boolean ("bubbles", NULL,
                              "Used to indicate whether or not an event is a bubbling event.",
                              TRUE,
                              G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_boolean ("cancelable", NULL,
                              "Used to indicate whether or not an event can have its default action prevented.",
                              TRUE,
                              G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
    
    g_object_interface_install_property (
        g_iface,
        g_param_spec_long ("time-stamp", NULL,
                           "Used to specify the time (in milliseconds relative to the epoch) at which the event was created.",
                           0, G_MAXLONG, 0,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
});

GOM_STUB_VOID (GOM_EVENT, gom_event, stop_propagation,
               (GomEvent *gom_event), (gom_event));

GOM_STUB_VOID (GOM_EVENT, gom_event, prevent_default,
               (GomEvent *gom_event), (gom_event));

GOM_STUB_VOID (GOM_EVENT, gom_event, init_event,
               (GomEvent *gom_event,
                const char *event_type_arg,
                gboolean    can_bubble_arg,
                gboolean    cancelable_arg),
               (gom_event, event_type_arg, cancelable_arg, cancelable_arg));

#include <gom/dom/gomeventexception.h>

GOM_DEFINE_QUARK (event_exception_error);

#include <gom/dom/gomeventtarget.h>

GOM_DEFINE_INTERFACE (GomEventTarget, gom_event_target, GOM_NOT_IMPLEMENTED);

GOM_STUB_VOID (GOM_EVENT_TARGET, gom_event_target, add_event_listener,
               (GomEventTarget *gom_event_target, const char *type, GomEventListener *listener, gboolean use_capture),
               (gom_event_target, type, listener, use_capture));

GOM_STUB_VOID (GOM_EVENT_TARGET, gom_event_target, remove_event_listener,
               (GomEventTarget *gom_event_target, const char *type, GomEventListener *listener, gboolean use_capture),
               (gom_event_target, type, listener, use_capture));

GOM_STUB_FUNC (GOM_EVENT_TARGET, gom_event_target, dispatch_event,
               (GomEventTarget *gom_event_target, GomEvent *evt, GError **error),
               (gom_event_target, evt, error), gboolean);

#include <gom/dom/gomeventlistener.h>

GOM_DEFINE_INTERFACE (GomEventListener, gom_event_listener, GOM_NOT_IMPLEMENTED);

GOM_STUB_VOID (GOM_EVENT_LISTENER, gom_event_listener, handle_event,
               (GomEventListener *gom_event_listener, GomEvent *evt),
               (gom_event_listener, evt));

#include <gom/dom/gommouseevent.h>

#include <gom/dom/gomuievent.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomMouseEvent, gom_mouse_event,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_long ("screen-x", NULL,
                           "The horizontal coordinate at which the event occurred relative to the origin of the screen coordinate system.",
                           0, G_MAXLONG, 0,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));


    g_object_interface_install_property (
        g_iface,
        g_param_spec_long ("screen-y", NULL,
                           "The vertical coordinate at which the event occurred relative to the origin of the screen coordinate system.",
                           0, G_MAXLONG, 0,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_long ("client-x", NULL,
                           "The horizontal coordinate at which the event occurred relative to the DOM implementation's client area.",
                           0, G_MAXLONG, 0,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_long ("client-y", NULL,
                           "The vertical coordinate at which the event occurred relative to the DOM implementation's client area.",
                           0, G_MAXLONG, 0,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_boolean ("ctrl-key", NULL,
                              "Used to indicate whether the 'ctrl' key was depressed during the firing of the event.",
                              FALSE,
                              G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));


    g_object_interface_install_property (
        g_iface,
        g_param_spec_boolean ("shift-key", NULL,
                              "Used to indicate whether the 'shift' key was depressed during the firing of the event.",
                              FALSE,
                              G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_boolean ("alt-key", NULL,
                              "Used to indicate whether the 'alt' key was depressed during the firing of the event.",
                              FALSE,
                              G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_boolean ("meta-key", NULL,
                              "Used to indicate whether the 'meta' key was depressed during the firing of the event.",
                              FALSE,
                              G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_uint ("button", NULL,
                           "During mouse events caused by the depression or release of a mouse button, button is used to indicate which mouse button changed state.",
                           0, 2, 0,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("related-target", NULL,
                             "Used to identify a secondary EventTarget related to a UI event.",
                             GOM_TYPE_EVENT_TARGET,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_UI_EVENT);

GOM_STUB_VOID (GOM_MOUSE_EVENT, gom_mouse_event, init_mouse_event,
               (GomMouseEvent *gom_mouse_event,
                const char      *type_arg,
                gboolean         can_bubble_arg,
                gboolean         cancelable_arg,
                GomAbstractView *view_arg,
                long             detail_arg,
                long             screen_x_arg,
                long             screen_y_arg,
                long             client_x_arg,
                long             client_y_arg,
                gboolean         ctrl_key_arg,
                gboolean         alt_key_arg,
                gboolean         shift_key_arg,
                gboolean         meta_key_arg,
                guint            button_arg,
                GomEventTarget  *related_target_arg),
               (gom_mouse_event, type_arg, can_bubble_arg, cancelable_arg, view_arg, detail_arg,
                screen_x_arg, screen_y_arg, client_x_arg, client_y_arg,
                ctrl_key_arg, alt_key_arg, shift_key_arg, meta_key_arg,
                button_arg, related_target_arg));

#include <gom/dom/gommutationevent.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomMutationEvent, gom_mutation_event,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("related-node", NULL,
                             "relatedNode is used to identify a secondary node related to a mutation event.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("prev-value", NULL,
                             "prevValue indicates the previous value of the Attr node in DOMAttrModified events, and of the CharacterData node in DOMCharDataModified events.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("new-value", NULL,
                             "newValue indicates the new value of the Attr node in DOMAttrModified events, and of the CharacterData node in DOMCharDataModified events.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("attr-name", NULL,
                             "attrName indicates the name of the changed Attr node in a DOMAttrModified event.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_enum ("attr-change", NULL,
                           "attrChange indicates the type of change which triggered the DOMAttrModified event.",
                           GOM_TYPE_MUTATION_EVENT_ATTR_CHANGE_TYPE,
                           GOM_MUTATION_EVENT_MODIFICATION,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_EVENT);

#include <gom/dom/gomnamednodemap.h>

GOM_DEFINE_INTERFACE (GomNamedNodeMap, gom_named_node_map,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_ulong ("length", NULL,
                            "The number of nodes in the map.",
                            0, G_MAXULONG, 0,
                            G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
});

GOM_STUB_FUNC (GOM_NAMED_NODE_MAP, gom_named_node_map, get_named_item,
               (GomNamedNodeMap *gom_named_node_map, const char *name),
               (gom_named_node_map, name), GomNode *);

GOM_STUB_FUNC (GOM_NAMED_NODE_MAP, gom_named_node_map, set_named_item,
               (GomNamedNodeMap *gom_named_node_map, GomNode *arg, GError **error),
               (gom_named_node_map, arg, error), GomNode *);

GOM_STUB_FUNC (GOM_NAMED_NODE_MAP, gom_named_node_map, remove_named_item,
               (GomNamedNodeMap *gom_named_node_map, const char *name, GError **error),
               (gom_named_node_map, name, error), GomNode *);

GOM_STUB_FUNC (GOM_NAMED_NODE_MAP, gom_named_node_map, item,
               (GomNamedNodeMap *gom_named_node_map, gulong index),
               (gom_named_node_map, index), GomNode *);

GOM_STUB_FUNC (GOM_NAMED_NODE_MAP, gom_named_node_map, get_named_item_ns,
               (GomNamedNodeMap *gom_named_node_map, const char *namespace_uri, const char *local_name),
               (gom_named_node_map, namespace_uri, local_name), GomNode *);

#include <gom/dom/gomnode.h>

GOM_DEFINE_INTERFACE (GomNode, gom_node,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("node-name", NULL,
                             "The name of this node, depending on its type",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("node-value", NULL,
                             "The value of this node, depending on its type",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_uint ("node-type", NULL,
                           "A code representing the type of the underlying object",
                           GOM_ELEMENT_NODE, GOM_NOTATION_NODE,
                           GOM_ELEMENT_NODE,
                           G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("parent-node", NULL,
                             "The parent of this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("child-nodes", NULL,
                             "A NodeList that contains all children of this node.",
                             GOM_TYPE_NODE_LIST,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("first-child", NULL,
                             "The first child of this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("last-child", NULL,
                             "The last child of this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("previous-sibling", NULL,
                             "The node immediately preceding this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("next-sibling", NULL,
                             "The node immediately following this node.",
                             GOM_TYPE_NODE,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("attributes", NULL,
                             "A NamedNodeMap containing the attributes of this node (if it is an Element) or null otherwise.",
                             GOM_TYPE_NAMED_NODE_MAP,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("owner-document", NULL,
                             "The Document object associated with this node.",
                             GOM_TYPE_DOCUMENT,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("namespace-uri", "namespace-u-r-i",
                             "The namespace URI of this node, or null if it is unspecified.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("prefix", NULL,
                             "The namespace prefix of this node, or null if it is unspecified.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("local-name", NULL,
                             "Returns the local part of the qualified name of this node.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
});

GOM_STUB_FUNC (GOM_NODE, gom_node, insert_before,
               (GomNode *gom_node, GomNode *new_child, GomNode *ref_child, GError **error),
               (gom_node, new_child, ref_child, error), GomNode *);
GOM_STUB_FUNC (GOM_NODE, gom_node, replace_child,
               (GomNode *gom_node, GomNode *new_child, GomNode *ref_child, GError **error),
               (gom_node, new_child, ref_child, error), GomNode *);
GOM_STUB_FUNC (GOM_NODE, gom_node, remove_child,
               (GomNode *gom_node, GomNode *old_child, GError **error),
               (gom_node, old_child, error), GomNode *);
GOM_STUB_FUNC (GOM_NODE, gom_node, append_child,
               (GomNode *gom_node, GomNode *new_child, GError **error),
               (gom_node, new_child, error), GomNode *);

GOM_STUB_FUNC (GOM_NODE, gom_node, has_child_nodes,
               (GomNode *gom_node), (gom_node), gboolean);
GOM_STUB_FUNC (GOM_NODE, gom_node, clone_node,
               (GomNode *gom_node, gboolean deep),
               (gom_node, deep), GomNode *);

GOM_STUB_VOID (GOM_NODE, gom_node, normalize,
               (GomNode *gom_node), (gom_node));

GOM_STUB_FUNC (GOM_NODE, gom_node, is_supported,
               (GomNode *gom_node, const char *feature, const char *version),
               (gom_node, feature, version), gboolean);

GOM_STUB_FUNC (GOM_NODE, gom_node, has_attributes,
               (GomNode *gom_node), (gom_node), gboolean);

#include <gom/dom/gomnodelist.h>

GOM_DEFINE_INTERFACE (GomNodeList, gom_node_list,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_ulong ("length", NULL,
                            "The number of nodes in the list.",
                            0, G_MAXULONG, 0,
                            G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
});

GOM_STUB_FUNC (GOM_NODE_LIST, gom_node_list, item,
               (GomNodeList *gom_node_list, gulong index),
               (gom_node_list, index), GomNode *);

#include <gom/dom/gomnotation.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomNotation, gom_notation,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("public-id", NULL,
                             "The public identifier of this notation.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("system-id", NULL,
                             "The system identifier of this notation.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

}, GOM_TYPE_NODE);

#include <gom/dom/gomprocessinginstruction.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomProcessingInstruction, gom_processing_instruction,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("target", NULL,
                             "The target of this processing instruction.",
                             NULL,
                             G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_string ("data", NULL,
                             "The content of this processing instruction.",
                             NULL,
                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_NODE);

#include <gom/dom/gomtext.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomText, gom_text, GOM_NOT_IMPLEMENTED, GOM_TYPE_CHARACTER_DATA);

GOM_STUB_FUNC (GOM_TEXT, gom_text, split_text, 
               (GomText *gom_text, gulong offset, GError **error),
               (gom_text, offset, error), GomText *);

#include <gom/dom/gomuievent.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomUIEvent, gom_ui_event,
{
    g_object_interface_install_property (
        g_iface,
        g_param_spec_object ("view", NULL,
                             "The view attribute identifies the AbstractView from which the event was generated.",
                             GOM_TYPE_ABSTRACT_VIEW,
                             G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

    g_object_interface_install_property (
        g_iface,
        g_param_spec_long ("detail", NULL,
                           "Specifies some detail information about the Event, depending on the type of event.",
                           0, G_MAXLONG, 0,
                           G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}, GOM_TYPE_EVENT);

GOM_STUB_VOID (GOM_UI_EVENT, gom_ui_event, init_ui_event,
               (GomUIEvent      *gom_ui_event,
                const char      *type_arg,
                gboolean         can_bubble_arg,
                gboolean         cancelable_arg,
                GomAbstractView *view_arg,
                long             detail_arg),
               (gom_ui_event, type_arg, can_bubble_arg, cancelable_arg, view_arg, detail_arg));
