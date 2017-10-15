/* native method for Redin class static method redin() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Redin.h"
#include <seplib.h>

/*ARGSUSED*/
JNIEXPORT jboolean JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Redin_redin(JNIEnv *env, jclass cls)
{
   int rc;
   rc = redin();

   return (jboolean) (rc == 1);
}
