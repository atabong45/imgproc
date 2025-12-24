#ifndef HOUGH_H
#define HOUGH_H

#include "core/image.h"

/**
 * @brief Applique la Transformée de Hough pour détecter des lignes.
 * 
 * Cette fonction effectue 3 tâches :
 * 1. Vote dans l'espace de Hough (Accumulateur rho/theta).
 * 2. Génère une image visuelle de l'accumulateur (pour le debug/visualisation).
 * 3. (Optionnel) Dessine les lignes détectées les plus fortes sur une copie de l'image.
 * 
 * @param edge_img Image binaire des contours (issue de Sobel + Threshold).
 * @param accumulator_view Pointeur pour récupérer l'image de l'accumulateur (peut être NULL).
 * @param threshold Seuil de vote pour considérer qu'une ligne est valide.
 * @return Une copie de l'image originale avec les lignes dessinées dessus.
 */
Image *hough_transform(const Image *edge_img, Image **accumulator_view, int threshold);

#endif