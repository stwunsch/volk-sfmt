VOLK module with SIMD-oriented Fast Mersenne-Twister (SFMT and dSFMT) implementations
=====================================================================================

This module is a stand-alone extension (or so-called out-of-tree module) of VOLK [0]. The purpose is porting the SFMT and dSFMT code [1,2,3] to the VOLK framework, which ensures portability of the code. VOLK selects the correct implementation (generic or SSE2) based on the compatibility of the used hardware. The difference between SFMT and dSFMT is the return value type. SFMT returns uint32_t values and dSFMT double values. The main advantage of the dSFMT algorithm is the direct output of a double value, no conversion is needed. This results in a huge performance boost.

Runtime  evaluation
-------------------

Runtime of the SFMT algorithm (not the dSFMT!) relative to standard Mersenne Twister to generate 62400000 pseudo random numbers. Output is an uint32_t value. The code is compiled with level 3 optimization.

    Standard Mersenne-Twister: 1  
    Boost.Random Mersenne-Twister: 1.02159  
    Original impl (SSE2): 0.997666  
    VOLK generic: 0.996441  
    VOLK SSE2: 1.74102  
    Original array impl (SSE2): 3.2169  
    VOLK array generic: 1.34155  
    VOLK array SSE2: 3.35664  

Note that for the array version of the SFMT implementations the function call to return a single pseudo random number is not included! This makes the comparison with the standard SFMT versions not fair, if single values are desired.

Often, only a level 2 optimization is used. Then following relative runtimes are given.

    Standard Mersenne-Twister: 1  
    Boost.Random Mersenne-Twister: 1.1719  
    Original SFMT impl (SSE2): 1.42074  
    VOLK SFMT generic: 1.15378  
    VOLK SFMT SSE2: 2.4919  
    Original SFMT array impl: 4.56878  
    VOLK SFMT array generic: 1.91401  
    VOLK SFMT array SSE2: 4.7696  

This comparison is only half-fair because the VOLK functions are always compiled with level 3 optimization (and the code used above does only link to the library). Nevertheless, the compile time of the main program is reduced due to the used O2 compiler flag, but the crucial parts regarding the runtime are still on high performance since they are linked to VOLK.

The dSFMT algorithm relative to the standard Mersenne-Twister implementation with conversion to double performs as shown below. The output is a double in the range [0,1). As well, the SFMT algorithm with a conversion to a double value in [0,1) is compared. The code is compiled with level 3 optimization and 114600 random numbers are created during the test.

    Standard Mersenne-Twister with conversion: 1  
    Boost.Random Mersenne-Twister: 0.537196  
    Original dSFMT impl (SSE2): 1.95065  
    VOLK dSFMT generic: 1.70295  
    VOLK SFMT with conversion: 0.85438  
    VOLK dSFMT SSE2: 2.10364  
    VOLK SFMT SSE2 with conversion: 1.05182  
    
And again, the same with the often used level 2 optimization. Note again, that VOLK is still compiled with the O3 compiler flag.

    Standard Mersenne-Twister with conversion: 1  
    Boost.Random Mersenne-Twister: 0.870902  
    Original dSFMT impl (SSE2): 2.27273  
    VOLK dSFMT generic: 1.83585  
    VOLK SFMT with conversion: 0.948661  
    VOLK dSFMT SSE2: 2.51479  
    VOLK SFMT SSE2 with conversion: 1.15489  
    
All in all, if a double in [0,1) is needed, the dSFMT is about 2.1x to 2.5x faster than a conventional Mersenne Twister. If a uint32_t type is desired, the SFMT algorithm is about 1.8x to 2.5x faster than the conventional algorithm. The runtime of both SIMD algorithms can be highly increased if arrays of pseudo random numbers are generated and not single values are returned.

Installation  
------------

    git clone https://github.com/stwunsch/volk-sfmt  
    cd volk-sfmt  
    mkdir build  
    cd build  
    ctest ..  
    make  
    ctest -V  
    make install  

References  
---------

[0] http://www.libvolk.org  
[1] http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/  
[2] http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/SFMT-src-1.4.1.tar.gz  
[3] http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/dSFMT-src-2.2.3.tar.gz
