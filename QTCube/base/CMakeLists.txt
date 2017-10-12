cmake_minimum_required(VERSION 2.8)
project(qtcube-base)

include_directories(${GENERICIOINCDIR})

#include_directories(/sep/bob/genericIO/include)
set(SOURCES dynamic.cpp    hypercube_float.cpp  norm_vector.cpp  pqueue.cpp)
set(SOURCES ${SOURCES} fastmarch.cpp  l2_norm.cpp	  )
set (SOURCES ${SOURCES} float_1d.cpp   lloyd_1d.cpp	    path.cpp	    )
set (SOURCES ${SOURCES} float_2d.cpp   lloyd.cpp	    percentile.cpp   viterbi.cpp)
set(INCLUDES float_1d.h  float_2d.h  hypercube_float.h percentile.h norm_vector.h  l2_norm.h)
set(INCLUDES ${INCLUDES}  norm.h  my_vector.h  lloyd_1d.h lloyd.h  fastmarch.h)
set(INCLUDES   pqueue.h viterbi.h my_score.h dynamic.h path.h)


add_library(base ${SOURCES} ${INCLUDES})

