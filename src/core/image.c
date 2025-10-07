#include "core/image.h" // On inclut notre propre header
#include <stdlib.h>     // Pour malloc() et free()
#include <stdio.h>      // Pour perror() en cas d'erreur

Image *createImage(int width, int height, int channels) {
    // 1. Allouer la mémoire pour la structure Image elle-même
    Image *img = (Image *)malloc(sizeof(Image));
    if (img == NULL) {
        perror("Erreur d'allocation pour la structure Image");
        return NULL;
    }

    // 2. Calculer la taille totale du buffer de pixels
    size_t size = width * height * channels * sizeof(uint8_t);

    // 3. Allouer la mémoire pour les données des pixels
    img->data = (uint8_t *)malloc(size);
    if (img->data == NULL) {
        perror("Erreur d'allocation pour les données de l'image");
        free(img); // Ne pas oublier de libérer la structure si le buffer échoue !
        return NULL;
    }

    // 4. Initialiser les champs de la structure
    img->width = width;
    img->height = height;
    img->channels = channels;

    return img;
}

void freeImage(Image *img) {
    if (img != NULL) {
        // On libère d'abord le buffer de données s'il existe
        if (img->data != NULL) {
            free(img->data);
        }
        // Ensuite, on libère la structure elle-même
        free(img);
    }
}