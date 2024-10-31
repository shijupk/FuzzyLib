#include "i_filter.h"
#include "MurMurHash3.h"
#include "bloom_filter.h"
#include "bloom_calculations.h"

namespace Fuzzy_lib
{
	namespace Filter
	{
        filter_ptr i_filter::get_filter(int64_t num_elements, double max_false_pos_probability, filter_format fformat) {


            if (max_false_pos_probability > 1.0) {
                throw std::invalid_argument(std::format("Invalid probability {:f}: must be lower than 1.0", max_false_pos_probability));
            }

            if (max_false_pos_probability == 1.0) {
                return std::make_unique<always_present_filter>();
            }

            int buckets_per_element = max_buckets_per_element(num_elements);
            auto spec = compute_bloom_spec(buckets_per_element, max_false_pos_probability);

        	return create_filter(spec.K, num_elements, spec.buckets_per_element, fformat);
        }

        size_t i_filter::get_filter_size(int64_t num_elements, double max_false_pos_probability) {
            if (max_false_pos_probability >= 1.0) {
                return 0;
            }

            int buckets_per_element = max_buckets_per_element(num_elements);
            auto spec = compute_bloom_spec(buckets_per_element, max_false_pos_probability);

            return get_bitset_size(num_elements, spec.buckets_per_element) / 8;
        }

        hashed_key make_hashed_key(Data_types::bytes_view b) {
            std::array<uint64_t, 2> h;
            Murmur_hash::hash3_x64_128(b, 0, h);
            return { h };
        }


	}
}
