/*
 * murmurhash.c
 * Stephen Sanders & Jake Singer
 *
 * MurmurHash3 was written by Austin Appleby, and is placed in the public
 * domain.
 * The original code can be found at <https://code.google.com/p/smhasher/>.
 */

#include "murmurhash.h"

/* defines */
#include <stdlib.h>
#if defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#define ROTL32(x,y)	_rotl(x,y)
#endif /* _MSC_VER */

/* finalization mix - force all bits to avalanche */
FORCE_INLINE uint32_t fmix(uint32_t h)
{
	h ^= h >> 16;
	h *= 0x85EBCA6B;
	h ^= h >> 13;
	h *= 0xC2B2AE35;
	h ^= h >> 16;
	return h;
}

/* hash implimentation */
void murmurhash(const void *key, int len, uint32_t seed, void *out)
{
	int i;
	uint32_t k1;
	const uint32_t *blocks;
	const uint8_t *tail;
	const uint8_t *data = (const uint8_t *)key;
	const int nblocks = len / 4;
	uint32_t h1 = seed;
	const uint32_t c1 = 0xCC9E2D51;
	const uint32_t c2 = 0x1B873593;

	/* body */
	blocks = (const uint32_t *)(data + nblocks * 4);
	for(i = -nblocks; i; i++) {
		k1 = blocks[i];
		k1 *= c1;
		k1 = ROTL32(k1, 15);
		k1 *= c2;
		h1 ^= k1;
		h1 = ROTL32(h1, 13); 
		h1 = h1 * 5 + 0xE6546B64;
	}

	/* tail */
	tail = (const uint8_t *)(data + nblocks * 4);
	k1 = 0;
	switch(len & 3) {
		case 3: k1 ^= tail[2] << 16;
		case 2: k1 ^= tail[1] << 8;
		case 1: k1 ^= tail[0];
		        k1 *= c1;
		        k1 = ROTL32(k1, 15);
		        k1 *= c2;
		        h1 ^= k1;
	};

	/* finalization */
	h1 ^= len;
	h1 = fmix(h1);
	*(uint32_t *)out = h1;
	return;
}
