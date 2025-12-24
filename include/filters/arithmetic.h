#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include "core/image.h"

// Types d'opérations
typedef enum { OP_ADD, OP_SUB, OP_MUL, OP_AND, OP_OR, OP_XOR } ArithmeticOp;

/**
 * @brief Combine deux images pixel par pixel.
 * Les images doivent avoir les mêmes dimensions.
 * 
 * @param src1 Première image (sera modifiée par le résultat).
 * @param src2 Seconde image (lecture seule).
 * @param op Le type d'opération.
 * @return 0 si succès, -1 si dimensions incompatibles.
 */
int apply_arithmetic(Image *src1, const Image *src2, ArithmeticOp op);

#endif