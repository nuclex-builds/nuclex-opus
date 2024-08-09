Custom LibOpus Build
--------------------

Opus is a free and open audio codec. A pretty amazing one, at that,
because it outperforms other lossy codec in every discipline - lower
latency, higher quality, better compression efficiency.

libopus is a C/C++ library that contains the reference encoder and
decoder for the format.


What's in this repository?
--------------------------

You should certainly use the `CMakeLists.txt` that ships with libopus
over this one.

This `CMakeLists.txt` is a custom build that *blindly* assumes a modern
desktop Linux build environment / Windows Visual Studio environment
and skips all configure checks.

It takes all C/C++ compiler options from an external CMake include file
(`../../build-system/cmake/cplusplus.cmake`). This ensures that my
libraries and applications are all compiled with exactly the same
compiler settings and that i.e. PGO can be used from end to end.
