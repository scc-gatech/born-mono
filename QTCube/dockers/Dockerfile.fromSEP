From rgc007/seplib:gcc6.2
MAINTAINER Bob Clapp <bob@sep.stanford.edu>
RUN yum -y install qt-devel; yum -y clean all
RUN git clone http://zapad.Stanford.EDU/bob/QTCube.git /opt/sepcube
RUN export SEP=/opt/SEP; cd /opt/sepcube; qmake-qt4 -recursive -o Makefile qt_cube.pro; make ; cp Sep_cube qt_cube /opt/SEP/bin; rm -rf /opt/sepcube
RUN echo export QTCUBE >>~/.bash_profile
