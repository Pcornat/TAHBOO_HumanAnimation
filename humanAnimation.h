#ifndef UNITYDLL_LIBRARY_H
#define UNITYDLL_LIBRARY_H

#include "for_windows.h"


#include <vector>
#include <memory>

#include <json.hpp>

#include <IUnityInterface.h>
#include <DTrackSDK.hpp>

#include <bodyParts.h>

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

		/**
		 * Mettre à jour les données dans DTrack.
		 * @return
		 */
		static int update();

		/**
		 * Initialisation d'un objet statique Human.
		 * @return
		 */
		static int start();

		const std::unique_ptr<DTrackSDK> &getDtrack() const;

		const std::vector<BodyParts> &getBodyParts() const;

		void setNumBodyParts(size_t num);

		void pushBodyParts(const DTrack_Body_Type_d *body);

		Human &operator=(Human &&h) noexcept;

		Human &operator=(const Human &h) = delete;

	};

	static std::string to_json();

	static Human instance;
}

#endif