#include "analysis/segmentation.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Structure simple pour un point
typedef struct { int x, y; } Point;

Image *region_growing(const Image *src, int seed_x, int seed_y, int tolerance) {
    if (!src || seed_x < 0 || seed_x >= src->width || seed_y < 0 || seed_y >= src->height) {
        fprintf(stderr, "Erreur: Germe invalide ou hors limites.\n");
        return NULL;
    }

    int w = src->width;
    int h = src->height;

    // Image de sortie (Masque binaire : 0 = fond, 255 = région)
    Image *mask = createImage(w, h, 1);
    // Initialiser à noir
    for(int i=0; i<w*h; i++) mask->data[i] = 0;

    // Tableau pour marquer les pixels visités (pour ne pas boucler)
    bool *visited = (bool*)calloc(w * h, sizeof(bool));
    
    // File d'attente pour les pixels à traiter
    // Taille max = nombre de pixels (pire cas)
    Point *queue = (Point*)malloc(w * h * sizeof(Point));
    int head = 0;
    int tail = 0;

    // Récupérer la valeur du germe
    // Note : On peut utiliser la valeur du pixel initial comme référence fixe,
    // ou la moyenne courante de la région (plus robuste mais plus complexe).
    // Ici : référence fixe au germe.
    uint8_t seed_val = src->data[seed_y * w + seed_x];

    // Ajouter le germe à la file
    queue[tail++] = (Point){seed_x, seed_y};
    visited[seed_y * w + seed_x] = true;
    mask->data[seed_y * w + seed_x] = 255;

    int region_size = 0;

    // Boucle de remplissage (Flood Fill)
    while (head < tail) {
        Point p = queue[head++];
        region_size++;

        // Vérifier les 4 voisins (Haut, Bas, Gauche, Droite)
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            // Vérifier les limites
            if (nx >= 0 && nx < w && ny >= 0 && ny < h) {
                int idx = ny * w + nx;
                
                if (!visited[idx]) {
                    // Critère d'homogénéité (Prédicat)
                    int val = src->data[idx];
                    if (abs(val - seed_val) <= tolerance) {
                        // Accepter le pixel dans la région
                        visited[idx] = true;
                        mask->data[idx] = 255;
                        queue[tail++] = (Point){nx, ny};
                    }
                }
            }
        }
    }

    printf("Région générée : %d pixels (Germe: %d,%d | Tolérance: %d)\n", region_size, seed_x, seed_y, tolerance);

    free(visited);
    free(queue);
    return mask;
}