#ifndef POINTWISE_H
#define POINTWISE_H

#include "core/image.h"

/**
 * @brief Applique une transformation linéaire à une image en niveaux de gris.
 *
 * I' = a * I + b
 *  où I est le pixel original, I' est le pixel transformé,
 * a est le gain (multiplier), et b est le biais (offset).
 *
 * @param img L'image source.
 * @param a Le facteur de gain.
 * @param b Le facteur de biais.
 */
void linear_transform(Image *img, double a, double b);

/**
 * @brief Applique une transformation linéaire avec saturation.
 *
 * Les pixels en dehors des seuils min/max sont écrêtés.
 *
 * @param img L'image source.
 * @param min_in Le niveau de gris d'entrée qui correspond à 0 en sortie.
 * @param max_in Le niveau de gris d'entrée qui correspond à 255 en sortie.
 */
void saturated_transform(Image *img, int min_in, int max_in);

#endif // POINTWISE_H