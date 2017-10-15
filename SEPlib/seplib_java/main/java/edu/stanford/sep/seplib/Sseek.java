package edu.stanford.sep.seplib.corelibs.sep;
/** Call seplib function seek().
 *
 * @author  Stewart A. Levin
 */
public class Sseek {
/**
 * Seek within a seplib file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param offset amount to move
 * @param whence and from where (SEEK_SET=0, SEEK_CUR=1, SEEK_END=2)
 *
 * @return	The new offset of the underlying seek pointer 
 */
    public static native int sseek(String tag, int offset, int whence);
/**
 * Seek within a seplib file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param offset number of blocks to move
 * @param blocksize unit size for seek
 * @param whence and from where (SEEK_SET=0, SEEK_CUR=1, SEEK_END=2)
 *
 * @return	The new offset in multiples of blocksize of the underlying
 *              seek pointer 
 */
    public static native int sseek_block(String tag, int offset, int blocksize, int whence);
/**
 * Seek within a seplib file.
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 * @param offset number of blocks to move
 * @param blocksize unit size for seek
 * @param whence and from where (SEEK_SET=0, SEEK_CUR=1, SEEK_END=2)
 *
 * @return	The new offset in multiples of blocksize of the underlying
 *              seek pointer 
 */
    public static native double sseek_block_d(String tag, int offset, int blocksize, int whence);
/**
 * Report position within a seplib file.
 *
 * @param tag	    The tag (or name), e.g. "in", for the seplib input file.
 * @param blocksize Unit size for seek
 * @param blocks    Number of full blocks to current file position
 * @param remainder Number of bytes past last full block to current file position
 *
 * @return	ierr 0 on success, 1 if block_size &gt; MAX_INT, -1 on I/O failure
 */
    public static native int file_position(String tag, int blocksize, int[] blocks, int[] remainder);
/**
 * Report if seplib file is seekable
 *
 * @param tag	The tag (or name), e.g. "in", for the seplib input file.
 *
 * @return	0 if seekable, 1 if not seekable (yes, odd choices)
 */
    public static native int sseekable(String tag);
    
    static {
       System.loadLibrary("sep_JNI");
    }
}
