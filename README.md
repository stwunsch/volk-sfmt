VOLK module with SIMD-oriented Fast Mersenne-Twister (SFMT) implementation  
==========================================================================

This module is a stand-alone extension (or so-called out-of-tree module) of VOLK [0]. The purpose is porting the SFMT code [1,2] to the VOLK framework, which ensures portability of the code. VOLK selects the correct implementation (generic or SSE2) based on the compatibility of the used hardware.

Runtime  evaluation  
-------------------

Runtime relative to standard Mersenne Twister to generate 62400000 pseudo random numbers:

    Standard Mersenne-Twister: 1  
    Boost.Random Mersenne-Twister: 1.02158  
    Original SFMT impl SSE2: 1.00007  
    Original SFMT array impl SSE2: 3.20311  
    VOLK SFMT generic: 0.903747  
    VOLK SFMT array generic: 1.3417  
    VOLK SFMT SSE2: 1.73701  
    VOLK SFMT array SSE2: 3.33381  

Note that for the array version of the SFMT implementations the function call to return a single pseudo random number is not included! This makes the comparison with the standard SFMT versions not fair, if single values are desired.

Installation  
------------

    git clone https://github.com/stwunsch/volk-sfmt  
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
