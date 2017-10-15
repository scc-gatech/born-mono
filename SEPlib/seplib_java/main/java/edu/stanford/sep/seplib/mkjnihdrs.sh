#!/bin/csh -f
foreach i (*.java)
set j = `basename $i .java`
cd classes
javah -jni edu.stanford.sep.seplib.corelibs.sep.$j
cd ..
echo $j
cd classes
mv -f *.h ../../../../../../../lib/.
cd ..
end
