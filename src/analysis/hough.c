#include "analysis/hough.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Fonction utilitaire pour dessiner une ligne (Algo de Bresenham simplifié ou paramétrique)
void draw_line_polar(Image *img, double rho, double theta_rad) {
    int w = img->width;
    int h = img->height;
    
    // Cas spécial : Lignes presque verticales
    // theta proche de 0 ou PI (car notre theta est l'angle de la normale)
    // Mais attention aux conventions. Ici : x*cos + y*sin = rho
    double c = cos(theta_rad);
    double s = sin(theta_rad);

    // On parcourt x pour calculer y, ou l'inverse, pour éviter les trous
    if (fabs(s) > fabs(c)) {
        // Ligne plutôt horizontale
        for (int x = 0; x < w; x++) {
            int y = (int)((rho - x * c) / s);
            if (y >= 0 && y < h) {
                // Dessiner en blanc (ou rouge si couleur, mais ici PGM)
                img->data[y * w + x] = 255;
            }
        }
    } else {
        // Ligne plutôt verticale
        for (int y = 0; y < h; y++) {
            int x = (int)((rho - y * s) / c);
            if (x >= 0 && x < w) {
                img->data[y * w + x] = 255;
            }
        }
    }
}

Image *hough_transform(const Image *edge_img, Image **accumulator_view, int threshold) {
    if (!edge_img) return NULL;

    int w = edge_img->width;
    int h = edge_img->height;
    
    // 1. Définir l'espace paramétrique
    // Diagonale max pour rho
    double hough_h = sqrt(w * w + h * h);
    int rho_dim = (int)(hough_h * 2); // De -rho à +rho
    int theta_dim = 180; // 0 à 180 degrés

    // Allocation de l'accumulateur (int)
    int *acc = (int *)calloc(rho_dim * theta_dim, sizeof(int));
    if (!acc) return NULL;

    // 2. Vote (Remplissage de l'accumulateur)
    // Pour chaque pixel blanc de l'image de contours
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (edge_img->data[y * w + x] > 0) { // Si c'est un bord
                
                for (int t = 0; t < theta_dim; t++) {
                    double theta_rad = t * M_PI / 180.0;
                    double rho = x * cos(theta_rad) + y * sin(theta_rad);
                    
                    // Conversion rho réel -> index tableau
                    int rho_idx = (int)(rho + hough_h); // Décalage pour index positif
                    
                    if (rho_idx >= 0 && rho_idx < rho_dim) {
                        acc[rho_idx * theta_dim + t]++;
                    }
                }
            }
        }
    }

    // 3. (Optionnel) Créer l'image de visualisation de l'accumulateur (Hough Space)
    if (accumulator_view) {
        *accumulator_view = createImage(theta_dim, rho_dim, 1);
        int max_vote = 0;
        for (int i = 0; i < rho_dim * theta_dim; i++) {
            if (acc[i] > max_vote) max_vote = acc[i];
        }
        
        // Normalisation pour affichage
        for (int i = 0; i < rho_dim * theta_dim; i++) {
            if (max_vote > 0)
                (*accumulator_view)->data[i] = (uint8_t)((acc[i] * 255) / max_vote);
        }
    }

    // 4. Détecter les lignes (Pics) et dessiner sur l'image de sortie
    Image *output = createImage(w, h, 1);
    // On copie l'image de contour originale en fond (assombrie pour bien voir les lignes)
    for(int i=0; i<w*h; i++) output->data[i] = edge_img->data[i] / 3;

    for (int r = 0; r < rho_dim; r++) {
        for (int t = 0; t < theta_dim; t++) {
            int votes = acc[r * theta_dim + t];
            
            if (votes >= threshold) {
                // C'est une ligne valide !
                // On vérifie si c'est un pic local (optionnel mais mieux)
                // Ici on fait simple : seuil dur.
                
                double rho = r - hough_h; // Retirer le décalage
                double theta = t * M_PI / 180.0;
                
                draw_line_polar(output, rho, theta);
            }
        }
    }

    free(acc);
    printf("Transformée de Hough terminée.\n");
    return output;
}