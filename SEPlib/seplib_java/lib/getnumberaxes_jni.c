/* native method for GetNumberAxes class static methods sep_get_number_data_axes() */
#include "sepConfig.h"
#include "edu_stanford_sep_seplib_corelibs_sep_GetNumberAxes.h"
#include <sep3d.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <errno.h>

/*ARGSUSED*/
JNIEXPORT jint JNICALL Java_edu_stanford_sep_seplib_corelibs_sep_GetNumberAxes_sep_1get_1number_1data_1axes
  (JNIEnv *env, jclass cls, jstring jtag, jintArray jn_axis) {
   jboolean isCopy;
   const char *tag;
   int n_axis;
   jint rc = -1;

   if((*env)->GetArrayLength(env, jn_axis) < 1) return rc;

   tag = (*env)->GetStringUTFChars(env,jtag,&isCopy);

   if(tag == ((const char *) NULL)) return rc;
   
   rc = (jint) sep_get_number_data_axes(tag, &n_axis);

   if(isCopy == JNI_TRUE) (*env)->ReleaseStringUTFChars(env,jtag,tag);

   if(rc == 0) (*env)->SetIntArrayRegion(env,jn_axis,0,1,&n_axis);

   return (rc);
}
