Summary: Geophysics software
Name: seplib-prog-local
Version: 6.0
Release: 1
Packager: Bob Clapp <bob@sep.stanford.edu>
Copyright: Stanford University
Group: Development/Libraries
URL: http://sepwww.stanford.edu/
Source: ftp://sepftp.stanford.edu/pub/sep-distr/seplib_prog-6.0.tar.gz
Requires: gcc-g77
Requires: gcc
Requires: make >= 3.74
Requires: perl >= 5.001
Requires: gawk >= 3.0.3
Requires: tcsh
Requires: ld-linux.so.2
Requires: libICE.so.6
Requires: libSM.so.6
Requires: libX11.so.6
Requires: libXaw.so.6
Requires: libXext.so.6
Requires: libXt.so.6 
Requires: libc.so.6
Requires: libm.so.6

%description
Geophysics visualization software. Useful for people studying what
goes on inside the earth.

%changelog
* Mon Jan 22 1999 Bob New version
* Mon Oct 25 1999 Bob New version
* Mon Dec 14 1998 Jeff Breidenbach <jeff@jab.org>
- Created this RPM


%prep
%setup -n seplib-prog-6.0

%build
csh DIST_BUILD
make all

%install
make install

mkdir /usr/local/SEP/src;cd /usr/local/SEP/src; gunzip -c /usr/src/redhat/SOURCES/seplib_prog-6.0.tar.gz | tar x; mv /usr/local/SEP/src/seplib-prog-6.0 /usr/local/SEP/src/seplib_prog

%clean
make clean
%files
%attr(-,root,root) /usr/local/SEP
