/* native method for Initpar class static method initpar() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Initpar.h"
#include <seplib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <errno.h>

/*ARGSUSED*/
JNIEXPORT void JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Initpar_initpar
  (JNIEnv * env, jclass cls, jint jargc, jobjectArray jargv) {

   jsize lenArgv;
   jboolean isCopy;
   char **myArgv = (char **) NULL;
   const char *strTemp;
   jstring jstrTemp;
   jint i;
   
   /* silently repair bad arg count */
   lenArgv = (*env)->GetArrayLength(env,jargv);
   if(jargc > lenArgv) jargc = (jint) lenArgv;

   if(jargc == 0) {
      myArgv = calloc(1, sizeof(char *));
      initpar((int) jargc, myArgv);
      return;
   }
   
   myArgv = calloc(jargc, sizeof(char *));
   if(myArgv == ((char **) NULL)) {
     /* throw memory exception, maybe? */
     return;
   }

   errno = 0;
   for(i=0; i<jargc; i++) {
      jstrTemp = (jstring) ((*env)->GetObjectArrayElement(env,jargv,i));
      strTemp = (*env)->GetStringUTFChars(env,jstrTemp,&isCopy);
      myArgv[i] = strdup(strTemp);
      if(isCopy) (*env)->ReleaseStringUTFChars(env,jstrTemp,strTemp);
      if(errno == ENOMEM) break;
   }

   initpar((int) i, myArgv);

   return;
}
