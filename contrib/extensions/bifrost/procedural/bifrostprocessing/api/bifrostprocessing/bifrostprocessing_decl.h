#ifndef BIFROSTPROCESSING_DECL_H
#define BIFROSTPROCESSING_DECL_H

#ifdef __APPLE__
    #define BIFROSTPROCESSING_APPLE
#elif __linux__
    #define BIFROSTPROCESSING_LINUX
#elif _WIN64
    #define BIFROSTPROCESSING_WINDOWS
#endif

#if defined(BIFROSTPROCESSING_API_NO_API)
    #define BIFROSTPROCESSING_API_DECL
#elif defined(BIFROSTPROCESSING_WINDOWS)
    #ifdef BIFROSTPROCESSING_API_IMPL
        #ifdef __GNUC__
            #define BIFROSTPROCESSING_API_DECL __attribute__((dllexport))
        #else
            #define BIFROSTPROCESSING_API_DECL __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define BIFROSTPROCESSING_API_DECL __attribute__((dllimport))
        #else
            #define BIFROSTPROCESSING_API_DECL __declspec(dllimport)
        #endif
    #endif
#else
    #if __GNUC__ >= 4
        #define BIFROSTPROCESSING_API_DECL __attribute__((visibility ("default")))
    #else
        #define BIFROSTPROCESSING_API_DECL
    #endif
#endif

#ifndef BIFROSTPROCESSING_INLINE
    #if defined(BIFROSTPROCESSING_WINDOWS)
        #define BIFROSTPROCESSING_INLINE __forceinline
    #else
        #define BIFROSTPROCESSING_INLINE inline
    #endif
#endif

#endif // BIFROSTPROCESSING_DECL_H
