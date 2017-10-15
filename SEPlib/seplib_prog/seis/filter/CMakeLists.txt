project(SEPLIB-prog-seis-filter)
cmake_minimum_required(VERSION 2.8)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../../seplib_base/lib/corelibs")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../include")

if(FFTW_LIBRARIES)
MESSAGE("FFTW INSTALLED")
set (sppflags "-DUSE_FFTW" )
include_directories(${FFTW_INCLUDES})
endif(FFTW_LIBRARIES)

set(FORTRANSEP 
Cfft.F90sep	Fx2d.F90sep	MostlyCausal.F90sep	Transf.F90sep
Halfint.F90sep	Helderiv.F90sep	Helicon.F90sep		Kolmogorov.F90sep
Median.F90sep	Miss.F90sep	Mute3d.F90sep	      	Dip.F90sep
Mute.F90sep	Edit.F90sep	Txdec.F90sep		Envelope.F90sep
Noise.F90sep	FFT5D.F90sep	Pef.F90sep		Detrend.F90sep
)

set(CSEP
Again.csep    Bandpass.csep  Filter.csep  Interp.csep  Spectra.csep
Agc.csep      Cwt.csep       Ft3d.csep	  Lpfilt.csep  Tpow.csep
Balance.csep  Energy.csep    Gpow.csep	  Smooth.csep  Trcamp.csep
PCAcluster.csep
)


FOREACH(fort ${FORTRANSEP})
  STRING(REGEX REPLACE ".F90sep\$" ".f90s" fortFileS "${fort}")
  STRING(REGEX REPLACE ".f90s\$" ".f90" fortFile ${fortFileS})
  STRING(REGEX REPLACE ".F90sep\$" "" prog "${fort}")
  set(PROGS "${PROGS} ${prog}")

   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFileS}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${fort} -fortran  >${fortFileS}
     DEPENDS "${fort}")
  ADD_CUSTOM_COMMAND(OUTPUT "${fortFile}"
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/tools/spp/spp < ${fortFileS} ${sppflags} >${fortFile}
     DEPENDS "${fortFileS}")
    ADD_EXECUTABLE(${prog}  ${prog}.f90)
   install(TARGETS ${prog} DESTINATION bin)
ENDFOREACH(fort)


FOREACH(cprog ${CSEP})
  STRING(REGEX REPLACE ".csep\$" ".c" fortFile "${cprog}")
  STRING(REGEX REPLACE ".csep\$" "" prog "${cprog}")
  set(PROGS "${PROGS} ${prog}")
   ADD_CUSTOM_COMMAND(OUTPUT  "${fortFile}"
     COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../../../seplib_base/include/sep_doc_it <${CMAKE_CURRENT_SOURCE_DIR}/${cprog} -C  >${fortFile}
     DEPENDS "${cprog}")
    ADD_EXECUTABLE(${prog}  ${prog}.c)
   install(TARGETS ${prog} DESTINATION bin)
    set_target_properties (${prog} PROPERTIES COMPILE_DEFINITIONS SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/${prog}.c")
ENDFOREACH(cprog)


TARGET_LINK_LIBRARIES(Transf sepfilter sepfft superset sep3d sep ${FFTW_LIBRARIES})
TARGET_LINK_LIBRARIES(Halfint sepfilter sep3d sep)
set_property(TARGET Mute3d PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(Mute3d sepaux sepgeef90 sepclassutil sepmath  superset sep3d sep)
TARGET_LINK_LIBRARIES(Edit superset sep3d sep)
TARGET_LINK_LIBRARIES(Helicon sepgeef90 sepclassutil sep3d sep)
set_property(TARGET Mute PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(Mute superset sep3d sep)
TARGET_LINK_LIBRARIES(Txdec superset sep3d sep)
TARGET_LINK_LIBRARIES(Envelope sepfilter sepfft sepvector sep3d sep ${FFTW_LIBRARIES})
set_property(TARGET Median PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(Median sepaux sepmath sep3d sep)
TARGET_LINK_LIBRARIES(Smooth superset sep3d sep)
set_property(TARGET Noise PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(Noise  sepmath superset sep3d sep)
set_property(TARGET FFT5D PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(FFT5D sepfft sepaux sepmath sep3d sep ${FFTW_LIBRARIES})
TARGET_LINK_LIBRARIES(Smooth superset sep3d sep m)
TARGET_LINK_LIBRARIES(Miss  sepgeef90 sepclassutil sep3d sep)
TARGET_LINK_LIBRARIES(Pef sepgeef90 sepclassutil superset sep3d sep)
TARGET_LINK_LIBRARIES(Dip sepdip sepfilter sepgeef90 seppvi sepbei sepclassutil superset sep3d sep)
set_property(TARGET MostlyCausal PROPERTY LINKER_LANGUAGE Fortran)
TARGET_LINK_LIBRARIES(MostlyCausal sepaux sepmath sep3d sep)
TARGET_LINK_LIBRARIES(Fx2d superset sep3d sep)
TARGET_LINK_LIBRARIES(Cfft superset sep3d sep)
TARGET_LINK_LIBRARIES(Trcamp superset sep3d sep m)
TARGET_LINK_LIBRARIES(Smooth superset sep3d sep m)
TARGET_LINK_LIBRARIES(Gpow  superset sep3d sep m)
TARGET_LINK_LIBRARIES(Energy sepfilter sep3d sep m)
TARGET_LINK_LIBRARIES(Balance superset sep3d sep m)
TARGET_LINK_LIBRARIES(Tpow superset sep3d sep m)
TARGET_LINK_LIBRARIES(Lpfilt superset sep3d sep m)
TARGET_LINK_LIBRARIES(Ft3d sepfft sep3d sep m)
TARGET_LINK_LIBRARIES(Cwt superset sep3d sep m)
TARGET_LINK_LIBRARIES(Agc superset sep3d sep m)
TARGET_LINK_LIBRARIES(Spectra sepfft superset sep3d sep m)
TARGET_LINK_LIBRARIES(Interp sepfilter sep3d sep pthread)
TARGET_LINK_LIBRARIES(Filter sepfft sepaux sepmath superset sep3d sep m)
TARGET_LINK_LIBRARIES(Bandpass superset sep3d sep m)
TARGET_LINK_LIBRARIES(Again superset sep3d sep m)
TARGET_LINK_LIBRARIES(PCAcluster sepmath sep)
TARGET_LINK_LIBRARIES(Kolmogorov sepgeef90 seppvi sepclassutil sepfft sepaux sep3d sep ${FFTW_LIBRARIES})
TARGET_LINK_LIBRARIES(Helderiv sepgeef90 sepclassutil sep3d sep)
TARGET_LINK_LIBRARIES(Detrend sepgeef90 sepclassutil sep3d sep)


install(PROGRAMS ${progs} DESTINATION bin)

