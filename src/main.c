#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "io/pnm.h"
#include "analysis/histogram.h"
#include "analysis/stats.h"
#include "filters/pointwise.h"
#include "filters/predefined_filters.h"
#include "filters/histogram_equalization.h"
#include "cli/parser.h"
#include "fft/fft.h"
#include "filters/arithmetic.h"
#include "geometry/transform.h"
#include "analysis/hough.h"
#include "analysis/segmentation.h"

int main(int argc, char *argv[]) {
    // ============================================================
    // ÉTAPE 1: PARSING DES ARGUMENTS
    // ============================================================
    Arguments args = parse_args(argc, argv);

    // ============================================================
    // ÉTAPE 2: CHARGEMENT DE L'IMAGE
    // ============================================================
    Image *img = loadPNM(args.input_path);
    if (!img) {
        fprintf(stderr, "Impossible de charger l'image '%s'.\n", args.input_path);
        return 1;
    }

    // ============================================================
    // ÉTAPE 3: ANALYSE DE L'IMAGE
    // ============================================================
    if (args.show_histogram) {
        int histogram[256];
        calculate_histogram(img, histogram);
        Image *hist_img = create_histogram_image(histogram, 512, 256);
        if (hist_img) {
            if (savePNM(hist_img, args.histogram_output_path) != 0) {
                fprintf(stderr, "Erreur lors de la sauvegarde de l'histogramme.\n");
            }
            freeImage(hist_img);
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

    // ============================================================
    // ÉTAPE 4: TRANSFORMATIONS PONCTUELLES
    // ============================================================
    
    // Transformation linéaire
    if (args.linear_gain != 1.0 || args.linear_bias != 0.0) {
        linear_transform(img, args.linear_gain, args.linear_bias);
    }

    // Transformation saturée
    if (args.saturated_min != -1 && args.saturated_max != -1) {
        saturated_transform(img, args.saturated_min, args.saturated_max);
    }

    // Correction Gamma
    if (args.gamma_value > 0) {
        apply_gamma_correction(img, args.gamma_value);
    }

    // Inversion
    if (args.apply_invert) {
        apply_invert(img);
    }

    // ============================================================
    // ÉTAPE 5: OPÉRATIONS ARITHMÉTIQUES
    // ============================================================
    if (args.apply_add || args.apply_sub || args.apply_and || args.apply_or || args.apply_xor) {
        if (args.second_image_path) {
            Image *img2 = loadPNM(args.second_image_path);
            if (img2) {
                ArithmeticOp op;
                if (args.apply_add) {
                    op = OP_ADD;
                    printf("Addition avec %s...\n", args.second_image_path);
                } else if (args.apply_sub) {
                    op = OP_SUB;
                    printf("Soustraction avec %s...\n", args.second_image_path);
                } else if (args.apply_and) {
                    op = OP_AND;
                } else if (args.apply_or) {
                    op = OP_OR;
                } else {
                    op = OP_XOR;
                }

                apply_arithmetic(img, img2, op);
                freeImage(img2);
            } else {
                fprintf(stderr, "Impossible de charger la seconde image '%s'.\n", args.second_image_path);
            }
        } else {
            fprintf(stderr, "Opération arithmétique demandée mais aucune seconde image fournie.\n");
        }
    }

    // ============================================================
    // ÉTAPE 6: TRANSFORMATIONS GÉOMÉTRIQUES
    // ============================================================
    
    // Redimensionnement
    if (args.resize_width > 0 && args.resize_height > 0) {
        Image *resized = NULL;
        if (args.resize_bilinear) {
            printf("Redimensionnement bilinéaire vers %dx%d...\n", args.resize_width, args.resize_height);
            resized = resize_bilinear(img, args.resize_width, args.resize_height);
        } else {
            printf("Redimensionnement (plus proche voisin) vers %dx%d...\n", args.resize_width, args.resize_height);
            resized = resize_nearest_neighbor(img, args.resize_width, args.resize_height);
        }
        
        if (resized) {
            freeImage(img);
            img = resized;
        }
    }

    // Rotation
    if (args.rotation_angle != 0.0) {
        printf("Rotation de %.2f degrés...\n", args.rotation_angle);
        Image *rotated = rotate_image(img, args.rotation_angle);
        if (rotated) {
            freeImage(img);
            img = rotated;
        }
    }

    // ============================================================
    // ÉTAPE 7: ÉGALISATION D'HISTOGRAMME
    // ============================================================
    
    // Égalisation locale (prioritaire sur globale)
    if (args.local_eq_window > 0) {
        printf("Application de l'égalisation d'histogramme locale (fenêtre=%d)...\n", args.local_eq_window);
        Image *eq_img = equalize_histogram_local(img, args.local_eq_window);
        if (eq_img) {
            freeImage(img);
            img = eq_img;
        }
    } else if (args.apply_equalization) {
        printf("Application de l'égalisation d'histogramme globale...\n");
        Image *equalized_img = equalize_histogram(img);
        if (equalized_img) {
            freeImage(img);
            img = equalized_img;
        }
    }

    // ============================================================
    // ÉTAPE 8: FILTRES SPATIAUX (CONVOLUTION)
    // ============================================================
    
    // Flou moyen
    if (args.blur_kernel_size > 0) {
        printf("Application d'un flou moyen %dx%d...\n", args.blur_kernel_size, args.blur_kernel_size);
        Image *blurred_img = apply_box_blur(img, args.blur_kernel_size);
        if (blurred_img) {
            freeImage(img);
            img = blurred_img;
        }
    }

    // Flou Gaussien
    if (args.gaussian_blur_kernel_size > 0) {
        printf("Application d'un flou Gaussien %dx%d...\n", args.gaussian_blur_kernel_size, args.gaussian_blur_kernel_size);
        Image *blurred_img = apply_gaussian_blur(img, args.gaussian_blur_kernel_size);
        if (blurred_img) {
            freeImage(img);
            img = blurred_img;
        }
    }

    // Netteté
    if (args.apply_sharpen) {
        printf("Application du filtre de netteté...\n");
        Image *sharpened_img = apply_sharpen_filter(img);
        if (sharpened_img) {
            freeImage(img);
            img = sharpened_img;
        }
    }

    // Filtre médian
    if (args.median_kernel_size > 0) {
        printf("Application du filtre médian %dx%d...\n", args.median_kernel_size, args.median_kernel_size);
        Image *median_img = apply_median_filter(img, args.median_kernel_size);
        if (median_img) {
            freeImage(img);
            img = median_img;
        }
    }

    // Filtre min
    if (args.min_kernel_size > 0) {
        printf("Application du filtre minimum %dx%d...\n", args.min_kernel_size, args.min_kernel_size);
        Image *min_img = apply_min_filter(img, args.min_kernel_size);
        if (min_img) {
            freeImage(img);
            img = min_img;
        }
    }

    // Filtre max
    if (args.max_kernel_size > 0) {
        printf("Application du filtre maximum %dx%d...\n", args.max_kernel_size, args.max_kernel_size);
        Image *max_img = apply_max_filter(img, args.max_kernel_size);
        if (max_img) {
            freeImage(img);
            img = max_img;
        }
    }

    // ============================================================
    // ÉTAPE 9: DÉTECTION DE CONTOURS
    // ============================================================
    
    // Sobel
    if (args.apply_sobel) {
        printf("Application du filtre de Sobel...\n");
        Image *sobel_img = apply_sobel_filter(img);
        if (sobel_img) {
            freeImage(img);
            img = sobel_img;
        }
    }

    // Prewitt
    if (args.apply_prewitt) {
        printf("Application du filtre de Prewitt...\n");
        Image *prewitt_img = apply_prewitt_filter(img);
        if (prewitt_img) {
            freeImage(img);
            img = prewitt_img;
        }
    }

    // Roberts
    if (args.apply_roberts) {
        printf("Application du filtre de Roberts...\n");
        Image *roberts_img = apply_roberts_filter(img);
        if (roberts_img) {
            freeImage(img);
            img = roberts_img;
        }
    }

    // Laplacien
    if (args.apply_laplacian) {
        printf("Application du filtre Laplacien...\n");
        Image *lap = apply_laplacian_filter(img);
        if (lap) {
            freeImage(img);
            img = lap;
        }
    }

    // ============================================================
    // ÉTAPE 10: TRAITEMENT FRÉQUENTIEL (FFT)
    // ============================================================
    
    // Vérifier si un traitement FFT est demandé
    int needs_fft = (args.test_fft || 
                     args.fft_spectrum_path || 
                     args.fft_lowpass_radius > 0 || 
                     args.fft_highpass_radius > 0 ||
                     args.fft_emphasis_radius > 0 ||
                     args.auto_notch_radius > 0);

    if (needs_fft) {
        printf("Début du traitement fréquentiel (FFT)...\n");
        
        int fft_w, fft_h;
        Complex **fft_result = fft2d(img, &fft_w, &fft_h);
        
        if (fft_result) {
            printf("FFT calculée avec succès (dimensions : %dx%d).\n", fft_w, fft_h);
            
            // Test FFT (vérification inverse)
            if (args.test_fft) {
                printf("Test de la FFT inverse...\n");
                Image *inversed_img = ifft2d(fft_result, fft_w, fft_h);
                if (inversed_img) {
                    printf("FFT Inverse calculée avec succès.\n");
                    savePNM(inversed_img, "test_ifft.pgm");
                    freeImage(inversed_img);
                }
            }
            
            // Génération du spectre
            if (args.fft_spectrum_path) {
                printf("Calcul du spectre de Fourier...\n");
                Image *spectrum = create_spectrum_image(fft_result, fft_w, fft_h);
                if (spectrum) {
                    if (savePNM(spectrum, args.fft_spectrum_path) == 0) {
                        printf("Spectre de Fourier sauvegardé dans '%s'.\n", args.fft_spectrum_path);
                    }
                    freeImage(spectrum);
                }
            }
            
            // Filtre passe-bas
            if (args.fft_lowpass_radius > 0) {
                printf("Application du filtre passe-bas fréquentiel (rayon=%d)...\n", args.fft_lowpass_radius);
                fft_lowpass_filter(fft_result, fft_w, fft_h, args.fft_lowpass_radius);
            }
            
            // Filtre passe-haut
            if (args.fft_highpass_radius > 0) {
                printf("Application du filtre passe-haut fréquentiel (rayon=%d)...\n", args.fft_highpass_radius);
                fft_highpass_filter(fft_result, fft_w, fft_h, args.fft_highpass_radius);
            }
            
            // Rehaussement spectral
            if (args.fft_emphasis_radius > 0) {
                printf("Application du rehaussement spectral (r=%d, L=%.1f, H=%.1f)...\n", 
                       args.fft_emphasis_radius, args.fft_emphasis_low, args.fft_emphasis_high);
                fft_emphasis_filter(fft_result, fft_w, fft_h, 
                                    args.fft_emphasis_radius, args.fft_emphasis_low, args.fft_emphasis_high);
            }
            
            // Suppression automatique du bruit
            if (args.auto_notch_radius > 0) {
                printf("Détection et suppression automatique du bruit (rayon=%d)...\n", args.auto_notch_radius);
                double default_threshold_factor = 10.0;
                fft_auto_notch_filter(fft_result, fft_w, fft_h, default_threshold_factor, args.auto_notch_radius);
            }
            
            // FFT Inverse pour revenir au domaine spatial
            if (args.fft_lowpass_radius > 0 || args.fft_highpass_radius > 0 || 
                args.fft_emphasis_radius > 0 || args.auto_notch_radius > 0) {
                printf("Application de la FFT inverse...\n");
                Image *filtered_img = ifft2d(fft_result, fft_w, fft_h);
                if (filtered_img) {
                    freeImage(img);
                    img = filtered_img;
                }
            }
            
            // Libération de la mémoire FFT
            free_fft_data(fft_result, fft_h);
        } else {
            fprintf(stderr, "Erreur: Le calcul de la FFT a échoué.\n");
        }
    }

   // ============================================================
    // ÉTAPE 11: TRANSFORMÉE DE HOUGH
    // ============================================================
    // Note: Hough attend normalement une image binaire (contours). 
    // Si l'utilisateur a fait --sobel --threshold avant, c'est parfait.
    
    if (args.hough_threshold > 0) {
        printf("Application de la Transformée de Hough (Seuil=%d)...\n", args.hough_threshold);
        
        Image *acc_view = NULL;
        Image *lines_img = hough_transform(img, &acc_view, args.hough_threshold);
        
        if (lines_img) {
            if (acc_view) {
                savePNM(acc_view, "hough_accumulator.pgm");
                freeImage(acc_view);
                printf("  -> Image de l'accumulateur sauvegardée dans 'hough_accumulator.pgm'\n");
            }
            freeImage(img);
            img = lines_img;
        }
    }

    // ============================================================
    // ÉTAPE 12: SEGMENTATION (Seuillage / Régions)
    // ============================================================
    
    // A. Croissance de régions
    if (args.region_tolerance >= 0) {
        printf("Application de la croissance de région (Graine: %d,%d | Tol: %d)...\n", 
               args.seed_x, args.seed_y, args.region_tolerance);
        Image *region_mask = region_growing(img, args.seed_x, args.seed_y, args.region_tolerance);
        if (region_mask) {
            freeImage(img);
            img = region_mask; // L'image de sortie devient le masque binaire
        }
    }
    // B. Seuillage (Automatique Otsu OU Manuel)
    else if (args.use_otsu) {
        printf("Calcul et application du seuillage automatique (Otsu)...\n");
        int otsu_val = calculate_otsu_threshold(img);
        apply_threshold(img, (uint8_t)otsu_val);
    } 
    else if (args.threshold_value != -1) {
        printf("Application du seuillage manuel (seuil=%d)...\n", args.threshold_value);
        apply_threshold(img, (uint8_t)args.threshold_value);
    }

    // ============================================================
    // ÉTAPE 13: MORPHOLOGIE MATHÉMATIQUE
    // ============================================================
    // Ces opérations sont plus efficaces sur des images binaires (après l'étape 12)

    if (args.morph_erode_size > 0) {
        printf("Application Érosion (taille %d)...\n", args.morph_erode_size);
        Image *res = morph_erode(img, args.morph_erode_size);
        if (res) { freeImage(img); img = res; }
    }
    if (args.morph_dilate_size > 0) {
        printf("Application Dilatation (taille %d)...\n", args.morph_dilate_size);
        Image *res = morph_dilate(img, args.morph_dilate_size);
        if (res) { freeImage(img); img = res; }
    }
    if (args.morph_open_size > 0) {
        printf("Application Ouverture (taille %d)...\n", args.morph_open_size);
        Image *res = morph_open(img, args.morph_open_size);
        if (res) { freeImage(img); img = res; }
    }
    if (args.morph_close_size > 0) {
        printf("Application Fermeture (taille %d)...\n", args.morph_close_size);
        Image *res = morph_close(img, args.morph_close_size);
        if (res) { freeImage(img); img = res; }
    }
    if (args.morph_gradient_size > 0) {
        printf("Calcul du Gradient Morphologique (taille %d)...\n", args.morph_gradient_size);
        Image *res = morph_gradient(img, args.morph_gradient_size);
        if (res) { freeImage(img); img = res; }
    }

    // ============================================================
    // ÉTAPE 14: SAUVEGARDE FINALE
    // ============================================================
    
    if (savePNM(img, args.output_path) != 0) {
        fprintf(stderr, "Erreur: Impossible de sauvegarder l'image dans '%s'.\n", args.output_path);
        freeImage(img);
        return 1;
    }

    printf("Image finale sauvegardée avec succès dans '%s'.\n", args.output_path);
    freeImage(img);

    printf("Opération terminée avec succès.\n");
    return 0;
}