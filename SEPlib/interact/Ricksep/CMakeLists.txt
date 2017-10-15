project(SEPLIB-interact-Ricksep)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${MOTIF_INCLUDE_DIR}")
include_directories("${X11_Xt_INCLUDE_PATH}/.")

set(CSOURCES
ano.c
axis.c
colorbar.c
color.c
data.c
draw.c
dummy.c
edit.c
grab_param.c
help.c
main.c
map.c
movie.c
pick.c
pik.c
plane.c
region.c
render.c
rgrow.c
rotated.c
search.c
section.c
semb.c
ui_canvas.c
ui_menu.c
ui_panel.c
ui_window.c
view.c
)


ADD_EXECUTABLE(Ricksep ${CSOURCES})

set_target_properties(Ricksep PROPERTIES LINK_FLAGS "${MOTIF_LIBRARIES} ${X11_Xt_LIB} ${X11_X11_LIB} ${X11_XP_LIB} ${X11_Xext_LIB}")
TARGET_LINK_LIBRARIES(Ricksep seppar sep m)

install(PROGRAMS Ricksep DESTINATION bin)

