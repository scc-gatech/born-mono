cmake_minimum_required(VERSION 2.8)
project(SEPLIB-vplot-libvplit)

set (SOURCES 
	  vp_coltab.c	  vp_message.c	    vp_rastershort.c  vp_uclip.c
	  vp_dash.c	  vp_move.c	    vp_scale.c	      vp_udraw.c
geth.c		  vp_draw.c	  vp_orig.c	    vp_setdash.c      vp_ufill.c
name_to_coltab.c  vp_egroup.c	  vp_patload.c	    vp_stdfile.c      vp_ugtext.c
puth.c		  vp_endplt.c	  vp_pendn.c	    vp_stretch.c      vp_umove.c
vp_arc.c	  vp_erase.c	  vp_penup.c	    vp_style.c	      vp_uorig.c
vp_area.c	  vp_fat.c	  vp_pline.c	    vp_text.c	      vp_upendn.c
vp_arrow.c	  vp_file.c	  vp_plot.c	    vp_tfont.c	      vp_upline.c
vp_bgroup.c	  vp_filep.c	  vp_pmark.c	    vp_tjust.c	      vp_uplot.c
vp_break.c	  vp_fill.c	  vp_purge.c	    vp_uarc.c	      vp_upmark.c
vp_circle.c	  vp_fixpc.c	  vp_rascol16tab.c  vp_uarea.c	      vp_uraster.c
vp_clip.c	  vp_gtext.c	  vp_rascoltab.c    vp_uarrow.c       vp_utext.c
vp_color.c	  vp_hatchload.c  vp_raster.c	    vp_ucircle.c      vp_where.c)



add_library(vplot ${SOURCES})
TARGET_LINK_LIBRARIES(vplot m)
install(TARGETS vplot DESTINATION lib)
