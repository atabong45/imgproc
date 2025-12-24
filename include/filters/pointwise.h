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


/**
 * @brief Applique un seuillage binaire à une image.
 *
 * Tous les pixels avec une valeur inférieure au seuil sont mis à 0 (noir),
 * les autres sont mis à 255 (blanc). L'opération est faite en place.
 *
 * @param img L'image à modifier.
 * @param threshold La valeur de seuil (entre 0 et 255).
 */
void apply_threshold(Image *img, uint8_t threshold);

/**
 * @brief Applique une correction gamma à l'image.
 * I' = 255 * (I / 255)^gamma
 * @param img L'image à modifier.
 * @param gamma Le facteur gamma (ex: 0.5 pour éclaircir, 2.2 pour assombrir/corriger écran).
 */
void apply_gamma_correction(Image *img, double gamma);

// Inverse les couleurs de l'image (Négatif).
void apply_invert(Image *img);

#endif // POINTWISE_H