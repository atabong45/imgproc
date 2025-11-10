#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "fft/fft.h"

// --- Partie 1 : Implémentation de la FFT 1D (Algorithme de Cooley-Tukey) ---

// Fonction interne récursive pour la FFT 1D.
// Opère "en place" sur le buffer.
static void _fft1d(Complex *buffer, int n) {
    if (n <= 1) return;

    // Diviser : séparer en éléments pairs et impairs
    Complex *evens = malloc((n / 2) * sizeof(Complex));
    Complex *odds = malloc((n / 2) * sizeof(Complex));
    for (int i = 0; i < n / 2; i++) {
        evens[i] = buffer[i * 2];
        odds[i] = buffer[i * 2 + 1];
    }

    // Conquérir : appels récursifs
    _fft1d(evens, n / 2);
    _fft1d(odds, n / 2);

    // Combiner
    for (int k = 0; k < n / 2; k++) {
        double angle = -2.0 * M_PI * k / n;
        Complex t = {cos(angle), sin(angle)};
        t = complex_mul(t, odds[k]);
        
        buffer[k] = complex_add(evens[k], t);
        buffer[k + n / 2] = complex_sub(evens[k], t);
    }

    free(evens);
    free(odds);
}

// Fonction interne pour la FFT 1D Inverse.
static void _ifft1d(Complex *buffer, int n) {
    // L'IFFT est presque identique à la FFT.
    // 1. On conjugue les entrées.
    for (int i = 0; i < n; i++) {
        buffer[i].imag = -buffer[i].imag;
    }

    // 2. On applique la FFT normale.
    _fft1d(buffer, n);

    // 3. On conjugue le résultat et on le normalise par N.
    for (int i = 0; i < n; i++) {
        buffer[i].imag = -buffer[i].imag;
        buffer[i].real /= n;
        buffer[i].imag /= n;
    }
}

// --- Partie 2 : Fonctions Publiques pour la FFT 2D ---

// Helper pour trouver la prochaine puissance de 2
static int next_power_of_2(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

Complex **fft2d(const Image *src, int *out_width, int *out_height) {
    if (!src || !src->data || src->channels != 1) {
        fprintf(stderr, "fft2d: Image invalide ou non supportée.\n");
        return NULL;
    }

    // Pré-traitement : padding pour atteindre des dimensions en puissance de 2
    int width = next_power_of_2(src->width);
    int height = next_power_of_2(src->height);
    *out_width = width;
    *out_height = height;

    // Allocation de la matrice 2D de nombres complexes
    Complex **data = malloc(height * sizeof(Complex *));
    for (int i = 0; i < height; i++) {
        data[i] = calloc(width, sizeof(Complex)); // calloc initialise à 0
    }

    // Copier les données de l'image dans la partie réelle de la matrice
    for (int y = 0; y < src->height; y++) {
        for (int x = 0; x < src->width; x++) {
            data[y][x].real = src->data[y * src->width + x];
        }
    }

    // FFT sur les lignes
    for (int y = 0; y < height; y++) {
        _fft1d(data[y], width);
    }
    
    // Transposition et FFT sur les colonnes (plus efficace pour le cache)
    // 1. Allouer un buffer temporaire pour une colonne
    Complex *column = malloc(height * sizeof(Complex));
    // 2. Appliquer la FFT colonne par colonne
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            column[y] = data[y][x];
        }
        _fft1d(column, height);
        for (int y = 0; y < height; y++) {
            data[y][x] = column[y];
        }
    }
    free(column);

    return data;
}

Image *ifft2d(Complex **fft_data, int width, int height) {
    // IFFT sur les lignes
    for (int y = 0; y < height; y++) {
        _ifft1d(fft_data[y], width);
    }
    
    // Transposition et IFFT sur les colonnes
    Complex *column = malloc(height * sizeof(Complex));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            column[y] = fft_data[y][x];
        }
        _ifft1d(column, height);
        for (int y = 0; y < height; y++) {
            fft_data[y][x] = column[y];
        }
    }
    free(column);

    // Créer l'image de sortie et copier la partie réelle
    Image *dest = createImage(width, height, 1);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double val = fft_data[y][x].real;
            if (val < 0) val = 0;
            if (val > 255) val = 255;
            dest->data[y * width + x] = (uint8_t)val;
        }
    }
    return dest;
}

void free_fft_data(Complex **data, int height) {
    if (data) {
        for (int i = 0; i < height; i++) {
            free(data[i]);
        }
        free(data);
    }
}


Image *create_spectrum_image(Complex **fft_data, int width, int height) {
    // 1. Allouer une matrice temporaire pour la magnitude et une image de sortie
    double *magnitude_spectrum = calloc(width * height, sizeof(double));
    Image *spectrum_image = createImage(width, height, 1);
    if (!magnitude_spectrum || !spectrum_image) {
        free(magnitude_spectrum);
        freeImage(spectrum_image);
        return NULL;
    }

    double max_magnitude = 0.0;

    // 2. Calculer la magnitude et la décaler (shift) pour centrer les basses fréquences
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculer la magnitude : |F(u,v)|
            double mag = complex_magnitude(fft_data[y][x]);

            // Mettre à l'échelle logarithmique : log(1 + |F(u,v)|)
            // Le "+1" évite log(0) qui est indéfini.
            mag = log(1.0 + mag);

            // Décalage des quadrants (FFT shift)
            // (y + height/2) % height et (x + width/2) % width
            // Cela déplace le coin supérieur gauche au centre.
            int shifted_y = (y + height / 2) % height;
            int shifted_x = (x + width / 2) % width;
            
            magnitude_spectrum[shifted_y * width + shifted_x] = mag;

            // Garder la trace du maximum pour la normalisation
            if (mag > max_magnitude) {
                max_magnitude = mag;
            }
        }
    }

    // 3. Normaliser les magnitudes entre 0 et 255
    if (max_magnitude > 0) {
        for (int i = 0; i < width * height; i++) {
            spectrum_image->data[i] = (uint8_t)((magnitude_spectrum[i] / max_magnitude) * 255.0);
        }
    }
    
    free(magnitude_spectrum);
    
    return spectrum_image;
}


void fft_lowpass_filter(Complex **fft_data, int width, int height, int radius) {
    int center_x = width / 2;
    int center_y = height / 2;
    double radius_squared = (double)radius * radius;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculer la distance par rapport au centre du spectre NON décalé
            // Le point (0,0) est le coin. On doit gérer les 4 quadrants.
            int dx = x < center_x ? x : width - x;
            int dy = y < center_y ? y : height - y;

            if ((double)(dx * dx + dy * dy) > radius_squared) {
                // Si le point est en dehors du cercle, on le met à zéro.
                fft_data[y][x].real = 0;
                fft_data[y][x].imag = 0;
            }
        }
    }
}

void fft_highpass_filter(Complex **fft_data, int width, int height, int radius) {
    int center_x = width / 2;
    int center_y = height / 2;
    double radius_squared = (double)radius * radius;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculer la distance par rapport au centre du spectre NON décalé
            int dx = x < center_x ? x : width - x;
            int dy = y < center_y ? y : height - y;

            if ((double)(dx * dx + dy * dy) <= radius_squared) {
                // Si le point est à l'intérieur du cercle, on le met à zéro.
                fft_data[y][x].real = 0;
                fft_data[y][x].imag = 0;
            }
        }
    }
}