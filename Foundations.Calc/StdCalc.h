#pragma once

#include <math.h>

#ifdef MATH_EXPORTS
#define MATH_LIB _declspec(dllexport)
#else
#define MATH_LIB _declspec(dllexport)
#endif

namespace Core::Calc
{
	MATH_LIB double MapLerp(double R1h, double R1l, double R2h, double R2l, double R1p); //R1h is range 1 high, R1l is rane 1 low, same for R2, and R1l is the position in range 1. 

	//Core::AaColor HSVtoRGB(double H, double S, double V);

	MATH_LIB int GreatestCommonDenominator(int A, int B);
#define gcd(A, B) (Core::GreatestCommonDenominator(A, B))
}