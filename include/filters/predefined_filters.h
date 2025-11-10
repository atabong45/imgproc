#ifndef PREDEFINED_FILTERS_H
#define PREDEFINED_FILTERS_H

#include "core/image.h"

/**
 * @brief Applique un filtre de flou moyenneur à une image.
 *
 * @param src L'image source.
 * @param kernel_size La taille du noyau (ex: 3 pour un noyau 3x3, 5 pour 5x5).
 *                    Doit être un nombre impair.
 * @return Une nouvelle image floutée, ou NULL en cas d'erreur.
 */
Image *apply_box_blur(const Image *src, int kernel_size);

/**
 * @brief Applique un filtre de flou Gaussien à une image.
 *
 * @param src L'image source.
 * @param kernel_size La taille du noyau (ex: 3, 5). Doit être un nombre impair.
 * @return Une nouvelle image avec flou Gaussien, ou NULL en cas d'erreur.
 */
Image *apply_gaussian_blur(const Image *src, int kernel_size);

/**
 * @brief Applique le filtre de Sobel pour la détection de contours.
 *
 * @param src L'image source.
 * @return Une nouvelle image montrant les contours, ou NULL en cas d'erreur.
 */
Image *apply_sobel_filter(const Image *src);

/**
 * @brief Applique un filtre de rehaussement de contours (netteté).
 *
 * Ce filtre utilise un noyau Laplacien pour trouver les contours
 * et les soustrait de l'image originale pour les assombrir.
 *
 * @param src L'image source.
 * @return Une nouvelle image avec les contours rehaussés, ou NULL en cas d'erreur.
 */
Image *apply_sharpen_filter(const Image *src);

/**
 * @brief Applique un filtre médian à une image.
 *
 * Filtre non-linéaire très efficace pour supprimer le bruit de type "poivre et sel".
 * Il remplace chaque pixel par la valeur médiane de son voisinage.
 *
 * @param src L'image source (ne sera pas modifiée).
 * @param kernel_size La taille du voisinage (ex: 3 pour 3x3). Doit être un nombre impair.
 * @return Une nouvelle image filtrée, ou NULL en cas d'erreur.
 */
Image *apply_median_filter(const Image *src, int kernel_size);

#endif // PREDEFINED_FILTERS_H