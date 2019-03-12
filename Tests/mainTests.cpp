#include <catch2/catch.hpp>
#include <humanAnimation.h>
#include <exception>

TEST_CASE("Constructor test", "[constructor][0] Constructor test") {
	REQUIRE_NOTHROW(human::Human());
	REQUIRE_THROWS_WITH(human::Human("config"), "No file extension found or wrong extension");
	REQUIRE_THROWS_WITH(human::Human("conf.json"), "File does not exist.");
}