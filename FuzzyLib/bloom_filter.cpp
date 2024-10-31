#include "bloom_filter.h"

#include "bloom_calculations.h"

namespace Fuzzy_lib
{
	namespace Filter
	{
		thread_local bloom_filter::stats bloom_filter::_shard_stats;

		template<typename Func>
		void for_each_index(hashed_key hk, int count, int64_t max, filter_format format, Func&& func) {
			auto h = hk.hash();
			int64_t base = (format == filter_format::k_l_format) ? h[0] : h[1];
			int64_t inc = (format == filter_format::k_l_format) ? h[1] : h[0];
			for (int i = 0; i < count; i++) {
				if (func(std::abs(base % max)) == stop_iteration::yes) {
					break;
				}
				base = static_cast<int64_t>(static_cast<uint64_t>(base) + static_cast<uint64_t>(inc));
			}
		}
		bloom_filter::bloom_filter(int hashes, bitmap&& bs, filter_format format) noexcept
			: _bitset(std::move(bs))
			, _hash_count(hashes)
			, _format(format)
		{
			_stats.memory_size += memory_size();
		}

		bloom_filter::~bloom_filter() noexcept {
			_stats.memory_size -= memory_size();
		}

		bool bloom_filter::is_present(hashed_key key) {
			bool result = true;
			for_each_index(key, _hash_count, _bitset.size(), _format, [this, &result](auto i) {
				if (!_bitset.test(i)) {
					result = false;
					return stop_iteration::yes;
				}
				return stop_iteration::no;
				});
			return result;
		}

		void bloom_filter::add(const Data_types::bytes_view& key) {
			for_each_index(make_hashed_key(key), _hash_count, _bitset.size(), _format, [this](auto i) {
				_bitset.set(i);
				return stop_iteration::no;
				});
		}

		bool bloom_filter::is_present(const Data_types::bytes_view& key) {
			return is_present(make_hashed_key(key));
		}

		size_t get_bitset_size(int64_t num_elements, int buckets_per) {
			int64_t num_bits = (num_elements * buckets_per) + EXCESS;
			num_bits = Fuzzy_Lib::Data_types::align_up<int64_t>(num_bits, 64);  // Seems to be implied in origin
			return num_bits;
		}

		filter_ptr create_filter(int hash, Fuzzy_Lib::Data_types::large_bitset&& bitset, filter_format format) {
			return std::make_unique<murmur3_bloom_filter>(hash, std::move(bitset), format);
		}

		filter_ptr create_filter(int hash, int64_t num_elements, int buckets_per, filter_format format) {
			return std::make_unique<murmur3_bloom_filter>(hash, Fuzzy_Lib::Data_types::large_bitset(get_bitset_size(num_elements, buckets_per)), format);
		}


	}
}