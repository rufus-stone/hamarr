#pragma once

#include <chrono>

#include <spdlog/spdlog.h>

namespace hmr::profile
{

////////////////////////////////////////////////////////////
template<typename lambda, typename = std::enable_if_t<std::is_invocable_v<lambda>>>
std::size_t benchmark(lambda f, bool report = false)
{
  auto const start = std::chrono::high_resolution_clock::now();

  f();

  auto const finish = std::chrono::high_resolution_clock::now();

  auto nanoseconds_taken = (finish - start).count();

  if (report)
  {
    auto milliseconds_taken = std::chrono::duration<double, std::milli>(finish - start).count();
    auto seconds_taken = std::chrono::duration<double>(finish - start).count();

    spdlog::info("Execution took {} ns ({} ms / {} s)", nanoseconds_taken, milliseconds_taken, seconds_taken);
  }

  return nanoseconds_taken;
}

} // namespace hmr::profile
