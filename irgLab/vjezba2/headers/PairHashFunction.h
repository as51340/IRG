#pragma once
#include <functional>

/*!
 * Hash function implementation for std::pair,
 * so we can use it as a key in an unordered_map.
 */
class PairHashFunction {
public:
	template <typename T, typename U>
	std::size_t operator()(const std::pair<T, U>& x) const
	{
		return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
	}
};