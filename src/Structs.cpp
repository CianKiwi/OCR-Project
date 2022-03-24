#include "Structs.h"

bool operator==(const CardinalBool& a, const CardinalBool& b){
	bool equal = true;
	for (int x = 0; x < 4; x++) {
		if (a.data[x] != b.data[x]){
			equal = false;
		}
	}
	return equal;
}

bool operator != (const CardinalBool& a, const CardinalBool& b){
	bool unequal = false;
	for (int x = 0; x < 4; x++) {
		if (a.data[x] != b.data[x]) {
			unequal = true;
		}
	}
	return unequal;
}

void CardinalBool::operator = (const CardinalBool& a){
	for (int x = 0; x < 4; x++){
		data[x] = a.data[x];
	}
}

CardinalBool::CardinalBool(){

}
CardinalBool::CardinalBool(bool n, bool e, bool s, bool w){
	N = n;
	E = e;
	S = s;
	W = w;
}