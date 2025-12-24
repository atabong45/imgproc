# 🎓 Guide de Correspondance : Cours vs Implémentation

Ce document référence toutes les fonctionnalités implémentées dans le projet **imgproc**, classées selon les chapitres du cours de Traitement d'Image. Il fournit la localisation théorique (numéro de page dans le PDF) et la commande pratique pour tester chaque concept.

> **Prérequis pour les tests :**
>
> 1. Compilez le projet : `make`
> 2. Assurez-vous d'avoir une image de test (ex: `lena.pgm`) dans le dossier racine.
> 3. Toutes les commandes s'exécutent depuis la racine du projet.

---

## 📘 Chapitre 2 : Introduction & Opérations de Base

Ce chapitre couvre l'analyse statistique, les opérations ponctuelles (pixel par pixel), arithmétiques et géométriques.

### 1. Histogramme

- **Page du cours :** 2
- **Description :** Calcule la distribution des niveaux de gris dans l'image. Indispensable pour analyser l'exposition.
- **Commande :** `--histogram <fichier_sortie>`
- **Test :** Générer l'image de l'histogramme de Lena.
  ```bash
  ./bin/imgproc --input lena.pgm --output out.pgm --histogram hist_lena.pgm
  ```

### 2. Luminance & Contraste

- **Page du cours :** 4 (Luminance) et 5 (Contraste)
- **Description :**
  - _Luminance :_ Moyenne des intensités des pixels.
  - _Contraste :_ Écart-type ou dynamique des intensités.
- **Commande :** `--luminance`, `--contrast`
- **Test :** Afficher les statistiques dans la console.
  ```bash
  ./bin/imgproc --input lena.pgm --output out.pgm --luminance --contrast
  ```

### 3. Transformation Linéaire (Extension de dynamique)

- **Page du cours :** 9
- **Description :** Applique une fonction affine $I' = a \cdot I + b$ pour ajuster luminosité et contraste manuellement.
- **Commande :** `--linear <gain> <biais>`
- **Test :** Augmenter le contraste (x1.5) et réduire la luminosité (-20).
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_linear.pgm --linear 1.5 -20
  ```

### 4. Transformation Linéaire avec Saturation

- **Page du cours :** 12
- **Description :** Étire une plage spécifique de niveaux de gris $[min, max]$ sur toute la dynamique $[0, 255]$.
- **Commande :** `--saturate <min> <max>`
- **Test :** Recadrer la dynamique entre les valeurs 50 et 200.
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_sat.pgm --saturate 50 200
  ```

### 5. Correction Gamma (Transformation Non-linéaire)

- **Page du cours :** 15
- **Description :** Correction exponentielle $I' = I^\gamma$. Permet d'éclaircir les ombres ($\gamma < 1$) ou d'assombrir les hautes lumières ($\gamma > 1$).
- **Commande :** `--gamma <valeur>`
- **Test :** Éclaircir une image (simulation de sous-exposition corrigée).
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_gamma.pgm --gamma 0.5
  ```

### 6. Négatif (Inversion)

- **Page du cours :** 16
- **Description :** Inverse les valeurs des pixels ($255 - I$).
- **Commande :** `--invert`
- **Test :** Créer le négatif de Lena.
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_neg.pgm --invert
  ```

### 7. Égalisation d'Histogramme (Globale)

