/*
 * nesutils.cpp
 *
 *  Created on: 7 de jul. de 2015
 *      Author: ibon
 */

int getBit(int word, int bitNumber) {
    return (word >> bitNumber) & 0x00000001;
}


int setBit(int word, int bitNumber, int bitValue) {
    int mask = 0x00000001;
    mask = mask << bitNumber;
    int result;

    if (bitValue)
        result = word | mask;
    else
    	result = word & (mask ^ 0xFFFFFFFF);

    return result;
}


int setBits(int word, int mask, int value) {
    return (word & mask) | value;
}


int c2_to_int(int byte) {
	if (byte & 0x80)
        return -(0x100 - byte);
    else
        return byte;
}


int intToC2(int n) {
	int c2;

	if (n >= 0)
        c2 = n;
    else
        c2 = (~n + 1) & 0xFF;

    return c2;
}

