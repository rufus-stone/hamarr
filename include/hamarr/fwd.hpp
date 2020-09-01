#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <type_traits>

// Forward declarations
namespace hmr::format
{

std::string to_upper(std::string_view input);
std::string to_lower(std::string_view input);
std::string escape(std::string_view input);
std::string unescape(std::string_view input);
auto split(std::string_view input, char delimiter) -> std::vector<std::string_view>;

}

namespace hmr::hex
{

template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::string encode(T input, bool delimited = true);

std::string decode(std::string_view input);

} // namespace hmr::hex