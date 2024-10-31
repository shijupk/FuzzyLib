#include "large_bitset.h"

using namespace Fuzzy_Lib::Data_types;

large_bitset::large_bitset(size_t nr_bits) : _nr_bits(nr_bits)
{
	size_t nr_ints = align_up(nr_bits, bits_per_int()) / bits_per_int();

	_storage.reserve(nr_bits);
	while (nr_ints)
	{
		_storage.push_back(0);
		--nr_ints;
	}
}
void
large_bitset::clear()
{
	for (auto&& pos : _storage)
	{
		pos = 0;
	}
}