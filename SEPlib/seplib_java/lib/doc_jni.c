/* native method for Doc class static method doc() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Doc.h"
#include <seplib.h>

/*ARGSUSED*/
JNIEXPORT void JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Doc_doc
  (JNIEnv * env, jclass cls, jstring jSource) {

   jboolean isCopy;
   const char *source = (*env)->GetStringUTFChars(env,jSource,&isCopy);

   if(source != ((const char *) NULL)) {
      doc((char *) source);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jSource,source);
   }
}
