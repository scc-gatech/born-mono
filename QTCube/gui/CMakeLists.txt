cmake_minimum_required(VERSION 2.8)
project(qtcube-gui)
   set(CMAKE_AUTOMOC TRUE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../base)

set(INCLUDES anno.h		      maps.h		    pick_draw.h
ano_panel.h	      mode.h		    pick_new.h
auto_brown.h	      mouse_anno.h	    pick_panel.h
autopick_2d.h	      mouse_func.h	    pick_plane_new.h
autopick.h	      mouse_orient.h	    position.h
auto_viteribi.h       mouse_overlay.h	    qcube_panel.h
bar_pt.h	      mouse_pick.h	    qcubetabbasics.h
buffer.h	      mouse_region.h	    qcubewidgetbasics.h
byte_buffer.h	      mouse_zoom.h	    raster.h
centerwindow.h	      moved_out.h	   
     move_semblance.h	    
clip_bar.h	      multiface.h	    
color_panel.h	      multiple.h	   
contour.h	      my_colors.h	    simple_color.h
created_data.h       my_fonts.h	    single.h
cube_cut.h	      navigate_panel.h	    slice.h
cube.h		      nmoed.h		    slice_types.h
data_panel.h	      nmo_semblance.h	    
dataset.h	      orientation_server.h  surface_data.h
datasets.h	      orient_cube.h	    threeface.h
display_panel.h       orient_map.h	    three_view.h
draw_other.h	      orient_panel.h	    trace_io.h
drawwindow.h	      outcore_data.h	    util.h
fileIO.h	      pairs_new.h	    view.h
float_buffer.h       panel.h		    view_panel.h
 panel_panel.h	    view_types.h
help_panel.h	      panels.h		    widget_tools.h
history.h	      partial_data.h	    wiggle.h
incore_data.h	      part_semb_data.h	    window_panel.h
interval_pick.h       path.h		    windows.h
io_func.h	      pick_bucket.h)
set(SOURCES anno.cpp	       mode.cpp	       pick_new.cpp
ano_panel.cpp	       mouse_anno.cpp	       pick_panel.cpp
auto_brown.cpp	       mouse_func.cpp	       pick_plane_new.cpp
autopick_2d.cpp        mouse_orient.cpp        position.cpp
autopick.cpp	       mouse_overlay.cpp       qcube_panel.cpp
auto_viteribi.cpp      mouse_pick.cpp	       qcubetabbasics.cpp
buffer.cpp	       mouse_region.cpp        qcubewidgetbasics.cpp
byte_buffer.cpp        mouse_zoom.cpp	       raster.cpp
centerwindow.cpp       moved_out.cpp	      
    move_semblance.cpp     
clip_bar.cpp	       multiface.cpp	     
color_panel.cpp        multiple.cpp	       
contour.cpp	       my_colors.cpp	       simple_color.cpp
created_data.cpp      my_fonts.cpp	       single.cpp
cube.cpp	       navigate_panel.cpp      slice.cpp
cube_cut.cpp	       nmoed.cpp	       slice_types.cpp
data_panel.cpp	       nmo_semblance.cpp       
dataset.cpp	       orientation_server.cpp  surface_data.cpp
datasets.cpp	       orient_cube.cpp	       threeface.cpp
display_panel.cpp      orient_map.cpp	       three_view.cpp
draw_other.cpp        orient_panel.cpp        trace_io.cpp
drawwindow.cpp	       outcore_data.cpp       util.cpp
fileIO.cpp	       pairs_new.cpp	       view.cpp
float_buffer.cpp       panel.cpp	       view_panel.cpp
panel_panel.cpp	       view_types.cpp
help_panel.cpp	       panels.cpp	       widget_tools.cpp
history.cpp	       partial_data.cpp       wiggle.cpp
incore_data.cpp        part_semb_data.cpp      window_panel.cpp
interval_pick.cpp      path.cpp	       windows.cpp
io_func.cpp	       pick_bucket.cpp
maps.cpp	       pick_draw.cpp)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
add_library(gui 	${SOURCES} ${INCLUDES})
