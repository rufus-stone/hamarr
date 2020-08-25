#pragma once

#include <chrono>

#include "logger.hpp"

namespace hmr::profile
{

////////////////////////////////////////////////////////////
template<typename lambda, typename = std::enable_if_t<std::is_invocable_v<lambda>>>
std::size_t benchmark(lambda f)
{
  auto start = std::chrono::high_resolution_clock::now();

  f();

  auto finish = std::chrono::high_resolution_clock::now();

  auto nanoseconds_taken = (finish - start).count();
  auto milliseconds_taken = std::chrono::duration<double, std::milli>(finish - start).count();
  auto seconds_taken = std::chrono::duration<double>(finish - start).count();

  LOG_INFO("Execution took " << nanoseconds_taken << " ns (" << milliseconds_taken << " ms / " << seconds_taken << " s)");

  return nanoseconds_taken;
}

} // namespace hmr::profile
