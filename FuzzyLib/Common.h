#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <iostream>
#include "sstring.h"

#include <cassert>

//using bytes = Fuzzy_lib::basic_sstring<int8_t, uint32_t, 31, false>;
//
//
//using bytes_view = std::basic_string_view<int8_t>;
//using bytes_opt = std::optional<bytes>;
//using sstring_view = std::string_view;


namespace Fuzzy_lib
{
#define FUZZY_ASSERT(x) do { if (!(x)) { assert(#x, __FILE__, __LINE__, __PRETTY_FUNCTION__); } } while (0)

	namespace Exceptions
	{
		class unsupported_operation_exception : public std::runtime_error {
		public:
			unsupported_operation_exception() : std::runtime_error("unsupported operation") {}
			unsupported_operation_exception(const sstring& msg) : std::runtime_error("unsupported operation: " + msg) {}
		};
	}

	template<typename Tag>
	class bool_class {
		bool _value;
	public:
		static const bool_class yes;
		static const bool_class no;

		/// Constructs a bool_class object initialised to \c false.
		constexpr bool_class() noexcept : _value(false) { }

		/// Constructs a bool_class object initialised to \c v.
		constexpr explicit bool_class(bool v) noexcept : _value(v) { }

		/// Casts a bool_class object to an untyped \c bool.
		explicit operator bool() const noexcept { return _value; }

		/// Logical OR.
		friend bool_class operator||(bool_class x, bool_class y) noexcept {
			return bool_class(x._value || y._value);
		}

		/// Logical AND.
		friend bool_class operator&&(bool_class x, bool_class y) noexcept {
			return bool_class(x._value && y._value);
		}

		/// Logical NOT.
		friend bool_class operator!(bool_class x) noexcept {
			return bool_class(!x._value);
		}

		/// Equal-to and not-equal-to operators.
		friend bool operator==(bool_class x, bool_class y) noexcept = default;

		/// Prints bool_class value to an output stream.
		friend std::ostream& operator<<(std::ostream& os, bool_class v) {
			return os << (v._value ? "true" : "false");
		}
	};

	template<typename Tag>
	const bool_class<Tag> bool_class<Tag>::yes{ true };
	template<typename Tag>
	const bool_class<Tag> bool_class<Tag>::no{ false };

	/// @}

	struct stop_iteration_tag { };
	using stop_iteration = bool_class<stop_iteration_tag>;



	namespace Data_types
	{
		using bytes = std::string;
		using bytes_view = std::string_view;
		using bytes_opt = std::optional<bytes>;
	}


}

