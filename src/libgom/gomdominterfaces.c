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

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE(GomAttr, gom_attr, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_ATTR, gom_attr, get_name, (GomAttr *gom_attr), (gom_attr), const char *);
GOM_STUB_FUNC (GOM_ATTR, gom_attr, get_specified, (GomAttr *gom_attr), (gom_attr), gboolean);
GOM_STUB_FUNC (GOM_ATTR, gom_attr, get_value, (GomAttr *gom_attr), (gom_attr), const char *);

#include <gom/dom/gomcdatasection.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomCDATASection, gom_cdata_section, GOM_TYPE_TEXT);

#include <gom/dom/gomcharacterdata.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE(GomCharacterData, gom_character_data, GOM_TYPE_NODE);

/* attributes */
GOM_STUB_FUNC (GOM_CHARACTER_DATA, gom_character_data, get_data, 
               (GomCharacterData *gom_character_data, GError **error), 
               (gom_character_data, error), const char *);

GOM_STUB_VOID (GOM_CHARACTER_DATA, gom_character_data, set_data, 
               (GomCharacterData *gom_character_data, const char *data, GError **error), 
               (gom_character_data, data, error));

GOM_STUB_FUNC (GOM_CHARACTER_DATA, gom_character_data, get_length,
               (GomCharacterData *gom_character_data), 
               (gom_character_data), gulong);

/* methods */
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

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomComment, gom_comment, GOM_TYPE_CHARACTER_DATA);

#include <gom/dom/gomdocument.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomDocument, gom_document, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, get_doctype,
               (GomDocument *gom_document), (gom_document), GomDocumentType *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, get_implementation,
               (GomDocument *gom_document), (gom_document), GomDOMImplementation *);

GOM_STUB_FUNC (GOM_DOCUMENT, gom_document, get_document_element,
               (GomDocument *gom_document), (gom_document), GomElement *);

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

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomDocumentFragment, gom_document_fragment, GOM_TYPE_NODE);

#include <gom/dom/gomdocumenttype.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomDocumentType, gom_document_type, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_DOCUMENT_TYPE, gom_document_type, get_name,
               (GomDocumentType *gom_document_type),
               (gom_document_type), const char *);

GOM_STUB_FUNC (GOM_DOCUMENT_TYPE, gom_document_type, get_entities,
               (GomDocumentType *gom_document_type),
               (gom_document_type), GomNamedNodeMap *);

GOM_STUB_FUNC (GOM_DOCUMENT_TYPE, gom_document_type, get_notations,
               (GomDocumentType *gom_document_type), (gom_document_type), GomNamedNodeMap *);

#include <gom/dom/gomdomimplementation.h>

GOM_DEFINE_INTERFACE (GomDOMImplementation, gom_dom_implementation);

GOM_STUB_FUNC (GOM_DOM_IMPLEMENTATION, gom_dom_implementation, has_feature,
               (GomDOMImplementation *gom_dom_implementation, const char *feature, const char *version),
               (gom_dom_implementation, feature, version),
               gboolean);

#include <gom/dom/gomelement.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomElement, gom_element, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_ELEMENT, gom_element, get_tag_name,
               (GomElement *gom_element),
               (gom_element), G_CONST_RETURN char *);

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

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomEntityReference, gom_entity_reference, GOM_TYPE_NODE);

#include <gom/dom/gomnode.h>

GOM_DEFINE_INTERFACE (GomNode, gom_node);

/* attributes */

GOM_STUB_FUNC (GOM_NODE, gom_node, get_node_name,
               (GomNode *gom_node), (gom_node), G_CONST_RETURN char *);

GOM_STUB_FUNC (GOM_NODE, gom_node, get_node_value,
               (GomNode *gom_node, GError **error), (gom_node, error), char *);
GOM_STUB_VOID (GOM_NODE, gom_node, set_node_value,
               (GomNode *gom_node, const char *value, GError **error),
               (gom_node, value, error));

GOM_STUB_FUNC (GOM_NODE, gom_node, get_node_type,
               (GomNode *gom_node), (gom_node), GomNodeType);
GOM_STUB_FUNC (GOM_NODE, gom_node, get_parent_node,
               (GomNode *gom_node), (gom_node), GomNode *);

GOM_STUB_FUNC (GOM_NODE, gom_node, get_child_nodes,
               (GomNode *gom_node), (gom_node), GomNodeList *);

GOM_STUB_FUNC (GOM_NODE, gom_node, get_first_child,
               (GomNode *gom_node), (gom_node), GomNode *);
GOM_STUB_FUNC (GOM_NODE, gom_node, get_last_child,
               (GomNode *gom_node), (gom_node), GomNode *);
GOM_STUB_FUNC (GOM_NODE, gom_node, get_previous_sibling,
               (GomNode *gom_node), (gom_node), GomNode *);
GOM_STUB_FUNC (GOM_NODE, gom_node, get_next_sibling,
               (GomNode *gom_node), (gom_node), GomNode *);

GOM_STUB_FUNC (GOM_NODE, gom_node, get_attributes,
               (GomNode *gom_node), (gom_node), GomNamedNodeMap *);
GOM_STUB_FUNC (GOM_NODE, gom_node, get_owner_document,
               (GomNode *gom_node), (gom_node), GomDocument *);

/* methods */

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

GOM_DEFINE_INTERFACE (GomNodeList, gom_node_list);

GOM_STUB_FUNC (GOM_NODE_LIST, gom_node_list, get_length,
               (GomNodeList *gom_node_list), (gom_node_list), gulong);
GOM_STUB_FUNC (GOM_NODE_LIST, gom_node_list, item,
               (GomNodeList *gom_node_list, gulong index),
               (gom_node_list, index), GomNode *);

#include <gom/dom/gomnotation.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomNotation, gom_notation, GOM_TYPE_NODE);

#include <gom/dom/gomprocessinginstruction.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomProcessingInstruction, gom_processing_instruction, GOM_TYPE_NODE);

GOM_STUB_FUNC (GOM_PROCESSING_INSTRUCTION, gom_processing_instruction, get_target,
               (GomProcessingInstruction *gom_processing_instruction),
               (gom_processing_instruction), const char *);

GOM_STUB_FUNC (GOM_PROCESSING_INSTRUCTION, gom_processing_instruction, get_data,
               (GomProcessingInstruction *gom_processing_instruction),
               (gom_processing_instruction), const char *);

GOM_STUB_VOID (GOM_PROCESSING_INSTRUCTION, gom_processing_instruction, set_data,
               (GomProcessingInstruction *gom_processing_instruction, GError **error),
               (gom_processing_instruction, error));

#include <gom/dom/gomtext.h>

GOM_DEFINE_INTERFACE_WITH_PREREQUISITE (GomText, gom_text, GOM_TYPE_CHARACTER_DATA);

GOM_STUB_FUNC (GOM_TEXT, gom_text, split_text, 
               (GomText *gom_text, gulong offset, GError **error),
               (gom_text, offset, error), GomText *);
