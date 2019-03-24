#include "humanAnimation.h"

#include <fstream>

#ifndef NDEBUG

#include <iostream>

#endif

std::unique_ptr<DTrackSDK> human::Human::dtrack = nullptr;
bool human::Human::dtrackCreated = false;

human::Human::Human(const std::string &filename) noexcept(false) {
	using json = nlohmann::json;
	/*
	 * Vérifier le nom du fichier, (.json)
	 * Vérifier existence du fichier.
	 */
	size_t pos = filename.find(".json");

	if (pos == std::string::npos)
		throw std::invalid_argument("No file extension found or wrong extension");

	std::ifstream file(filename);
	if (!file.good())
		throw std::invalid_argument("File does not exist.");

	json config = json::parse(file);
	file.close();

#pragma omp critical
	{
		if (!human::Human::dtrackCreated) {
			human::Human::dtrackCreated = true;
			human::Human::dtrack = std::make_unique<DTrackSDK>(config["dtrack"]["port"].get<int>());
#ifndef NDEBUG
			std::cout << config["dtrack"]["port"].get<int>() << std::endl;
#endif
		}
	}
	this->filename = std::move(filename);
}

human::Human &human::Human::operator=(human::Human &&h) noexcept {

	if (&h != this) {
		this->bodyParts = std::move(h.bodyParts);
		this->filename = std::move(h.filename);
	}

	return *this;
}

void human::Human::setNumBodyParts(size_t num) {
	this->bodyParts.resize(num);
}

void human::Human::pushBodyParts(const DTrack_Body_Type_d *body) {
	auto &bodyPart = this->bodyParts.at(static_cast<size_t >(body->id));

	bodyPart.setId(body->id);
	bodyPart.setRotation(body->rot);
	bodyPart.setPosition(body->loc);
}

const std::vector<human::BodyParts> &human::Human::getBodyParts() const {
	return bodyParts;
}

human::Human *human::Human::Human_create(const char *filename) {
	auto *ptr = new Human(std::string(filename));

	return ptr;
}

void human::Human::Human_destroy(human::Human *ptr) {
	delete ptr;
}

void human::Human::DTrack_destroy() {
	human::Human::dtrack.reset(nullptr);
}

int32_t human::Human::update(human::Human *ptr) noexcept(false) {
	human::Human &ref = *ptr;
	auto &dtrack = human::Human::dtrack;

	if (dtrack->receive()) {
		ref.setNumBodyParts(static_cast<size_t>(dtrack->getNumBody()));
		for (int32_t i = 0; i < dtrack->getNumBody(); ++i) {
			auto bodyPtr = dtrack->getBody(i);
			if (bodyPtr == nullptr)
				throw std::out_of_range("nullptr access");

			if (bodyPtr->quality == -1)
				continue;

			ref.pushBodyParts(bodyPtr);
		}
	}
	return EXIT_SUCCESS;
}

double *human::Human::getBodyPartPos(const human::Human *ptr, size_t id) {
	auto *pos = new double[3];
	auto &vecRef = ptr->getBodyParts().at(id).getPosition();

	pos[0] = vecRef.x;
	pos[1] = vecRef.y;
	pos[2] = vecRef.z;

	return pos;
}

double *human::Human::getBodyPartQuat(const human::Human *ptr, size_t id) {
	auto *rot = new double[4];
	auto &quatRef = ptr->getBodyParts().at(id).getRotation();

	rot[0] = quatRef.x;
	rot[1] = quatRef.y;
	rot[2] = quatRef.z;
	rot[3] = quatRef.w;

	return rot;
}

size_t human::Human::getNumBodyParts(const human::Human *ptr) {
	using json = nlohmann::json;
	std::ifstream fileStr(ptr->filename);

	json file = json::parse(fileStr);
	fileStr.close();

	return file.at("human_models").at(0).size();
}

int *human::Human::getIds(const human::Human *ptr) {
	using json = nlohmann::json;
	int *ids = nullptr;
	std::ifstream fileStr(ptr->filename);
	json file = json::parse(fileStr);
	fileStr.close();

	auto &object = file.at("human_models").at(0/* here the humanID in the future*/);

	ids = new int[object.size()];

	ids[0] = object.at("head").get<int>();
	ids[1] = object.at("foot_l").get<int>();
	ids[2] = object.at("foot_r").get<int>();
	ids[3] = object.at("calf_r").get<int>();
	ids[4] = object.at("hand_l").get<int>();
	ids[5] = object.at("lowerarm_l").get<int>();
	ids[6] = object.at("calf_l").get<int>();
	ids[7] = object.at("lowerarm_r").get<int>();
	ids[8] = object.at("clavicle_l").get<int>();
	ids[9] = object.at("clavicle_r").get<int>();
	ids[10] = object.at("hand_r").get<int>();
	ids[11] = object.at("pelvis").get<int>();

	return ids;
}

const std::unique_ptr<DTrackSDK> &human::Human::getDTrack(human::Human *ptr) {
	return ptr->dtrack;
}