package edu.stanford.sep.seplib.corelibs.sep;
/**
 *  Call seplib routines h2c() and c2h().
 *
 * @author  Stewart A. Levin
 *
 * Direct transcription of C library code.  Not worth native method calls.
 */
public class HelixCart {
    /**
     * Convert helix coordinate to Cartesian n-D coordinates.
     *
     * @param hindex input helix index
     * @param n list of Cartesian grid dimensions
     * @param ndim number of Cartesian grid dimensions
     * @param cindex output Cartesian grid indices
     *
     * Note: All coordinates are zero-based.
     */
    public static void h2c(long hindex, int[] n, int ndim, int[] cindex){
       long block = 1;
       long trunc;
       int i1;
       
       for(i1=0; i1<ndim; i1++) {
          trunc = hindex/block;
          cindex[i1]=(int) (trunc%(n[i1]));
          block *= n[i1];
       }
    }

    /**
     * Convert Cartesian n-D coordinates to helix coordinate.
     *
     * @param hindex output helix index
     * @param n list of Cartesian grid dimensions
     * @param ndim number of Cartesian grid dimensions
     * @param cindex input Cartesian grid indices
     *
     * Note: All coordinates are zero-based.
     */
    public static void c2h(long[] hindex, int[] n, int ndim, int[] cindex) {
       long block = 1; 
       int i1;

       hindex[0] = 0;
       for(i1=0; i1<ndim; i1++) {
          hindex[0] += cindex[i1]*block;
          block *= n[i1];
       }
    }

//    static {
//       System.loadLibrary("sep_JNI");
//    }
}

