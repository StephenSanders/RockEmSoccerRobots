/*
 * <engine/bits.h>
 * Stephen Sanders & Jake Singer
 *
 * Bitwise operations.
 */

#ifndef __H_ENGINE_BITS__
#define __H_ENGINE_BITS__
#ifdef __cplusplus
extern "C" {
#endif

#include <engine/define.h>
#include <engine/types.h>

#ifdef _MSC_VER
#include <intrin.h>
#endif

/*
 * Count Leading Zeros
 *
 * Return number of leading zeros (undefined if value is 0).
 * Usefull for finding index of first set bit.
 */
static uint32_t FORCEINLINE FASTCALL
bit_clz(uint32_t value)
{
#ifdef _MSC_VER
	uint32_t result = 0;
	_BitScanForward((unsigned long *)&result, value);
	return result;
#else
	return __builtin_clz(value);
#endif
}

/*
 * Count Leading Zeros in Range
 *
 * Requires 4-byte aligned little-endian values (return end + 1 if not found).
 * Usefull for finding index of first set bit within a range.
 */
extern uint32_t FASTCALL
bit_clz_range(void const *root, uint32_t start, uint32_t end);

/*
 * Count Leading Bits in Range
 *
 * Requires 4-byte aligned little-endian values (return end + 1 if not found).
 * Usefull for finding index of first zero bit within a range.
 */
extern uint32_t FASTCALL
bit_clb_range(void const *root, uint32_t start, uint32_t end);

/*
 * Count Trailing Zeros
 *
 * Return number of trailing zeros (undefined if value is 0).
 */
static uint32_t FORCEINLINE FASTCALL
bit_ctz(uint32_t value)
{
#ifdef _MSC_VER
	uint32_t result = 0;
	_BitScanReverse((unsigned long *)&result, value);
	return result;
#else
	return __builtin_ctz(value);
#endif
}

/*
 * Set Bit
 *
 * 4-byte aligned little-endian values only.
 */
static void FORCEINLINE FASTCALL
bit_set(void *root, uint32_t index)
{
	uint32_t *word = (uint32_t *)root + index / 32;
	*word |= 1 << (index & 31);
	return;
}

/*
 * Set Bits in Range
 *
 * 4-byte aligned little-endian values only.
 */
extern void FASTCALL
bit_set_range(void *root, uint32_t start, uint32_t end);

/*
 * Clear Bit
 *
 * 4-byte aligned little-endian values only.
 */
static void FORCEINLINE FASTCALL
bit_clear(void *root, uint32_t index)
{
	uint32_t *word = (uint32_t *)root + index / 32;
	*word &= ~(1 << (index & 31));
	return;
}

/*
 * Clear Bits in Range
 *
 * 4-byte aligned little-endian values only.
 */
extern void FASTCALL
bit_clear_range(void *root, uint32_t start, uint32_t end);

#ifdef __cplusplus
}
#endif
#endif /* __H_ENGINE_BITS__ */
