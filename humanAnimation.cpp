#include "humanAnimation.h"

#include <utility>

int human::Human::update() {
	return 0;
}

int human::Human::start() {

	return EXIT_SUCCESS;
}

human::Human::Human(const std::string &filename) {
	/*
	 * Vérifier le nom du fichier, (.json toussa)
	 * Vérifier existence du fichier.
	 */
}

human::Human &human::Human::operator=(human::Human &&h) noexcept {

	if (&h != this) {
		this->bodyParts = std::move(h.bodyParts);
		this->file = std::move(h.file);
		this->dtrack = std::move_if_noexcept(h.dtrack);
	}

	return *this;
}
