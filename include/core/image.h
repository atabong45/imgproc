#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h> // Pour utiliser des types d'entiers explicites comme uint8_t

/**
 * @struct Image
 * @brief Représente une image en mémoire.
 *
 * L'image est stockée comme un tableau 1D de pixels (data).
 * Le nombre de canaux (channels) détermine la nature de l'image :
 *  - 1: Niveaux de gris
 *  - 3: Couleur (RVB)
 */
typedef struct {
    int width;      // Largeur de l'image en pixels
    int height;     // Hauteur de l'image en pixels
    int channels;   // Nombre de canaux par pixel (1 pour gris, 3 pour RVB)
    uint8_t *data;  // Pointeur vers le buffer contenant les données des pixels
} Image;

/**
 * @brief Alloue de la mémoire pour une nouvelle image.
 *
 * @param width La largeur de l'image.
 * @param height La hauteur de l'image.
 * @param channels Le nombre de canaux par pixel.
 * @return Un pointeur vers la nouvelle structure Image, ou NULL en cas d'échec d'allocation.
 */
Image *createImage(int width, int height, int channels);

/**
 * @brief Libère la mémoire allouée pour une image.
 *
 * @param img Pointeur vers l'image à libérer.
 */
void freeImage(Image *img);

#endif // IMAGE_H