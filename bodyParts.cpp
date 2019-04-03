//
// Created by postaron on 27/02/2019.
//

#include "bodyParts.h"
#include <glm/gtc/quaternion.hpp>

const glm::dquat &human::BodyParts::getRotation() const {
	return rotation;
}

void human::BodyParts::setRotation(const double (&rotation)[9]) {
	//DTrack a ses matrices de rotation sous forme FORTRAN, column-major (column-wise comme dit dans la doc d'ART). Voilà ce que cela donne.
	BodyParts::rotation = glm::quat_cast(glm::dmat3(rotation[0], rotation[3], rotation[6],
													rotation[1], rotation[4], rotation[7],
													rotation[2], rotation[5], rotation[8]));
}

const std::string &human::BodyParts::getName() const {
	return name;
}

const glm::dvec3 &human::BodyParts::getPosition() const {
	return position;
}

void human::BodyParts::setPosition(const double (&position)[3]) {

	BodyParts::position = glm::dvec3(position[0], position[1], position[2]);
}

int32_t human::BodyParts::getId() const {
	return id;
}

void human::BodyParts::setId(int32_t id) {
	BodyParts::id = id;
}

#ifndef NDEBUG
std::ostream &human::operator<<(std::ostream &os, const human::BodyParts &parts) {
	os << "name: " << parts.name << "\nposition: " << parts.position.x << ' ' << parts.position.y << ' ' << parts.position.z << "\nrotation: " << parts.rotation.x
	<< ' ' << parts.rotation.y << ' ' << parts.rotation.z << ' ' << parts.rotation.w << "\nid: " << parts.id;
	return os;
}
#endif

human::BodyParts::BodyParts(size_t id) : id(id) {}