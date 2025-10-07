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

#endif // HISTOGRAM_EQUALIZATION_H