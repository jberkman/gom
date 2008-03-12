# -*- Makefile -*-

noinst_PROGRAMS += gtkschema

src/schema/gtkschema.c: gomwidgets.c

gtkschema_SOURCES := src/schema/gtkschema.c
gtkschema_CFLAGS = $(GOM_CFLAGS)
gtkschema_LDADD := $(GOM_LIBS)

# i don't know much about this crazy java stuff, this path is lame yes
.rnc.rng:
	java -jar ~/Scratch/trang-20030619/trang.jar $< $@

src/schema/gom-gtk.rnc: gtkschema
	./gtkschema > $@

src/schema/gom-gtk.rng: src/schema/gom.rng

validate: src/schema/gom-gtk.rng
	@for i in $(wildcard examples/*.gom) ; do  \
		echo "xmllint --noout --relaxng $< $$i" ; \
		xmllint --noout --relaxng $< $$i || exit 1 ; \
	done
