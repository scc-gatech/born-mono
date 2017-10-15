project(SEPLIB-interact-qt_cube)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include")
include(${QT_USE_FILE})
include_directories("${QT_INCLUDE_DIR}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_QT3SUPPORT ON)
set(CMAKE_AUTOMOC ON)

set(SOURCES
allpass.cpp
auto_brown.cpp
autopick_2d.cpp
autopick.cpp
autopick_menu.cpp
auto_viteribi.cpp
axis.cpp
buffer.cpp
byte_buffer.cpp
centerwindow.cpp
cg.cpp
change.cpp
clip_bar.cpp
clipwindow.cpp
color_bar.cpp
color_menu.cpp
contour.cpp
created_data.cpp
cube.cpp
cube_cut.cpp
cuemake_image_collection.cpp
dataset.cpp
datasets.cpp
draw_other.cpp
drawwindow.cpp
dynamic.cpp
fastmarch.cpp
float_1d.cpp
float_2d.cpp
float_buffer.cpp
float_mmap_buffer.cpp
help_menu.cpp
hypercube.cpp
hypercube_float.cpp
incore_data.cpp
interval_pick.cpp
io_func.cpp
l2_norm.cpp
lloyd_1d.cpp
lloyd.cpp
main.cpp
main_menu.cpp
menu_bar.cpp
menu_base.cpp
mode.cpp
mouse_func.cpp
mouse_orient.cpp
mouse_pick.cpp
mouse_zoom.cpp
moved_out.cpp
move_semblance.cpp
multiple.cpp
my_colors.cpp
navigate_menu.cpp
nmoed.cpp
nmo_semblance.cpp
norm_vector.cpp
orient_cube.cpp
orient_map.cpp
orient_menu.cpp
orientation_server.cpp
outcore_data.cpp
pairs_new.cpp
param_func.cpp
partial_data.cpp
part_semb_data.cpp
path.cpp
percentile.cpp
pick_bucket.cpp
pick_draw.cpp
pick_menu.cpp
pick_new.cpp
pick_plane_new.cpp
position.cpp
pqueue.cpp
qrc_qt_cube.cpp
raster.cpp
segy_reg_io.cpp
seispak_reg_io.cpp
sep_params.cpp
sep_reg_io.cpp
sep_reg_mmap_io.cpp
simple_color.cpp
single.cpp
slice.cpp
slice_types.cpp
sreg.cpp
sregf.cpp
statuswindow.cpp
su_reg_io.cpp
surface_data.cpp
threeface.cpp
three_view.cpp
tmp_tree.cpp
trace_io.cpp
updater.cpp
util.cpp
view.cpp
view_menu.cpp
viterbi.cpp
wiggle.cpp
)
set(MOD_HEADERS
autopick_menu.h
centerwindow.h
clip_bar.h
clipwindow.h
color_bar.h
dataset.h
drawwindow.h
io_func.h
main_menu.h
menu_bar.h
menu_base.h
navigate_menu.h
orient_menu.h
pick_menu.h
statuswindow.h
updater.h
view_menu.h
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HEADERS})

ADD_EXECUTABLE(qt_cube ${SOURCES} ${MOC_SRCS})

TARGET_LINK_LIBRARIES(qt_cube ${QT_LIBRARIES} ${XT_LIBRARIES} seppar sep)

install(PROGRAMS qt_cube DESTINATION bin)

configure_file(
    "${PROJECT_SOURCE_DIR}/Sep_cube.py.in"
    "${PROJECT_BINARY_DIR}/Sep_cube"
)

install(PROGRAMS Sep_cube DESTINATION bin)
