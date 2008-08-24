# -*- Makefile -*-

all: xulapp

xulapp: include/gomwidgets.h xulapp/app/application.ini xulapp/confvars.sh xulapp/mozconfig
	$(MAKE) -C mozilla.obj
