#pragma once

#include <chrono>

namespace hmr::profile
{

////////////////////////////////////////////////////////////
template<typename lambda, typename = std::enable_if_t<std::is_invocable_v<lambda>>>
auto benchmark(lambda f) -> std::chrono::nanoseconds
{
  auto const start = std::chrono::high_resolution_clock::now();

  f();

  auto const finish = std::chrono::high_resolution_clock::now();

  auto nanoseconds_taken = (finish - start);

  return nanoseconds_taken;
}

} // namespace hmr::profile
