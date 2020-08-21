#define CATCH_CONFIG_MAIN // This tells the Catch2 header to generate a main

#include <string>

#include <hamarr/hamarr.hpp>

#include "catch.hpp"


using namespace std::string_literals;

SCENARIO("Strings can be converted between uppercase and lowercase")
{
  GIVEN("A std::string with some data")
  {
    const auto test_data = std::string{"Hello, World!"};

    REQUIRE(!test_data.empty());

    WHEN("hmr::format::to_upper() is applied")
    {
      auto upper = hmr::format::to_upper(test_data);

      THEN("the string is converted to all uppercase.")
      {
        REQUIRE(upper == "HELLO, WORLD!"s);

        WHEN("hmr::format::to_upper() is applied")
        {
          auto lower = hmr::format::to_lower(test_data);

          THEN("the string is converted to all lowercase.")
          {
            REQUIRE(lower == "hello, world!"s);
          }
        }
      }
    }
  }
}