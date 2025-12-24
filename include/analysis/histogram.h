#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "core/image.h" // On a besoin de la définition de la structure Image

/**
 * @brief Calcule l'histogramme d'une image en niveaux de gris.
 *
 * @param img L'image source.
 * @param histogram Un tableau d'entiers (de taille 256) pour stocker l'histogramme.
 *        histogram[i] contiendra le nombre de pixels de niveau de gris i.
 *        Ce tableau doit être alloué par l'appelant.
 */
void calculate_histogram(const Image *img, int *histogram);


/**
 * @brief Crée une image représentant l'histogramme.
 *
 * @param histogram Le tableau d'entiers (taille 256) contenant l'histogramme.
 * @param width La largeur souhaitée pour l'image de l'histogramme.
 * @param height La hauteur souhaitée pour l'image de l'histogramme.
 * @return Un pointeur vers une nouvelle Image PGM représentant l'histogramme,
 *         ou NULL en cas d'erreur.
 */
Image *create_histogram_image(const int *histogram, int width, int height);

/**
 * @brief Calcule le seuil optimal selon la méthode d'Otsu.
 * @param img Image en niveaux de gris.
 * @return La valeur du seuil (0-255).
 */
int calculate_otsu_threshold(const Image *img);


#endif // HISTOGRAM_H