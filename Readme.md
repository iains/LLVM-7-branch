# LLVM 7.1 Darwin WIP

** WARNING **  *as things progress, rebasing WILL occur - so caveat!*

* Rebased 2019-04-18
  - added in the base parts of powerpc darwin support to clang
  - still incomplete (missing the ABI changes to the backend)

## Summary:

 * Mostly this is an attempt to complete the powerpc*-apple-darwin LLVM/clang
   port, but:
 
   * Actually the "llas" component (below) is intended for use also in GCC-
     based tool-chains to provide a uniform assembler (and support for
     .cfi_xxxx insns).
   * We use this to build a libLTO.dylib for ld64.
   * We also build a version of "tapi" to provide libtapi.dylib for ld64.
   * We build dsymutil, and other llvm-xxxx tools to be included in our
     toolchains.

 * "llas" is a cctools-as lookalike that calls the LLVM backend directly,
   rather than via clang, and tries to respond in a useful way to thhe same
   set of command line options handled by cctools "as"..

## General notes.

 1 *as things progress, rebasing WILL occur - so caveat!* 
   Yes this was already mentioned ... but ... 

 2 Building 'tapi' assumes that you have checked out the 'tapi' component as
   a sibling directory to this project.
   
   * At some point it will be hopefully possible to back-port the upstream
    TextAPI impl. to avoid having a separate repo. but not yet.

 3 As things stand, the LLVM backend generates wrong compact unwind for
   GCC's hot/cold partitioned output
   * there's an option added here to disable the automatic generation of
     compact unwind from clang / clang -cc1as.  This will be needed before
     use of .cfi_xxxx can be enabled for GCC (currently stuff disabled).

   * "llas" disables compact unwind by default at present.
   * A proper fix for the output is the ideal solution, if someone has the
     time...

 4 It's probably going to be better in the long-term to base libc++ on top
   of libsupc++ (the GCC libstdc++ ABI lib) for Darwin8-10 (MacOS X 10.4-6).
   
   * At the moment it's assumed that any ["old Darwin system"] user of libc++
     will be packaged with libraries in a stand-alone manner, and will include
     libc++abi too.

## PowerPC (32 and 64)

### LLVM:

 * The MC layer is in good shape on PowerPC (32 and 64 bit), good enough to
   build a working assembler (i.e. llas) capable of building GCC.

 * It includes the changes to Data Layout to match those of the revised clang.

 * **IT DOES NOT** (yet) include all the necessary patches to make the PPC
   backend ABI-compatible with the Apple gcc-4.x compilers!!!! So it *will*
   produce broken code (even if it appears to work in a self-consistent manner).

### Clang:

 * This includes a forward-port of my patches for ABI fixes to match the
   aggregate layout rules.  It doesn't include handling of the fallout from
   that in the LLVM support unit tests (some will fail if you try to self-host
   on powerpc-darwin).

 * This includes a forward port of the base ABIInfo implementation for both
   32 and 64b powerpc darwin.  Tests added for parts that have been compared
   with the output of gcc-4-2-1 (5666.3).  More comparisons are needed with
   4.0.1 etc.

## X86_64

 * Should be OK (there are no intentional changes there).

## i686

 * i686 is known to be broken with some of the section switches output by
 GCC for 10.4 code with picsymbol stubs.  So watch out if you want to build
 for "ld_classic" (picsymbol stubs are not needed when using ld64, which is
 available for 10.4).
