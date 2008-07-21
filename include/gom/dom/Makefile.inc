# -*- Makefile -*-

domincludedir := $(includedir)/gom-0/gom/dom
dominclude_HEADERS :=				   \
	include/gom/dom/gomabstractview.h	   \
	include/gom/dom/gomattr.h		   \
	include/gom/dom/gomcdatasection.h	   \
	include/gom/dom/gomcharacterdata.h	   \
	include/gom/dom/gomcomment.h		   \
	include/gom/dom/gomcustomevent.h	   \
	include/gom/dom/gomdocument.h		   \
	include/gom/dom/gomdocumentevent.h	   \
	include/gom/dom/gomdocumentfragment.h	   \
	include/gom/dom/gomdocumenttype.h	   \
	include/gom/dom/gomdocumentview.h	   \
	include/gom/dom/gomdomexception.h	   \
	include/gom/dom/gomdomimplementation.h	   \
	include/gom/dom/gomelement.h		   \
	include/gom/dom/gomentityreference.h	   \
	include/gom/dom/gomevent.h		   \
	include/gom/dom/gomeventexception.h	   \
	include/gom/dom/gomeventlistener.h	   \
	include/gom/dom/gomeventtarget.h	   \
	include/gom/dom/gomkeyboardevent.h	   \
	include/gom/dom/gommouseevent.h		   \
	include/gom/dom/gommutationevent.h	   \
	include/gom/dom/gommutationnameevent.h	   \
	include/gom/dom/gomnamednodemap.h	   \
	include/gom/dom/gomnode.h		   \
	include/gom/dom/gomnodelist.h		   \
	include/gom/dom/gomnotation.h		   \
	include/gom/dom/gomprocessinginstruction.h \
	include/gom/dom/gomtext.h		   \
	include/gom/dom/gomtextevent.h		   \
	include/gom/dom/gomuievent.h		   \
	include/gom/dom/gomxmlhttprequest.h

include/gom/dom/gomdombuiltins.h: include/gom/dom/stamp-gomdombuiltins.h
	@true
include/gom/dom/stamp-gomdombuiltins.h: $(dominclude_HEADERS) Makefile
	mkdir -p include/gom/dom && \
	( cd $(srcdir) && $(GLIB_MKENUMS) \
			--fhead "#ifndef __GOM_DOM_BUILTINS_H__\n#define __GOM_DOM_BUILTINS_H__\n\n#include <gom/dom/gomdomexception.h>\n#include <gom/dom/gomkeyboardevent.h>\n#include <gom/dom/gomevent.h>\n#include <gom/dom/gomeventexception.h>\n#include <gom/dom/gommutationevent.h>\n#include <gom/dom/gomnode.h>\n#include <gom/dom/gomxmlhttprequest.h>\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS\n" \
			--fprod "/* enumerations from \"@filename@\" */\n" \
			--vhead "GType @enum_name@_get_type (void) G_GNUC_CONST;\n#define GOM_TYPE_@ENUMSHORT@ (@enum_name@_get_type())\n" \
			--ftail "G_END_DECLS\n\n#endif /* __GOM_DOM_BUILTINS_H__ */" \
		$(dominclude_HEADERS) ) > xgen-gdbh \
	&& (cmp -s xgen-gdbh include/gom/dom/gomdombuiltins.h || cp xgen-gdbh include/gom/dom/gomdombuiltins.h ) \
	&& rm -f xgen-gdbh \
	&& echo timestamp > include/gom/dom/$(@F)

CLEANFILES += xgen-gdbh include/gom/dom/gomdombuiltins.h include/gom/dom/stamp-gomdombuiltins.h
