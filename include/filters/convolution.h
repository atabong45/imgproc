#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "core/image.h"

/**
 * @struct Kernel
 * @brief Représente un noyau (ou masque) de convolution.
 *
 * Le noyau est une petite matrice (ex: 3x3, 5x5) qui est appliquée à chaque pixel de l'image.
 * La somme des coefficients du noyau est stockée dans 'factor' pour la normalisation.
 */
typedef struct {
    int width;      // Largeur du noyau (généralement un nombre impair)
    int height;     // Hauteur du noyau (généralement un nombre impair)
    float *data;    // Pointeur vers les coefficients du noyau (tableau 1D)
} Kernel;

/**
 * @brief Applique une convolution sur une image.
 *
 * Crée une nouvelle image pour stocker le résultat de la convolution.
 *
 * @param src L'image source (ne sera pas modifiée).
 * @param kernel Le noyau de convolution à appliquer.
 * @return Un pointeur vers une nouvelle Image contenant le résultat,
 *         ou NULL en cas d'erreur.
 */
Image *apply_convolution(const Image *src, const Kernel *kernel);

#endif // CONVOLUTION_H