//
// Created by postaron on 27/02/2019.
//

#ifndef HUMANANIMATION_BODYPARTS_H
#define HUMANANIMATION_BODYPARTS_H

#include "for_windows.h"

#include <string>
#include <glm/ext/quaternion_double.hpp>


namespace human {

	/**
	 * @class BodyParts
	 *
	 * Classe utilisé en interne uniquement de la library.
	 */
	class BodyParts {
	protected:
		std::string name;
		glm::dvec3 position;
		//rotation : quaternion. Donc utilisation de GLM.
		glm::dquat rotation;
		int32_t id;

	public:
		const std::string &getName() const;

		const glm::dquat &getRotation() const;

		int32_t getId() const;

		const glm::dvec3 &getPosition() const;

		void setPosition(const double (&position)[3]);

		/**
		 * Set the rotation matrix of the body parts.
		 * Converts it to a quaternion.
		 * @param rotation rotation matrix in a one-dimensional array
		 */
		void setRotation(const double (&rotation)[9]);
	};

}


#endif //HUMANANIMATION_BODYPARTS_H
