cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-glplot)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../seplib_base/include)
set (SOURCES 
gl_arrow.c     gl_erase.c	gl_minmax.c	gl_plottic.c	 gl_uarea.c
gl_axisint.c   gl_fat.c		gl_move.c	gl_plottitle.c	 gl_uarrow.c
gl_barint.c    gl_fillin.c	gl_nice.c	gl_purge.c	 gl_uclip.c
gl_barplot.c   gl_framenum.c	gl_opttic.c	gl_rotate1.c	 gl_udraw.c
gl_clip.c      gl_getscl.c	gl_padint.c	gl_rotate.c	 gl_umove.c
gl_color.c     gl_gridint.c	gl_penup.c	gl_simpleaxis.c  gl_upendn.c
gl_colorint.c  gl_gtext.c	gl_plotaxis.c	gl_stdplot.c	 gl_upmark.c
gl_coordint.c  gl_invmassage.c	gl_plotframe.c	gl_tfont.c	 gl_vplotint.c
gl_dash.c      gl_labelaxis.c	gl_plotgrid.c	gl_titleint.c	 gl_where.c
gl_dashfig.c   gl_labeltic.c	gl_plotint.c	gl_tjust.c
gl_draw.c      gl_massage.c	gl_plotpram.c	gl_transp.c)




add_library(glplot ${SOURCES})
install(TARGETS glplot DESTINATION lib)
