#include "humanAnimation.h"

#include <fstream>
#include <string>
#include <cstdlib>
#include <utility>
#include <DTrackDataTypes.h>

int human::Human::update() {
	if (human::instance.getDtrack()->receive()) {
		human::instance.setNumBodyParts(static_cast<size_t>(human::instance.getDtrack()->getNumBody()));
		for (size_t i = 0; i < static_cast<size_t>(human::instance.getDtrack()->getNumBody()); ++i) {
			auto ptr = human::instance.getDtrack()->getBody(i);
			if (ptr == nullptr)
				throw std::out_of_range("nullptr access");

			if (ptr->quality == -1)
				continue;

			human::instance.pushBodyParts(ptr);
		}
	}
	return EXIT_SUCCESS;
}

int human::Human::start() {
	human::instance = human::Human();

	return EXIT_SUCCESS;
}

human::Human::Human(const std::string &filename) {
	/*
	 * Vérifier le nom du fichier, (.json toussa)
	 * Vérifier existence du fichier.
	 */
	size_t pos = filename.find(".json");

	if (pos == std::string::npos)
		throw std::invalid_argument("No file extension found or wrong extension");

	std::ifstream file(filename);
	if (!file.good())
		throw std::invalid_argument("File does not exist.");
	file.close();
}

human::Human &human::Human::operator=(human::Human &&h) noexcept {

	if (&h != this) {
		this->bodyParts = std::move(h.bodyParts);
		this->dtrack = std::move_if_noexcept(h.dtrack);
	}

	return *this;
}

const std::unique_ptr<DTrackSDK> &human::Human::getDtrack() const {
	return dtrack;
}

void human::Human::setNumBodyParts(size_t num) {
	this->bodyParts.resize(num);
}

void human::Human::pushBodyParts(const DTrack_Body_Type_d *body) {
	auto &bodyPart = this->bodyParts.at(static_cast<unsigned long>(body->id));
	bodyPart.setRotation(body->rot);
	bodyPart.setPosition(body->loc);
}

const std::vector<human::BodyParts> &human::Human::getBodyParts() const {
	return bodyParts;
}

std::string human::to_json() {
	nlohmann::json j;
	for (const auto &bodyPart : human::instance.getBodyParts()) {
		j["human"] += human::to_json(j, bodyPart);
	}
	return j.dump();
}