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