#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "core/image.h"

/**
 * @brief Redimensionne une image en utilisant l'interpolation du plus proche voisin.
 * 
 * @param src Image source.
 * @param new_width Nouvelle largeur.
 * @param new_height Nouvelle hauteur.
 * @return Nouvelle image redimensionnée, ou NULL en cas d'erreur.
 */
Image *resize_nearest_neighbor(const Image *src, int new_width, int new_height);

/**
 * @brief Redimensionne une image avec interpolation bilinéaire.
 * Donne un résultat plus lisse que le plus proche voisin.
 */
Image *resize_bilinear(const Image *src, int new_width, int new_height);

/**
 * @brief Applique une rotation à l'image autour de son centre.
 * @param src Image source.
 * @param angle_deg Angle en degrés (sens horaire).
 * @return Nouvelle image tournée (dimensions identiques, bords noirs).
 */
Image *rotate_image(const Image *src, double angle_deg);

#endif