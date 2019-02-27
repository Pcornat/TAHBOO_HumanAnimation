#ifndef UNITYDLL_LIBRARY_H
#define UNITYDLL_LIBRARY_H

#include <IUnityInterface.h>
#include <vector>
#include <bodyParts.h>

extern "C" {

namespace human {

	/**
	 * @class Human
	 *
	 * Classe utilisé pour interfacer un être humain avec DTrack vers Unity pour le C#
	 */
	class UNITY_INTERFACE_EXPORT Human {
	private:
		std::vector<BodyParts> bodyParts;

	public:

		static update();

		static start();

	};
}

}

#endif