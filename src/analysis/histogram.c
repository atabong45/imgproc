#include "analysis/histogram.h"
#include <stdio.h> // Pour printf
#include <stdlib.h> // Pour malloc
#include <string.h> // Pour memset

void calculate_histogram(const Image *img, int *histogram) {
    // 1. Initialiser l'histogramme à zéro
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }

    // 2. Parcourir les pixels de l'image
    if (img != NULL && img->data != NULL) {
        for (int i = 0; i < img->width * img->height; i++) {
            // On suppose que l'image est en niveaux de gris (channels == 1)
            if (img->channels == 1) {
                uint8_t pixel_value = img->data[i];
                histogram[pixel_value]++; // Incrémenter le compteur du niveau de gris
            }
            // Gérer les cas où les canaux ne sont pas égaux à 1 (ex : 3 pour RVB)
            else if (img->channels == 3){
                uint8_t pixel_value = img->data[i*3]; // Utiliser la composante rouge
                histogram[pixel_value]++; // Incrémenter le compteur du niveau de gris
            }

             else {
                fprintf(stderr, "calculate_histogram: Nombre de canaux non supporté : %d\n", img->channels);
                return; // Sortir si le nombre de canaux n'est pas géré.
            }
        }
        printf("Histogramme calculé avec succès.\n");
    } else {
        fprintf(stderr, "calculate_histogram: Image invalide (NULL).\n");
    }
}

Image *create_histogram_image(const int *histogram, int width, int height) {
    // 1. Trouver la valeur maximale de l'histogramme pour normaliser les barres
    int max_val = 0;
    for (int i = 0; i < 256; i++) {
        if (histogram[i] > max_val) {
            max_val = histogram[i];
        }
    }

    if (max_val == 0) {
        fprintf(stderr, "create_histogram_image: Histogramme vide, impossible de générer l'image.\n");
        return NULL;
    }

    // 2. Créer une nouvelle image pour l'histogramme (fond blanc)
    Image *hist_img = createImage(width, height, 1); // 1 canal = PGM
    if (!hist_img) {
        return NULL;
    }
    // Remplir l'image de blanc (valeur 255)
    memset(hist_img->data, 255, width * height * sizeof(uint8_t));

    // 3. Dessiner les barres de l'histogramme (en noir)
    double bar_width = (double)width / 256.0;

    for (int i = 0; i < 256; i++) {
        // Calculer la hauteur de la barre normalisée
        int bar_height = (int)(((double)histogram[i] / max_val) * (height - 1));

        // Déterminer les colonnes de l'image pour cette barre
        int start_x = (int)(i * bar_width);
        int end_x = (int)((i + 1) * bar_width);
        if (end_x > width) end_x = width;

        // Dessiner la barre verticale
        for (int x = start_x; x < end_x; x++) {
            for (int y = 0; y < bar_height; y++) {
                // L'origine (0,0) est en haut à gauche, donc on dessine depuis le bas
                // de l'image (height - 1 - y).
                int pixel_index = (height - 1 - y) * width + x;
                if (pixel_index < width * height) {
                    hist_img->data[pixel_index] = 0; // Pixel noir
                }
            }
        }
    }

    printf("Image de l'histogramme générée avec succès.\n");
    return hist_img;
}