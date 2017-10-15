project(SEPLIB-vplot-filters-fonts)
cmake_minimum_required(VERSION 2.8)



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
ADD_EXECUTABLE(makefont makefont.c)
TARGET_LINK_LIBRARIES(makefont vplot)


FILE(GLOB FONTS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.vplot_font")
  
FOREACH(font ${FONTS})
  STRING(REGEX REPLACE ".vplot_font\$" "" base "${font}")
  set(baseloc "${CMAKE_CURRENT_BINARY_DIR}/${base}")
  set(infile "${CMAKE_CURRENT_SOURCE_DIR}/${base}.vplot_font")
  set(myinc "${baseloc}.include")
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
   ADD_CUSTOM_COMMAND(OUTPUT  "${outfile}"
     COMMAND  ${CMAKE_CURRENT_BINARY_DIR}/makefont <${infile}  ${base} >${myinc}
     COMMAND  cat ${mycheck} ${myheader}  ${mydim} ${myaddr} ${mywidthl} ${mywidthr} ${mysymbol} ${mysvec} ${myig} > ${outfile}
     DEPENDS "${infile}" makefont
     )
    set_source_files_properties(
    ${outfile}
    PROPERTIES GENERATED TRUE
    )
   ADD_DEPENDENCIES(genlib ${outfile})
   MESSAGE("TRIED TO DO THIS genlib ${outfile}")
  set(fontFiles ${fontFiles} "${outfile}")
ENDFOREACH(font)
ADD_CUSTOM_TARGET(ProcessFiles ALL DEPENDS ${fontFiles})
install(FILES ${fontFiles} DESTINATION include)
