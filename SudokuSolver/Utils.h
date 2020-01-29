#pragma once
#include <type_traits>

namespace SudokuUtils {
	template <
		class _Ty,
		typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value>
	>
	constexpr const _Ty isqrt_impl(_Ty sq, _Ty dlt, _Ty value) {
		// I got this from some where, I just added the template part.
		return sq <= value ?
			isqrt_impl<_Ty>(sq + dlt, dlt + 2, value) : (dlt >> 1) - 1;
	}

	template <
		class _Ty,
		typename = std::enable_if<std::is_arithmetic<_Ty>::value && std::is_unsigned<_Ty>::value>
	>
	constexpr const _Ty isqrt(_Ty value) {
		// I got this from some where, I just added the template part.
		return isqrt_impl<_Ty>(1, 3, value);
	}
}

