# -*- Makefile -*-

dist_noinst_DATA += include/gomwidgets.in
dist_noinst_HEADERS += include/gommacros.h

CLEANFILES += include/gomwidgets.h

include/gomwidgets.h: include/gomwidgets.in include/Makefile.inc
	-mkdir -p include
	cp $< $@.tmp
	chmod +w $@.tmp
	awk '/GTK_TYPE_.*get_type/ { print "WIDGET ("$$2")" }' \
		$(shell pkg-config --variable includedir gtk+-2.0)/gtk-2.0/gtk/*.h \
		>> $@.tmp
	@if ! cmp $@.tmp $@ ; then		\
		echo cp $@.tmp $@;		\
		cp $@.tmp $@;			\
	fi
	rm -f $@.tmp
