#include "filters/convolution.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Image *apply_convolution(const Image *src, const Kernel *kernel) {
    if (!src || !kernel || !src->data || !kernel->data) {
        fprintf(stderr, "apply_convolution: Arguments invalides.\n");
        return NULL;
    }
    // Pour l'instant, on ne gère que les images en niveaux de gris
    if (src->channels != 1) {
        fprintf(stderr, "apply_convolution: Ne supporte que les images en niveaux de gris (1 canal).\n");
        return NULL;
    }

    // 1. Créer une image de destination de la même taille
    Image *dest = createImage(src->width, src->height, src->channels);
    if (!dest) {
        return NULL;
    }

    int kernel_center_x = kernel->width / 2;
    int kernel_center_y = kernel->height / 2;

    // 2. Parcourir chaque pixel de l'image source
    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            
            float sum = 0.0f;

            // 3. Appliquer le noyau sur le voisinage du pixel (x, y)
            for (int ky = 0; ky < kernel->height; ky++) {
                for (int kx = 0; kx < kernel->width; kx++) {
                    
                    // Coordonnées du pixel voisin dans l'image source
                    int ix = x + (kx - kernel_center_x);
                    int iy = y + (ky - kernel_center_y);

                    // 4. GESTION DES BORDS (stratégie : "clamp to edge")
                    // Si le voisin est en dehors de l'image, on prend le pixel le plus proche sur le bord.
                    if (ix < 0) ix = 0;
                    if (ix >= src->width) ix = src->width - 1;
                    if (iy < 0) iy = 0;
                    if (iy >= src->height) iy = src->height - 1;
                    
                    // Valeur du pixel voisin
                    uint8_t src_pixel = src->data[iy * src->width + ix];
                    // Coefficient du noyau
                    float kernel_val = kernel->data[ky * kernel->width + kx];

                    sum += src_pixel * kernel_val;
                }
            }

            // Normalisation et écrêtage (clamping)
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            
            dest->data[y * src->width + x] = (uint8_t)sum;
        }
    }

    return dest;
}

// **Explication de la Logique :**
// 1.  **Création de l'image destination :** On alloue une nouvelle image pour le résultat.
// 2.  **Double boucle sur l'image :** On visite chaque pixel `(x, y)` de l'image source.
// 3.  **Double boucle sur le noyau :** Pour chaque pixel, on applique le noyau. On superpose virtuellement le centre du noyau sur le pixel `(x, y)` et on regarde les pixels voisins.
// 4.  **Gestion des bords :** C'est la partie la plus délicate (slide 11 du cours). La stratégie `clamp to edge` (ou "réplication de bord") est simple et efficace : si le noyau dépasse de l'image, on utilise la valeur du pixel le plus proche sur le bord. C'est mieux que de mettre du noir (ce qui créerait un cadre sombre).
// 5.  **Calcul :** On fait la somme pondérée des pixels du voisinage et on stocke le résultat (après l'avoir ramené dans l'intervalle) dans l'image de destination.
