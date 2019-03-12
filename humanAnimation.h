/**
 * \mainpage Human Animation in Unity with ART-Tracker.
 * \todo Translate in English.
 * \section intro_sec Introduction
 * \subsection need_sec Le besoin
 * Le besoin de cette librarie est pour pouvoir animer en temps réel un avatar humain en 3D sous Unity grâce au <a href="https://ar-tracking.com/products/markers-targets/motion-capture/">système de trakcing d'ART</a>.
 *
 */


#ifndef UNITYDLL_LIBRARY_H
#define UNITYDLL_LIBRARY_H

#include "for_windows.h"


#include <vector>
#include <memory>

#include <json.hpp>

#include <IUnityInterface.h>
#include <DTrackSDK.hpp>

#include <bodyParts.h>

/**
 * \namespace human
 * Namespace utilisé pour les classes de la librarie.
 */
namespace human {

	/**
	 * \class Human
	 *
	 * Classe utilisée pour interfacer un être humain avec DTrack vers Unity pour le C#
	 */
	class Human {
	protected:
		std::vector<BodyParts> bodyParts;
		static std::unique_ptr<DTrackSDK> dtrack;
		static bool dtrackCreated;

	public:
		/**
		 * Constructeur pour l'objet Human.
		 * @param filename Nom du fichier de configuration, par défaut : config.json.
		 * @throw std::invalid_argument Si le nom de fichier n'est pas valide ou que le fichier n'existe pas.
		 */
		explicit Human(const std::string &filename = "config.json") throw(std::invalid_argument);

		virtual ~Human() noexcept = default;

		/*const std::shared_ptr<DTrackSDK> &getDtrack() const;*/

		const std::vector<BodyParts> &getBodyParts() const;

		/**
		 * @brief Permet de réserver à l'avance l'espace nécessaire pour le nombre de BodyParts dans le vector.
		 * Cela permet d'éviter un surplus d'allocations dynamiques (temps système consommé) mais aussi d'avoir un espace alloué trop grand
		 * (l'allocation progressive se fait de façon exponnentielle).
		 * @param num Le nombre de Bodypart nécessaire à allouer.
		 */
		void setNumBodyParts(size_t num);

		/**
		 *
		 * @param body
		 */
		void pushBodyParts(const DTrack_Body_Type_d *body);

		Human &operator=(Human &&h) noexcept;

		Human &operator=(const Human &h) = delete;

		/**
		 * Mettre à jour les données dans DTrack.
		 * @return
		 */
		static int32_t UNITY_INTERFACE_EXPORT update(human::Human *ptr);

		/**
		 * Création d'un objet Human.
		 * @return
		 */
		static Human *UNITY_INTERFACE_EXPORT Human_create(const char *filename);

		/**
		 * Destruction d'un objet Human.
		 */
		static void UNITY_INTERFACE_EXPORT Human_destroy(human::Human *ptr);

		/**
		 * Destruction de l'instance unique de DTrack.
		 */
		static void UNITY_INTERFACE_EXPORT DTrack_destroy();

	};

}

#endif