/*
 * bits.c
 * Stephen Sanders & Jake Singer
 */

#include <engine/bits.h>

uint32_t FASTCALL
bit_clz_range(void const *root, uint32_t start, uint32_t end)
{
	uint32_t *word = (uint32_t *)root + start / 32;
	uint32_t bits_to_search = end - start + 1;
	uint32_t bits_left_in_word = (32 - (start & 31));

	/* start and end within first word? */
	if (bits_to_search < bits_left_in_word) {
		uint32_t value = *word;
		uint32_t mask = ((1 << bits_to_search) - 1);
		uint32_t overlap;

		value >>= start & 31;
		overlap = value & mask;
		if (overlap) {
			return bit_clz(overlap) + start;
		} else {
			/* did not find bit */
			return end + 1;
		}
	/* multiple words */
	} else {
		uint32_t value = *word;

		value >>= start & 31;
		if (value)  {
			/* found bit */
			return bit_clz(value) + start;
		}
		word++;
		bits_to_search -= bits_left_in_word;
		start += bits_left_in_word;
	}

	/* search word at a time */
	while (bits_to_search >= 32) {
		uint32_t value = *word++;

		if (value) {
			return bit_clz(value) + start;
		}
		bits_to_search -= 32;
		start += 32;
	}

	/* extra bits */
	if (bits_to_search > 0) {
		uint32_t value = *word;

		value &= ((1 << bits_to_search) - 1);
		if (value) {
			return bit_clz(value) + start;
		}
	}
	
	/* did not find bit */
	return end + 1;
}

uint32_t FASTCALL
bit_clb_range(void const *root, uint32_t start, uint32_t end)
{
	uint32_t *word = (uint32_t *)root + start / 32;
	uint32_t bits_to_search = end - start + 1;
	uint32_t bits_left_in_word = (32 - (start & 31));

	/* start and end within first word? */
	if (bits_to_search < bits_left_in_word) {
		uint32_t value = *word;
		uint32_t mask = ((1 << bits_to_search) - 1);
		uint32_t overlap;

		value >>= start & 31;
		overlap = (value & mask) ^ mask;
		if (overlap) {
			return bit_clz(overlap) + start;
		} else {
			/* did not find bit */
			return end + 1;
		}
	/* multiple words */
	} else {
		uint32_t value = *word;
		uint32_t mask = 0xFFFFFFFF >> (start & 31);

		value >>= start & 31;
		value = (value & mask) ^ mask;
		if (value)  {
			/* found bit */
			return bit_clz(value) + start;
		}
		word++;
		bits_to_search -= bits_left_in_word;
		start += bits_left_in_word;
	}

	/* search word at a time */
	while (bits_to_search >= 32) {
		uint32_t value = ~(*word++);

		if (value) {
			return bit_clz(value) + start;
		}
		bits_to_search -= 32;
		start += 32;
	}

	/* extra bits */
	if (bits_to_search > 0) {
		uint32_t value = *word;
		uint32_t mask = ((1 << bits_to_search)-1);

		value &= (value & mask) ^ mask;
		if (value) {
			return bit_clz(value) + start;
		}
	}
	
	/* did not find bit */
	return end + 1;
}

void FASTCALL
bit_set_range(void *root, uint32_t start, uint32_t end)
{
	uint32_t *word = (uint32_t *)root + start / 32;
	uint32_t bits_to_set = end - start + 1;
	uint32_t bits_left_in_word = (32 - (start & 31));

	/* start and end within first word? */
	if (bits_to_set < bits_left_in_word) {
		uint32_t value = *word;
		value |= ((1 << bits_to_set) - 1) << (start & 31);
		*word = value;
		return;
	/* multiple words */
	} else {
		uint32_t value = *word;
		value |= 0xFFFFFFFF << (start & 31);
		*word++ = value;
		bits_to_set -= bits_left_in_word;
	}

	/* set word at a time */
	while (bits_to_set >= 32) {
		*word++ = 0xFFFFFFFF;
		bits_to_set -= 32;
	}

	/* extra bits */
	if (bits_to_set > 0) {
		uint32_t value = *word;
		value |= ((1 << bits_to_set) - 1);
		*word = value;
	}
	return;
}

void FASTCALL
bit_clear_range(void *root, uint32_t start, uint32_t end)
{
	uint32_t *word = (uint32_t *)root + start / 32;
	uint32_t bits_to_clear = end - start + 1;
	uint32_t bits_left_in_word = (32 - (start & 31));

	/* start and end within first word? */
	if (bits_to_clear < bits_left_in_word) {
		uint32_t value = *word;
		value &= ~((1 << bits_to_clear) - 1) << (start & 31);
		*word = value;
		return;
	/* multiple words */
	} else {
		uint32_t value = *word;
		value &= ~(0xFFFFFFFF << (start & 31));
		*word++ = value;
		bits_to_clear -= bits_left_in_word;
	}

	/* set word at a time */
	while (bits_to_clear >= 32) {
		*word++ = 0;
		bits_to_clear -= 32;
	}

	/* extra bits */
	if (bits_to_clear > 0) {
		uint32_t value = *word;
		value |= ~((1 << bits_to_clear) - 1);
		*word = value;
	}
	return;
}
