
#include "nicefp.h"
#include <math.h>

namespace geom {
	// wrapper for doubles to avoid mistakes made in fp comparison

	nicefp::nicefp(double avalue) {

		value = avalue;
	}


	nicefp nicefp::operator+ (const nicefp& other) {return nicefp((value + other.value));}
	nicefp nicefp::operator- (const nicefp& other) {return nicefp((value - other.value));}
	nicefp nicefp::operator- () {return nicefp(-value);}
	nicefp nicefp::operator* (const nicefp& other) {return nicefp((value * other.value));}
	nicefp nicefp::operator/ (const nicefp& other) {return nicefp((value / other.value));}

	nicefp nicefp::operator+ (const double other) {return value + other;}
	nicefp nicefp::operator- (const double other) {return value - other;}
	nicefp nicefp::operator* (const double other) {return value * other;}
	nicefp nicefp::operator/ (const double other) {return value / other;}

	bool nicefp::operator< (const nicefp& other) {

		double difference = value - other.value;
		if (difference < FPCUTOFF) return true;
		return false; 

	}
	
	bool nicefp::operator== (const nicefp& other) {

		double difference = value - other.value;

		if (difference < FPCUTOFF && difference > -FPCUTOFF) return true;
		return false;
	}

	nicefp sqrt(nicefp x) {

		return nicefp(sqrt(x.value));
	}


	// calculate the negative exp of a nicecp;
	nicefp nicenexp(nicefp x) {

		if (x.value = 0) return 1;
		else return exp(-x.value);

	}
}
