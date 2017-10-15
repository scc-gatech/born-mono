cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-seis-wei)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../include)


set (FSOURCES 
lloyd.f90s)

if(FFTW_LIBRARIES)

set (sppflags "-DUSE_FFTW" )

set(FSOURCES ${FSOURCES}
	  wei_fk_share.f90s  wei_path.f90s      wei_timers.f90s
weiattr.f90s	  wei_flags.f90s     wei_pspi.f90s      wei_verb.f90s
wei_basics.f90s	  wei_image.f90s     wei_wavespace.f90s
wei_cam.f90s	  wei_imaging.f90s   wei_run.f90s       wei_wem.f90s
weicig.f90s	  wei_kmap.f90s	    wei_sll.f90s       wei_xmap.f90s
wei_data.f90s	  wei_mem.f90s	    wei_slow.f90s
wei_external.f90s  wei_mig_base.f90s  wei_ssf.f90s
wei_fft.f90s	  wei_par.f90s	    weitds.f90s)

endif(FFTW_LIBRARIES)

FOREACH (fort ${FSOURCES})
  STRING(REGEX REPLACE ".f90s\$" ".f90" fortFile "${fort}")
  ADD_CUSTOM_COMMAND(OUTPUT "${fortFile}"
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../../../tools/spp/spp <${CMAKE_CURRENT_SOURCE_DIR}/${fort} ${sppflags} >${fortFile}
    DEPENDS "${fort}" )
  set (SOURCES ${SOURCES} ${fortFile})
ENDFOREACH(fort)

add_library(sepwei ${SOURCES})

if(FFTW_LIBRARIES)
target_link_libraries (sepwei sepgeef90 sep3d sep ${FFTW_LIBRARIES} m)
else(FFTW_LIBRARIES)
target_link_libraries (sepwei m)
endif(FFTW_LIBRARIES)

install(TARGETS sepwei DESTINATION lib)
