bin_PROGRAMS = gnome-inm-forecast

localedir = $(datadir)/locale

DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

gnome_inm_forecast_SOURCES = \
		main.c \
		main.h \
		callbacks.c \
		preferences.c \
		preferences.h \
		callbacks.h 

AM_CFLAGS = @GTK_CFLAGS@ @LIBGLADE_CFLAGS@ @LIBPANELAPPLET_CFLAGS@ @LIBGVFS_CFLAGS@
gnome_inm_forecast_LDADD = @LIBS@ @GTK_LIBS@ @LIBGLADE_LIBS@ @LIBPANELAPPLET_LIBS@ @LIBGVFS_LIBS@
