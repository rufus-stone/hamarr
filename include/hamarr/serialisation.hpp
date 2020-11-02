#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <algorithm>

#include <spdlog/spdlog.h>

#include "format.hpp"

namespace hmr::kvp
{

using kvps = std::map<std::string_view, std::string_view>;

////////////////////////////////////////////////////////////
std::string serialise(kvps input, char kv_delimiter = '=', char kvp_delimiter = '&');

////////////////////////////////////////////////////////////
kvps deserialise(std::string_view input, char kv_delimiter = '=', char kvp_delimiter = '&');

} // namespace hmr::kvp
