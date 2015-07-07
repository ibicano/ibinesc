/*
 * nesutils.hpp
 *
 * Módulo con distintas funciones de utilidad
 *
 *  Created on: 7 de jul. de 2015
 *      Author: ibon
 */

#ifndef NESUTILS_HPP_
#define NESUTILS_HPP_


/* Devuelve el valor del bit indicado por "bit_number" de la palabra
 especificada por "word" */
int getBit(int word, int bitNumber);


/* Establece el bit especificado por "bit_number" al valor especificado en
 "bit_value" en la palabra "word" */
int setBit(int word, int bitNumber, int bitValue);


int setBits(int word, int mask, int value);


// Devuelve el valor entero de un byte en complemento a 2
int c2ToInt(int byte);


// Convierte un entero a complemento a 2
int intToC2(int n);


#endif /* NESUTILS_HPP_ */
