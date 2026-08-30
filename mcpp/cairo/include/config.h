/* cairo's config.h — the probe answers for glibc/Linux with a GCC-compatible
 * toolchain. Each is traceable to a `conf.set` in upstream's meson.build.
 *
 * Hand-written rather than generated, for the reason the manifest gives: these
 * are decisions about the target, and a decision belongs where a reader can see
 * it. cairo has no code generator at all, so there is nothing else config.h
 * could have been.
 */
#ifndef MCPP_CAIRO_CONFIG_H
#define MCPP_CAIRO_CONFIG_H

#define HAVE_STDINT_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_MMAN_H 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_SYS_POLL_H 1
#define HAVE_SYS_UN_H 1
#define HAVE_UNISTD_H 1
#define HAVE_FCNTL_H 1
#define HAVE_SIGNAL_H 1
#define HAVE_SCHED_H 1
#define HAVE_TIME_H 1
#define HAVE_LIBGEN_H 1
#define HAVE_DLFCN_H 1

#define HAVE_MMAP 1
#define HAVE_MPROTECT 1
#define HAVE_GETPAGESIZE 1
#define HAVE_FLOCKFILE 1
#define HAVE_FUNLOCKFILE 1
#define HAVE_FORK 1
#define HAVE_WAITPID 1
#define HAVE_RAISE 1
#define HAVE_STRNDUP 1
#define HAVE_CLOCK_GETTIME 1
#define HAVE_SCHED_GETAFFINITY 1
#define HAVE_ALARM 1
#define HAVE_LINK 1

#define HAVE_FT_GET_X11_FONT_FORMAT 1
#define HAVE_FT_GET_VAR_DESIGN_COORDINATES 1
#define HAVE_FT_PALETTE_SET_FOREGROUND_COLOR 1

/* CAIRO_HAS_PTHREAD is what makes cairo's caches thread-safe; without it a
   toolkit calling cairo from two threads corrupts them silently. */
#define CAIRO_HAS_PTHREAD 1
#define CAIRO_HAS_REAL_PTHREAD 1

/* The 64-bit integer strategy. cairo probes for a native 64-bit type and falls
   back to a struct-based emulation; on x86-64 the native one is right and the
   fallback would be slower for no benefit. */
#define HAVE_UINT64_T 1

/* HAVE_UINT128_T is deliberately ABSENT. cairo's probe looks for the type
   spelled `uint128_t`, and no C library provides that name — gcc and clang
   have `__uint128_t`, which is a different identifier. Claiming it makes
   cairo-wideint-type-private.h:140 typedef from a type that does not exist.
   Without it cairo uses its own struct-based 128-bit emulation, which is what
   every distribution's build does. */

#define SIZEOF_VOID_P 8
#define SIZEOF_INT 4
#define SIZEOF_LONG 8
#define SIZEOF_LONG_LONG 8
#define SIZEOF_SIZE_T 8

/* WORDS_BIGENDIAN and FLOAT_WORDS_BIGENDIAN are DELIBERATELY ABSENT, not
 * defined to 0.
 *
 * cairo tests them with `#ifdef`, not `#if` — cairoint.h:196 and
 * cairo-fixed-private.h:124. `#define WORDS_BIGENDIAN 0` therefore says
 * BIG-ENDIAN, and on x86-64 that is wrong in the worst possible way: nothing
 * fails to compile, nothing reports an error, `cairo_paint` still works, and
 * every PATH silently gets garbage coordinates. Measured:
 *
 *     cairo_rectangle(cr, 4, 4, 16, 16);
 *     cairo_path_extents(...)  ->  -8.03e+06 -8.03e+06 4.37e+06 4.37e+06
 *     cairo_in_fill(cr, 40, 40) -> 1        (the point is outside)
 *
 * so `cairo_fill` and `cairo_stroke` change zero pixels with
 * `cairo_status` reporting success. An hour of looking for a missing source
 * file, and the cause was a `0`.
 *
 * On a big-endian target the fix is to define them (to anything), not to 1.
 */

#endif
