/* native method for Putlin class static method putlin() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Putlin.h"
#include <seplib.h>

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Putlin_putlin
  (JNIEnv * env, jclass cls, jstring jtextline) {

   jboolean isCopy;
   const char *textline = (*env)->GetStringUTFChars(env,jtextline,&isCopy);
   jint rc = -1;

   if(textline != ((const char *) NULL)) {
      rc = (jint) putlin((char *) textline);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtextline,textline);
   }
 
   return rc;
}
