/**
 * \mainpage Human Animation in Unity with ART-Tracker.
 * \todo Translate in English.
 * \section intro_sec Introduction
 *
 * \subsection license_sec Licence
 * \copyright GPLv3
 *
 * \subsection need_subsec Le besoin
 * Cette librairie existe pour pouvoir animer en temps réel un avatar humain en 3D sous Unity grâce au <a href="https://ar-tracking.com/products/markers-targets/motion-capture/">système de trakcing d'ART</a>.
 *
 * \subsection constraint_subsec Les contraintes
 * Cette librairie existe parce que le SDK de DTrack n'existe qu'en C++ ainsi qu'OpenCV qui sera utilisé plus tard pour du traitement d'image.
 *
 * \section backend_sec Le Back-end
 *
 * \section inter_sec « Interfaçage » vers C#
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
	 * \brief Classe utilisée pour interfacer un être humain avec DTrack vers Unity pour le C#
	 * \authors Florent Denef, Kévin Fornasiero
	 */
	class Human {
	protected:
		std::vector<BodyParts> bodyParts;
		std::string filename;

		/// Unused for now
		uint32_t humanID;

		static std::unique_ptr<DTrackSDK> dtrack;
		static bool dtrackCreated;

	public:
		/**
		 * Constructeur pour l'objet Human.
		 * \param filename Nom du fichier de configuration, par défaut : config.json.
		 * \throw std::invalid_argument Si le nom de fichier n'est pas valide ou que le fichier n'existe pas.
		 */
		explicit Human(const std::string &filename) noexcept(false);

		virtual ~Human() noexcept = default;

		const std::vector<BodyParts> &getBodyParts() const;

		/**
		 * \brief Permet de réserver à l'avance l'espace nécessaire pour le nombre de BodyParts dans le vector.
		 * Cela permet d'éviter un surplus d'allocations dynamiques (temps système consommé) mais aussi d'avoir un espace alloué trop grand
		 * (l'allocation progressive se fait de façon exponnentielle).
		 * \param num Le nombre de Bodypart nécessaire à allouer.
		 */
		void setNumBodyParts(size_t num);

		/**
		 *
		 * \param body
		 */
		void pushBodyParts(const DTrack_Body_Type_d *body);

		Human &operator=(Human &&h) noexcept;

		Human &operator=(const Human &h) = delete;

		/**
		 * Mettre à jour les données dans DTrack.
		 * \return
		 */
		static int32_t UNITY_INTERFACE_EXPORT update(human::Human *ptr) noexcept(false);

		/**
		 * Création d'un objet Human.
		 * \return
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

		/**
		 * Permet de connaître le nombre de BodyParts présent à partir du fichier JSON (allocation urile en C#).
		 * \param ptr L'objet Human dont on veut connaître le nombre de BodyParts
		 * \return Le nombre de BodyParts.
		 */
		static size_t UNITY_INTERFACE_EXPORT getNumBodyParts(const human::Human *ptr);

		/**
		 * Ça fonctionne uniquement pour le nombre de partie que l'on a ainsi que la configuration présente (les IDs).
		 * \param ptr L'objet Human dont on veut connaître le nombre de BodyParts
		 * \return Les id des BodyParts
		 */
		static int *UNITY_INTERFACE_EXPORT getIds(const human::Human *ptr);

		/**
		 * Obtenir la position d'un BodyPart.
		 * \param ptr L'être humain dont on veut obtenir les données.
		 * \param id L'identifiant ART du BodyPart
		 * \return Tableau de double contenant la position (x, y, z)
		 */
		static double *UNITY_INTERFACE_EXPORT getBodyPartPos(const human::Human *ptr, size_t id);

		/**
		 * Obtenir le quaternion de rotation d'un BodyPart.
		 * @param ptr L'être humain dont on veut obtenir les données.
		 * @param id L'identifiant ART du BodyPart
		 * @return Tableau de double contenant le quaternion (x, y, z, w)
		 */
		static double *UNITY_INTERFACE_EXPORT getBodyPartQuat(const human::Human *ptr, size_t id);

		/**
		 * Utile seulement pour les tests unitaires. Inutilisables en C#
		 * \param ptr
		 * \return
		 */
		static const std::unique_ptr<DTrackSDK> &getDTrack(human::Human *ptr);

	};

}

#endif