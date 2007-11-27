#bin_PROGRAMS=src/gnome_inm
#gnome_inm_SOURCES=src/main.c system.h aclocal.m4 gnome_inm-opt.c gnome_inm-opt.h

#  add " -lm" to the following if gnome_inm requires the math library
#gnome_inm_LDADD=-lopts

schemasdir = @GCONF_SCHEMA_FILE_DIR@
schemas_in_files = gnome_inm.schemas

EXTRA_DIST= config.rpath gnome-inm-forecast.lsm.in gnome-inm-forecast.spec.in gnome_inm-opt.def gnome-inm-forecast.1 manpage.tpl gnome_inm.schemas local_codes.txt stations_list_es.txt

SUBDIRS = po src

if GCONF_SCHEMAS_INSTALL                                                                                                    
install-data-local: install-schemas
install-schemas:
	GCONF_CONFIG_SOURCE=@GCONF_SCHEMA_CONFIG_SOURCE@ @GCONFTOOL@ --makefile-install-rule gnome_inm.schemas
install-schemas:
	GCONF_CONFIG_SOURCE=@GCONF_SCHEMA_CONFIG_SOURCE@ @GCONFTOOL@ --makefile-uninstall-rule gnome_inm.schemas
else                                                                                                                        
install-data-local:                                                                                                         
endif      

#  if you write a self-test script named `chk', uncomment the
#  following and add `chk' to the EXTRA_DIST list
#TESTS=chk

#gnome_inm.o: gnome_inm-opt.h
#gnome_inm-opt.o: gnome_inm-opt.c
#gnome_inm-opt.c gnome_inm-opt.h: $(srcdir)/gnome_inm-opt.def
#	autogen -L/usr/share/autogen $(srcdir)/gnome_inm-opt.def

gnome-inm-forecast.1: $(srcdir)/gnome_inm-opt.def
	autogen -T manpage.tpl  $(srcdir)/gnome_inm-opt.def
	awk -f updater gnome-inm-forecast.1 > tmp$$$$ && mv tmp$$$$ gnome-inm-forecast.1
#	rm updater

# info_TEXINFOS = gnome_inm.texinfo
# gnome_inm_TEXINFOS = gpl.texinfo

#  install the man page
man_MANS=gnome-inm-forecast.1

ACLOCAL_AMFLAGS = -I m4
