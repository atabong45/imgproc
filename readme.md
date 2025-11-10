# imgproc - Boîte à Outils de Traitement d'Image en C

![Version](https://img.shields.io/badge/version-1.0.0-blue)
![Langage](<https://img.shields.io/badge/langage-C_(gnu99)-brightgreen>)
![Licence](https://img.shields.io/badge/licence-MIT-orange)

## 📖 Table des Matières

1.  [Introduction](#-introduction)
2.  [Fonctionnalités](#-fonctionnalités)
    - [Fonctionnalités Implémentées](#-fonctionnalités-implémentées)
    - [Fonctionnalités Prévues](#-fonctionnalités-prévues)
3.  [Compilation et Installation](#-compilation-et-installation)
4.  [Guide d'Utilisation](#-guide-dutilisation)
    - [Syntaxe Générale](#syntaxe-générale)
    - [Analyse d'Image](#analyse-dimage)
    - [Transformations Ponctuelles](#transformations-ponctuelles-correction-des-couleurs-et-du-contraste)
    - [Filtrage par Convolution](#filtrage-par-convolution-flou-netteté-détection-de-contours)
    - [Amélioration d'Histogramme](#amélioration-dhistogramme)
    - [Chaînage des Opérations](#chaînage-des-opérations)
5.  [Architecture du Projet](#-architecture-du-projet)
6.  [Comment Contribuer](#-comment-contribuer)

---

## 🌟 Introduction

**imgproc** est un programme en ligne de commande (CLI) développé en C pur pour le traitement d'images. Conçu pour être léger, performant et hautement modulaire, il offre une large gamme d'opérations allant de l'analyse d'image aux transformations géométriques, en passant par le filtrage spatial avancé via la convolution.

Ce projet met l'accent sur la clarté du code, la robustesse des algorithmes et une gestion rigoureuse de la mémoire. Il ne repose sur aucune bibliothèque de traitement d'image externe, tous les algorithmes étant implémentés "from scratch".

Les formats d'image supportés pour l'instant sont le **PPM (couleur)** et le **PGM (niveaux de gris)**, en version binaire (P6/P5).

---

## ✨ Fonctionnalités

### ✅ Fonctionnalités Implémentées

| Catégorie                   | Fonctionnalité                          | Description                                                                                                                            |
| --------------------------- | --------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| **I/O**                     | Lecture/Écriture PPM/PGM                | Chargement et sauvegarde d'images aux formats PGM (P5, binaire) et PPM (P6, binaire).                                                  |
| **Analyse**                 | Calcul de Luminance                     | Calcule la valeur moyenne des pixels de l'image (intensité lumineuse globale).                                                         |
| **Analyse**                 | Calcul de Contraste                     | Calcule le contraste de l'image en se basant sur la variation entre les niveaux de gris minimum et maximum.                            |
| **Analyse**                 | Génération d'Histogramme                | Calcule la distribution des niveaux de gris et peut générer une image PGM représentant visuellement cet histogramme.                   |
| **Transformations**         | Transformation Linéaire                 | Modifie la luminosité et le contraste en appliquant une fonction `I' = a*I + b` à chaque pixel.                                        |
| **Transformations**         | Transformation Linéaire avec Saturation | Augmente le contraste en étirant une plage de niveaux de gris spécifiée sur toute la dynamique (0-255).                                |
| **Amélioration**            | Égalisation d'Histogramme               | Transformation non-linéaire qui redistribue les intensités pour maximiser le contraste global. Très efficace sur les images ternes.    |
| **Filtrage (Convolution)**  | Moteur de Convolution Générique         | Un moteur robuste capable d'appliquer n'importe quel noyau de convolution (masque) à une image. Gère les bords par réplication.        |
| **Filtrage (Convolution)**  | Flou Moyenneur (Box Blur)               | Lisse l'image et réduit le bruit en remplaçant chaque pixel par la moyenne de ses voisins. La force du flou est paramétrable.          |
| **Filtrage (Convolution)**  | Flou Gaussien                           | Applique un flou plus naturel et de meilleure qualité que le flou moyenneur, en donnant plus de poids aux pixels centraux.             |
| **Filtrage (Convolution)**  | Détection de Contours (Sobel)           | Met en évidence les contours de l'image en calculant le gradient d'intensité.                                                          |
| **Filtrage (Convolution)**  | Rehaussement de Netteté (Sharpen)       | Augmente la netteté perçue de l'image en assombrissant les bords des contours, les rendant plus "tranchés".                            |
| **Filtrage (Non-linéaire)** | Filtre Médian                           | Supprime efficacement le bruit "poivre et sel" en préservant les contours, en remplaçant chaque pixel par la médiane de son voisinage. |
| **Domaine Fréquentiel**     | Visualisation du Spectre de Fourier     | Calcule la Transformée de Fourier 2D et génère une image de son spectre de magnitude, avec les basses fréquences centrées.             |
| **Domaine Fréquentiel**     | Filtre Passe-Bas Fréquentiel            | Applique un flou puissant en supprimant les hautes fréquences de l'image via une multiplication dans le domaine de Fourier.            |
| **Domaine Fréquentiel**     | Filtre Passe-Haut Fréquentiel           | Isole les contours et les détails de l'image en supprimant les basses fréquences.                                                      |

### 🚀 Fonctionnalités Prévues

| Catégorie                        | Fonctionnalité              | Description                                                                                                      |
| -------------------------------- | --------------------------- | ---------------------------------------------------------------------------------------------------------------- |
| **Transformations Géométriques** | Redimensionnement (Scaling) | Changer la taille de l'image en utilisant des algorithmes d'interpolation (Plus Proche Voisin, Bilinéaire).      |
| **Transformations Géométriques** | Rotation                    | Faire pivoter une image de 90, 180 ou 270 degrés.                                                                |
| **Transformations Géométriques** | Symétrie (Flip)             | Appliquer un effet miroir horizontal ou vertical à l'image.                                                      |
| **Expérience Utilisateur**       | Assistant Interactif        | Un mode menu en console pour guider les utilisateurs à travers les fonctionnalités sans mémoriser les commandes. |
| **I/O**                          | Support du format BMP       | Ajouter la capacité de lire et écrire des images au format BMP 24-bits non compressé.                            |
| **Qualité**                      | Tests Unitaires Automatisés | Mise en place d'un framework de tests pour valider le comportement des fonctions et éviter les régressions.      |

---

## 🛠️ Compilation et Installation

Ce projet utilise `make` pour la compilation. Assurez-vous d'avoir `gcc` et `make` installés sur votre système.

1.  **Clonez le dépôt :**

    ```bash
    git clone https://votre-url-de-depot/imgproc.git
    cd imgproc
    ```

2.  **Compilez le projet :**
    Exécutez `make` à la racine du projet.

    ```bash
    make
    ```

    Cette commande va compiler tous les fichiers source `.c` du répertoire `src/` et placer l'exécutable final dans `bin/imgproc`.

3.  **Nettoyer le projet :**
    Pour supprimer tous les fichiers générés (objets et exécutable), utilisez la commande :
    ```bash
    make clean
    ```

## 🖥️ Guide d'Utilisation

### Syntaxe Générale

La structure d'une commande `imgproc` est la suivante :

```bash
./bin/imgproc --input <fichier_entree> --output <fichier_sortie> [options...]
```

- `--input <path>` : **(Obligatoire)** Spécifie le chemin vers l'image à traiter.
- `--output <path>` : **(Obligatoire)** Spécifie le chemin où sauvegarder l'image résultante.

Les opérations sont appliquées dans un ordre logique prédéfini, quel que soit l'ordre des options dans la commande.

### Analyse d'Image

Ces options affichent des informations dans la console ou génèrent des fichiers d'analyse.

- `--luminance` : Calcule et affiche la luminance moyenne de l'image.

  ```bash
  ./bin/imgproc --input image.pgm --output out.pgm --luminance
  ```

- `--contrast` : Calcule et affiche le contraste de l'image.

  ```bash
  ./bin/imgproc --input image.pgm --output out.pgm --contrast
  ```

- `--histogram <path_hist.pgm>` : Calcule l'histogramme de l'image et le sauvegarde sous forme d'une image PGM.
  ```bash
  ./bin/imgproc --input photo.pgm --output out.pgm --histogram histogramme.pgm
  ```

### Transformations Ponctuelles (Correction des couleurs et du contraste)

- `--linear <gain> <biais>` : Applique une transformation linéaire `I' = gain * I + biais`. Utile pour ajuster manuellement la luminosité et le contraste.

  ```bash
  # Augmente le contraste (gain > 1) et assombrit légèrement l'image (biais < 0)
  ./bin/imgproc --input image.pgm --output out.pgm --linear 1.5 -20
  ```

- `--saturate <min> <max>` : Étire les niveaux de gris compris entre `<min>` et `<max>` sur toute la plage 0-255. Les valeurs en dehors de cet intervalle sont "saturées" (mises à 0 ou 255).
  ```bash
  # Se concentre sur les niveaux de gris entre 50 et 200 pour maximiser le contraste
  ./bin/imgproc --input image.pgm --output out.pgm --saturate 50 200
  ```

### Filtrage par Convolution (Flou, Netteté, Détection de contours)

- `--blur <taille>` : Applique un flou moyenneur. `<taille>` doit être un entier impair (ex: 3, 5, 9). Plus la taille est grande, plus le flou est prononcé.

  ```bash
  ./bin/imgproc --input image.pgm --output out.pgm --blur 5
  ```

- `--gaussian-blur <taille>` : Applique un flou Gaussien, plus doux et de meilleure qualité. `<taille>` doit être un entier impair.

  ```bash
  ./bin/imgproc --input image.pgm --output out.pgm --gaussian-blur 5
  ```

- `--sobel` : Applique le filtre de Sobel pour détecter les contours. Le résultat est une image en noir et blanc où les contours sont mis en évidence.

  ```bash
  ./bin/imgproc --input batiment.pgm --output contours.pgm --sobel
  ```

- `--sharpen` : Applique un filtre de rehaussement de netteté. Il rend les détails et les textures plus "tranchés".
  ```bash
  ./bin/imgproc --input portrait.pgm --output portrait_net.pgm --sharpen
  ```
- `--median <taille>` : Applique un filtre médian, très efficace contre le bruit de type "poivre et sel". `<taille>` doit être un entier impair (typiquement 3 ou 5).
  ```bash
  ./bin/imgproc --input image_bruitee.pgm --output image_propre.pgm --median 3
  ```

### Analyse dans le Domaine Fréquentiel

- `--fft-spectrum <path_spectrum.pgm>` : Calcule la Transformée de Fourier de l'image et sauvegarde une représentation visuelle de son spectre dans un fichier PGM.
  ```bash
  # Générer le spectre de Fourier de l'image
  ./bin/imgproc --input image.pgm --output out.pgm --fft-spectrum spectre.pgm
  ```
- `--fft-lowpass <rayon>` : Applique un filtre passe-bas idéal (flou). `<rayon>` définit la taille des basses fréquences à conserver. Une petite valeur (ex: 20) produit un flou très fort.

  ```bash
  ./bin/imgproc --input image.pgm --output flou.pgm --fft-lowpass 30
  ```

- `--fft-highpass <rayon>` : Applique un filtre passe-haut idéal (détection de contours). `<rayon>` définit la taille des basses fréquences à supprimer. Une petite valeur (ex: 10) conserve beaucoup de détails.
  ```bash
  ./bin/imgproc --input image.pgm --output contours.pgm --fft-highpass 15
  ```

### Amélioration d'Histogramme

- `--equalize` : Applique une égalisation d'histogramme. C'est la méthode la plus puissante pour corriger automatiquement les images sous-exposées, sur-exposées ou ternes.
  ```bash
  ./bin/imgproc --input photo_sombre.pgm --output photo_corrigee.pgm --equalize
  ```

### Chaînage des Opérations

Vous pouvez combiner plusieurs opérations en une seule commande. Elles seront appliquées dans un ordre logique (par exemple, le flou avant la détection de contours).

```bash
# Appliquer un léger flou Gaussien pour réduire le bruit, puis détecter les contours avec Sobel
./bin/imgproc --input image.pgm --output out.pgm --gaussian-blur 3 --sobel

# Augmenter la netteté, puis corriger le contraste global
./bin/imgproc --input image.pgm --output out.pgm --sharpen --equalize
```

---

## 🏗️ Architecture du Projet

Le projet est structuré de manière modulaire pour garantir la séparation des responsabilités, la testabilité et l'extensibilité.

```
imgproc/
│
├── bin/              # Contient l'exécutable final
├── build/            # Contient les fichiers objets (.o) intermédiaires
├── include/          # Contient les fichiers d'en-tête publics (.h)
│   ├── analysis/     # Headers pour l'analyse (histogramme, stats)
│   ├── cli/          # Header pour le parseur d'arguments
│   ├── core/         # Header pour les structures de données (Image)
│   ├── filters/      # Headers pour les filtres (convolution, etc.)
│   └── io/           # Headers pour la lecture/écriture de fichiers
├── src/              # Contient les fichiers d'implémentation (.c)
│   ├── analysis/
│   ├── cli/
│   ├── core/
│   ├── filters/
│   ├── io/
│   └── main.c        # Point d'entrée du programme
├── tests/            # (Futur) Scripts et données de test
├── Makefile          # Gère la compilation du projet
└── README.md         # Cette documentation
```

Chaque module est conçu pour être aussi indépendant que possible :

- Le module `core` ne connaît rien aux formats de fichiers.
- Le module `filters` opère sur la structure `Image` sans se soucier de la manière dont elle a été chargée.
- Le module `cli` orchestre les appels aux autres modules en fonction des commandes de l'utilisateur.

---

## 🤝 Comment Contribuer

Les contributions sont les bienvenues ! Si vous souhaitez ajouter une fonctionnalité ou corriger un bug, veuillez suivre ces étapes :

1.  **Forkez** le projet.
2.  Créez une nouvelle branche pour votre fonctionnalité (`git checkout -b feature/NomDeLaFonctionnalite`).
3.  Commitez vos changements (`git commit -m 'Ajout de la fonctionnalité X'`).
4.  Pushez vers votre branche (`git push origin feature/NomDeLaFonctionnalite`).
5.  Ouvrez une **Pull Request**.

Veuillez respecter le style de code existant et documenter toute nouvelle fonction publique.
