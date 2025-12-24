#include "cli/parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Arguments parse_args(int argc, char *argv[]) {
    // 1. Initialisation de la structure avec des valeurs par défaut.
    // L'initialisation "{0}" met tous les champs à 0, false, ou NULL.
    Arguments args = {0}; 
    args.linear_gain = 1.0;
    args.linear_bias = 0.0;
    args.saturated_min = -1; // -1 sert de marqueur "non défini"
    args.saturated_max = -1;
    args.blur_kernel_size = 0;
    args.gaussian_blur_kernel_size = 0;
    args.apply_sobel = false;
    args.apply_sharpen = false;
    args.apply_equalization = false;
    args.median_kernel_size = 0;
    args.test_fft = false;
    args.fft_spectrum_path = NULL;
    args.fft_lowpass_radius = 0; 
    args.fft_highpass_radius = 0;
    args.apply_prewitt = false;
    args.apply_roberts = false;
    args.threshold_value = -1;
    args.auto_notch_radius = 0;
    args.gamma_value = -1.0;
    args.resize_width = 0;
    args.second_image_path = NULL;
    args.resize_height = 0;
    args.resize_bilinear = false;
    args.local_eq_window = 0;
    args.apply_invert = false;
    args.apply_and = false;
    args.apply_or = false;
    args.apply_xor = false;
    args.min_kernel_size = 0;
    args.max_kernel_size = 0;
    args.rotation_angle = 0.0;
    args.fft_emphasis_radius = 0;
    args.fft_emphasis_low = 1.0;
    args.fft_emphasis_high = 1.0;
    args.hough_threshold = 0;
    args.use_otsu = false;
    args.region_tolerance = -1;
    args.seed_x = 0;
    args.seed_y = 0;

    // 2. Boucle sur tous les arguments de la ligne de commande (sauf le nom du programme)
    for (int i = 1; i < argc; i++) {

        // --- Options principales ---
        if (strcmp(argv[i], "--input") == 0) {
            if (i + 1 < argc) {
                args.input_path = argv[++i]; // Consomme la valeur et avance l'index
            } else {
                fprintf(stderr, "Erreur: L'option --input nécessite un chemin de fichier.\n");
                exit(1);
            }
        } 
        else if (strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                args.output_path = argv[++i]; // Consomme la valeur et avance l'index
            } else {
                fprintf(stderr, "Erreur: L'option --output nécessite un chemin de fichier.\n");
                exit(1);
            }
        }

        // --- Options d'analyse ---
        else if (strcmp(argv[i], "--histogram") == 0) {
            // Vérifie si l'argument suivant existe et n'est pas une autre option
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args.show_histogram = true; 
                args.histogram_output_path = argv[++i]; // Consomme le chemin du fichier
            } else {
                // Si --histogram est seul, on active un simple booléen
                args.show_histogram = true; 
            }
        }
        else if (strcmp(argv[i], "--luminance") == 0) {
            args.show_luminance = true;
        }
        else if (strcmp(argv[i], "--contrast") == 0) {
            args.show_contrast = true;
        }

        // --- Options de transformation ---
        else if (strcmp(argv[i], "--linear") == 0) {
            if (i + 2 < argc) {
                args.linear_gain = atof(argv[++i]); // Consomme le gain
                args.linear_bias = atof(argv[++i]); // Consomme le biais
            } else {
                fprintf(stderr, "Erreur: L'option --linear nécessite deux arguments (gain et biais).\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--saturate") == 0) {
            if (i + 2 < argc) {
                args.saturated_min = atoi(argv[++i]); // Consomme le min
                args.saturated_max = atoi(argv[++i]); // Consomme le max
            } else {
                fprintf(stderr, "Erreur: L'option --saturate nécessite deux arguments (min et max).\n");
                exit(1);
            }
        }

        else if (strcmp(argv[i], "--blur") == 0) {
            if (i + 1 < argc) {
                args.blur_kernel_size = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: L'option --blur nécessite une taille de noyau (ex: 3).\n");
                exit(1);
            }
        }

        else if (strcmp(argv[i], "--gaussian-blur") == 0) {
            if (i + 1 < argc) {
                args.gaussian_blur_kernel_size = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: L'option --gaussian-blur nécessite une taille.\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--sobel") == 0) {
            args.apply_sobel = true;
        }
        else if (strcmp(argv[i], "--sharpen") == 0) {
            args.apply_sharpen = true;
        }
        else if (strcmp(argv[i], "--equalize") == 0) {
            args.apply_equalization = true;
        }
        else if (strcmp(argv[i], "--equalize") == 0) {
            args.apply_equalization = true;
        }
        else if (strcmp(argv[i], "--median") == 0) {
            if (i + 1 < argc) {
                args.median_kernel_size = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: L'option --median nécessite une taille de noyau (ex: 3).\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--test-fft") == 0) {
            args.test_fft = true;
        }
        else if (strcmp(argv[i], "--test-fft") == 0) {
            args.test_fft = true;
        }
        else if (strcmp(argv[i], "--fft-spectrum") == 0) {
            if (i + 1 < argc) {
                args.fft_spectrum_path = argv[++i];
            } else {
                fprintf(stderr, "Erreur: L'option --fft-spectrum nécessite un chemin de fichier de sortie.\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--fft-lowpass") == 0) {
            if (i + 1 < argc) {
                args.fft_lowpass_radius = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: L'option --fft-lowpass nécessite un rayon.\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--fft-highpass") == 0) {
            if (i + 1 < argc) {
                args.fft_highpass_radius = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: L'option --fft-highpass nécessite un rayon.\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--prewitt") == 0) {
            args.apply_prewitt = true;
        }
        else if (strcmp(argv[i], "--roberts") == 0) {
            args.apply_roberts = true;
        }
        else if (strcmp(argv[i], "--threshold") == 0) {
            if (i + 1 < argc) {
                args.threshold_value = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: L'option --threshold nécessite une valeur (0-255).\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--auto-notch") == 0) {
            if (i + 1 < argc) {
                args.auto_notch_radius = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: L'option --auto-notch nécessite un rayon pour les filtres.\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--gamma") == 0) {
            if (i + 1 < argc) args.gamma_value = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "--add") == 0) {
            if (i + 1 < argc) {
                args.apply_add = true;
                args.second_image_path = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--sub") == 0) {
            if (i + 1 < argc) {
                args.apply_sub = true;
                args.second_image_path = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--resize") == 0) {
            if (i + 2 < argc) {
                args.resize_width = atoi(argv[++i]);
                args.resize_height = atoi(argv[++i]);
            }
        }
        // ...
        else if (strcmp(argv[i], "--invert") == 0) {
            args.apply_invert = true;
        }
        else if (strcmp(argv[i], "--bilinear") == 0) {
            args.resize_bilinear = true; // S'utilise en combinaison avec --resize
        }
        else if (strcmp(argv[i], "--equalize-local") == 0) {
            if (i + 1 < argc) args.local_eq_window = atoi(argv[++i]);
            else { fprintf(stderr, "Erreur: --equalize-local attend une taille de fenêtre.\n"); exit(1); }
        }
        else if (strcmp(argv[i], "--and") == 0) {
            if (i + 1 < argc) { args.apply_and = true; args.second_image_path = argv[++i]; }
        }
        else if (strcmp(argv[i], "--or") == 0) {
            if (i + 1 < argc) { args.apply_or = true; args.second_image_path = argv[++i]; }
        }
        else if (strcmp(argv[i], "--xor") == 0) {
            if (i + 1 < argc) { args.apply_xor = true; args.second_image_path = argv[++i]; }
        }
        else if (strcmp(argv[i], "--min") == 0) {
            if (i + 1 < argc) {
                args.min_kernel_size = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: --min nécessite une taille de noyau.\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--max") == 0) {
            if (i + 1 < argc) {
                args.max_kernel_size = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: --max nécessite une taille de noyau.\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--rotate") == 0) {
            if (i + 1 < argc) args.rotation_angle = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "--fft-emphasis") == 0) {
            if (i + 3 < argc) {
                args.fft_emphasis_radius = atoi(argv[++i]);
                args.fft_emphasis_low = atof(argv[++i]);
                args.fft_emphasis_high = atof(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: --fft-emphasis attend <radius> <k_low> <k_high>\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--laplacian") == 0) {
            args.apply_laplacian = true;
        }
        else if (strcmp(argv[i], "--hough") == 0) {
            if (i + 1 < argc) args.hough_threshold = atoi(argv[++i]);
            else { fprintf(stderr, "Erreur: --hough attend un seuil (ex: 100).\n"); exit(1); }
        }
        else if (strcmp(argv[i], "--otsu") == 0) {
            args.use_otsu = true;
        }
        else if (strcmp(argv[i], "--region-growing") == 0) {
            if (i + 3 < argc) {
                args.seed_x = atoi(argv[++i]);
                args.seed_y = atoi(argv[++i]);
                args.region_tolerance = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Erreur: --region-growing attend <x> <y> <tolerance>\n");
                exit(1);
            }
        }
        else if (strcmp(argv[i], "--opening") == 0) {
            if (i + 1 < argc) args.morph_open_size = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--closing") == 0) {
            if (i + 1 < argc) args.morph_close_size = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--morph-gradient") == 0) {
            if (i + 1 < argc) args.morph_gradient_size = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--erode") == 0) {
            if (i + 1 < argc) args.morph_erode_size = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--dilate") == 0) {
            if (i + 1 < argc) args.morph_dilate_size = atoi(argv[++i]);
        }



        // --- Gestion des options inconnues ---
        else if (argv[i][0] == '-') {
            // Si l'argument commence par un '-' mais n'a pas été reconnu par les 'else if' précédents,
            // c'est une option inconnue.
            fprintf(stderr, "Avertissement: Option inconnue ignorée : %s\n", argv[i]);
        }
        // Les arguments qui ne commencent pas par '-' sont ignorés (car ils sont supposés
        // être des valeurs pour des options, déjà consommées par i++).
    }

    // 3. Validation finale des arguments obligatoires
    if (args.input_path == NULL || args.output_path == NULL) {
        fprintf(stderr, "\nErreur: Les options --input et --output sont obligatoires.\n\n");
        fprintf(stderr, "Usage: %s --input <fichier_entree> --output <fichier_sortie> [options...]\n", argv[0]);
        fprintf(stderr, "Exemple: %s --input in.pgm --output out.pgm --contrast --histogram hist.pgm\n\n", argv[0]);
        exit(1);
    }

    return args;
}

