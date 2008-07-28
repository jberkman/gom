# -*- Makefile -*-

TESTS += checkgom
check_PROGRAMS += checkgom

checkgom_SOURCES :=				\
	empty.cc				\
	tests/checkgom.c			\
	tests/checkgom.h			\
	tests/checkgomvalue.c			\
	tests/checkgomvalue.h

checkgom_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)/include $(TEST_CFLAGS) $(GOM_CFLAGS)
checkgom_CXXFLAGS = -fshort-wchar
checkgom_LDADD := libgom.a $(TEST_LIBS) $(GOM_LIBS)
