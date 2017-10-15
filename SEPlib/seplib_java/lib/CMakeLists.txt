cmake_minimum_required(VERSION 2.8)
project(SEPLIB-java-lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../..)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../seplib_base/include)
include_directories(${JNI_INCLUDE_DIRS})

set (SOURCES 
auxpar_jni.c
auxputch_jni.c
datapath_jni.c
doc_jni.c
fetch_jni.c
getch_jni.c
getnumberaxes_jni.c
getputaxis_jni.c
hclose_jni.c
hetch_jni.c
initpar_jni.c
noheader_jni.c
putch_jni.c
putlin_jni.c
redin_jni.c
redout_jni.c
sreed_jni.c
sreedwindow_jni.c
srite_jni.c
sritewindow_jni.c
sseek_jni.c
ssize_jni.c
tetch_jni.c
)

add_library(sep_JNI SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(sep_JNI sep)
install(TARGETS sep_JNI DESTINATION lib)
