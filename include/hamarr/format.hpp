#pragma once

#include <string>
#include <string_view>
#include <algorithm>

#include <spdlog/spdlog.h>

#include "hex.hpp"

namespace hmr::format
{

////////////////////////////////////////////////////////////
std::string to_upper(std::string_view input);


////////////////////////////////////////////////////////////
std::string to_lower(std::string_view input);


////////////////////////////////////////////////////////////
std::string escape(std::string_view input);


////////////////////////////////////////////////////////////
std::string unescape(std::string_view input);


////////////////////////////////////////////////////////////
auto split(std::string_view input, char delimiter) -> std::vector<std::string_view>;

} // namespace hmr::format
