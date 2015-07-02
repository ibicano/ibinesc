//============================================================================
// Name        : ibinesc.cpp
// Author      : Ibon Cano
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================


#include "NES.hpp"
#include <iostream>
using namespace std;

int main() {
	NES *nes = new NES();

	nes->run();

	return 0;
}
