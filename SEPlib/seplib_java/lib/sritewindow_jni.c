/* native method for SriteWindow class static methods srite_window() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_SriteWindow.h"
#include <seplib.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <errno.h>

#define BODYCODE(Typeval,typeval) \
   const char *tag; \
   jboolean isCopyTag, isCopyVals; \
   int n_dim, esize; \
   jsize start, len, nInBuf; \
   jint rc = -1; \
   jsize nbytesToWrite; \
   jint nread; \
   jint *n_cube; \
   jint *n_wind; \
   jint *f_wind; \
   jint *j_wind; \
   j ## typeval *vals; \
   size_t i; \
   jclass intarrayclass; \
   n_dim = (int) jn_dim; esize = (int) jesize; \
   if(n_dim < 1) return (rc); \
   switch ((int) sizeof(j ## typeval)) { \
   case 4: \
      if(esize != 4 && esize != 8) return (rc); \
      intarrayclass = (*env)->FindClass(env,"[I"); \
      if(intarrayclass != ((jclass) NULL)) { \
         if((*env)->IsInstanceOf(env,jvals,intarrayclass) == JNI_TRUE) { \
            if(esize != 4) return (rc); \
         } \
      } \
      break; \
   case 8: \
      if(esize != 8 && esize != 16) return (rc); \
      break; \
   default: \
      if(esize < 1) return (rc); \
      break; \
   } \
  /* sanity checks */ \
   if(n_dim > (*env)->GetArrayLength(env, jn_cube)) return(rc); \
   if(n_dim > (*env)->GetArrayLength(env, jn_wind)) return(rc); \
   if(n_dim > (*env)->GetArrayLength(env, jf_wind)) return(rc); \
   if(n_dim > (*env)->GetArrayLength(env, jj_wind)) return(rc); \
   n_cube = (jint *)calloc(n_dim,sizeof(int)); \
   n_wind = (jint *)calloc(n_dim,sizeof(int)); \
   f_wind = (jint *)calloc(n_dim,sizeof(int)); \
   j_wind = (jint *)calloc(n_dim,sizeof(int)); \
   if(n_cube == ((jint *) NULL) || n_wind == ((jint *) NULL) || \
      f_wind == ((jint *) NULL) || j_wind == ((jint *) NULL)) return (rc); \
   (*env)->GetIntArrayRegion(env,jn_cube,0,n_dim,n_cube); \
   (*env)->GetIntArrayRegion(env,jn_wind,0,n_dim,n_wind); \
   (*env)->GetIntArrayRegion(env,jf_wind,0,n_dim,f_wind); \
   (*env)->GetIntArrayRegion(env,jj_wind,0,n_dim,j_wind); \
   for(i=0, nbytesToWrite=esize; i<n_dim; ++i) nbytesToWrite *= n_wind[i]; \
   nInBuf = (*env)->GetArrayLength(env, jvals); \
   if((nInBuf*sizeof(j ## typeval)) < nbytesToWrite) return (rc); \
   vals = (*env)->Get ## Typeval ## ArrayElements(env,jvals,&isCopyVals); \
   if(vals == ((j ## typeval *) NULL)) return (rc); \
   tag = (*env)->GetStringUTFChars(env, jtag, &isCopyTag); \
   if(tag == ((const char *) NULL)) return (rc); \
   rc = (jint) srite_window(tag,&n_dim,n_cube,n_wind,f_wind,j_wind,esize,vals); \
   if(isCopyTag) (*env)->ReleaseStringUTFChars(env,jtag,tag); \
   (*env)->Release ## Typeval ## ArrayElements(env,jvals,vals,(rc == 0) ? 0 : JNI_ABORT); \
   return (rc)

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_SriteWindow_srite_1window__Ljava_lang_String_2I_3I_3I_3I_3II_3B
  (JNIEnv *env, jclass cls, jstring jtag, jint jn_dim, jintArray jn_cube, jintArray jn_wind, jintArray jf_wind, jintArray jj_wind, jint jesize, jbyteArray jvals) {
  BODYCODE(Byte,byte);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_SriteWindow_srite_1window__Ljava_lang_String_2I_3I_3I_3I_3II_3I
  (JNIEnv *env, jclass cls, jstring jtag, jint jn_dim, jintArray jn_cube, jintArray jn_wind, jintArray jf_wind, jintArray jj_wind, jint jesize, jintArray jvals) {
  BODYCODE(Int,int);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_SriteWindow_srite_1window__Ljava_lang_String_2I_3I_3I_3I_3II_3F
  (JNIEnv *env, jclass cls, jstring jtag, jint jn_dim, jintArray jn_cube, jintArray jn_wind, jintArray jf_wind, jintArray jj_wind, jint jesize, jfloatArray jvals) {
  BODYCODE(Float,float);
}

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_SriteWindow_srite_1window__Ljava_lang_String_2I_3I_3I_3I_3II_3D
  (JNIEnv *env, jclass cls, jstring jtag, jint jn_dim, jintArray jn_cube, jintArray jn_wind, jintArray jf_wind, jintArray jj_wind, jint jesize, jdoubleArray jvals) {
  BODYCODE(Double,double);
}
