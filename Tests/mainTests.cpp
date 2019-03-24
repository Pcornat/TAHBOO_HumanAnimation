#include <catch2/catch.hpp>
#include <humanAnimation.h>
#include <iostream>

TEST_CASE("Constructor and destructors test", "[constructor][0] Constructor test") {
	REQUIRE_THROWS_WITH(human::Human("config"), "No file extension found or wrong extension");
	REQUIRE_THROWS_WITH(human::Human("conf.json"), "File does not exist.");
	REQUIRE_NOTHROW(human::Human("config.json"));

	auto *h = human::Human::Human_create("config.json");
	auto *test = human::Human::Human_create("config.json");
	REQUIRE(human::Human::getDTrack(h) != nullptr);
	REQUIRE(human::Human::getDTrack(test) != nullptr);
	REQUIRE(human::Human::getDTrack(test) == human::Human::getDTrack(h));

	REQUIRE_NOTHROW(human::Human::Human_destroy(h));
	REQUIRE_NOTHROW(human::Human::Human_destroy(test));
	REQUIRE(human::Human::getDTrack(nullptr) != nullptr);

	REQUIRE_NOTHROW(human::Human::DTrack_destroy());
	REQUIRE(human::Human::getDTrack(nullptr) == nullptr);
}

TEST_CASE("Get ids test", "[getter][0]") {
	int *ptr = nullptr;
	human::Human test("config.json");
	REQUIRE_NOTHROW(ptr = human::Human::getIds(&test));
	//std::cout << ptr;
	human::Human::DTrack_destroy();
	delete ptr;
}

TEST_CASE("Get number of BodyParts", "[getter][1]") {
	human::Human test("config.json");
	int num = 0;
	REQUIRE_NOTHROW(num = human::Human::getNumBodyParts(&test));
	REQUIRE(num == 12);
	human::Human::DTrack_destroy();
}