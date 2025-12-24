#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include "core/image.h"

/**
 * @brief Applique une érosion (via filtre Min).
 * Réduit les zones blanches.
 */
Image *morph_erode(const Image *src, int kernel_size);

/**
 * @brief Applique une dilatation (via filtre Max).
 * Agrandi les zones blanches.
 */
Image *morph_dilate(const Image *src, int kernel_size);

/**
 * @brief Applique une Ouverture (Opening).
 * Érosion -> Dilatation. Supprime les petits objets clairs (bruit).
 */
Image *morph_open(const Image *src, int kernel_size);

/**
 * @brief Applique une Fermeture (Closing).
 * Dilatation -> Érosion. Comble les petits trous sombres.
 */
Image *morph_close(const Image *src, int kernel_size);

/**
 * @brief Calcule le Gradient Morphologique.
 * Dilatation - Érosion. Donne les contours de l'objet.
 */
Image *morph_gradient(const Image *src, int kernel_size);

#endif