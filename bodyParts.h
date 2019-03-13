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
	 * \class BodyParts
	 * \brief Classe utilisé en interne uniquement de la library.
	 *
	 * \authors Florent Denef, Kévin Fornasiero
	 */
	class BodyParts {
	protected:
		std::string name;

		///Store the position of the body part.
		glm::dvec3 position;

		///Store the rotation of the body part (quaternion format because it is Unity's format)
		glm::dquat rotation;

		///ART ID of the body part.
		int32_t id;

	public:
		const std::string &getName() const;

		const glm::dquat &getRotation() const;

		int32_t getId() const;

		const glm::dvec3 &getPosition() const;

		/**
		 * Sets the id of the body part.
		 * \param id
		 */
		void setId(int32_t id);

		/**
		 * Set the position vector of the body part.
		 * Store it inside a glm::dvec3
		 * \param position
		 */
		void setPosition(const double (&position)[3]);

		/**
		 * Set the rotation matrix of the body part.
		 * Converts it to a quaternion.
		 * \param rotation rotation matrix in a one-dimensional array
		 */
		void setRotation(const double (&rotation)[9]);
	};

}


#endif //HUMANANIMATION_BODYPARTS_H
