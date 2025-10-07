#ifndef PNM_H
#define PNM_H

#include "core/image.h" // On a besoin de la définition de la structure Image

/**
 * @brief Charge une image depuis un fichier au format PPM (P6) ou PGM (P5).
 *
 * @param filename Le chemin du fichier à charger.
 * @return Un pointeur vers une nouvelle structure Image contenant les données,
 *         ou NULL en cas d'erreur (fichier non trouvé, format invalide, etc.).
 */
Image *loadPNM(const char *filename);

/**
 * @brief Sauvegarde une image dans un fichier au format PPM (P6) ou PGM (P5).
 *
 * Le format est choisi automatiquement en fonction du nombre de canaux de l'image.
 * @param img Pointeur vers l'image à sauvegarder.
 * @param filename Le chemin du fichier de destination.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int savePNM(const Image *img, const char *filename);

#endif // PNM_H