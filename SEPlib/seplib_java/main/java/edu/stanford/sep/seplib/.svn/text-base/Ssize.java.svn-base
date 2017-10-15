package edu.stanford.sep.seplib.corelibs.sep;
/** Call seplib function ssize().
 *
 * @author  Stewart A. Levin
 */
public class Ssize {
/**
 * Return size of a seplib file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 *
 * @return	The size of the seplib file or -1 on error.
 */
    public static native int ssize(String tag);
/**
 * Return number of blocks in a seplib file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param blocksize  The number of bytes in a block.
 *
 * @return	The number of full blocks the seplib file or -1 on error.
 */
    public static native int ssize_block(String tag, int blocksize);
    
    static {
       System.loadLibrary("sep_JNI");
    }
}
