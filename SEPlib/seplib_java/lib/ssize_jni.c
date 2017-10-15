/* native method for Sreed class static methods sseek() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Ssize.h"
#include <seplib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <errno.h>

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Ssize_ssize
  (JNIEnv *env, jclass cls, jstring jtag) {
   char *tag;
   jboolean isCopy;
   jint rc = -1;

   tag = (char *) ((*env)->GetStringUTFChars(env,jtag,&isCopy));

   if(tag != ((const char *) NULL)) {
      rc = (jint) ssize(tag);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag, tag);
   }

   return rc;
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Ssize_ssize_1block
  (JNIEnv *env, jclass cls, jstring jtag, jint jblocksize) {
   char *tag;
   jboolean isCopy;
   jint rc = -1;
   int blocksize;

   tag = (char *) ((*env)->GetStringUTFChars(env,jtag,&isCopy));

   if(tag != ((const char *) NULL)) {
      blocksize = (int) jblocksize;
      rc = (jint) ssize_block(tag, blocksize);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag, tag);
   }

   return rc;
}
