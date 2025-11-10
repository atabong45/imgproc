#ifndef FFT_H
#define FFT_H

#include "core/image.h"
#include "fft/complex.h"

/**
 * @brief Calcule la Transformée de Fourier Rapide (FFT) 2D d'une image.
 *
 * L'image d'entrée est d'abord convertie en une matrice de nombres complexes
 * et potentiellement agrandie (padding) pour que ses dimensions soient des
 * puissances de 2, condition nécessaire pour l'algorithme FFT.
 *
 * @param src L'image source en niveaux de gris.
 * @param out_width Pointeur pour stocker la largeur de la matrice de sortie.
 * @param out_height Pointeur pour stocker la hauteur de la matrice de sortie.
 * @return Une matrice 2D de nombres complexes allouée dynamiquement, ou NULL en cas d'erreur.
 *         L'appelant est responsable de libérer cette mémoire avec free_fft_data().
 */
Complex **fft2d(const Image *src, int *out_width, int *out_height);

/**
 * @brief Calcule la FFT 2D Inverse pour revenir au domaine spatial.
 *
 * @param fft_data La matrice de nombres complexes.
 * @param width La largeur de la matrice.
 * @param height La hauteur de la matrice.
 * @return Une nouvelle image en niveaux de gris, ou NULL en cas d'erreur.
 */
Image *ifft2d(Complex **fft_data, int width, int height);

/**
 * @brief Libère la mémoire allouée pour une matrice de données FFT.
 *
 * @param data La matrice à libérer.
 * @param height La hauteur de la matrice.
 */
void free_fft_data(Complex **data, int height);

/**
 * @brief Crée une image visible du spectre de magnitude de la FFT.
 *
 * Le spectre est décalé pour centrer les basses fréquences et mis à l'échelle
 * de manière logarithmique pour une meilleure visualisation.
 *
 * @param fft_data La matrice de nombres complexes issue de fft2d.
 * @param width La largeur de la matrice.
 * @param height La hauteur de la matrice.
 * @return Une nouvelle image PGM représentant le spectre, ou NULL en cas d'erreur.
 */
Image *create_spectrum_image(Complex **fft_data, int width, int height);


/**
 * @brief Applique un filtre passe-bas idéal dans le domaine fréquentiel.
 *
 * Ce filtre supprime toutes les fréquences au-delà d'un certain rayon,
 * produisant un effet de flou.
 *
 * @param fft_data La matrice de nombres complexes (sera modifiée en place).
 * @param width La largeur de la matrice.
 * @param height La hauteur de la matrice.
 * @param radius Le rayon de coupure (en pixels). Les fréquences à l'extérieur
 *               de ce rayon seront mises à zéro.
 */
void fft_lowpass_filter(Complex **fft_data, int width, int height, int radius);

/**
 * @brief Applique un filtre passe-haut idéal dans le domaine fréquentiel.
 *
 * Ce filtre supprime toutes les fréquences en deçà d'un certain rayon,
 * ne conservant que les détails et les contours.
 *
 * @param fft_data La matrice de nombres complexes (sera modifiée en place).
 * @param width La largeur de la matrice.
 * @param height La hauteur de la matrice.
 * @param radius Le rayon de coupure (en pixels). Les fréquences à l'intérieur
 *               de ce rayon seront mises à zéro.
 */
void fft_highpass_filter(Complex **fft_data, int width, int height, int radius);



#endif // FFT_H