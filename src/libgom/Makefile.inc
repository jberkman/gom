# -*- Makefile -*-

noinst_LIBRARIES += libgom.a

libgom_a_SOURCES :=				\
	include/gommacros.h			\
	src/libgom/gomcamel.c			\
	src/libgom/gomcdata.c			\
	src/libgom/gomchardata.c		\
	src/libgom/gomcmt.c			\
	src/libgom/gomdoc.c			\
	src/libgom/gomdom.c			\
	src/libgom/gomdombuiltins.c		\
	src/libgom/gomdominterfaces.c		\
	src/libgom/gomelem.c			\
	src/libgom/gomevt.c			\
	src/libgom/gomevttgt.c			\
	src/libgom/gomgcmanager.c		\
	src/libgom/gomglist.c			\
	src/libgom/gominterfaces.c		\
	src/libgom/gomjscontext.c		\
	src/libgom/gomjsdocument.c		\
	src/libgom/gomjsdomexception.c		\
	src/libgom/gomjselement.c		\
	src/libgom/gomjsevent.c			\
	src/libgom/gomjseventexception.c	\
	src/libgom/gomjseventlistener.c		\
	src/libgom/gomjseventtarget.c		\
	src/libgom/gomjsexception.c		\
	src/libgom/gomjsgerrorexception.c	\
	src/libgom/gomjskeyboardevent.c		\
	src/libgom/gomjsmouseevent.c		\
	src/libgom/gomjsnavigator.c		\
	src/libgom/gomjsnode.c			\
	src/libgom/gomjsobject.c		\
	src/libgom/gomjsuievent.c		\
	src/libgom/gomjswindow.c		\
	src/libgom/gomjsxmlhttprequest.c	\
	src/libgom/gomkeyboardevt.c		\
	src/libgom/gomlistenerlist.c		\
	src/libgom/gommouseevt.c		\
	src/libgom/gomnoodle.c			\
	src/libgom/gomobject.c			\
	src/libgom/gomtxt.c			\
	src/libgom/gomuievt.c			\
	src/libgom/gomuriutils.c		\
	src/libgom/gomvalue.c			\
	src/libgom/gomwidget.c			\
	src/libgom/gomxhr.c

libgom_a_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include $(GOM_CFLAGS)
libgom_a_CXXFLAGS = -fshort-wchar

src/libgom/gomdominterfaces.c: include/gom/dom/gomdombuiltins.h

src/libgom/gomwidget.c: gomwidgets.c

src/libgom/gomdombuiltins.c: $(dominclude_HEADERS) Makefile include/gom/dom/gomdombuiltins.h
	( cd $(srcdir) && $(GLIB_MKENUMS) \
			--fhead "#include \"gom/dom/gomdombuiltins.h\"\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "GType\n@enum_name@_get_type (void)\n{\n  static GType etype = 0;\n  if (etype == 0) {\n    static const G@Type@Value values[] = {" \
			--vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "      { 0, NULL, NULL }\n    };\n    etype = g_@type@_register_static (g_intern_static_string (\"@EnumName@\"), values);\n  }\n  return etype;\n}\n" \
		$(dominclude_HEADERS) ) > xgen-gdbc \
	&& cp xgen-gdbc $@  \
	&& rm -f xgen-gdbc

CLEANFILES += gomwidgets.c gomwidgets.c.tmp xgen-gdbc src/libgom/gomdombuiltins.c
