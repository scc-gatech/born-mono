package edu.stanford.sep.seplib.corelibs.sep;
/** Call seplib function srite_window().
 *
 * @author  Stewart A. Levin
 */
public class SriteWindow {
/**
 * Write window to seplib file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib file.
 * @param n_dim The number of dimensions in seplib dataset.
 * @param n_cube Vector[n_dim] of seplib axis lengths.
 * @param n_wind Vector[n_dim] of window axis lengths.
 * @param f_wind Vector[n_dim] of first element for window (0 &lt;= f &lt; n)
 * @param j_wind Vector[n_dim] of subsample rate for window (1 &lt;= j &lt; n)
 * @param esize Number of bytes per element.
 * @param vals	Array containing data to be written.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: For byte[] arrays, esize can be larger than 1
 */
    public static native int srite_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, byte[] vals);

/**
 * Write window to seplib file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib file.
 * @param n_dim The number of dimensions in seplib dataset.
 * @param n_cube Vector[n_dim] of seplib axis lengths.
 * @param n_wind Vector[n_dim] of window axis lengths.
 * @param f_wind Vector[n_dim] of first element for window (0 &lt;= f &lt; n)
 * @param j_wind Vector[n_dim] of subsample rate for window (1 &lt;= j &lt; n)
 * @param esize Number of bytes per element.
 * @param vals	Array containing data to be written.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: esize must be 4
 */
    public static native int srite_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, int[] vals);
    
/**
 * Write window to seplib file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib file.
 * @param n_dim The number of dimensions in seplib dataset.
 * @param n_cube Vector[n_dim] of seplib axis lengths.
 * @param n_wind Vector[n_dim] of window axis lengths.
 * @param f_wind Vector[n_dim] of first element for window (0 &lt;= f &lt; n)
 * @param j_wind Vector[n_dim] of subsample rate for window (1 &lt;= j &lt; n)
 * @param esize Number of bytes per element.
 * @param vals	Array containing data to be written.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: esize must be 4 for floats or 8 for complex pairs
 */
    public static native int srite_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, float[] vals);

/**
 * Write window to seplib file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib file.
 * @param n_dim The number of dimensions in seplib dataset.
 * @param n_cube Vector[n_dim] of seplib axis lengths.
 * @param n_wind Vector[n_dim] of window axis lengths.
 * @param f_wind Vector[n_dim] of first element for window (0 &lt;= f &lt; n)
 * @param j_wind Vector[n_dim] of subsample rate for window (1 &lt;= j &lt; n)
 * @param esize Number of bytes per element.
 * @param vals	Array containing data to be written.
 *
 * @return	0 if success, -2 for bad window params, -1 otherwise
 *
 * Note: esize must be 8 for doubles or 16 for double complex pairs
 */
    public static native int srite_window(String tag, int n_dim, int[] n_cube, int[] n_wind, int[] f_wind, int[] j_wind, int esize, double[] vals);
    
    static {
       System.loadLibrary("sep_JNI");
    }
}
