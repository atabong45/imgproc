#include "filters/pointwise.h"
#include <stdio.h>
#include <math.h> 


void linear_transform(Image *img, double a, double b) {
    if (!img || !img->data) {
        fprintf(stderr, "linear_transform: Image invalide.\n");
        return;
    }

    for (int i = 0; i < img->width * img->height * img->channels; i++) {
        double pixel = img->data[i];
        pixel = a * pixel + b;

        // Écrêtage (clamping)
        if (pixel < 0) {
            pixel = 0;
        } else if (pixel > 255) {
            pixel = 255;
        }
        img->data[i] = (uint8_t)pixel;
    }
    printf("Transformation linéaire appliquée avec succès.\n");
}

void saturated_transform(Image *img, int min_in, int max_in) {
    if (!img || !img->data) {
        fprintf(stderr, "saturated_transform: Image invalide.\n");
        return;
    }

    if (min_in >= max_in) {
        fprintf(stderr, "saturated_transform: min_in doit être inférieur à max_in.\n");
        return;
    }

    for (int i = 0; i < img->width * img->height * img->channels; i++) {
        int pixel = img->data[i];

        if (pixel <= min_in) {
            pixel = 0;
        } else if (pixel >= max_in) {
            pixel = 255;
        } else {
            // Transformation linéaire entre min_in et max_in
            pixel = 255.0 * (pixel - min_in) / (max_in - min_in);
        }
        img->data[i] = (uint8_t)pixel;
    }
    printf("Transformation linéaire avec saturation appliquée avec succès.\n");
}



void apply_threshold(Image *img, uint8_t threshold) {
    if (!img || !img->data) {
        fprintf(stderr, "apply_threshold: Image invalide.\n");
        return;
    }
    
    // Pour l'instant, on ne gère que les niveaux de gris
    if (img->channels != 1) {
        fprintf(stderr, "apply_threshold: Ne supporte que les images en niveaux de gris.\n");
        return;
    }

    long total_pixels = (long)img->width * img->height;
    for (long i = 0; i < total_pixels; i++) {
        img->data[i] = (img->data[i] < threshold) ? 0 : 255;
    }
    
    printf("Seuillage appliqué avec le seuil %d.\n", threshold);
}


void apply_gamma_correction(Image *img, double gamma) {
    if (!img || !img->data) return;

    // Création d'une LUT (Look Up Table) pour accélérer le traitement
    // Au lieu de faire pow() pour chaque pixel, on le fait 256 fois.
    uint8_t lut[256];
    for (int i = 0; i < 256; i++) {
        double normalized = i / 255.0;
        double corrected = pow(normalized, gamma) * 255.0;
        
        // Clamp
        if (corrected < 0) corrected = 0;
        if (corrected > 255) corrected = 255;
        
        lut[i] = (uint8_t)corrected;
    }

    // Application de la LUT
    long total_pixels = img->width * img->height * img->channels;
    for (long i = 0; i < total_pixels; i++) {
        img->data[i] = lut[img->data[i]];
    }
    printf("Correction Gamma (%.2f) appliquée.\n", gamma);
}

void apply_invert(Image *img) {
    if (!img || !img->data) return;
    long total = img->width * img->height * img->channels;
    for (long i = 0; i < total; i++) {
        img->data[i] = 255 - img->data[i];
    }
    printf("Inversion (Négatif) appliquée.\n");
}