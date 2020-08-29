#pragma once

#include <string>
#include <type_traits>

// Forward declarations
namespace hmr::hex
{

template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::string encode(T input, bool delimited = true);

std::string decode(std::string_view input);

} // namespace hmr::hex