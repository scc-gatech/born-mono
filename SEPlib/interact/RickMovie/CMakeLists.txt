project(SEPLIB-interact-RickMovie)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include_directories("${MOTIF_INCLUDE_DIR}")
include_directories("${X11_Xt_INCLUDE_PATH}/.")

set(CSEP
    sep_axis.c
    sep_colorbar.c
    sep_color.c
    sep_data.c
    sep_draw.c
    sep_edit.c
    sep_help.c
    sep_main.c
    sep_map.c
    sep_movie.c
    sep_pick.c
    sep_pik.c
    sep_plane.c
    sep_region.c
    sep_render.c
    sep_section.c
    sep_ui_canvas.c
    sep_ui_menu.c
    sep_ui_panel.c
    sep_ui_window.c
    sep_view.c
)


ADD_EXECUTABLE(RickMovie ${CSEP})

set_target_properties(RickMovie PROPERTIES LINK_FLAGS "${MOTIF_LIBRARIES} ${X11_Xt_LIB} ${X11_X11_LIB} ${X11_XP_LIB} ${X11_Xext_LIB}")

TARGET_LINK_LIBRARIES(RickMovie sep)

install(PROGRAMS RickMovie DESTINATION bin)

