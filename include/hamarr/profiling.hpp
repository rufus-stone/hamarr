#pragma once

#include <chrono>

namespace hmr::profile
{

////////////////////////////////////////////////////////////
template<typename lambda, typename = std::enable_if_t<std::is_invocable_v<lambda>>>
std::size_t benchmark(lambda f)
{
  auto const start = std::chrono::high_resolution_clock::now();

  f();

  auto const finish = std::chrono::high_resolution_clock::now();

  auto nanoseconds_taken = (finish - start).count();

  return nanoseconds_taken;
}

} // namespace hmr::profile
