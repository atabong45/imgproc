#include "geometry/transform.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>  

Image *resize_nearest_neighbor(const Image *src, int new_width, int new_height) {
    if (!src || new_width <= 0 || new_height <= 0) return NULL;

    Image *dest = createImage(new_width, new_height, src->channels);
    if (!dest) return NULL;

    // Ratios d'échelle
    double x_ratio = (double)src->width / new_width;
    double y_ratio = (double)src->height / new_height;

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            // Trouver le pixel correspondant dans l'image source (Plus proche voisin)
            int src_x = (int)(x * x_ratio);
            int src_y = (int)(y * y_ratio);

            // Gestion simple des indices
            if (src_x >= src->width) src_x = src->width - 1;
            if (src_y >= src->height) src_y = src->height - 1;

            // Copie des canaux (Gris ou RVB)
            for (int c = 0; c < src->channels; c++) {
                int dest_index = (y * new_width + x) * src->channels + c;
                int src_index = (src_y * src->width + src_x) * src->channels + c;
                dest->data[dest_index] = src->data[src_index];
            }
        }
    }
    
    printf("Redimensionnement (Voisin) : %dx%d -> %dx%d\n", src->width, src->height, new_width, new_height);
    return dest;
}

Image *resize_bilinear(const Image *src, int new_width, int new_height) {
    if (!src || new_width <= 0 || new_height <= 0) return NULL;

    Image *dest = createImage(new_width, new_height, src->channels);
    if (!dest) return NULL;

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            
            // 1. Calculer la position correspondante dans l'image source (flottante)
            // Le -0.5 permet de centrer les pixels pour un redimensionnement plus précis
            float src_x = (x + 0.5f) * ((float)src->width / new_width) - 0.5f;
            float src_y = (y + 0.5f) * ((float)src->height / new_height) - 0.5f;

            // 2. Trouver les coordonnées du pixel en haut à gauche (x1, y1)
            int x1 = (int)floor(src_x);
            int y1 = (int)floor(src_y);
            int x2 = x1 + 1;
            int y2 = y1 + 1;

            // 3. Calculer les poids (partie fractionnaire)
            float dx = src_x - x1;
            float dy = src_y - y1;
            float one_minus_dx = 1.0f - dx;
            float one_minus_dy = 1.0f - dy;

            // 4. Gérer les bords (Clamp)
            if (x1 < 0) x1 = 0; 
            if (y1 < 0) y1 = 0;
            if (x2 >= src->width) x2 = src->width - 1;
            if (y2 >= src->height) y2 = src->height - 1;

            // 5. Calculer pour chaque canal
            for (int c = 0; c < src->channels; c++) {
                // Récupérer les valeurs des 4 voisins
                uint8_t p11 = src->data[(y1 * src->width + x1) * src->channels + c]; // Haut-Gauche
                uint8_t p12 = src->data[(y1 * src->width + x2) * src->channels + c]; // Haut-Droite
                uint8_t p21 = src->data[(y2 * src->width + x1) * src->channels + c]; // Bas-Gauche
                uint8_t p22 = src->data[(y2 * src->width + x2) * src->channels + c]; // Bas-Droite

                // Formule Bilinéaire : somme pondérée
                float val = 
                    p11 * one_minus_dx * one_minus_dy +
                    p12 * dx * one_minus_dy +
                    p21 * one_minus_dx * dy +
                    p22 * dx * dy;

                dest->data[(y * new_width + x) * src->channels + c] = (uint8_t)val;
            }
        }
    }
    printf("Redimensionnement (Bilinéaire) : %dx%d -> %dx%d\n", src->width, src->height, new_width, new_height);
    return dest;
}

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Image *rotate_image(const Image *src, double angle_deg) {
    if (!src) return NULL;
    
    Image *dest = createImage(src->width, src->height, src->channels);
    if (!dest) return NULL;

    // Convertir en radians
    double theta = angle_deg * M_PI / 180.0;
    double cos_t = cos(theta);
    double sin_t = sin(theta);

    int cx = src->width / 2;
    int cy = src->height / 2;

    // Pour éviter les trous, on parcourt l'image de DESTINATION (Inverse Mapping)
    for (int y = 0; y < dest->height; y++) {
        for (int x = 0; x < dest->width; x++) {
            
            // Coordonnées par rapport au centre
            int dx = x - cx;
            int dy = y - cy;

            // Rotation inverse
            // src_x = x' cos(t) + y' sin(t)
            // src_y = -x' sin(t) + y' cos(t)
            int src_x = (int)(dx * cos_t + dy * sin_t) + cx;
            int src_y = (int)(-dx * sin_t + dy * cos_t) + cy;

            // Vérifier si on est dans l'image source
            if (src_x >= 0 && src_x < src->width && src_y >= 0 && src_y < src->height) {
                for (int c = 0; c < src->channels; c++) {
                    dest->data[(y * dest->width + x) * src->channels + c] = 
                        src->data[(src_y * src->width + src_x) * src->channels + c];
                }
            } else {
                // Fond noir
                for (int c = 0; c < src->channels; c++) {
                    dest->data[(y * dest->width + x) * src->channels + c] = 0;
                }
            }
        }
    }
    printf("Rotation de %.2f degrés appliquée.\n", angle_deg);
    return dest;
}