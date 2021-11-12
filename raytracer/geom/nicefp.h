#pragma once 
namespace geom{

	double constexpr FPCUTOFF = .000001;
	
	struct nicefp {

		double value;

		nicefp(double avalue);

		nicefp() = default;

		nicefp operator+ (const nicefp& other);
		nicefp operator- (const nicefp& other);
		nicefp operator* (const nicefp& other);
		nicefp operator/ (const nicefp& other);
		nicefp operator+ (const double other);
		nicefp operator- (const double other);
		nicefp operator* (const double other);
		nicefp operator/ (const double other);
		nicefp operator-();

		bool operator< (const nicefp& other);
		bool operator== (const nicefp& other);

		

	};

	nicefp sqrt(nicefp x);

	// calculate the negative exp of a nicecp;
	nicefp nicenexp(nicefp x);

}