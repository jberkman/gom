# -*- Makefile -*-

noinst_PROGRAMS += gtkschema

src/schema/gtkschema.c: gomwidgets.c

gtkschema_SOURCES := src/schema/gtkschema.c
gtkschema_CFLAGS = $(GOM_CFLAGS)
gtkschema_LDADD := $(GOM_LIBS)

src/schema/gom-gtk.rnc: gtkschema
	./gtkschema > $@

validate: src/schema/gom-gtk.rnc
	@for i in $(wildcard src/schema/*.rnc) ; do \
		echo "rnv -c $$i" ;		    \
		rnv -c $$i ;			    \
	done
	@for i in $(wildcard examples/*.gom) ; do \
		echo "rnv $< $$i" ;		  \
		rnv $< $$i ;			  \
	done
