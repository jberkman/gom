# -*- Makefile -*-

idl_headers += idl/xgIParser.hh
idl_headers += idl/xgIWindow.hh
idl_headers += idl/xgPIWrapped.hh

dist_noinst_DATA += $(idl_headers)
noinst_DATA += $(idl_headers:.idl=.xpt)
