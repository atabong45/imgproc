#include "analysis/stats.h"
#include <stdio.h>

double calculate_luminance(const Image *img) {
    if (!img || !img->data) {
        fprintf(stderr, "calculate_luminance: Image invalide.\n");
        return -1.0;
    }

    double sum = 0.0;
    for (int i = 0; i < img->width * img->height * img->channels; i++) {
        sum += img->data[i];
    }

    return sum / (img->width * img->height * img->channels);
}

double calculate_contrast(const Image *img) {
    if (!img || !img->data) {
        fprintf(stderr, "calculate_contrast: Image invalide.\n");
        return -1.0;
    }

    uint8_t min_val = 255;
    uint8_t max_val = 0;

    for (int i = 0; i < img->width * img->height * img->channels; i++) {
         if(img->data[i] < min_val)
             min_val = img->data[i];
        if(img->data[i] > max_val)
            max_val = img->data[i];
    }

    if (max_val == min_val) {
        return 0.0; // Pour éviter la division par zéro si l'image est uniforme
    }

    return (double)(max_val - min_val) / (max_val + min_val);
}