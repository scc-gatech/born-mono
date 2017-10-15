#!/bin/csh -f
#
# SEPsetup.csh: Set up paths for invoking SEP commands in csh or tcsh
#
# To use run the command
#
#    source  $SEP/bin/SEPsetup.sh
#
if ( "$0" =~ SEPsetup ) then
echo 'Do not execute SEPsetup.csh, "source" it instead'
else
if ( ! ${?SEP} ) then
echo 'Required environment variable SEP missing'
else
if ( ! -d ${SEP} ) then
echo 'Required environment variable SEP is not a directory'
else
setenv SEPINC ${SEP}/include
if ( ! ${?path} ) then
set path=(${SEP}/bin)
else
set path=(${SEP}/bin $path)
endif
set myld=""
if ( 0 ) then
set mytmp1=/tmp/${USER}_1junk$$
set mytmp2=/tmp/${USER}_2junk$$
/sbin/ldconfig -p | grep '=>' | awk '{print $NF }' > ${mytmp1}
cat /dev/null > ${mytmp2}
foreach i (`cat ${mytmp1}`)
dirname $i >> ${mytmp2}
end
sort -r ${mytmp2} | uniq > ${mytmp1}
rm -f ${mytmp2}
foreach j (`cat ${mytmp1}`)
set myld=${j}:${myld}
end
rm -f ${mytmp1}
endif
if ( ! ${?LD_LIBRARY_PATH} ) then
setenv LD_LIBRARY_PATH ${myld}${SEP}/lib:${SEP}/lib/syslibs
else
setenv LD_LIBRARY_PATH ${myld}${LD_LIBRARY_PATH}:${SEP}/lib:${SEP}/lib/syslibs
endif
if ( ! ${?DYLD_FALLBACK_LIBRARY_PATH} ) then
setenv DYLD_FALLBACK_LIBRARY_PATH ${myld}${SEP}/lib:${SEP}/lib/syslibs
else
setenv DYLD_FALLBACK_LIBRARY_PATH ${myld}${DYLD_FALLBACK_LIBRARY_PATH}:${SEP}/lib:${SEP}/lib/syslibs
endif
if ( ! ${?MANPATH} ) then
setenv MANPATH ${SEP}/share/man
else
setenv MANPATH ${SEP}/share/man:${MANPATH}
endif
if ( ! ${?PYTHONPATH} ) then
setenv PYTHONPATH ${SEP}/lib/python
else
setenv PYTHONPATH ${SEP}/lib/python:${PYTHONPATH}
endif
if ( ! ${?LANG} ) then
setenv LANG 'C'
endif
endif
endif
endif
