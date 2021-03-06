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

	for (const auto &truc : config["human_models"].at(0)) {
		this->bodyParts[truc.get<size_t>()] = BodyParts(truc.get<size_t>());
	}

#pragma omp critical
	{
		if (!human::Human::dtrackCreated) {
			human::Human::dtrackCreated = true;
			human::Human::dtrack = std::make_unique<DTrackSDK>(config["dtrack"]["port"].get<int>());
#ifndef NDEBUG
			std::cout << config["dtrack"]["port"].get<int>() << std::endl;
			for (const auto &part : bodyParts) {
				std::cout << "id : " << part.first << std::endl;
			}
#endif
		}
	}
	this->filename = filename;
}

const std::string &human::Human::getFilename() const {
	return filename;
}

human::Human &human::Human::operator=(human::Human &&h) noexcept {

	if (&h != this) {
		this->bodyParts = std::move(h.bodyParts);
		this->filename = std::move(h.filename);
	}

	return *this;
}

void human::Human::pushBodyParts(const DTrack_Body_Type_d *body) {
	auto &bodyPart = this->bodyParts.at(static_cast<size_t >(body->id + 1));

	//bodyPart.setId(body->id);
	bodyPart.setRotation(body->rot);
	bodyPart.setPosition(body->loc);
}

const std::unordered_map<size_t, human::BodyParts> &human::Human::getBodyParts() const {
	return bodyParts;
}

const std::unique_ptr<DTrackSDK> &human::Human::getDTrack() {
	return human::Human::dtrack;
}

size_t human::Human::getNumBodyParts(const human::Human *ptr) {
	using json = nlohmann::json;
	std::ifstream fileStr(ptr->filename);

	json file = json::parse(fileStr);
	fileStr.close();

	return file.at("human_models").at(0).size();
}

void human::Human::destroyDtrack() {
	human::Human::dtrack.reset(nullptr);
	human::Human::dtrackCreated = false;
}

void *Human_create(const char *filename) {
	auto *ptr = new human::Human(std::string(filename));

	return ptr;
}

void Human_destroy(void *ptr) {
	auto *toDelete = static_cast<human::Human *>(ptr);
	delete toDelete;
}

void DTrack_destroy() {
	human::Human::destroyDtrack();
}

int32_t update(void *ptr) {
	human::Human &ref = *static_cast<human::Human *>(ptr);
	auto &dtrack = human::Human::getDTrack();

	if (dtrack->receive()) {
		for (const auto &bodyPart : ref.getBodyParts()) {
			auto bodyPtr = dtrack->getBody(bodyPart.first - 1);
			if (bodyPtr == nullptr)
				throw std::out_of_range("nullptr access");

			if (bodyPtr->quality == -1)
				continue;

			ref.pushBodyParts(bodyPtr);
		}
		return EXIT_SUCCESS;
	} else
		return EXIT_FAILURE;

}

double *getBodyPartPos(const void *ptr, size_t id) {
	auto *pos = new double[3];
	const human::Human* ptrhuman = (const human::Human *)(ptr);
	const glm::dvec3 &vecRef =ptrhuman->getBodyParts().at(id).getPosition();

	pos[0] = vecRef.x;
	pos[1] = vecRef.y;
	pos[2] = vecRef.z;


	return pos;
}

double *getBodyPartQuat(const void *ptr, size_t id) {
	auto *rot = new double[4];
	const human::Human* ptrhuman = static_cast<const human::Human *>(ptr);
	const glm::dquat &quatRef =ptrhuman->getBodyParts().at(id).getRotation();

	rot[0] = quatRef.x;
	rot[1] = quatRef.y;
	rot[2] = quatRef.z;
	rot[3] = quatRef.w;


	return rot;
}

size_t *getIds(const void *ptr) {
	using json = nlohmann::json;
	size_t *ids = nullptr;
	std::ifstream fileStr(static_cast<const human::Human *>(ptr)->getFilename());
	json file = json::parse(fileStr);
	fileStr.close();

	auto &object = file.at("human_models").at(0/* here the humanID in the future*/);

	ids = new size_t[object.size() + 1];

	ids[0] = object.size();
	ids[1] = object.at("head").get<size_t>();
	ids[2] = object.at("foot_l").get<size_t>();
	ids[3] = object.at("foot_r").get<size_t>();
	ids[4] = object.at("calf_r").get<size_t>();
	ids[5] = object.at("hand_l").get<size_t>();
	ids[6] = object.at("lowerarm_l").get<size_t>();
	ids[7] = object.at("calf_l").get<size_t>();
	ids[8] = object.at("lowerarm_r").get<size_t>();
	ids[9] = object.at("clavicle_l").get<size_t>();
	ids[10] = object.at("clavicle_r").get<size_t>();
	ids[11] = object.at("hand_r").get<size_t>();
	ids[12] = object.at("pelvis").get<size_t>();

	return ids;
}

void destroyDoubleArray(double *ptr) {
	delete[] ptr;
}

void destroyIdsArray(size_t *ptr) {
	delete[] ptr;
}