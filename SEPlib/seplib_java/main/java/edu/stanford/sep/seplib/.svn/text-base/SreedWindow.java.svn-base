package edu.stanford.sep.seplib.corelibs.sep;
/** Call seplib function sreed_window().
 *
 * @author  Stewart A. Levin
 */
public class SreedWindow {
/**
 * Read window from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param n_dim The number of dimensions in input seplib dataset.
 * @param n_cube Vector[n_dim] of input axis lengths.
 * @param n_wind Vector[n_dim] of cube axis lengths after windowing.
 * @param f_wind Vector[n_dim] of first element for windowing (0 <= f < n)
 * @param j_wind Vector[n_dim] of subsample rate for windowing (1 <= j < n)
 * @param esize Number of bytes per element.
 * @param vals	Array to contain data to be read from input.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: For byte[] arrays, esize can be larger than 1
 */
    public static native int sreed_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, byte[] vals);

/**
 * Read window from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param n_dim The number of dimensions in input seplib dataset.
 * @param n_cube Vector[n_dim] of input axis lengths.
 * @param n_wind Vector[n_dim] of cube axis lengths after windowing.
 * @param f_wind Vector[n_dim] of first element for windowing (0 <= f < n)
 * @param j_wind Vector[n_dim] of subsample rate for windowing (1 <= j < n)
 * @param esize Number of bytes per element.
 * @param vals	Array to contain data to be read from input.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: esize must be 4
 */
    public static native int sreed_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, int[] vals);
    
/**
 * Read window from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param n_dim The number of dimensions in input seplib dataset.
 * @param n_cube Vector[n_dim] of input axis lengths.
 * @param n_wind Vector[n_dim] of cube axis lengths after windowing.
 * @param f_wind Vector[n_dim] of first element for windowing (0 <= f < n)
 * @param j_wind Vector[n_dim] of subsample rate for windowing (1 <= j < n)
 * @param esize Number of bytes per element.
 * @param vals	Array to contain data to be read from input.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: esize must be 4 for floats or 8 for complex pairs
 */
    public static native int sreed_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, float[] vals);

/**
 * Read window from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param n_dim The number of dimensions in input seplib dataset.
 * @param n_cube Vector[n_dim] of input axis lengths.
 * @param n_wind Vector[n_dim] of cube axis lengths after windowing.
 * @param f_wind Vector[n_dim] of first element for windowing (0 <= f < n)
 * @param j_wind Vector[n_dim] of subsample rate for windowing (1 <= j < n)
 * @param esize Number of bytes per element.
 * @param vals	Array to contain data to be read from input.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: esize must be 8 for doubles or 16 for double complex pairs
 */
    public static native int sreed_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, double[] vals);
    
    static {
       System.loadLibrary("sep_JNI");
    }
}
