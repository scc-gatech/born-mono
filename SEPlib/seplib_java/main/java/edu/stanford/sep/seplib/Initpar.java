package edu.stanford.sep.seplib.corelibs.sep;
/**
 *  Call seplib initpar() routine
 *
 * @author  Stewart A. Levin
 */
public class Initpar {
    /**
     * @param argc  Number of command line arguments passed in argv.
     * @param argv  Array of command line strings.
     *
     * Note: This is declared private because Java main() methods
     *       aren't provided the tradition argv[0] program name.
     */
    private static native void initpar(int argc, String[] argv);

    // Java main will just pass its args list to here
    /**
     * Wrapper to divine the caller's program filename and prepend it
     * to the argv list before calling initpar().
     *
     * @param argv  Array of command line strings from the Java main()
     *              argument array.
     */
    public static void initpar(String[] argv) {

        String[] newArgv = new String[argv.length+1];
        System.arraycopy(argv,0,newArgv,1,argv.length);
        newArgv[0] = Thread.currentThread().getStackTrace()[1].getFileName();
        // while length is redundant, need different signature than
        // our public method.
        initpar(newArgv.length, newArgv); 
    }

    static {
       System.loadLibrary("sep_JNI");
    }
}
