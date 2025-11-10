#ifndef COMPLEX_H
#define COMPLEX_H

// Notre structure pour représenter un nombre complexe z = a + ib
typedef struct {
    double real; // Partie réelle (a)
    double imag; // Partie imaginaire (b)
} Complex;

// Fonctions utilitaires pour l'arithmétique des nombres complexes
Complex complex_add(Complex a, Complex b);
Complex complex_sub(Complex a, Complex b);
Complex complex_mul(Complex a, Complex b);
double complex_magnitude(Complex c);

#endif // COMPLEX_H