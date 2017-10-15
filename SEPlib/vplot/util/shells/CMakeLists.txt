cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-tool-makedepend)



set(SCRIPTS vp_Movie   vp_SideBySideAniso
vp_Overlay		      vp_SideBySideIso
pstexpen	vp_OverUnderAniso
vp_annotate vp_Unrotate
Rasterize	vp_OverUnderIso)

install(PROGRAMS ${SCRIPTS} DESTINATION bin)
