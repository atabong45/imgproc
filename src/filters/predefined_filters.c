#include "filters/predefined_filters.h"
#include "filters/convolution.h" // On a besoin de la structure Kernel et de apply_convolution
#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include <string.h> 


Image *apply_box_blur(const Image *src, int kernel_size) {
    if (kernel_size % 2 == 0) {
        fprintf(stderr, "apply_box_blur: La taille du noyau doit être impaire.\n");
        return NULL;
    }

    // 1. Créer le noyau de convolution pour le flou moyenneur
    Kernel kernel;
    kernel.width = kernel_size;
    kernel.height = kernel_size;
    
    int num_coeffs = kernel_size * kernel_size;
    kernel.data = (float*)malloc(num_coeffs * sizeof(float));
    if (!kernel.data) {
        perror("apply_box_blur: Impossible d'allouer la mémoire pour le noyau");
        return NULL;
    }

    // Le filtre moyenneur a des coefficients égaux à 1 / (nombre total de coeffs)
    // pour que l'image ne devienne ni plus claire, ni plus sombre.
    float coeff = 1.0f / num_coeffs;
    for (int i = 0; i < num_coeffs; i++) {
        kernel.data[i] = coeff;
    }

    // 2. Appeler le moteur de convolution générique avec le noyau que nous venons de créer
    Image *result = apply_convolution(src, &kernel);

    // 3. Libérer la mémoire allouée pour le noyau
    free(kernel.data);

    return result;
}



Image *apply_gaussian_blur(const Image *src, int kernel_size) {
    if (kernel_size % 2 == 0) {
        fprintf(stderr, "apply_gaussian_blur: La taille du noyau doit être impaire.\n");
        return NULL;
    }

    // 1. Créer le noyau Gaussien
    Kernel kernel;
    kernel.width = kernel_size;
    kernel.height = kernel_size;
    
    int num_coeffs = kernel_size * kernel_size;
    kernel.data = (float*)malloc(num_coeffs * sizeof(float));
    if (!kernel.data) {
        perror("apply_gaussian_blur: Impossible d'allouer la mémoire pour le noyau");
        return NULL;
    }

    float sum = 0.0f;
    int center = kernel_size / 2;
    float sigma = (float)kernel_size / 6.0; // Approximation raisonnable pour sigma

    // Générer les coefficients du noyau Gaussien
    for (int y = 0; y < kernel_size; y++) {
        for (int x = 0; x < kernel_size; x++) {
            int dx = x - center;
            int dy = y - center;
            float val = exp(-((dx * dx + dy * dy) / (2 * sigma * sigma))) / (2 * M_PI * sigma * sigma);
            kernel.data[y * kernel_size + x] = val;
            sum += val;
        }
    }
    
    // Normaliser le noyau (s'assurer que la somme des coefficients est 1)
    for (int i = 0; i < num_coeffs; i++) {
        kernel.data[i] /= sum;
    }

    // 2. Appeler le moteur de convolution
    Image *result = apply_convolution(src, &kernel);

    // 3. Libérer la mémoire
    free(kernel.data);

    return result;
}



Image *apply_sobel_filter(const Image *src) {
    // Noyau de Sobel pour les gradients horizontaux (détecte les contours verticaux)
    float kx_data[] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    Kernel kernel_x = {3, 3, kx_data};

    // Noyau de Sobel pour les gradients verticaux (détecte les contours horizontaux)
    float ky_data[] = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    };
    Kernel kernel_y = {3, 3, ky_data};

    // 1. Appliquer les deux convolutions
    Image *grad_x = apply_convolution(src, &kernel_x);
    Image *grad_y = apply_convolution(src, &kernel_y);

    if (!grad_x || !grad_y) {
        freeImage(grad_x); // Libère au cas où l'un des deux a réussi
        freeImage(grad_y);
        return NULL;
    }

    // 2. Créer une image pour le résultat final
    Image *result = createImage(src->width, src->height, src->channels);
    if (!result) {
        freeImage(grad_x);
        freeImage(grad_y);
        return NULL;
    }

    // 3. Combiner les deux gradients (magnitude)
    for (int i = 0; i < src->width * src->height * src->channels; i++) {
        // La convolution peut donner des résultats "négatifs", mais ils sont stockés 
        // comme des entiers. Le calcul correct nécessite une conversion.
        // Ici, on fait une approximation simple avec la valeur absolue.
        // Pour une implémentation parfaite, apply_convolution devrait retourner
        // des float, mais restons simples pour l'instant.
        float gx = grad_x->data[i];
        float gy = grad_y->data[i];
        
        // Calcul de la magnitude : sqrt(gx^2 + gy^2)
        float magnitude = sqrt(gx * gx + gy * gy);

        if (magnitude > 255) magnitude = 255;
        result->data[i] = (uint8_t)magnitude;
    }

    // 4. Libérer les images intermédiaires
    freeImage(grad_x);
    freeImage(grad_y);

    return result;
}


// ... à la fin de src/filters/predefined_filters.c ...

Image *apply_sharpen_filter(const Image *src) {
    // 1. Définir le noyau de convolution Laplacien.
    // Ce noyau est très bon pour détecter les contours dans toutes les directions.
    // La somme des coefficients est 0, ce qui signifie que les zones plates
    // de l'image donneront un résultat proche de 0.
    float laplacian_data[] = {
         0, -1,  0,
        -1,  4, -1,
         0, -1,  0
    };
    Kernel laplacian_kernel = {3, 3, laplacian_data};

    // 2. Appliquer la convolution pour obtenir l'image des contours.
    // Cette image sera principalement noire (0) avec les contours en blanc.
    Image *edges = apply_convolution(src, &laplacian_kernel);
    if (!edges) {
        fprintf(stderr, "apply_sharpen_filter: Échec de l'extraction des contours.\n");
        return NULL;
    }

    // 3. Créer une copie de l'image originale pour y soustraire les contours.
    // On ne peut pas modifier l'image source directement.
    Image *result = createImage(src->width, src->height, src->channels);
    if (!result) {
        freeImage(edges);
        return NULL;
    }
    memcpy(result->data, src->data, src->width * src->height * src->channels);

    // 4. Soustraire les contours de l'image copiée.
    for (int i = 0; i < src->width * src->height * src->channels; i++) {
        // La valeur du pixel dans 'result' (l'original)
        int original_pixel = result->data[i];
        
        // La valeur du contour (plus c'est blanc, plus le contour est fort)
        int edge_pixel = edges->data[i];

        // On soustrait. Le résultat est un entier qui peut être négatif.
        int final_pixel = original_pixel - edge_pixel;
        
        // Écrêtage (clamping) pour s'assurer que la valeur reste entre 0 et 255.
        if (final_pixel < 0) final_pixel = 0;
        if (final_pixel > 255) final_pixel = 255;
        
        result->data[i] = (uint8_t)final_pixel;
    }

    // 5. Libérer la mémoire de l'image intermédiaire des contours.
    freeImage(edges);

    return result;
}