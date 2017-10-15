project(SEPLIB-vplot-filters)
cmake_minimum_required(VERSION 2.8)

ADD_EXECUTABLE(plas plas.c)
TARGET_LINK_LIBRARIES(plas vplot)

ADD_EXECUTABLE(pldb pldb.c)
TARGET_LINK_LIBRARIES(pldb vplot)

