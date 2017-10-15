/* native method for Hclose class static method hclose() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Hclose.h"
#include <seplib.h>

/*ARGSUSED*/
JNIEXPORT void JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Hclose_hclose(JNIEnv *env, jclass cls)
{
   hclose();
}
