package edu.stanford.sep.seplib.corelibs.sep;
/** Call seplib function sreed().
 *
 * @author  Stewart A. Levin
 */
public class Sreed {
/**
 * Read bytes from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array to contain data to be read from input.
 * @param nvals The number of bytes to read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static native int sreed(String tag, byte[] buf, int nvals);
/**
 * Read bytes from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array for data to be read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static int sreed(String tag, byte[] buf) {
       return sreed(tag,buf,(int) buf.length);
    }
/**
 * Read ints from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array to contain data to be read from input.
 * @param nvals The number of data values to read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static native int sreed(String tag, int[] buf, int nvals);
/**
 * Read ints from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array for data to be read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static int sreed(String tag, int[] buf) {
       return sreed(tag, buf, (int) buf.length);
    }
/**
 * Read floats from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array to contain data to be read from input.
 * @param nvals The number of data values to read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static native int sreed(String tag, float[] buf, int nvals);
/**
 * Read floats from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array for data to be read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static int sreed(String tag, float[] buf) {
       return sreed(tag,buf,(int) buf.length);
    }
/**
 * Read doubles from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array to contain data to be read from input.
 * @param nvals The number of data values to read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static native int sreed(String tag, double[] buf, int nvals);
/**
 * Read doubles from seplib input file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param buf	Array for data to be read from input.
 *
 * @return	The number of data values actually read or -1 on error.
 */
    public static int sreed(String tag, double[] buf) {
       return sreed(tag,buf,(int) buf.length);
    }
    
    static {
       System.loadLibrary("sep_JNI");
    }
}