- **Page du cours :** 20-21
- **Description :** Aplanit l'histogramme pour maximiser le contraste global de l'image.
- **Commande :** `--equalize`
- **Test :** Améliorer une image (utile si l'image originale est terne).
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_eq.pgm --equalize
  ```

### 8. Égalisation d'Histogramme Locale

- **Page du cours :** 24
- **Description :** Applique l'égalisation sur une fenêtre glissante autour de chaque pixel. Révèle les détails locaux.
- **Commande :** `--equalize-local <taille_fenetre>`
- **Test :** Égalisation avec une fenêtre de 21x21 pixels (Attention : traitement lent).
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_eq_loc.pgm --equalize-local 21
  ```

### 9. Opérations Logiques (ET, OU, XOR)

- **Page du cours :** 25-26
- **Description :** Opérations bit à bit entre deux images. Utile pour le masquage.
- **Commande :** `--and`, `--or`, `--xor`
- **Test :**
  1. Créer un masque binaire (noir et blanc).
  2. Appliquer ce masque sur l'image originale.
  ```bash
  # Étape 1 : Créer un masque (seuil à 128)
  ./bin/imgproc --input lena.pgm --output masque.pgm --threshold 128
  # Étape 2 : Appliquer le masque (ET logique)
  ./bin/imgproc --input lena.pgm --output lena_masquee.pgm --and masque.pgm
  ```

### 10. Opérations Arithmétiques (Addition, Soustraction)

- **Page du cours :** 27-28
- **Description :**
  - _Addition :_ Augmente la luminance ou fusionne des images.
  - _Soustraction :_ Détecte les différences ou le mouvement.
- **Commande :** `--add`, `--sub`
- **Test :** Soustraire une version floue de l'image originale (Rehaussement de contours manuel).
  ```bash
  # 1. Créer flou
  ./bin/imgproc --input lena.pgm --output lena_flou.pgm --blur 5
  # 2. Soustraire (Original - Flou = Détails)
  ./bin/imgproc --input lena.pgm --output details.pgm --sub lena_flou.pgm
  ```

### 11. Redimensionnement & Interpolation

- **Page du cours :** 32-34
- **Description :** Changement d'échelle (Zoom).
  - _Plus proche voisin :_ Rapide mais pixelisé.
  - _Bilinéaire :_ Plus lent mais lisse (moyenne pondérée).
- **Commande :** `--resize <w> <h>` (+ `--bilinear`)
- **Test :** Comparer les deux méthodes en zoomant x2.

  ```bash
  # Méthode "Plus proche voisin" (pixelisé)
  ./bin/imgproc --input lena.pgm --output zoom_nn.pgm --resize 1024 1024

  # Méthode "Bilinéaire" (lisse)
  ./bin/imgproc --input lena.pgm --output zoom_bili.pgm --resize 1024 1024 --bilinear
  ```

---

## 📙 Chapitre 3 : Filtrage Spatial (Convolution & Ordre)

Ce chapitre traite des méthodes pour lisser l'image, supprimer le bruit ou accentuer les traits en utilisant le voisinage des pixels.

### 1. Le Filtre Moyenneur (Mean Filter)

- **Page du cours :** 15-17
- **Principe :** Convolution avec un noyau dont tous les coefficients valent $1/N$. Lisse l'image mais floute les bords.
- **Commande :** `--blur <taille>`
- **Test :** Appliquer un flou 5x5 sur Lena.
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_mean.pgm --blur 5
  ```

### 2. Le Filtre Gaussien

- **Page du cours :** 18-19
- **Principe :** Convolution avec un noyau en forme de cloche (Gaussienne). Donne un flou plus naturel et préserve mieux les structures que le moyenneur.
- **Commande :** `--gaussian-blur <taille>`
- **Test :** Comparer avec le moyenneur pour voir la différence de qualité.
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_gauss.pgm --gaussian-blur 5
  ```

### 3. Le Filtre Médian (Non-linéaire)

- **Page du cours :** 20-22
- **Principe :** Remplace chaque pixel par la médiane de ses voisins.
- **Usage clé :** C'est l'arme absolue contre le bruit "Poivre et Sel" (points blancs et noirs aléatoires) sans flouter les contours nets.
- **Commande :** `--median <taille>`
- **Test :** Si possible, utiliser une image bruitée. Sinon, tester sur Lena.
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_median.pgm --median 3
  ```

### 4. Filtres Min et Max (Filtres d'Ordre)

- **Page du cours :** 24 (Visibles dans les exemples "Min V8" et "Max V8")
- **Principe :**
  - **Min :** Prend la valeur minimale du voisinage (assombrit, érosion).
  - **Max :** Prend la valeur maximale du voisinage (éclaircit, dilatation).
- **Commande :** `--min <taille>` et `--max <taille>` _(Si implémentés)_
- **Test :** Observer l'effet d'érosion (les traits noirs grossissent) et de dilatation (les traits blancs grossissent).

  ```bash
  # Érosion (Min)
  ./bin/imgproc --input lena.pgm --output lena_min.pgm --min 3

  # Dilatation (Max)
  ./bin/imgproc --input lena.pgm --output lena_max.pgm --max 3
  ```

### 5. Gestion des Bords

- **Page du cours :** 11-12
- **Note technique :** Le cours évoque plusieurs stratégies (Mise à zéro, Miroir).
- **Dans ce projet :** Nous utilisons la **Réplication des bords** (_Clamp to edge_). Si le filtre dépasse de l'image, on utilise la valeur du pixel le plus proche à l'intérieur de l'image. Cela évite les cadres noirs artificiels.

### 6. Détection de Contours (Sobel / Prewitt / Roberts)

- **Concept :** Calcul du gradient de l'image (dérivée spatiale).
- **Commande :** `--sobel`, `--prewitt`, `--roberts`
- **Test :** Détecter les contours avec Sobel.
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_sobel.pgm --sobel
  ```

### 7. Rehaussement de Netteté (Sharpening)

- **Concept :** Utilisation d'un Laplacien pour accentuer les transitions.
- **Commande :** `--sharpen`
- **Test :** Rendre l'image plus "piquée".
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_sharp.pgm --sharpen
  ```

---

## 📘 Chapitre 4 : Le Domaine Fréquentiel

Traitement de l'image en tant que signal, en passant par la Transformée de Fourier (FFT).

### 1. Transformée de Fourier et Spectre

- **Page du cours :** 7-8
- **Principe :** Convertit l'image spatiale en fréquences. Les points au centre sont les basses fréquences (zones homogènes), les points éloignés sont les hautes fréquences (détails).
- **Commande :** `--fft-spectrum <sortie>`
- **Test :** Visualiser le spectre de Lena.
  ```bash
  ./bin/imgproc --input lena.pgm --output out.pgm --fft-spectrum spectre_lena.pgm
  ```

### 2. Relation Rotation Spatiale / Fréquentielle

- **Page du cours :** 16-17
- **Concept :** La rotation d'une image entraîne la rotation identique de son spectre de Fourier.
- **Commande :** `--rotate <angle>`
- **Test :**
  1. Générer le spectre de l'image normale.
  2. Tourner l'image de 45 degrés.
  3. Générer le spectre de l'image tournée et comparer.
  ```bash
  # Étape 2 & 3
  ./bin/imgproc --input lena.pgm --output lena_rot.pgm --rotate 45 --fft-spectrum spectre_rot.pgm
  ```

### 3. Filtre Passe-Bas (Lissage Spectral)

- **Page du cours :** 21-22
- **Concept :** On garde le centre du spectre (cercle blanc), on met le reste à noir. Résultat : image très floue.
- **Commande :** `--fft-lowpass <rayon>`
- **Test :**
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_low.pgm --fft-lowpass 30
  ```

### 4. Filtre Passe-Haut (Contours)

- **Page du cours :** 26-28
- **Concept :** On met le centre du spectre à noir, on garde l'extérieur. Résultat : image noire avec contours blancs.
- **Commande :** `--fft-highpass <rayon>`
- **Test :**
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_high.pgm --fft-highpass 20
  ```

### 5. Rehaussement de Contraste (High Frequency Emphasis)

- **Page du cours :** 29-31 (Exemple Radiographie)
- **Concept :** Contrairement au passe-haut simple qui supprime le fond, ici on **garde** les basses fréquences (k_low = 1) et on **amplifie** les hautes fréquences (k_high > 1).
- **Commande :** `--fft-emphasis <rayon> <k_low> <k_high>`
- **Test :** Rendre l'image plus "piquée" et contrastée.
  ```bash
  # Rayon 20, on garde le fond (1.0) et on multiplie les détails par 2.0
  ./bin/imgproc --input lena.pgm --output lena_sharp_fft.pgm --fft-emphasis 20 1.0 2.0
  ```

### 6. Suppression du Bruit Périodique (Notch)

- **Page du cours :** 24-25
- **Concept :** Supprime des pics brillants spécifiques dans le spectre (bruit répétitif).
- **Commande :** `--auto-notch <rayon>`
- **Test :** (Nécessite une image avec bruit périodique, sinon test sur Lena pour voir si ça ne dégrade pas trop).
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_notch.pgm --auto-notch 5
  ```

## 📕 Chapitre 5 : Détection de Contours et Hough

Ce chapitre traite de l'extraction d'informations structurelles de l'image : d'abord les contours locaux (variations brusques d'intensité), puis les formes globales (lignes droites).

### 1. Opérateurs de Gradient (1ère Dérivée)

- **Page du cours :** 9-18
- **Principe :** Ces filtres calculent le gradient de l'image (la vitesse de changement de l'intensité). Les contours correspondent aux maxima locaux du gradient.
  - **Sobel (Page 13-14) :** Le plus utilisé. Combine un lissage (pour réduire le bruit) et une dérivée.
  - **Prewitt (Page 13-14) :** Similaire à Sobel mais avec un masque moyenneur simple.
  - **Roberts (Page 12) :** Le plus ancien (1965), utilise un masque 2x2. Très sensible au bruit.
- **Commandes :** `--sobel`, `--prewitt`, `--roberts`
- **Test Comparatif :** Générer les contours de Lena avec les trois méthodes.

  ```bash
  # Sobel (Recommandé)
  ./bin/imgproc --input lena.pgm --output contours_sobel.pgm --sobel

  # Prewitt
  ./bin/imgproc --input lena.pgm --output contours_prewitt.pgm --prewitt

  # Roberts
  ./bin/imgproc --input lena.pgm --output contours_roberts.pgm --roberts
  ```

### 2. Nettoyage des Contours (Seuillage du Gradient)

- **Page du cours :** 19
- **Principe :** Les opérateurs de gradient renvoient des valeurs pour _tous_ les pixels (même les zones plates ont un gradient non-nul à cause du bruit). Le seuillage permet de ne garder que les contours significatifs ("bords forts").
- **Commande :** `--threshold <valeur>` (à utiliser après un filtre de contours)
- **Test :** Comparer une image de contours brute et une image nettoyée.

  ```bash
  # Contours bruts (grisâtre et bruitée)
  ./bin/imgproc --input lena.pgm --output brut.pgm --sobel

  # Contours nettoyés (Binaire : Noir et Blanc pur)
  # Le seuil de 50 élimine le bruit de fond
  ./bin/imgproc --input lena.pgm --output net.pgm --sobel --threshold 50
  ```

### 3. Opérateur Laplacien (2ème Dérivée)

- **Page du cours :** 20-27
- **Principe :** Utilise la dérivée seconde ($\nabla^2 f$). Les contours correspondent aux "passages par zéro" (zero-crossing) de la fonction.
  - Contrairement au gradient qui donne des bords épais, le Laplacien donne des bords plus fins mais doubles.
  - **Avantage :** Isotrope (indépendant de la rotation).
  - **Inconvénient :** Extrêmement sensible au bruit ("bruit poivre et sel").
- **Commande :** `--laplacian`
- **Test :** Appliquer le Laplacien sur une image (idéalement peu bruitée).
  ```bash
  ./bin/imgproc --input lena.pgm --output lena_lap.pgm --laplacian
  ```

### 4. La Transformée de Hough (Détection Globale de Lignes)

- **Page du cours :** 28-41
- **Principe :** Méthode puissante pour relier des pixels de contours isolés et reconstruire des formes géométriques globales (ici, des lignes droites).
  1.  On part d'une image binaire de contours.
  2.  Chaque point blanc "vote" pour toutes les lignes possibles passant par lui dans un espace paramétrique $(\rho, \theta)$.
  3.  Les intersections (pics) dans l'accumulateur indiquent les vraies lignes.
- **Commande :** `--hough <seuil_de_vote>`
  - `<seuil_de_vote>` : Nombre minimum de pixels alignés requis pour valider une ligne (ex: 80 ou 100).
- **Workflow de Test Complet :**
  La transformée de Hough ne fonctionne pas sur une image normale. Elle nécessite impérativement une image binaire en entrée.

  1.  **Détection :** Sobel.
  2.  **Nettoyage :** Threshold (essentiel pour éliminer le bruit qui fausserait les votes).
  3.  **Hough :** Calcul des lignes.

  ```bash
  # Pipeline complet : Sobel -> Seuil -> Hough
  # Le fichier 'hough_accumulator.pgm' sera aussi généré pour visualiser les votes (les sinusoïdes vues page 40).

  ./bin/imgproc --input batiment.pgm --output lignes_detectees.pgm \
    --sobel \
    --threshold 100 \
    --hough 80
  ```

## 📒 Chapitre 6 : Segmentation

La segmentation est l'étape qui permet de passer du traitement d'image (améliorer la qualité) à l'analyse d'image (comprendre le contenu). Elle vise à diviser l'image en régions correspondant aux objets réels.

### 1. Seuillage Global Manuel (Approche Pixel)

- **Page du cours :** 8-12
- **Principe :** La méthode la plus simple. On définit une valeur limite $T$.
  - Si $pixel < T \rightarrow 0$ (Fond/Noir)
  - Si $pixel \ge T \rightarrow 255$ (Objet/Blanc)
  - Rapide mais suppose que l'éclairage est uniforme et que l'objet a un contraste distinct.
- **Commande :** `--threshold <valeur>`
- **Test :** Segmenter Lena manuellement (par essais-erreurs).
  ```bash
  # Essai avec un seuil moyen
  ./bin/imgproc --input lena.pgm --output seg_manuelle.pgm --threshold 128
  ```

### 2. Seuillage Automatique d'Otsu (Histogramme)

- **Page du cours :** 18-20
- **Principe :** Au lieu de deviner le seuil, l'algorithme d'Otsu analyse l'histogramme pour trouver mathématiquement le seuil optimal qui sépare le mieux les deux classes (Fond et Objet) en minimisant la variance à l'intérieur de ces classes.
- **Commande :** `--otsu`
- **Test :** Laisser l'ordinateur trouver le meilleur seuil pour Lena.
  ```bash
  ./bin/imgproc --input lena.pgm --output seg_otsu.pgm --otsu
  ```

### 3. Croissance de Régions (Approche Région)

- **Page du cours :** 40-42
- **Principe :** Au lieu de traiter l'image globalement, on part d'un point précis (germe/seed) et on "inonde" l'image. On ajoute les pixels voisins s'ils sont similaires au germe (écart d'intensité < tolérance).
- **Utilité :** Idéal pour extraire un objet unique dans un environnement complexe ou bruité où le seuillage échoue.
- **Commande :** `--region-growing <x> <y> <tolerance>`
- **Test :** Isoler une zone uniforme (ex: le chapeau ou une partie du visage).
  1. Identifiez les coordonnées d'un point (ex: x=250, y=250).
  2. Lancez la segmentation avec une tolérance (ex: 20 niveaux de gris).
  ```bash
  ./bin/imgproc --input lena.pgm --output region.pgm --region-growing 250 250 20
  ```

### 4. Nettoyage post-segmentation (Morphologie/Filtrage)

- **Page du cours :** 33 ("Il faut nettoyer les résultats... éliminer les pixels seuls")
- **Principe :** Le seuillage brut laisse souvent du bruit ("trous" dans l'objet ou "points" isolés dans le fond). On utilise des filtres spatiaux (vus au Chapitre 3) sur l'image binaire pour nettoyer le masque.
  - **Filtre Médian :** Supprime les points isolés et lisse les contours.
  - **Min (Érosion) :** Supprime les petits objets blancs.
  - **Max (Dilatation) :** Comble les petits trous noirs.
- **Commandes :** `--median`, `--min`, `--max` (à appliquer après un seuillage).
- **Test :** Segmentation propre (Seuillage + Nettoyage).
  ```bash
  # 1. Seuillage Otsu (produit une image binaire potentiellement bruitée)
  # 2. Filtre Médian (nettoie le masque binaire)
  ./bin/imgproc --input lena.pgm --output seg_propre.pgm --otsu --median 3
  ```

## 📘 Chapitre 7 : Images Binaires & Morphologie

La morphologie mathématique est un ensemble d'outils pour traiter la forme des objets dans une image, principalement utilisé sur les images binaires (résultat de segmentation).

### 1. Érosion et Dilatation

- **Page du cours :** 30-34
- **Principe :**
  - **Érosion :** Utilise un filtre Min. Rétrécit les objets blancs, agrandit les trous noirs.
  - **Dilatation :** Utilise un filtre Max. Agrandit les objets blancs, bouche les trous noirs.
- **Commande :** `--erode <taille>`, `--dilate <taille>`
- **Test :** Créer un masque binaire et le modifier.

  ```bash
  # Créer un masque binaire
  ./bin/imgproc --input lena.pgm --output bin.pgm --threshold 100

  # Tester l'érosion (les traits s'affinent)
  ./bin/imgproc --input bin.pgm --output eroded.pgm --erode 5

  # Tester la dilatation (les traits s'épaississent)
  ./bin/imgproc --input bin.pgm --output dilated.pgm --dilate 5
  ```

### 2. Ouverture (Opening)

- **Page du cours :** 35-37
- **Principe :** Érosion suivie d'une Dilatation.
- **Effet :** Supprime les petits objets blancs (bruit) sans changer la taille des gros objets. C'est un "nettoyage".
- **Commande :** `--opening <taille>`
- **Test :** Supprimer du bruit blanc.
  ```bash
  ./bin/imgproc --input bruit_blanc.pgm --output clean.pgm --opening 3
  ```

### 3. Fermeture (Closing)

- **Page du cours :** 35-37
- **Principe :** Dilatation suivie d'une Érosion.
- **Effet :** Bouche les petits trous noirs à l'intérieur des objets blancs et relie les objets proches, sans changer leur taille globale.
- **Commande :** `--closing <taille>`
- **Test :** Combler les trous dans une segmentation imparfaite.
  ```bash
  ./bin/imgproc --input trous.pgm --output plein.pgm --closing 5
  ```

### 4. Gradient Morphologique

- **Page du cours :** 40-41
- **Principe :** Différence entre l'image dilatée et l'image érodée ($Dilatation - Erosion$).
- **Effet :** Extrait le contour exact des objets binaires (l'épaisseur dépend de la taille de l'élément structurant).
- **Commande :** `--morph-gradient <taille>`
- **Test :** Extraire les contours d'une forme binaire.
  ```bash
  ./bin/imgproc --input bin.pgm --output contours_morpho.pgm --morph-gradient 3
  ```

```

```
