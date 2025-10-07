#include "filters/pointwise.h"
#include <stdio.h>

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