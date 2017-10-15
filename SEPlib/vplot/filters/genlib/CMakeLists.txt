cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-filters-genlib)

set (SOURCES 
genarea.c	  geninteract.c  genpatarea.c  genraster.c  nulldev.c	vecarea.c
gen_do_dovplot.c  genmarker.c	 genpoint.c    gentext.c    polyfix.c
genhatch.c	  genmessage.c	 genraster1.c  genvector.c  polysubs.c)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
ADD_EXECUTABLE(makefont makefont.c)
TARGET_LINK_LIBRARIES(makefont m c)


add_library(genlib ${SOURCES})

FILE(GLOB FONTS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vplot_font")

MACRO(ADD_FILE_DEPENDANCY file) 
   SET(${file}_deps ${${file}_deps} ${ARGN}) 
   SET_SOURCE_FILES_PROPERTIES( ${file} 
      PROPERTIES OBJECT_DEPENDS "${${file}_deps}" ) 
ENDMACRO(ADD_FILE_DEPENDANCY) 

FOREACH(font ${FONTS})
  STRING(REGEX REPLACE ".vplot_font\$" "" base "${font}")
  set(baseloc "${CMAKE_CURRENT_BINARY_DIR}/${base}")
  set(infile "${CMAKE_CURRENT_SOURCE_DIR}/${base}.vplot_font")
  set(myinc "${base}.include")
  set(myheader "${baseloc}_header")
  set(mydim "${baseloc}_dim")
  set(mycheck "${baseloc}_check")
  set(myaddr "${baseloc}_addr")
  set(mywidthl "${baseloc}_widthl")
  set(mywidthr "${baseloc}_widthr")
  set(mysymbol "${baseloc}_symbol")
  set(mysvec "${baseloc}_svec")
  set(mylig "${baseloc}_lig")
  set(mybin "${baseloc}.bin")
  SET(outfile "${mybin}")
   ADD_CUSTOM_TARGET("${myinc}"
     COMMAND  ${CMAKE_CURRENT_BINARY_DIR}/makefont ${base} <${infile} >${myinc}
     COMMAND  cat ${mycheck} ${myheader}  ${mydim} ${myaddr} ${mywidthl} ${mywidthr} ${mysymbol} ${mysvec} ${myig} > ${outfile}
     DEPENDS "${infile}" makefont
     )
    add_dependencies(genlib  ${myinc})
ENDFOREACH(font)
ADD_CUSTOM_TARGET(ProcessFiles ALL DEPENDS ${fontFiles})

install(FILES ${fontFiles} DESTINATION include)
install(TARGETS genlib DESTINATION lib)
