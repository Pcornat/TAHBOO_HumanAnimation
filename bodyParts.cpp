//
// Created by postaron on 27/02/2019.
//

#include "bodyParts.h"
#include <glm/gtc/quaternion.hpp>

const glm::dquat &human::BodyParts::getRotation() const {
	return rotation;
}

void human::BodyParts::setRotation(const double (&rotation)[9]) {
	//DTrack a ses matrices de rotation sous forme FORTRAN, column-major. Voilà ce que cela donne.
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

nlohmann::json human::to_json(nlohmann::json &j, const human::BodyParts &body) {
	j[body.getName()] = {
			{"rotation", {
								 body.getRotation().x,
								 body.getRotation().y,
								 body.getRotation().z,
								 body.getRotation().w
						 }
			},
			{"position", {
								 body.getPosition().x,
								 body.getPosition().y,
								 body.getPosition().z
						 }},
			{"id",       body.getId()}
	};
	return j;
}
