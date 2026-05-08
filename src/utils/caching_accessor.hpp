/*
	Copyright (C) 2021 - 2025
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#pragma once

#include <functional>
#include <chrono>

#include "utils/optional_fwd.hpp"

namespace utils
{
	template<typename T>
	class caching_accessor
	{
	public:
		using real_accessor_t = std::function<T()>;

		explicit caching_accessor(const real_accessor_t& real_accessor, std::chrono::system_clock::duration maximum_cache_age) :
			real_accessor_(real_accessor),
			maximum_cache_age_(maximum_cache_age)
		{
		}

		const T& value() const
		{
			auto now = std::chrono::system_clock::now();

			if((now - last_updated_) > maximum_cache_age_) {
				cached_value_ = real_accessor_();
				last_updated_ = now;
			}

			return cached_value_.value();
		}

		void invalidate_cached_value() {
			last_updated_ = std::chrono::system_clock::time_point::min();
			// TODO: might be most correct to clear cached_value_ here too, but less performant
		}

	private:
		real_accessor_t real_accessor_;
		std::chrono::system_clock::duration maximum_cache_age_;

		mutable std::chrono::system_clock::time_point last_updated_;
		mutable optional<T> cached_value_;
	};
} // namespace utils
