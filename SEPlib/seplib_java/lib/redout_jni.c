/* native method for Redout class static method redout() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Redout.h"
#include <seplib.h>

/*ARGSUSED*/
JNIEXPORT jboolean JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Redout_redout(JNIEnv *env, jclass cls)
{
   int rc;
   rc = redout();

   return (jboolean) (rc == 1);
}
