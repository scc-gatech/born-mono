Summary: Geophysics software
Name: seplib-distr
Version: 1.0
Release: 1
Packager: Bob Clapp <bob@sep.stanford.edu>
Copyright: Stanford University
Group: Development/Libraries
URL: http://sepwww.stanford.edu/
Source: ftp://sepftp.stanford.edu/pub/sep-distr/ratfor90-8.0.1.tar.gz
Requires: perl >= 5.001

%description
Geophysics visualization software. Useful for people studying what
goes on inside the earth.

%changelog
* Mon Jan 22 2001 bob@sep.stanford.edu
- Created this RPM


%prep
%setup -n ratfor90-8.0.1

%build
./configure 
make

%install
make install
%clean
make clean
%files
%attr(-,root,root) /usr/local/bin/ratfor90
