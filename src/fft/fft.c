#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "fft/fft.h"
#include <string.h>




// Ajouter cette fonction de comparaison pour qsort (si pas déjà dans ce fichier)
static int compare_doubles(const void *a, const void *b) {
    double val1 = *(const double *)a;
    double val2 = *(const double *)b;
    if (val1 < val2) return -1;
    if (val1 > val2) return 1;
    return 0;
}

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

// Fonction pour appliquer UN filtre notch (et son symétrique)
void fft_notch_filter(Complex **fft_data, int width, int height, int u, int v, int radius) {
    double radius_squared = (double)radius * radius;
    int center_x = width / 2;
    int center_y = height / 2;

    // Coordonnées du bruit (u,v) et de son symétrique (-u,-v) par rapport au centre
    int u1 = u;
    int v1 = v;
    int u2 = -u;
    int v2 = -v;

    for (int y_shifted = 0; y_shifted < height; y_shifted++) {
        for (int x_shifted = 0; x_shifted < width; x_shifted++) {
            // Coordonnées par rapport au centre du spectre
            int current_u = x_shifted - center_x;
            int current_v = y_shifted - center_y;
            
            // Calcul des distances aux deux points de bruit
            double d1_sq = pow(current_u - u1, 2) + pow(current_v - v1, 2);
            double d2_sq = pow(current_u - u2, 2) + pow(current_v - v2, 2);

            if (d1_sq <= radius_squared || d2_sq <= radius_squared) {
                // Traduire les coordonnées décalées en coordonnées de la matrice réelle
                int y_actual = (y_shifted + center_y) % height;
                int x_actual = (x_shifted + center_x) % width;
                
                fft_data[y_actual][x_actual].real = 0;
                fft_data[y_actual][x_actual].imag = 0;
            }
        }
    }
}


// Ajouter la nouvelle fonction à la fin de fft.c
int fft_auto_notch_filter(Complex **fft_data, int width, int height, double threshold_factor, int radius) {
    long total_pixels = (long)width * height;
    double *magnitudes = malloc(total_pixels * sizeof(double));
    if (!magnitudes) return 0;

    int center_x = width / 2;
    int center_y = height / 2;

    // 1. Calculer le spectre de magnitude (non logarithmique, non décalé)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            magnitudes[y * width + x] = complex_magnitude(fft_data[y][x]);
        }
    }
    
    // 2. Trouver une valeur de seuil robuste basée sur la médiane
    // La médiane est moins sensible aux valeurs extrêmes (les pics) que la moyenne.
    double *sorted_magnitudes = malloc(total_pixels * sizeof(double));
    if (!sorted_magnitudes) {
        free(magnitudes);
        return 0;
    }
    memcpy(sorted_magnitudes, magnitudes, total_pixels * sizeof(double));
    qsort(sorted_magnitudes, total_pixels, sizeof(double), compare_doubles);
    
    double median_magnitude = sorted_magnitudes[total_pixels / 2];
    double noise_threshold = median_magnitude * threshold_factor;

    free(sorted_magnitudes); // Plus besoin de la version triée

    printf("Détection auto: Médiane=%.2f, Seuil de bruit=%.2f\n", median_magnitude, noise_threshold);
    
    int detected_peaks_count = 0;
    NotchFilter detected_notches[20]; // Stocker les pics trouvés

    // 3. Détecter les pics qui dépassent le seuil
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Ignorer la zone centrale (basses fréquences)
            int dx = x < center_x ? x : width - x;
            int dy = y < center_y ? y : height - y;
            // On ignore un rayon de 5% autour du centre pour ne pas toucher à l'image
            if (sqrt(dx*dx + dy*dy) < (width * 0.05)) {
                continue;
            }

            if (magnitudes[y * width + x] > noise_threshold) {
                if (detected_peaks_count < 20) {
                    // Convertir les coordonnées de la matrice en coordonnées relatives au centre
                    int u = (x + center_x) % width - center_x;
                    int v = (y + center_y) % height - center_y;
                    
                    // Ajouter le pic à notre liste
                    detected_notches[detected_peaks_count].u = u;
                    detected_notches[detected_peaks_count].v = v;
                    detected_notches[detected_peaks_count].radius = radius;
                    detected_peaks_count++;
                }
            }
        }
    }
    
    free(magnitudes);

    // 4. Appliquer les filtres notch pour chaque pic détecté
    if (detected_peaks_count > 0) {
        printf("%d pic(s) de bruit détecté(s). Application des filtres...\n", detected_peaks_count);
        // On applique le filtre pour chaque pic. La symétrie est déjà gérée dans fft_notch_filter.
        // On ne parcourt que la moitié des pics, car chaque appel traite une paire symétrique.
        for (int i = 0; i < detected_peaks_count; i++) {
             NotchFilter n = detected_notches[i];
             printf("  - Suppression du bruit autour de (%d, %d)\n", n.u, n.v);
             fft_notch_filter(fft_data, width, height, n.u, n.v, n.radius);
        }
    }

    // On retourne le nombre de paires (un pic et son symétrique)
    return detected_peaks_count / 2;
}

void fft_emphasis_filter(Complex **fft_data, int width, int height, int radius, double k_low, double k_high) {
    int center_x = width / 2;
    int center_y = height / 2;
    double radius_squared = (double)radius * radius;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calcul distance au centre
            int dx = x < center_x ? x : width - x;
            int dy = y < center_y ? y : height - y;
            double dist_sq = (double)(dx * dx + dy * dy);

            double factor = (dist_sq <= radius_squared) ? k_low : k_high;

            // Multiplication par le facteur (Partie Réelle et Imaginaire)
            fft_data[y][x].real *= factor;
            fft_data[y][x].imag *= factor;
        }
    }
}