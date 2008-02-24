# -*- Makefile -*-

TESTS += checkgom
check_PROGRAMS += checkgom

checkgom_SOURCES :=				\
	tests/checkgom.c			\
	tests/checkgom.h			\
	tests/checkgomvalue.c			\
	tests/checkgomvalue.h

checkgom_CFLAGS = -I$(top_srcdir)/include $(TEST_CFLAGS) $(JS_CFLAGS)
checkgom_LDADD := libgom.la $(TEST_LIBS) $(JS_LIBS)
