#ifndef STATS_H
#define STATS_H

#include "core/image.h" // On a besoin de la définition de la structure Image

/**
 * @brief Calcule la luminance (moyenne) d'une image en niveaux de gris.
 *
 * @param img L'image source.
 * @return La luminance de l'image, ou -1 en cas d'erreur.
 */
double calculate_luminance(const Image *img);

/**
 * @brief Calcule le contraste d'une image en niveaux de gris.
 *
 *  Utilise la formule : contraste = (max - min) / (max + min)
 * @param img L'image source.
 * @return Le contraste de l'image, ou -1 en cas d'erreur.
 */
double calculate_contrast(const Image *img);

#endif