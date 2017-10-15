package edu.stanford.sep.seplib.corelibs.sep;
/** Call seplib function srite().
 *
 * @author  Stewart A. Levin
 */
public class Srite {
/**
 * Write bytes to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array containing data to be written to output.
 * @param nvals The number of bytes to write to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static native int srite(String tag, byte[] buf, int nvals);
/**
 * Write bytes to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array of data to be written to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static int srite(String tag, byte[] buf) {
       return srite(tag,buf,(int) buf.length);
    }
/**
 * Write ints to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array containing data to be written to output.
 * @param nvals The number of data values to write to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static native int srite(String tag, int[] buf, int nvals);
/**
 * Write ints to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array of data to be written to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static int srite(String tag, int[] buf) {
       return srite(tag, buf, (int) buf.length);
    }
/**
 * Write floats to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array containing data to be written to output.
 * @param nvals The number of data values to write to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static native int srite(String tag, float[] buf, int nvals);
/**
 * Write floats to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array of data to be written to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static int srite(String tag, float[] buf) {
       return srite(tag,buf,(int) buf.length);
    }
/**
 * Write doubles to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array containing data to be written to output.
 * @param nvals The number of data values to write to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static native int srite(String tag, double[] buf, int nvals);
/**
 * Write doubles to seplib output file.
 *
 * @param tag	The tag (or name), e.g. "out", for the seplib output file.
 * @param buf	Array of data to be written to output.
 *
 * @return	The number of data values actually written or -1 on error.
 */
    public static int srite(String tag, double[] buf) {
       return srite(tag,buf,(int) buf.length);
    }
    
    static {
       System.loadLibrary("sep_JNI");
    }
}
