cmake_minimum_required(VERSION 2.8)
project(Generic-cpp)

set(SOURCES  basicIO.cpp  genericFile.cpp  jsoncpp.cpp)
set(SOURCES ${SOURCES} jsonParamObj.cpp jsonGenericFile.cpp genericIO.cpp genericC.cpp)
set (SOURCES ${SOURCES} jsonGenericIO.cpp ioModes.cpp segyIO.cpp)
set(INCLUDES  genericIO.h jsonGenericFile.h	jsonParamObj.h basicIO.h  genericC.h)
set(INCLUDES ${INCLUDES} jsonGenericIO.h	paramObj.h genericFile.h json.h ioModes.h segyIO.h)

if(USE_SEP)
include_directories(${SEPlib_DIRNAME_include})
  set(SOURCES  ${SOURCES} sepIO.cpp  sep3dFile.cpp sep_reg_file.cpp  sep_params.cpp )
  set(INCLUDES ${INCLUDES} sep_params.h sepIO.h sep3dFile.h sep_reg_file.h)
endif(USE_SEP)

if(USE_RSF)
include_directories(${RSFINCDIR})
  set(SOURCES  ${SOURCES} rsfIO.cpp	rsfParams.cpp  rsfRegFile.cpp)
  set(INCLUDES ${INCLUDES}  rsfIO.h rsfParams.h rsfRegFiles.h ) 
endif(USE_RSF)

add_library(genericCpp ${SOURCES} ${INCLUDES})
install(FILES ${INCLUDES} DESTINATION include)
install(TARGETS genericCpp DESTINATION lib EXPORT genericIOExports)
