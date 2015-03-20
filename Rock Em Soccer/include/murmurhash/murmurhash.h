/*
 * <murmurhash/murmurhash.h>
 * Stephen Sanders & Jake Singer
 *
 * MurmurHash3 was written by Austin Appleby, and is placed in the public
 * domain.
 * The original code can be found at <https://code.google.com/p/smhasher/>.
 */

#ifndef __H_MURMURHASH__
#define __H_MURMURHASH__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* use MurmurHash3 x86 to generate a 32-bit output */
extern void murmurhash(const void *key, int len, uint32_t seed, void *out);

#ifdef __cplusplus
}
#endif
#endif /* __H_MURMURHASH__ */
