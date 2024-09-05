#define PACKAGE_VERSION "1.4"

#define USE_ALLOCA            1

/* #undef FIXED_POINT */

#define OPUS_X86_MAY_HAVE_SSE
#define OPUS_X86_MAY_HAVE_SSE2
#define OPUS_X86_MAY_HAVE_SSE4_1

// This build always assumes SSE and SSE2 are present.
// All amd64 CPUs have SSE2, so unless you try to build a 32-bit library
// for a really ancient (as in, 2002's Pentium III or older) platform,
// it's safe to assume any desktop CPU has SSE and SSE2.
#define OPUS_X86_PRESUME_SSE 1
#define OPUS_X86_PRESUME_SSE2 1

// If the entire library is compiled targeting a CPU achitecture with SSE 4.1,
// then SSE 4.1 code paths can be unconditionally enabled.
#if defined(_MSC_VER)
  #if defined(__AVX__)
    #define OPUS_X86_PRESUME_SSE4_1 1
  #endif
#elif defined(__clang__) || (defined(__GNUC__) || defined(__GNUG__))
  #if defined(__SSE4A__) || defined(__SSE4_1__)
    #define OPUS_X86_PRESUME_SSE4_1 1
  #endif
#endif

// RTCD = run-time CPU detection.
// If presence of SSE 4.1 isn't required, Opus checks for it at runtime
// and uses it conditionally based on hardware capabilities.
#if !defined(OPUS_X86_PRESUME_SSE4_1)
  #define OPUS_HAVE_RTCD 1
#endif
