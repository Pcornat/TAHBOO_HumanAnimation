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
 * \subsection dtrack_part DTrack
 * \paragraph explication_dtrack Explication
 * DTrack est utilisé en combinaison de la solution ART-Tracker.
 * Si un Human est créé, il suffit de faire un update. En interne : DTrack fait un receive.
 * Ensuite, on alloue le vector de BodyParts en récupérant le nombre de Body que DTrack nous donne. Un Body dans DTrack est une partie du corps, comme en motion capture.\\
 * Après cela, les BodyPart sont push dans le vector.
 *
 * \subsection bodypart_backend BodyPart
 * Cet objet représente une Body de DTrack pour nous : une position, une rotation et un ID.\\
 * \paragraph explication_body Explication
 * Cette classe BodyPart est utile pour les attributs rotation et position de façon transparente : en entrée : des matrices/vecteurs et en interne un vec3 et un quaternion.
 * La particularité de DTrack est qu'il donne une matrice de rotation (3x3) mais les données sont rangées au format column-wise (comme en Fortran) de ce fait une petite conversion interne se fait. L'utilisation est transparente.
 * Il est impossible d'utiliser un BodyPart en dehors de la library.
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
		static UNITY_INTERFACE_EXPORT int32_t update(human::Human *ptr) noexcept(false);

		/**
		 * Création d'un objet Human.
		 * \return
		 */
		static UNITY_INTERFACE_EXPORT Human *Human_create(const char *filename);

		/**
		 * Destruction d'un objet Human.
		 */
		static UNITY_INTERFACE_EXPORT void Human_destroy(human::Human *ptr);

		/**
		 * Destruction de l'instance unique de DTrack.
		 */
		static UNITY_INTERFACE_EXPORT void DTrack_destroy();

		/**
		 * Permet de connaître le nombre de BodyParts présent à partir du fichier JSON (allocation urile en C#).
		 * \param ptr L'objet Human dont on veut connaître le nombre de BodyParts
		 * \return Le nombre de BodyParts.
		 */
		static UNITY_INTERFACE_EXPORT size_t getNumBodyParts(const human::Human *ptr);

		/**
		 * Ça fonctionne uniquement pour le nombre de partie que l'on a ainsi que la configuration présente (les IDs).
		 * \param ptr L'objet Human dont on veut connaître le nombre de BodyParts
		 * \return Les id des BodyParts
		 */
		static UNITY_INTERFACE_EXPORT int *getIds(const human::Human *ptr);

		/**
		 * Obtenir la position d'un BodyPart.
		 * \param ptr L'être humain dont on veut obtenir les données.
		 * \param id L'identifiant ART du BodyPart
		 * \return Tableau de double contenant la position (x, y, z)
		 */
		static UNITY_INTERFACE_EXPORT double *getBodyPartPos(const human::Human *ptr, size_t id);

		/**
		 * Obtenir le quaternion de rotation d'un BodyPart.
		 * @param ptr L'être humain dont on veut obtenir les données.
		 * @param id L'identifiant ART du BodyPart
		 * @return Tableau de double contenant le quaternion (x, y, z, w)
		 */
		static UNITY_INTERFACE_EXPORT double *getBodyPartQuat(const human::Human *ptr, size_t id);

		/**
		 * Utile seulement pour les tests unitaires. Inutilisables en C#
		 * \param ptr
		 * \return
		 */
		static const std::unique_ptr<DTrackSDK> &getDTrack(human::Human *ptr);

	};

}

#endif