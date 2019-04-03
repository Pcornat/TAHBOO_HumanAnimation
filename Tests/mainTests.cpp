#include <catch2/catch.hpp>
#include <humanAnimation.h>

#ifndef NDEBUG
#include <iostream>
#endif

TEST_CASE("Constructor and destructors test", "[constructor][0] Constructor test") {
	REQUIRE_THROWS_WITH(human::Human("config"), "No file extension found or wrong extension");
	REQUIRE_THROWS_WITH(human::Human("conf.json"), "File does not exist.");
	REQUIRE_NOTHROW(human::Human("config.json"));

	auto *h = Human_create("config.json");
	auto *test = Human_create("config.json");
	REQUIRE(human::Human::getDTrack() != nullptr);
	REQUIRE(human::Human::getDTrack() != nullptr);
	REQUIRE(human::Human::getDTrack() == human::Human::getDTrack());

	REQUIRE_NOTHROW(Human_destroy(h));
	REQUIRE_NOTHROW(Human_destroy(test));
	REQUIRE(human::Human::getDTrack() != nullptr);

	REQUIRE_NOTHROW(DTrack_destroy());
	REQUIRE(human::Human::getDTrack() == nullptr);
}

TEST_CASE("Get ids test", "[getter][0]") {
	size_t *ptr = nullptr;
	human::Human test("config.json");
	REQUIRE_NOTHROW(ptr = getIds(&test));
	DTrack_destroy();
	delete[] ptr;
}

TEST_CASE("Get number of BodyParts", "[getter][1]") {
	human::Human test("config.json");
	size_t num = 0/*, numEnter = 0*/;
	REQUIRE_NOTHROW(num = human::Human::getNumBodyParts(&test));
	REQUIRE(num == 12);
	DTrack_destroy();
}

TEST_CASE("DTrack test", "[dtrack][0]") {
	void *ptr = Human_create("config.json");
	auto *hPtr = static_cast<human::Human *>(ptr);
	int32_t id = 0;
	while (update(ptr) == EXIT_FAILURE);

#ifndef NDEBUG
	for (auto &bodyPart : hPtr->getBodyParts()) {
		std::cout << "key : " << bodyPart.first << '\n' << "BodyPart : " << bodyPart.second << std::endl;
	}
#endif

	REQUIRE_NOTHROW(id = hPtr->getBodyParts().at(3).getId());
	REQUIRE(id == 3);
}