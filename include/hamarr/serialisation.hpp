#pragma once

#include <map>
#include <string>
#include <string_view>


namespace hmr::kvp
{

using kvps = std::map<std::string, std::string>;

////////////////////////////////////////////////////////////
std::string serialise(kvps input, char kv_delimiter = '=', char kvp_delimiter = '&');

////////////////////////////////////////////////////////////
kvps deserialise(std::string_view input, char kv_delimiter = '=', char kvp_delimiter = '&');

} // namespace hmr::kvp
