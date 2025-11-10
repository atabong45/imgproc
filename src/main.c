#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Pour exit

#include "io/pnm.h"
#include "analysis/histogram.h"
#include "analysis/stats.h"
#include "filters/pointwise.h"
#include "filters/predefined_filters.h"
#include "filters/histogram_equalization.h"
#include "cli/parser.h"
#include "fft/fft.h"

int main(int argc, char *argv[]) {

    Arguments args = parse_args(argc, argv); // Utiliser la fonction parse_args mise à jour

    // 1. Charger l'image d'entrée
    Image *img = loadPNM(args.input_path);
    if (!img) {
        fprintf(stderr, "Impossible de charger l'image '%s'.\n", args.input_path);
        return 1;
    }

    // 2. Analyse de l'image (si demandé)
    if (args.show_histogram) {
        int histogram[256];
        calculate_histogram(img, histogram);

        // Créer l'image de l'histogramme
        Image *hist_img = create_histogram_image(histogram, 512, 256); // Dimensions 512x256
        if (hist_img) {
            // Sauvegarder l'image de l'histogramme
            if (savePNM(hist_img, args.histogram_output_path) != 0) {
                fprintf(stderr, "Erreur lors de la sauvegarde de l'histogramme.\n");
            }
            freeImage(hist_img); // Ne pas oublier de la libérer !
        }
    }
    if (args.show_luminance) {
        double luminance = calculate_luminance(img);
        if (luminance >= 0) {
            printf("Luminance: %f\n", luminance);
        }
    }
    if (args.show_contrast) {
        double contrast = calculate_contrast(img);
        if (contrast >= 0) {
            printf("Contraste: %f\n", contrast);
        }
    }
    // ------------------ Transformations ------------------
    if (img) {
        if (args.linear_gain != 1.0 || args.linear_bias != 0.0) {
            linear_transform(img, args.linear_gain, args.linear_bias);
        }
        if (args.saturated_min != -1 && args.saturated_max != -1) {
            saturated_transform(img, args.saturated_min, args.saturated_max);
        }
    }
    // -------------------------------------------------------

    // --- Section des transformations par convolution ---
    if (args.blur_kernel_size > 0) {
        printf("Application d'un flou %dx%d...\n", args.blur_kernel_size, args.blur_kernel_size);
        Image *blurred_img = apply_box_blur(img, args.blur_kernel_size);
        if (blurred_img) {
            // Le résultat est dans blurred_img. L'ancienne image 'img' n'est plus utile.
            freeImage(img);       // On libère l'ancienne image
            img = blurred_img;    // Et on pointe 'img' vers la nouvelle image floutée
        }
    }

    if (args.gaussian_blur_kernel_size > 0) {
        printf("Application d'un flou Gaussien %dx%d...\n", args.gaussian_blur_kernel_size, args.gaussian_blur_kernel_size);
        Image *blurred_img = apply_gaussian_blur(img, args.gaussian_blur_kernel_size);
        if (blurred_img) {
            freeImage(img);
            img = blurred_img;
        }
    }

    if (args.apply_sobel) {
        printf("Application du filtre de Sobel...\n");
        Image *sobel_img = apply_sobel_filter(img);
        if (sobel_img) {
            freeImage(img);
            img = sobel_img;
        }
    }

    if (args.apply_sharpen) {
        printf("Application du filtre de netteté...\n");
        Image *sharpened_img = apply_sharpen_filter(img);
        if (sharpened_img) {
            freeImage(img);       // Libère l'ancienne image
            img = sharpened_img;  // Pointe vers la nouvelle
        }
    }
    if (args.apply_equalization) {
        printf("Application de l'égalisation d'histogramme...\n");
        Image *equalized_img = equalize_histogram(img);
        if (equalized_img) {
            freeImage(img);       // Libère l'ancienne image
            img = equalized_img;  // Pointe vers la nouvelle
        }
    }
    if (args.median_kernel_size > 0) {
        printf("Application du filtre médian %dx%d...\n", args.median_kernel_size, args.median_kernel_size);
        Image *median_img = apply_median_filter(img, args.median_kernel_size);
        if (median_img) {
            freeImage(img);       // Libère l'ancienne image
            img = median_img;     // Pointe vers la nouvelle
        }
    }
    if (args.test_fft) {
        printf("Test du module FFT...\n");
        int fft_w, fft_h;
        Complex **fft_result = fft2d(img, &fft_w, &fft_h);
        if (fft_result) {
            printf("FFT calculée avec succès (dimensions : %dx%d).\n", fft_w, fft_h);
            
            // Test de l'inverse
            Image *inversed_img = ifft2d(fft_result, fft_w, fft_h);
            if(inversed_img) {
                printf("FFT Inverse calculée avec succès.\n");
                // Sauvegarder pour vérifier si l'image est revenue à l'original (à peu près)
                savePNM(inversed_img, "test_ifft.pgm");
                freeImage(inversed_img);
            }

            free_fft_data(fft_result, fft_h);
        } else {
            fprintf(stderr, "Le calcul de la FFT a échoué.\n");
        }
    }
    if (args.fft_spectrum_path) {
        printf("Calcul du spectre de Fourier...\n");
        int fft_w, fft_h;
        Complex **fft_result = fft2d(img, &fft_w, &fft_h);
        if (fft_result) {
            Image *spectrum = create_spectrum_image(fft_result, fft_w, fft_h);
            if (spectrum) {
                if (savePNM(spectrum, args.fft_spectrum_path) == 0) {
                    printf("Spectre de Fourier sauvegardé dans '%s'.\n", args.fft_spectrum_path);
                }
                freeImage(spectrum);
            }
            free_fft_data(fft_result, fft_h);
        }
    }

    // --- Pipeline de Filtrage Fréquentiel ---
    if (args.fft_lowpass_radius > 0 || args.fft_highpass_radius > 0) {
    printf("Début du filtrage fréquentiel...\n");
    
    // Étape 1: Appliquer la FFT
    int fft_w, fft_h;
    Complex **fft_result = fft2d(img, &fft_w, &fft_h);
    
    if (fft_result) {
        // Étape 2: Appliquer les filtres demandés
        if (args.fft_lowpass_radius > 0) {
            printf("Application du filtre passe-bas fréquentiel (rayon=%d)...\n", args.fft_lowpass_radius);
            fft_lowpass_filter(fft_result, fft_w, fft_h, args.fft_lowpass_radius);
        }
        if (args.fft_highpass_radius > 0) {
            printf("Application du filtre passe-haut fréquentiel (rayon=%d)...\n", args.fft_highpass_radius);
            fft_highpass_filter(fft_result, fft_w, fft_h, args.fft_highpass_radius);
        }

        // Étape 3: Appliquer la FFT Inverse pour revenir au domaine spatial
        Image *filtered_img = ifft2d(fft_result, fft_w, fft_h);
        if (filtered_img) {
            // Remplacer l'image originale par le résultat du filtrage
            freeImage(img);
            img = filtered_img;
        }

        // Nettoyer la mémoire des données FFT
        free_fft_data(fft_result, fft_h);
    }
}



    // -------------------------------------------------------


    // 3. Sauvegarder l'image dans le fichier de sortie
    if (savePNM(img, args.output_path) != 0) {
        fprintf(stderr, "Impossible de sauvegarder l'image dans '%s'.\n", args.output_path);
        freeImage(img);
        return 1;
    }

    // 4. Libérer la mémoire
    freeImage(img);

    printf("Opération terminée avec succès.\n");
    return 0;
}