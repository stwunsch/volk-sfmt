VOLK module with SIMD-oriented Fast Mersenne-Twister (SFMT) implementation  
==========================================================================

This module is a stand-alone extension (or so-called out-of-tree module) of VOLK [0]. The purpose is porting the SFMT code [1,2] to the VOLK framework, which ensures portability of the code. VOLK selects the correct implementation (generic or SSE2) based on the compatibility of the used hardware.

Runtime  evaluation  
-------------------

Runtime relative to standard Mersenne Twister to generate 62400000 pseudo random numbers. The code is compiled with level 3 optimization.

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
    Original impl (SSE2): 1.42074  
    VOLK generic: 1.15378  
    VOLK SSE2: 2.4919  
    Original array impl: 4.56878  
    VOLK array generic: 1.91401  
    VOLK array SSE2: 4.7696  

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
