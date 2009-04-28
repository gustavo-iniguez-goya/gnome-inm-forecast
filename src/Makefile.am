bin_PROGRAMS = gnome-inm-forecast

localedir = $(datadir)/locale

DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

gnome_inm_forecast_SOURCES = \
		main.c \
		main.h \
		preferences.c \
		preferences.h \
		html_parser.c \
		html_parser.h \
		callbacks.c \
		callbacks.h 

AM_CFLAGS = @GTK_CFLAGS@ @LIBGLADE_CFLAGS@ @LIBPANELAPPLET_CFLAGS@ @LIBGVFS_CFLAGS@ -DG_DISABLE_DEPRECATED
gnome_inm_forecast_LDADD = @LIBS@ @GTK_LIBS@ @LIBGLADE_LIBS@ @LIBPANELAPPLET_LIBS@ @LIBGVFS_LIBS@
