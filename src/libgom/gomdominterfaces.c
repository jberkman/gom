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
GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, get_element_by_id,
               (GomDocument *gom_document, const char *element_id),
               (gom_document, element_id), GomElement *);

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
}, GOM_TYPE_NODE);

#include <gom/dom/gomdomimplementation.h>

GOM_DEFINE_INTERFACE (GomDOMImplementation, gom_dom_implementation, GOM_NOT_IMPLEMENTED);

GOM_STUB_FUNC (GOM_DOM_IMPLEMENTATION, gom_dom_implementation, has_feature,
               (GomDOMImplementation *gom_dom_implementation, const char *feature, const char *version),
               (gom_dom_implementation, feature, version),
               gboolean);

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

GOM_STUB_VOID (GOM_ELEMENT, gom_element, set_attribute_node,
               (GomElement *gom_element, GomAttr *new_attr, GError **error),
               (gom_element, new_attr, error));

GOM_STUB_VOID (GOM_ELEMENT, gom_element, remove_attribute_node,
               (GomElement *gom_element, GomAttr *old_attr, GError **error),
               (gom_element, old_attr, error));

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_elements_by_tag_name,
               (GomElement *gom_element, const char *name),
               (gom_element, name), GomNodeList *);

GOM_STUB_VOID (GOM_ELEMENT, gom_element, normalize,
               (GomElement *gom_element), (gom_element));

#include <gom/dom/gomentityreference.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomEntityReference, gom_entity_reference, GOM_NOT_IMPLEMENTED, GOM_TYPE_NODE);

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
})

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
