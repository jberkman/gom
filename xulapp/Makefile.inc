# -*- Makefile -*-

all: xulapp

xulapp: include/gomwidgets.h xulapp/app/application.ini xulapp/confvars.sh xulapp/mozconfig
	$(MAKE) -C mozilla.obj

clean:
	$(MAKE) -C mozilla.obj clean

.PHONY: xulapp

distclean: maintainer-clean
	rm -rf mozilla.obj

dist_noinst_DATA += xulapp/Makefile.in
dist_noinst_DATA += xulapp/build.mk
dist_noinst_DATA += xulapp/configure-mozilla
dist_noinst_DATA += xulapp/confvars.sh

dist_noinst_DATA += xulapp/app/Makefile.in
dist_noinst_DATA += xulapp/app/application.ini
dist_noinst_DATA += xulapp/app/default-prefs.js
dist_noinst_DATA += xulapp/app/channel-prefs.js

dist_noinst_DATA += xulapp/chrome/Makefile.in

dist_noinst_DATA += xulapp/chrome/branding/Makefile.in
dist_noinst_DATA += xulapp/chrome/branding/brand.properties
dist_noinst_DATA += xulapp/chrome/branding/jar.mn

dist_noinst_DATA += xulapp/chrome/content/Makefile.in
dist_noinst_DATA += xulapp/chrome/content/gom.js
dist_noinst_DATA += xulapp/chrome/content/gom.xul
dist_noinst_DATA += xulapp/chrome/content/jar.mn

dist_noinst_DATA += xulapp/chrome/locales/Makefile.in
dist_noinst_DATA += xulapp/chrome/locales/jar.mn

dist_noinst_DATA += xulapp/chrome/locales/en-US/updater.ini

dist_noinst_DATA += xulapp/components/Makefile.in

dist_noinst_DATA += xulapp/components/gom/Makefile.in

dist_noinst_DATA += xulapp/components/gom/public/Makefile.in
dist_noinst_DATA += xulapp/components/gom/public/xgIGObjectWrapper.idl

dist_noinst_DATA += xulapp/components/gom/src/Makefile.in
dist_noinst_DATA += xulapp/components/gom/src/xgGomElementFactory.cpp
dist_noinst_DATA += xulapp/components/gom/src/xgGomElementFactory.h
dist_noinst_DATA += xulapp/components/gom/src/xgGomModule.cpp
dist_noinst_DATA += xulapp/components/gom/src/xgGtkElement.cpp
dist_noinst_DATA += xulapp/components/gom/src/xgGtkElement.h
dist_noinst_DATA += xulapp/components/gom/src/xgGtkElementFactory.cpp
dist_noinst_DATA += xulapp/components/gom/src/xgGtkElementFactory.h
dist_noinst_DATA += xulapp/components/gom/src/xgScriptElement.cpp
dist_noinst_DATA += xulapp/components/gom/src/xgScriptElement.h
dist_noinst_DATA += xulapp/components/gom/src/xgString.h
