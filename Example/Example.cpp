// Example.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include "../FuzzyLib/i_filter.h"
#include "../FuzzyLib/MurMurHash3.h"
#include "../FuzzyLib/MurMurHash2.h"
#include "../FuzzyLib/large_bitset.h"

using namespace Fuzzy_lib::Murmur_hash;
using namespace Fuzzy_Lib::Data_types;

int main()
{
	constexpr uint64_t seed = 0;
	Fuzzy_lib::Data_types::bytes src;
	src = Fuzzy_lib::Data_types::bytes("012012012012012");
	uint64_t sink = 0;
	std::vector<uint8_t> src2 = {'0', '1', '2', '0', '1', '2', '0', '1', '2', '0', '1', '2', '0', '1', '2'};
	std::array<uint64_t, 2> dst;
	std::array<uint64_t, 2> dst2;

	auto bits = large_bitset(100);

	auto s = bits.size();

	bits.set(0);

	auto val = bits.test(1);

	const Fuzzy_lib::Filter::filter_ptr bloom = Fuzzy_lib::Filter::i_filter::get_filter(
		5, 0.01, Fuzzy_lib::Filter::filter_format::k_l_format);

	bloom->add("Apple1");
	bloom->add("Apple2");
	bloom->add("Apple3");
	bloom->add("Apple4");
	bloom->add("Apple5");
	bloom->add("Apple6");
	bloom->add("Apple7");

	bloom->add("Apple8");
	bloom->add("Apple9");
	bloom->add("Apple10");
	bloom->add("Apple11");
	bloom->add("Apple12");
	bloom->add("Apple13");
	bloom->add("Apple14");

	std::cout << bloom->is_present("Apple1") << std::endl;
	std::cout << bloom->is_present("Apple2") << std::endl;
	std::cout << bloom->is_present("Apple3") << std::endl;
	std::cout << bloom->is_present("Apple4") << std::endl;
	std::cout << bloom->is_present("Apple5") << std::endl;
	std::cout << bloom->is_present("Apple6") << std::endl;
	std::cout << bloom->is_present("Apple7") << std::endl;

	std::cout << bloom->is_present("Apple8") << std::endl;
	std::cout << bloom->is_present("Apple9") << std::endl;
	std::cout << bloom->is_present("Apple10") << std::endl;
	std::cout << bloom->is_present("Apple11") << std::endl;
	std::cout << bloom->is_present("Apple12") << std::endl;
	std::cout << bloom->is_present("Apple13") << std::endl;
	std::cout << bloom->is_present("Apple14") << std::endl;

	std::cout << "No" << std::endl;
	std::cout << bloom->is_present("Apple81") << std::endl;
	std::cout << bloom->is_present("Apple92") << std::endl;
	std::cout << bloom->is_present("Apple103") << std::endl;
	std::cout << bloom->is_present("Apple114") << std::endl;
	std::cout << bloom->is_present("Apple125") << std::endl;
	std::cout << bloom->is_present("Apple136") << std::endl;
	std::cout << bloom->is_present("Apple149") << std::endl;


	uint64_t res = MurmurHash64B("res", 3, 0);
	hash3_x64_128(src, seed, dst);
	hash3_x64_128(src2.begin(), src2.size(), seed, dst2);
}
