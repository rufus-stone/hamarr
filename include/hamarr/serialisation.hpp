#pragma once

#include <map>
#include <string>
#include <string_view>


namespace hmr::kvp
{

using kvps = std::map<std::string, std::string>;

////////////////////////////////////////////////////////////
auto serialise(kvps const &input, char kv_delimiter = '=', char kvp_delimiter = '&') -> std::string;

////////////////////////////////////////////////////////////
auto deserialise(std::string_view input, char kv_delimiter = '=', char kvp_delimiter = '&') -> kvps;

} // namespace hmr::kvp
