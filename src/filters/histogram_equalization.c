#include "filters/histogram_equalization.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Image *equalize_histogram(const Image *src) {
    // 1. Vérifications initiales robustes
    if (!src || !src->data || src->channels != 1) {
        fprintf(stderr, "equalize_histogram: Image invalide ou non supportée (doit être en niveaux de gris).\n");
        return NULL;
    }

    // Vérifier que les dimensions sont valides
    if (src->width <= 0 || src->height <= 0) {
        fprintf(stderr, "equalize_histogram: Dimensions d'image invalides.\n");
        return NULL;
    }

    // Utiliser des types appropriés pour éviter les débordements
    size_t total_pixels = (size_t)src->width * (size_t)src->height;
    
    if (total_pixels == 0) {
        fprintf(stderr, "equalize_histogram: L'image est vide.\n");
        return NULL;
    }

    // 2. Calcul de l'histogramme
    long hist[256] = {0};
    for (size_t i = 0; i < total_pixels; i++) {
        uint8_t pixel_val = src->data[i];
        hist[pixel_val]++;
    }

    // 3. Calcul de la fonction de distribution cumulative (CDF)
    long cdf[256] = {0};
    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + hist[i];
    }

    // 4. Trouver la première valeur de CDF non nulle (cdf_min)
    long cdf_min = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }

    // 5. Créer la table de correspondance (LUT)
    uint8_t lut[256];
    long denominator = (long)total_pixels - cdf_min;

    if (denominator <= 0) {
        // Image uniforme, pas d'égalisation possible
        fprintf(stderr, "Avertissement: Image uniforme, l'égalisation n'a pas d'effet.\n");
        for (int i = 0; i < 256; i++) {
            lut[i] = (uint8_t)i;
        }
    } else {
        // Formule d'égalisation d'histogramme standard
        double scale_factor = 255.0 / (double)denominator;
        for (int i = 0; i < 256; i++) {
            if (cdf[i] <= cdf_min) {
                lut[i] = 0;
            } else {
                long numerator = cdf[i] - cdf_min;
                double new_val = (double)numerator * scale_factor;
                
                // Arrondir et s'assurer que la valeur est dans [0, 255]
                int rounded = (int)(new_val + 0.5);
                if (rounded > 255) rounded = 255;
                if (rounded < 0) rounded = 0;
                
                lut[i] = (uint8_t)rounded;
            }
        }
    }

    // 6. Créer l'image de destination
    Image *dest = createImage(src->width, src->height, src->channels);
    if (!dest) {
        fprintf(stderr, "equalize_histogram: Échec de la création de l'image de destination.\n");
        return NULL;
    }

    // 7. Appliquer la LUT pixel par pixel
    for (size_t i = 0; i < total_pixels; i++) {
        uint8_t original_value = src->data[i];
        dest->data[i] = lut[original_value];
    }
    
    return dest;
}