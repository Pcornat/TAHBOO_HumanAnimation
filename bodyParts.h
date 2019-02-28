//
// Created by postaron on 27/02/2019.
//

#ifndef HUMANANIMATION_BODYPARTS_H
#define HUMANANIMATION_BODYPARTS_H

#include "for_windows.h"

#define GLM_EXT_INCLUDED

#include <string>
#include <array>
#include <glm/ext/quaternion_float.hpp>


extern "C" {

namespace human {

	/**
	 * @class BodyParts
	 *
	 * Classe utilisé en interne uniquement de la library.
	 */
	class BodyParts {
	protected:
		std::string name;
		std::array<float, 3> position;
		std::array<float, 3> scale;
		//rotation : quaternion. Donc utilisation de GLM.
		glm::quat rotation; //On devra convertir de la matrice 3x3 de DTrack (un simple float[3][3])vers le quaternion.

	public:
	};

}
}


#endif //HUMANANIMATION_BODYPARTS_H
