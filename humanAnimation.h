#ifndef UNITYDLL_LIBRARY_H
#define UNITYDLL_LIBRARY_H

#include "for_windows.h"


#include <vector>
#include <memory>

#include <json.hpp>

#include <IUnityInterface.h>
#include <DTrackSDK.hpp>

#include <bodyParts.h>

extern "C" {

namespace human {

	/**
	 * @class Human
	 *
	 * Classe utilisé pour interfacer un être humain avec DTrack vers Unity pour le C#
	 */
	class UNITY_INTERFACE_EXPORT Human {
	protected:
		std::vector<BodyParts> bodyParts;
		std::unique_ptr<DTrackSDK> dtrack{nullptr};

	public:
		explicit Human(const std::string &filename = "config.json");

		virtual ~Human() noexcept = default;

		static int update();

		static int start();

		Human &operator=(Human &&h) noexcept;

		Human &operator=(const Human &h) = delete;

	};
}

}

#endif