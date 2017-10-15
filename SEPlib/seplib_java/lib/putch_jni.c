/* native method for Putch class static methods putch() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_Putch.h"
#include <seplib.h>

#define BODYCODE(Typeval, typeval, mytypeval) \
  j ## typeval *valBuf; \
  jsize valBufLen; \
  jboolean isCopyName, isCopyFormat; \
  const char *name, *format; \
  jint rc; \
  jint i; \
  j ## typeval *tmpVals; \
  mytypeval *myVals; \
  name = (*env)->GetStringUTFChars(env,jname,&isCopyName); \
  if(name == ((char *) NULL)) return(-1); /* an exception was thrown */ \
  format = (*env)->GetStringUTFChars(env,jformat,&isCopyFormat); \
  if(format == ((char *) NULL)) return(-1); /* an exception was thrown */ \
  valBufLen = (*env)->GetArrayLength(env, jvals); \
  if(valBufLen < 1) return(-1); /* exception will have occurred */ \
  tmpVals = (j ## typeval *) calloc(valBufLen,sizeof(j ## typeval)); \
  if(tmpVals != (j ## typeval *) NULL) { \
  (*env)->Get ## Typeval ## ArrayRegion(env,jvals,0,valBufLen,tmpVals); \
  myVals = (mytypeval *) calloc(valBufLen+1, sizeof(mytypeval)); \
  if(myVals != (mytypeval *) NULL) {\
  for(i = 0; i < valBufLen; i++) myVals[i] = (mytypeval) tmpVals[i]; \
  rc = putch((char *)name, (char *)format, myVals); \
  free(myVals); \
  } else { rc = -1; } \
  free(tmpVals); \
  } else { rc = -1; } \
  if(isCopyFormat == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jformat,format); \
  if(isCopyName == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jname,name); \
  return(rc)

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Putch_putch__Ljava_lang_String_2Ljava_lang_String_2_3I
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jintArray jvals) {

   BODYCODE(Int,int,int);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Putch_putch__Ljava_lang_String_2Ljava_lang_String_2_3F
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jfloatArray jvals){

   BODYCODE(Float,float,float);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Putch_putch__Ljava_lang_String_2Ljava_lang_String_2_3D
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jdoubleArray jvals){

   BODYCODE(Double,double,double);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_Putch_putch__Ljava_lang_String_2Ljava_lang_String_2_3B
  (JNIEnv *env, jclass cls, jstring jname, jstring jformat, jbyteArray jvals){

  BODYCODE(Byte,byte,char);
}
