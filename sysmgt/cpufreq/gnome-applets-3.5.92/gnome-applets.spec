%define name	gnome-applets
%define ver	3.5.92
%define RELEASE	1
%define rel	%{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}

Summary:	Small applications which embed themselves in the GNOME panel
Name:     	%name
Version: 	%ver
Release:	%rel
Copyright:	GPL
Group:		User Interface/Desktops
Source: 	ftp://ftp.gnome.org/pub/GNOME/sources/unstable/%{name}/%{name}-%{ver}.tar.gz
URL:		http://www.gnome.org/
BuildRoot:	/var/tmp/%{name}-%{ver}-root
Docdir:		%{_prefix}/doc

Requires:	libxml
Requires:	libgtop
Requires:	libghttp
Requires:	gnome-core >= 1.1.2
Requires:	gdk-pixbuf >= 0.7.0

%description
GNOME (GNU Network Object Model Environment) is a user-friendly
set of applications and desktop tools to be used in conjunction with a
window manager for the X Window System.  GNOME is similar in purpose and
scope to CDE and KDE, but GNOME is based completely on Open Source
software.  The gnome-applets package provides Panel applets which
enhance your GNOME experience.

You should install the gnome-applets package if you would like embed small
utilities in the GNOME panel.

%prep
%setup -q

%build
%ifarch alpha
  MYARCH_FLAGS="--host=alpha-redhat-linux"
%endif

if [ ! -f configure ]; then
CFLAGS="$RPM_OPT_FLAGS" ./autogen.sh $MYARCH_FLAGS --prefix=/usr --sysconfdir=/etc
else
CFLAGS="$RPM_OPT_FLAGS" ./configure $MYARCH_FLAGS --prefix=/usr --sysconfdir=/etc
fi

if [ "$SMP" != "" ]; then
  (make "MAKE=make -k -j $SMP"; exit 0)
  make
else
  make
fi

%install
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT
make install \
	prefix=$RPM_BUILD_ROOT%{_prefix} \
	sysconfdir=$RPM_BUILD_ROOT/etc
# no binary stripping
if [ -f %{name}.files ] ; then
  rm -f %{name}.files
fi
##############################################################################
##
function ProcessLang() {
 # rpm provides a handy scriptlet to do the locale stuff lets use that.
 if [ -f /usr/lib/rpm/find-lang.sh ] ; then
  /usr/lib/rpm/find-lang.sh $RPM_BUILD_ROOT %name
  sed "s:(644, root, root, 755):(444, bin, bin, 555):" %{name}.lang >tmp.lang && mv tmp.lang %{name}.lang
  if [ -f %{name}.files ] ; then
    cat %{name}.files %{name}.lang >tmp.files && mv tmp.files %{name}.files
  fi
 fi
}
function ProcessBin() {
  # Gather up all the executable files. Stripping if requested.
  # This will not recurse.
  if [ -d $RPM_BUILD_ROOT%{_prefix}/bin ] ; then
    echo "%defattr (0555, bin, bin)" >>%{name}.files
    find $RPM_BUILD_ROOT%{_prefix}/bin -type f -print | sed "s:^$RPM_BUILD_ROOT::g" >>%{name}.files
  fi
}

function ProcessLib() {
  # Gather up any libraries.
  # Usage: ProcessLib <dir> <type> <output file>
  # Type is either 'runtime' or 'devel'
  if [ -d $1 ] ; then
    echo "%defattr (0555, bin, bin)" >>$3
    case "$2" in
      runtime)
       # Grab runtime libraries
       find $1 -name "*.so.*" -print | sed "s:^$RPM_BUILD_ROOT::g" >>$3
       ;;
      devel)
       find $1 -name "*.so" -print | sed "s:^$RPM_BUILD_ROOT::g" >>$3
       find $1 -name "*.la" -print | sed "s:^$RPM_BUILD_ROOT::g" >>$3
       find $1 -name "*.a" -print | sed "s:^$RPM_BUILD_ROOT::g" >>$3
       find $1 -name "*.sh" -print | sed "s:^$RPM_BUILD_ROOT::g" >>$3
       ;;
    esac
   fi
}
function ProcessDir() {
  # Build a list of files in the specified dir sticking
  # a %defattr line as specified in front of the mess. This is intended
  # for normal dirs. Use ProcessLib for library dirs
  # for include dirs. Appending to <output file>.
  # This will recurse.
  #
  # Usage: ProcessDir <dir> <output file> <attr>
  #
  if [ -d $1 ] ; then
   if [ ! -z "$3" ] ; then
     echo "%defattr ($3)" >>$2
   fi
   echo "*** Processing $1"
   find $1 -type f -print | sed "s:^$RPM_BUILD_ROOT::g" >>$2
  fi
}

function BuildFiles() {
  ProcessBin
  ProcessLang
  for i in `find $RPM_BUILD_ROOT%{_prefix}/share -maxdepth 1 -type d -print | \
     sed "s:^$RPM_BUILD_ROOT%{_prefix}/share::g"` ; do
    echo $i
    case $i in
     /applets|/asclock|/clockmail|/geyes|/gnome|/gweather|/odometer|/sound-monitor|/pixmaps|/tickastat|/xmodmap|/omf)

         ProcessDir $RPM_BUILD_ROOT%{_prefix}/share$i %{name}.files "0444, bin, bin, 0555"
         ;;
     *)
         ;;
   esac
  done
  ProcessDir $RPM_BUILD_ROOT/etc %{name}.files "0444, bin, bin, 0555"
  ProcessLib $RPM_BUILD_ROOT%{_prefix}/lib runtime %{name}.files
}

BuildFiles

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

%post   -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files -f %{name}.files
%doc AUTHORS COPYING ChangeLog NEWS README
