#ifndef HISTOGRAM_EQUALIZATION_H
#define HISTOGRAM_EQUALIZATION_H

#include "core/image.h"

/**
 * @brief Applique une égalisation d'histogramme globale sur une image.
 *
 * Cette transformation redistribue les intensités des pixels pour
 * étaler l'histogramme et améliorer le contraste global.
 *
 * @param src L'image source (ne sera pas modifiée).
 * @return Une nouvelle image avec son histogramme égalisé, ou NULL en cas d'erreur.
 */
Image *equalize_histogram(const Image *src);

/**
 * @brief Applique une égalisation locale d'histogramme.
 * @param src Image source.
 * @param window_size Taille de la fenêtre locale (ex: 7, 15, 31).
 * @return Nouvelle image traitée.
 */
Image *equalize_histogram_local(const Image *src, int window_size);

#endif // HISTOGRAM_EQUALIZATION_H