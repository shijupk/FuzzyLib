#pragma once
#include <memory>
#include <array>

#include "Common.h"

namespace Fuzzy_lib
{
	namespace Filter
	{
		struct i_filter;

		using filter_ptr = std::unique_ptr<i_filter>;

		enum class filter_format
		{
			k_l_format,
			m_format,
		};

		class hashed_key
		{
		private:
			std::array<uint64_t, 2> _hash;
		public:
			hashed_key(std::array<uint64_t, 2> h) : _hash(h) {}
			std::array<uint64_t, 2> hash() const
			{
				return _hash;
			}
			
		};

		hashed_key make_hashed_key(Data_types::bytes_view key);


		struct i_filter
		{
			virtual ~i_filter() {}

			virtual void add(const Data_types::bytes_view& key) = 0;
			virtual bool is_present(const Data_types::bytes_view& key) = 0;
			virtual bool is_present(hashed_key) = 0;
			virtual void clear() = 0;
			virtual void close() = 0;

			virtual size_t memory_size() = 0;

			/**
			 * @return The smallest bloom_filter that can provide the given false
			 *         positive probability rate for the given number of elements.
			 *
			 *         Asserts that the given probability can be satisfied using this
			 *         filter.
			 */
			static filter_ptr get_filter(int64_t num_elements, double max_false_pos_prob, filter_format format);

			/**
			 * @return the size of the smallest filter (in bytes), according to the conditions described at get_filter()
			 */
			static size_t get_filter_size(int64_t num_elements, double max_false_pos_prob);
		};



	}
}

