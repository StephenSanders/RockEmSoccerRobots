/*
 * <engine/define.h>
 * Stephen Sanders & Jake Singer
 *
 * Reference:
 *  - http://predef.sourceforge.net/
 */

#ifndef __H_ENGINE_DEFINE__
#define __H_ENGINE_DEFINE__
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Architecture
 */

#define X86                             defined __i386__ || defined _M_IX86

/*
 * Compiler
 */

#define MSVC                            defined _MSC_VER

/*
 * Platform
 */

#define WINDOWS                         defined _WIN32

/*
 * Support
 */

/* math stuff */
#ifndef PI
#define PI                              (3.141592653589793f)
#endif

#ifndef HALF_PI
#define HALF_PI                         (1.570796326794897f)
#endif

/* forceinline */
#if MSVC
#define FORCEINLINE                     __forceinline
#else
#define FORCEINLINE                     __attribute__((__always_inline__))
#endif

/* trim the windows header files */
#if WINDOWS && !defined WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

/* strict typing */
#if WINDOWS && !defined STRICT
#define STRICT
#endif

/* setup the c com interface */
#if WINDOWS && !defined COBJMACROS
#define COBJMACROS
#endif

/* redirect main */
#if WINDOWS && !defined __ENGINE_MAIN
#define __ENGINE_MAIN
#define main __engine_main
extern int __engine_main(int argc, char *argv[]);
#endif

/* calling conventions */
#if defined(_M_IX86)
#define CDECL __cdecl
#define STDCALL __stdcall
#define FASTCALL __fastcall
#else
#define CDECL
#define STDCALL
#define FASTCALL
#endif

#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_DEFINE__ */
