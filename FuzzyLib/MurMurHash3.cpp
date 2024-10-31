#include "MurMurHash3.h"



void  Fuzzy_lib::Murmur_hash::hash3_x64_128(Data_types::bytes_view key, uint64_t seed, std::array<uint64_t, 2>& result)
{
	uint32_t length = key.size();
	const uint32_t nblocks = length >> 4; // Process as 128-bit blocks.

	uint64_t h1 = seed;
	uint64_t h2 = seed;

	uint64_t c1 = 0x87c37b91114253d5L;
	uint64_t c2 = 0x4cf5ad432745937fL;

	//----------
	// body

	for (uint32_t i = 0; i < nblocks; i++)
	{
		uint64_t k1 = getblock(key, i * 2 + 0);
		uint64_t k2 = getblock(key, i * 2 + 1);

		k1 *= c1; k1 = _rotl64(k1, 31); k1 *= c2; h1 ^= k1;

		h1 = _rotl64(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729;

		k2 *= c2; k2 = _rotl64(k2, 33); k2 *= c1; h2 ^= k2;

		h2 = _rotl64(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5;
	}

	//----------
	// tail

	// Advance offset to the unprocessed tail of the data.
	key.remove_prefix(nblocks * 16);

	uint64_t k1 = 0;
	uint64_t k2 = 0;

	switch (length & 15)
	{
	case 15: k2 ^= ((uint64_t)key[14]) << 48;
		[[fallthrough]];
	case 14: k2 ^= ((uint64_t)key[13]) << 40;
		[[fallthrough]];
	case 13: k2 ^= ((uint64_t)key[12]) << 32;
		[[fallthrough]];
	case 12: k2 ^= ((uint64_t)key[11]) << 24;
		[[fallthrough]];
	case 11: k2 ^= ((uint64_t)key[10]) << 16;
		[[fallthrough]];
	case 10: k2 ^= ((uint64_t)key[9]) << 8;
		[[fallthrough]];
	case  9: k2 ^= ((uint64_t)key[8]) << 0;
		k2 *= c2; k2 = _rotl64(k2, 33); k2 *= c1; h2 ^= k2;
		[[fallthrough]];
	case  8: k1 ^= ((uint64_t)key[7]) << 56;
		[[fallthrough]];
	case  7: k1 ^= ((uint64_t)key[6]) << 48;
		[[fallthrough]];
	case  6: k1 ^= ((uint64_t)key[5]) << 40;
		[[fallthrough]];
	case  5: k1 ^= ((uint64_t)key[4]) << 32;
		[[fallthrough]];
	case  4: k1 ^= ((uint64_t)key[3]) << 24;
		[[fallthrough]];
	case  3: k1 ^= ((uint64_t)key[2]) << 16;
		[[fallthrough]];
	case  2: k1 ^= ((uint64_t)key[1]) << 8;
		[[fallthrough]];
	case  1: k1 ^= ((uint64_t)key[0]);
		k1 *= c1; k1 = _rotl64(k1, 31); k1 *= c2; h1 ^= k1;
	};

	//----------
	// finalization

	h1 ^= length; h2 ^= length;

	h1 += h2;
	h2 += h1;

	h1 = fmix(h1);
	h2 = fmix(h2);

	h1 += h2;
	h2 += h1;

	result[0] = h1;
	result[1] = h2;
}
