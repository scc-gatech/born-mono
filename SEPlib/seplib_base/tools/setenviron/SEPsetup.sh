#!/bin/sh
#
# SEPsetup.sh: Set up paths for invoking SEP commands in sh or bash
#
# To use run the command
#
#    .  $SEP/bin/SEPsetup.sh
#
if [ `basename ./$0` = SEPsetup.sh ] ; then
echo "Do not execute SEPsetup.sh!  Use   \". SEPsetup.sh\"   instead."
else
newPATH87=${SEP:?'required environment variable SEP missing'}/bin:${PATH}
if [ ! -d ${SEP} ] ; then
echo "${SEP} is not a directory. Exiting ..."
else
PATH="${newPATH87}"
export PATH
SEPINC=${SEP}/include
export SEPINC
myld=""
if [ ] ; then
mytmp=/tmp/${USER}_junk$$
( for i in `/sbin/ldconfig -p | grep '=>' | awk '{ print $NF }'` ; do dirname $i ; done ) | sort -r | uniq > ${mytmp}
for j in `cat ${mytmp}` ; do myld=${j}:${myld} ; done
rm -f ${mytmp}
fi
LD_LIBRARY_PATH=${myld}${LD_LIBRARY_PATH}:${SEP}/lib:${SEP}/lib/syslibs
export LD_LIBRARY_PATH
DYLD_FALLBACK_LIBRARY_PATH=${myld}${DYLD_FALLBACK_LIBRARY_PATH}:${SEP}/lib:${SEP}/lib/syslibs
export LD_LIBRARY_PATH DYLD_FALLBACK_LIBRARY_PATH
MANPATH=${SEP}/share/man:${MANPATH}
export MANPATH
PYTHONPATH=${SEP}/lib/python:${PYTHONPATH}
export PYTHONPATH
LANG=${LANG:-'C'}
export LANG
fi
unset newPATH87
fi
