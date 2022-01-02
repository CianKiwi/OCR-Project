#include "Structs.h"

bool operator==(const CardinalBool& lhs, const CardinalBool& rhs){
	return(
		lhs.N == rhs.N &&
		lhs.E == rhs.E &&
		lhs.S == rhs.S &&
		lhs.W == rhs.W
		);
}
