#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include "core/image.h"

/**
 * @brief Applique une croissance de région à partir d'un germe.
 * 
 * @param src Image source (niveaux de gris).
 * @param seed_x Coordonnée X du germe.
 * @param seed_y Coordonnée Y du germe.
 * @param tolerance Tolérance de différence d'intensité pour inclure un voisin.
 * @return Une image binaire (masque) de la région segmentée.
 */
Image *region_growing(const Image *src, int seed_x, int seed_y, int tolerance);

#endif