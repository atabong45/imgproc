#include "fft/complex.h"
#include <math.h>

Complex complex_add(Complex a, Complex b) {
    Complex result = {a.real + b.real, a.imag + b.imag};
    return result;
}

Complex complex_sub(Complex a, Complex b) {
    Complex result = {a.real - b.real, a.imag - b.imag};
    return result;
}

// Multiplication : (a + ib) * (c + id) = (ac - bd) + i(ad + bc)
Complex complex_mul(Complex a, Complex b) {
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

// Magnitude : |z| = sqrt(a^2 + b^2)
double complex_magnitude(Complex c) {
    return sqrt(c.real * c.real + c.imag * c.imag);
}