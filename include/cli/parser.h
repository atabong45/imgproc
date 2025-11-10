#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h> // Pour le type booléen
//  ... vos autres includes
#include "filters/pointwise.h" // Pour les transformations
#include "analysis/stats.h" // pour calculer la luminance et le contraste

// Déclaration de la structure qui contiendra tous les arguments parsés.
// Elle est ici pour que main.c sache à quoi ressemble le type "Arguments".
typedef struct {
    const char *input_path;
    const char *output_path;
    bool show_histogram;
    bool show_luminance;
    bool show_contrast;
    double linear_gain;
    double linear_bias;
    int saturated_min;
    int saturated_max;
    const char *histogram_output_path;
    int blur_kernel_size;
    int gaussian_blur_kernel_size;
    bool apply_sobel;
    bool apply_sharpen;
    bool apply_equalization;
    int median_kernel_size;
    bool test_fft;
    const char *fft_spectrum_path;
    int fft_lowpass_radius;  
    int fft_highpass_radius;
} Arguments;

/**
 * @brief Analyse les arguments de la ligne de commande.
 *
 * @param argc Le nombre d'arguments (venant de main).
 * @param argv Le tableau d'arguments (venant de main).
 * @return Une structure Arguments remplie avec les valeurs parsées.
 *         Le programme se termine avec une erreur si les arguments obligatoires
 *         sont manquants.
 */
Arguments parse_args(int argc, char *argv[]);





#endif // PARSER_H