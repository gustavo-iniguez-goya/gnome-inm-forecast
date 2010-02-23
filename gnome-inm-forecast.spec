# gnome-inm-forecast.spec
#
# Copyright (c) 2007-2009 Gustavo Iñiguez Goya ga@kutxa.homeunix.org
#
%define name gnome-inm-forecast
%define version 0.8.1
%define release 1
%define manifest %{_builddir}/%{name}-%{version}-%{release}.manifest

# required items
Name: %{name}
Version: %{version}
Release: %{release}
License: GPL
Group: Application/Misc

# optional items
#Vendor: Gustavo Iñiguez Goya
#Distribution:
#Icon:
URL: http://kutxa.homeunix.org/trac/gnome-inm-forecast/
Packager: Gustavo Iñiguez Goya ga@kutxa.homeunix.org

# source + patches
Source: http://kutxa.homeunix.org/debian/dists/unstable/i386/%{name}-%{version}.tar.gz
#Source1:
#Patch:
#Patch1:

# RPM info
#Provides:
Requires(pre): GConf2
Requires(post): GConf2
Requires(preun): GConf2
#Conflicts:
#Prereq:

#Prefix: /usr
BuildRoot: /var/tmp/%{name}-%{version}

Summary: Spanish weather forecast

%description
Spanish weather forecast applet for the gnome panel
gnome-inm-forecast.spec.  Generated from gnome-inm-forecast.spec.in by configure.

%prep
%setup -q
#%patch0 -p1

%build
./bootstrap
%configure
make

%install
rm -rf $RPM_BUILD_ROOT
export GCONF_DISABLE_MAKEFILE_SCHEMA_INSTALL=1
mkdir -p $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_sysconfdir}/gconf/schemas/
mkdir -p $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/pixmaps/inm/
mkdir -p $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/pixmaps/aemet/
mkdir -p $RPM_BUILD_ROOT/usr/libexec/
mkdir -p $RPM_BUILD_ROOT/usr/lib/bonobo/servers/
mkdir -p $RPM_BUILD_ROOT/usr/share/pixmaps/
install -m 644 GNOME_inm-forecast.server_rpm $RPM_BUILD_ROOT/usr/lib/bonobo/servers/GNOME_inm-forecast.server
install -m 644 gnome-inm-glade.glade $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/gnome-inm-glade.glade
install -m 644 GNOME_INM_menu.xml $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/
install -m 644 pixmaps/inm/* $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/pixmaps/inm/
install -m 644 pixmaps/aemet/* $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/pixmaps/aemet/
install -m 644 pixmaps/espcan.jpg $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/pixmaps/
install -m 644 pixmaps/gnome-inm-forecast.png $RPM_BUILD_ROOT/usr/share/pixmaps/
install -m 655 src/gnome-inm-forecast $RPM_BUILD_ROOT/usr/libexec/
install -m 644 local_codes.txt $RPM_BUILD_ROOT/usr/share/gnome-inm-forecast/
install -m 644 gnome_inm.schemas $RPM_BUILD_ROOT/%{_sysconfdir}/gconf/schemas/
%makeinstall

# __os_install_post is implicitly expanded after the
# %install section... do it now, and then disable it,
# so all work is done before building manifest.

%{?__os_install_post}
%define __os_install_post %{nil}

# build the file list automagically into %{manifest}

cd $RPM_BUILD_ROOT
rm -f %{manifest}
find . -type d \
        | sed '1,2d;s,^\.,\%attr(-\,root\,root) \%dir ,' >> %{manifest}
find . -type f \
        | sed 's,^\.,\%attr(-\,root\,root) ,' >> %{manifest}
find . -type l \
        | sed 's,^\.,\%attr(-\,root\,root) ,' >> %{manifest}

#%pre
#%post
#%preun
#%postun

%pre
if [ "$1" -gt 1 ] ; then
export GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source`
gconftool-2 --makefile-uninstall-rule \
%{_sysconfdir}/gconf/schemas/gnome_inm.schemas >/dev/null || :
fi

%post
export GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source`
gconftool-2 --makefile-install-rule \
%{_sysconfdir}/gconf/schemas/gnome_inm.schemas > /dev/null || :

%preun
if [ "$1" -eq 0 ] ; then
export GCONF_CONFIG_SOURCE=`gconftool-2 --get-default-source`
gconftool-2 --makefile-uninstall-rule \
%{_sysconfdir}/gconf/schemas/gnome_inm.schemas > /dev/null || :
fi

%files -f ../%{name}-%{version}-%{release}.manifest
%defattr(-,root,root)
%doc README AUTHORS COPYING INSTALL THANKS
#%docdir
#%config

%clean
rm -f %{manifest}
rm -rf $RPM_BUILD_ROOT

%changelog
