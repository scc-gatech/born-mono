/* native method for Sreed class static methods sseek() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Sseek.h"
#include <seplib.h>
#include <sep_main_external.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <errno.h>

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Sseek_sseek
  (JNIEnv *env, jclass cls, jstring jtag, jint joffset, jint jwhence) {
   const char *tag;
   jboolean isCopy;
   jint rc = -1;
   int offset, whence;

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag != ((const char *) NULL)) {
      offset = (int) joffset;
      whence = (int) jwhence;
      rc = (jint) sseek(tag, offset, whence);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag, tag);
   }

   return rc;
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Sseek_sseek_1block
  (JNIEnv *env, jclass cls, jstring jtag, jint joffset, jint jblocksize, jint jwhence) {
   const char *tag;
   jboolean isCopy;
   jint rc = -1;
   int offset, blocksize, whence;

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag != ((const char *) NULL)) {
      offset = (int) joffset;
      blocksize = (int) jblocksize;
      whence = (int) jwhence;
      rc = (jint) sseek_block(tag, offset, blocksize, whence);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag, tag);
   }

   return rc;
}

/*ARGSUSED*/
JNIEXPORT jdouble JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Sseek_sseek_1block_1d
  (JNIEnv *env, jclass cls, jstring jtag, jint joffset, jint jblocksize, jint jwhence) {
   const char *tag;
   jboolean isCopy;
   jdouble rc = -1.0;
   int offset, blocksize, whence;

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag != ((const char *) NULL)) {
      offset = (int) joffset;
      blocksize = (int) jblocksize;
      whence = (int) jwhence;
      rc = (jdouble) sseek_block_d(tag, offset, blocksize, whence);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag, tag);
   }

   return rc;
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Sseek_file_1position
  (JNIEnv *env, jclass cls, jstring jtag, jint jblocksize, jintArray jblocks, jintArray jremainder) {
   const char *tag;
   jboolean isCopy;
   jint rc = -1;
   int blocksize, blocks, remainder;
   jint jbl, jrm;

   /* ensure we have space in output arguments */
   if(1 > (*env)->GetArrayLength(env,jblocks) ||
      1 > (*env)->GetArrayLength(env,jremainder)) return rc;

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag != ((const char *) NULL)) {
      blocksize = (int) jblocksize;
      rc = (jint) file_position(tag,blocksize,&blocks,&remainder);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag, tag);
   }

   if(rc == 0) { /* copy out results */
      jbl = blocks;
      (*env)->SetIntArrayRegion(env,jblocks,0,1,&jbl);
      jrm = remainder;
      (*env)->SetIntArrayRegion(env,jremainder,0,1,&jrm);
   }

   return rc;
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Sseek_sseekable
  (JNIEnv *env, jclass cls, jstring jtag){
   const char *tag;
   jboolean isCopy;
   jint rc = 1; /* default to not seekable initially */

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag != ((const char *) NULL)) {
      rc = (jint) sseekable(tag);
      if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag, tag);
   }

   return(rc);
}
