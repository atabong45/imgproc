#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h> // Pour le type booléen
//  ... vos autres includes
#include "filters/pointwise.h" // Pour les transformations
#include "analysis/stats.h" // pour calculer la luminance et le contraste
#include "fft/fft.h"


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
    bool apply_prewitt;    
    bool apply_roberts;    
    int threshold_value;  
    NotchFilter notches[10]; 
    int notch_count;
    int auto_notch_radius;  
    double gamma_value;    
    
    const char *second_image_path; 
    bool apply_add;
    bool apply_sub;
    
    int resize_width;
    int resize_height;

    bool apply_invert;
    bool resize_bilinear; // Si true -> bilinéaire, sinon voisin
    int local_eq_window;  // Taille fenêtre, 0 si inactif
    
    // Logique
    bool apply_and;
    bool apply_or;
    bool apply_xor;

    int min_kernel_size; // Pour --min
    int max_kernel_size; // Pour --max

    // Dans Arguments :
    double rotation_angle;
    
    // Pour l'emphasis
    int fft_emphasis_radius;
    double fft_emphasis_low;
    double fft_emphasis_high;
    // Dans Arguments
    bool apply_laplacian;
    int hough_threshold; // Si > 0, active Hough
    bool use_otsu; // Si true, utiliser Otsu pour le seuillage
    int seed_x;
    int seed_y;
    int region_tolerance; // Si > 0, active region growing
    // Dans Arguments
    int morph_open_size;
    int morph_close_size;
    int morph_gradient_size;
    // Note : Pour erode et dilate simples, on peut utiliser --min et --max du chapitre 3
    // ou ajouter --erode / --dilate pour être plus clair. Ajoutons-les :
    int morph_erode_size;
    int morph_dilate_size;
    
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