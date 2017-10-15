cmake_minimum_required(VERSION 2.8)
project(SEPLIB-java-main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)

set (JCLASSDIR1 $(JSDIR)/classes/edu/stanford/sep/seplib/corelibs/sep/)
set (JSDIR java/edu/stanford/sep/seplib/)
set (SOURCES 
${JSDIR}/Auxpar.java
${JSDIR}/Auxputch.java
${JSDIR}/Datapath.java
${JSDIR}/Doc.java
${JSDIR}/Fetch.java
${JSDIR}/Getch.java
${JSDIR}/GetNumberAxes.java
${JSDIR}/GetPutAxis.java
${JSDIR}/Hclose.java
${JSDIR}/HelixCart.java
${JSDIR}/Hetch.java
${JSDIR}/Initpar.java
${JSDIR}/Noheader.java
${JSDIR}/Putch.java
${JSDIR}/Putlin.java
${JSDIR}/Redin.java
${JSDIR}/Redout.java
${JSDIR}/Sreed.java
${JSDIR}/SreedWindow.java
${JSDIR}/Srite.java
${JSDIR}/SriteWindow.java
${JSDIR}/Sseek.java
${JSDIR}/Ssize.java
${JSDIR}/Tetch.java
)

add_jar(sepcore ${SOURCES} MANIFEST ${CMAKE_CURRENT_SOURCE_DIR}/sepcore_manifest)

install_jar(sepcore DESTINATION lib)
