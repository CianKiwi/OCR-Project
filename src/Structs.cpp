#include "Structs.h"

bool operator==(const CardinalBool& lhs, const CardinalBool& rhs){
	return(
		lhs.N == rhs.N &&
		lhs.E == rhs.E &&
		lhs.S == rhs.S &&
		lhs.W == rhs.W
		);
}

bool operator != (const CardinalBool& a, const CardinalBool& b){
        return  a.N != b.N ||
                a.E != b.E ||
                a.S != b.S ||
                a.W != b.W;
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