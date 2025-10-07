#include "io/pnm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction utilitaire pour ignorer les commentaires dans l'en-tête PNM
void ignore_comments(FILE *fp) {
    int ch;
    char line[1024];
    while ((ch = fgetc(fp)) != EOF && (ch == '#' || ch == '\n' || ch == ' ' || ch == '\t' || ch == '\r')) {
        if (ch == '#') {
            if (fgets(line, sizeof(line), fp) == NULL) break;
        }
    }
    ungetc(ch, fp); // Remet le dernier caractère lu dans le flux
}

Image *loadPNM(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("loadPNM: Impossible d'ouvrir le fichier");
        return NULL;
    }

    char magic_number[3];
    if (fscanf(fp, "%2s", magic_number) != 1) {
        fprintf(stderr, "loadPNM: Erreur de lecture du nombre magique.\n");
        fclose(fp);
        return NULL;
    }

    int channels;
    if (strcmp(magic_number, "P5") == 0) {
        channels = 1; // PGM
    } else if (strcmp(magic_number, "P6") == 0) {
        channels = 3; // PPM
    } else {
        fprintf(stderr, "loadPNM: Format non supporté '%s'. Uniquement P5 (PGM) et P6 (PPM).\n", magic_number);
        fclose(fp);
        return NULL;
    }

    ignore_comments(fp);
    int width, height, max_val;
    if (fscanf(fp, "%d %d", &width, &height) != 2) {
        fprintf(stderr, "loadPNM: Impossible de lire les dimensions.\n");
        fclose(fp);
        return NULL;
    }

    ignore_comments(fp);
    if (fscanf(fp, "%d", &max_val) != 1 || max_val != 255) {
        fprintf(stderr, "loadPNM: Profondeur de couleur non supportée (doit être 255).\n");
        fclose(fp);
        return NULL;
    }
    
    // Consomme le dernier caractère de nouvelle ligne avant les données binaires
    fgetc(fp); 

    Image *img = createImage(width, height, channels);
    if (!img) {
        fclose(fp);
        return NULL;
    }

    size_t pixels_to_read = width * height * channels;
    if (fread(img->data, sizeof(uint8_t), pixels_to_read, fp) != pixels_to_read) {
        fprintf(stderr, "loadPNM: Erreur de lecture des données de pixels ou fichier corrompu.\n");
        freeImage(img);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    printf("Image '%s' chargée avec succès (%dx%d).\n", filename, width, height);
    return img;
}

int savePNM(const Image *img, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("savePNM: Impossible d'ouvrir le fichier en écriture");
        return -1;
    }

    // Choisit le format PGM ou PPM en fonction des canaux
    if (img->channels == 1) {
        fprintf(fp, "P5\n");
    } else if (img->channels == 3) {
        fprintf(fp, "P6\n");
    } else {
        fprintf(stderr, "savePNM: Nombre de canaux non supporté pour PNM: %d\n", img->channels);
        fclose(fp);
        return -1;
    }

    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "255\n");

    size_t pixels_to_write = img->width * img->height * img->channels;
    if (fwrite(img->data, sizeof(uint8_t), pixels_to_write, fp) != pixels_to_write) {
        fprintf(stderr, "savePNM: Erreur lors de l'écriture des données.\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    printf("Image sauvegardée avec succès dans '%s'.\n", filename);
    return 0;
}

