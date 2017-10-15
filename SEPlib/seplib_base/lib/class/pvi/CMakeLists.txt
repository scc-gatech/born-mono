cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-class-pvi)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../corelibs/sep)


set (SOURCES 
advance.f90   conjnull.f90     interp0.f90	mpwave.f90    shaper.f90
burg2.f90     contrunc.f90     invstack.f90	nonstat2.f90  slider.f90
burg.f90      convin.f90       kolmogoroff.f90	nonstat.f90   slowft.f90
butter.f90    convolve.f90     leak.f90		parcel.f90    tris.f90
causint2.f90  deghost.f90      leaky.f90	patch.f90     vspray.f90
cgmeth.f90    ftderivslow.f90  lomoplan.f90	pe2.f90       wavekill1.f90
cgstep.f90    ftlagslow.f90    misfip.f90	polydiv.f90   wcontrunc.f90
cinjof.f90    hestenes.f90     miss1.f90	polyft.f90    zero.f90
cinlof.f90    hope.f90	       miss2.f90	pop.f90
cinloi.f90    imo1.f90	       missif.f90	puck.f90
cinloip.f90   imospray.f90     mkwallwt.f90	pvi.f90
cinloiwt.f90  iner.f90	       moplan.f90	scaleit.f90)

add_library(seppvi ${SOURCES})
target_link_libraries(seppvi sepclassutil superset sep3d sep)
install(TARGETS seppvi DESTINATION lib)
