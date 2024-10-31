#pragma once
#include <cstdint>
#include <limits>
#include <vector>
#include <cstdlib>


namespace Fuzzy_Lib
{
	namespace Data_types
	{
		template <typename T>
		inline constexpr
			T align_up(T v, T align) {
			return (v + align - 1) & ~(align - 1);
		}

		template <typename T>
		inline constexpr
			T* align_up(T* v, size_t align) {
			static_assert(sizeof(T) == 1, "align byte pointers only");
			return reinterpret_cast<T*>(align_up(reinterpret_cast<uintptr_t>(v), align));
		}

		template <typename T>
		inline constexpr
			T align_down(T v, T align) {
			return v & ~(align - 1);
		}

		template <typename T>
		inline constexpr
			T* align_down(T* v, size_t align) {
			static_assert(sizeof(T) == 1, "align byte pointers only");
			return reinterpret_cast<T*>(align_down(reinterpret_cast<uintptr_t>(v), align));
		}


		class large_bitset
		{
			using int_type = uint64_t;

			static constexpr size_t bits_per_int()
			{
				return std::numeric_limits<int_type>::digits;
			}
			size_t _nr_bits = 0;
			std::vector<int_type> _storage;
		public:
			explicit large_bitset(size_t nr_bits);
			explicit large_bitset(size_t nr_bits, std::vector<int_type> storage) :
				_nr_bits(nr_bits), _storage(std::move(storage)) {}

			large_bitset(large_bitset&&) = default;
			large_bitset(const large_bitset&) = delete;
			large_bitset& operator=(const large_bitset&) = delete;
			size_t size() const
			{
				return _nr_bits;
			}

			size_t memory_size() const
			{
				return _storage.capacity();
			}

			bool test(size_t idx) const {
				auto idx1 = idx / bits_per_int();
				idx %= bits_per_int();
				auto idx2 = idx;
				return (_storage[idx1] >> idx2) & 1;
			}
			void set(size_t idx) {
				auto idx1 = idx / bits_per_int();
				idx %= bits_per_int();
				auto idx2 = idx;
				_storage[idx1] |= int_type(1) << idx2;
			}
			void clear(size_t idx) {
				auto idx1 = idx / bits_per_int();
				idx %= bits_per_int();
				auto idx2 = idx;
				_storage[idx1] &= ~(int_type(1) << idx2);
			}
			void clear();

			const std::vector<int_type>& get_storage() const {
				return _storage;
			}

			
		};
	}
}
