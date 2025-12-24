#include "filters/arithmetic.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

int apply_arithmetic(Image *src1, const Image *src2, ArithmeticOp op) {
    if (!src1 || !src2 || src1->width != src2->width || src1->height != src2->height || src1->channels != src2->channels) {
        fprintf(stderr, "Erreur: Les images doivent avoir les mêmes dimensions et canaux pour les opérations arithmétiques.\n");
        return -1;
    }

    long total_size = src1->width * src1->height * src1->channels;

    for (long i = 0; i < total_size; i++) {
        int val1 = src1->data[i];
        int val2 = src2->data[i];
        int res = 0;

        switch (op) {
            case OP_ADD:
                res = val1 + val2; 
                break;
            case OP_SUB:
                res = abs(val1 - val2); // On peut aussi faire abs(val1 - val2) pour voir les différences
                break;
            case OP_MUL:
                // Multiplication normalisée : (A * B) / 255
                res = (val1 * val2) / 255;
                break;
            case OP_AND:
                res = val1 & val2;
                break;
            case OP_OR:
                res = val1 | val2;
                break;
            case OP_XOR:             
                res = val1 ^ val2;
                break;
        }

        // Saturation (Clamping) pour rester entre 0 et 255
        if (res > 255) res = 255;
        if (res < 0) res = 0;

        src1->data[i] = (uint8_t)res;
    }
    return 0;
}