#include "filters/morphology.h"
#include "filters/predefined_filters.h" // Pour Min et Max
#include "filters/arithmetic.h"       // Pour Sub (Gradient)
#include <stdlib.h>
#include <stdio.h>

// Wrappers simples pour la sémantique
Image *morph_erode(const Image *src, int kernel_size) {
    return apply_min_filter(src, kernel_size);
}

Image *morph_dilate(const Image *src, int kernel_size) {
    return apply_max_filter(src, kernel_size);
}

Image *morph_open(const Image *src, int kernel_size) {
    // 1. Érosion
    Image *eroded = morph_erode(src, kernel_size);
    if (!eroded) return NULL;

    // 2. Dilatation du résultat
    Image *opened = morph_dilate(eroded, kernel_size);
    
    freeImage(eroded); // Nettoyage intermédiaire
    return opened;
}

Image *morph_close(const Image *src, int kernel_size) {
    // 1. Dilatation
    Image *dilated = morph_dilate(src, kernel_size);
    if (!dilated) return NULL;

    // 2. Érosion du résultat
    Image *closed = morph_erode(dilated, kernel_size);
    
    freeImage(dilated); // Nettoyage intermédiaire
    return closed;
}

Image *morph_gradient(const Image *src, int kernel_size) {
    // Gradient = Dilaté - Érodé
    Image *dilated = morph_dilate(src, kernel_size);
    Image *eroded = morph_erode(src, kernel_size);

    if (!dilated || !eroded) {
        if(dilated) freeImage(dilated);
        if(eroded) freeImage(eroded);
        return NULL;
    }

    // On utilise la fonction arithmétique existante
    // Attention : apply_arithmetic modifie la première image (dilated ici)
    apply_arithmetic(dilated, eroded, OP_SUB);
    
    freeImage(eroded);
    return dilated; // 'dilated' contient maintenant le résultat de la soustraction
}