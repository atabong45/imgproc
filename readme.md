# imgproc - Boîte à Outils de Traitement d'Image en C

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![Langage](<https://img.shields.io/badge/langage-C_(gnu99)-brightgreen>)
![Licence](https://img.shields.io/badge/licence-MIT-orange)

## 📖 Table des Matières

1.  [Introduction](#-introduction)
2.  [Fonctionnalités](#-fonctionnalités)
3.  [Compilation et Installation](#-compilation-et-installation)
4.  [Guide d'Utilisation](#-guide-dutilisation)
    - [1. Analyse d'Image](#1-analyse-dimage)
    - [2. Transformations Ponctuelles](#2-transformations-ponctuelles)
    - [3. Géométrie et Interpolation](#3-géométrie-et-interpolation)
    - [4. Filtrage Spatial (Convolution)](#4-filtrage-spatial-convolution)
    - [5. Domaine Fréquentiel (FFT)](#5-domaine-fréquentiel-fft)
    - [6. Détection de Contours et Hough](#6-détection-de-contours-et-hough)
    - [7. Segmentation](#7-segmentation)
    - [8. Morphologie Mathématique](#8-morphologie-mathématique)
    - [9. Opérations Multi-images](#9-opérations-multi-images)
5.  [Architecture du Projet](#-architecture-du-projet)

---

## 🌟 Introduction

**imgproc** est un programme en ligne de commande (CLI) développé en C pur pour le traitement d'images. Conçu pour être léger, performant et hautement modulaire, il implémente "from scratch" les algorithmes fondamentaux de la vision par ordinateur, allant de l'analyse d'histogramme à la segmentation, en passant par le filtrage fréquentiel et la morphologie mathématique.

Les formats d'image supportés sont le **PPM (couleur)** et le **PGM (niveaux de gris)**, en version binaire (P6/P5).

---

## ✨ Fonctionnalités

| Catégorie           | Fonctionnalité               | Description                                                                                 |
| ------------------- | ---------------------------- | ------------------------------------------------------------------------------------------- |
| **I/O**             | Lecture/Écriture PPM/PGM     | Gestion native des formats Netpbm (P5/P6).                                                  |
| **Analyse**         | Statistiques & Histogramme   | Calcul de luminance, contraste et génération d'image d'histogramme.                         |
| **Transformations** | Correction Radiométrique     | Ajustement linéaire (gain/biais), saturation, correction Gamma, Négatif.                    |
| **Géométrie**       | Redimensionnement & Rotation | Zoom avec interpolation (Voisin/Bilinéaire) et rotation spatiale.                           |
| **Filtrage**        | Lissage & Débruitage         | Flou Moyenneur, Gaussien et Filtre Médian (non-linéaire).                                   |
| **Fréquentiel**     | FFT 2D                       | Analyse spectrale, filtres passe-bas/passe-haut et suppression de bruit périodique (Notch). |
| **Contours**        | Détection & Hough            | Filtres de Sobel, Prewitt, Roberts, Laplacien et Transformée de Hough pour les lignes.      |
| **Segmentation**    | Seuillage & Régions          | Seuillage manuel, automatique (Otsu) et Croissance de régions (Region Growing).             |
| **Morphologie**     | Opérateurs Binaires          | Érosion, Dilatation, Ouverture, Fermeture, Gradient morphologique.                          |
| **Multi-images**    | Arithmétique & Logique       | Addition, Soustraction, ET, OU, XOR pour le masquage et la comparaison.                     |

---

## 🛠️ Compilation et Installation

Ce projet utilise `make`. Assurez-vous d'avoir `gcc` installé.

1.  **Clonez le dépôt :**

    ```bash
    git clone https://github.com/atabong45/imgproc.git
    cd imgproc
    ```

2.  **Compilez le projet :**

    ```bash
    make
    ```

    L'exécutable est généré dans `bin/imgproc`.

3.  **Nettoyage :**
    ```bash
    make clean
    ```

---

## 🖥️ Guide d'Utilisation

**Syntaxe Générale :**

```bash
./bin/imgproc --input <entrée.pgm> --output <sortie.pgm> [options...]
```

### 1. Analyse d'Image

- `--luminance` : Affiche la luminance moyenne.
- `--contrast` : Affiche le contraste global.
- `--histogram <fichier.pgm>` : Génère une image de l'histogramme.
  ```bash
  ./bin/imgproc --input image.pgm --output out.pgm --histogram hist.pgm
  ```

### 2. Transformations Ponctuelles

- `--linear <gain> <biais>` : Applique $I' = a \cdot I + b$.
- `--saturate <min> <max>` : Étire la dynamique sur la plage [min, max].
- `--gamma <valeur>` : Correction Gamma ($I^\gamma$). `<1` éclaircit, `>1` assombrit.
- `--invert` : Négatif de l'image.
- `--equalize` : Égalisation d'histogramme globale.
- `--equalize-local <taille>` : Égalisation locale (fenêtre glissante).
  ```bash
  ./bin/imgproc --input sombre.pgm --output claire.pgm --equalize
  ```

### 3. Géométrie et Interpolation

- `--resize <w> <h>` : Redimensionne l'image (Plus proche voisin par défaut).
- `--bilinear` : Active l'interpolation bilinéaire (à combiner avec `--resize`).
- `--rotate <angle>` : Rotation de l'image (en degrés).
  ```bash
  ./bin/imgproc --input in.pgm --output out.pgm --resize 1024 1024 --bilinear --rotate 45
  ```

### 4. Filtrage Spatial (Convolution)

- `--blur <taille>` : Flou moyenneur.
- `--gaussian-blur <taille>` : Flou Gaussien (plus naturel).
- `--median <taille>` : Filtre médian (suppression bruit poivre/sel).
- `--sharpen` : Rehaussement de netteté.
  ```bash
  ./bin/imgproc --input bruitee.pgm --output nette.pgm --median 3 --sharpen
  ```

### 5. Domaine Fréquentiel (FFT)

- `--fft-spectrum <fichier>` : Sauvegarde le spectre de magnitude.
- `--fft-lowpass <rayon>` : Filtre passe-bas (flou).
- `--fft-highpass <rayon>` : Filtre passe-haut (contours).
- `--fft-emphasis <r> <k_low> <k_high>` : Rehaussement spectral (High Frequency Emphasis).
- `--auto-notch <rayon>` : Suppression automatique du bruit périodique.
  ```bash
  ./bin/imgproc --input in.pgm --output out.pgm --fft-emphasis 20 1.0 2.0
  ```

### 6. Détection de Contours et Hough

- `--sobel` / `--prewitt` / `--roberts` : Détection de contours par gradient.
- `--laplacian` : Détection par dérivée seconde.
- `--hough <seuil>` : Transformée de Hough pour détecter les lignes (nécessite une image binaire en entrée, ex: après Sobel + Threshold).
  ```bash
  # Pipeline complet : Contours -> Binarisation -> Lignes
  ./bin/imgproc --input batiment.pgm --output lignes.pgm --sobel --threshold 100 --hough 80
  ```

### 7. Segmentation

- `--threshold <valeur>` : Seuillage manuel simple.
- `--otsu` : Seuillage automatique (Méthode d'Otsu).
- `--region-growing <x> <y> <tolérance>` : Segmentation par croissance de région depuis un germe.
  ```bash
  # Segmentation automatique
  ./bin/imgproc --input in.pgm --output seg.pgm --otsu
  ```

### 8. Morphologie Mathématique

Opérations sur images binaires (idéalement après segmentation).

- `--erode <taille>` / `--dilate <taille>` : Érosion / Dilatation.
- `--opening <taille>` : Ouverture (suppression bruit blanc).
- `--closing <taille>` : Fermeture (comblement trous noirs).
- `--morph-gradient <taille>` : Gradient morphologique (contours).
  ```bash
  # Nettoyer une segmentation
  ./bin/imgproc --input seg.pgm --output clean.pgm --opening 3
  ```

### 9. Opérations Multi-images

Nécessite une seconde image via l'argument.

- **Arithmétique :** `--add <img2>`, `--sub <img2>`
- **Logique :** `--and <img2>`, `--or <img2>`, `--xor <img2>`
  ```bash
  # Masquage : Image & Masque
  ./bin/imgproc --input lena.pgm --output masked.pgm --and masque.pgm
  ```

---

## 🏗️ Architecture du Projet

Le projet est structuré de manière modulaire :

```
imgproc/
│
├── bin/              # Exécutable final
├── build/            # Fichiers objets (.o)
├── include/          # En-têtes (.h)
│   ├── analysis/     # Stats, Histogramme, Hough, Segmentation
│   ├── cli/          # Parsing des arguments
│   ├── core/         # Structure Image
│   ├── filters/      # Convolution, Arithmétique, Pointwise, Morpho
│   ├── geometry/     # Redimensionnement, Interpolation
│   ├── fft/          # Fourier Transform
│   └── io/           # Lecture/Écriture PNM
├── src/              # Code source (.c)
├── Makefile          # Script de compilation
└── README.md         # Documentation
```
