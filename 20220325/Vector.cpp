#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
 #include <ctime>
 
typedef float real;

/*
class Vector
{
	public:
		real x;
		real y;
		real z;
		
	private:
		real pad;
	}
	
	*/
	
//	/*
struct Vector
{
	float x;
	float y;
	float z;
};
//	*/	
	
Vector Vec_plus(Vector v1, Vector v2)
{
	Vector vp;
	vp.x = v1.x + v2.x;
	vp.y = v1.y + v2.y;
	vp.z = v1.z + v2.z;
	
	return vp;
}

Vector Vec_minus(Vector v1, Vector v2)
{
	Vector vm;
	vm.x = v1.x - v2.x;
	vm.y = v1.y - v2.y;
	vm.z = v1.z - v2.z;
	
	return vm;
}

Vector Scalar (Vector v1, int n)  // scalar
{
	Vector v_scalar;
	v1.x = v1.x * n;
	v1.y = v1.y * n;
	v1.z = v1.z * n;
	
	return v_scalar;
}

float in_product(Vector v1, Vector v2)  // in
{
	float in_v = 0;
	
	in_v = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	
	return in_v;
}


Vector out_product(Vector v1, Vector v2)  // out
{
	Vector out_v;
	out_v.x = v1.y * v2.z - v1.z * v2.y;
	out_v.y = v1.z * v2.x - v1.x * v2.z;
	out_v.z = v1.x * v2.y - v1.y * v2.x;
	
	return out_v;
}

/*
class Matrix
{
	public:
		real data[9];
		
	public:
	Matrix(){}
	Matrix(Vector v)
	{
		data[0]; data[1]; data[2];
		data[3]; data[4]; data[5];
		data[6]; data[7]; data[8];
	}
};
*/


int main()
{
	Vector v1;
	Vector v2;
	int n = rand()%10+1;
	v1.x = rand()%10+1;
	v1.y = rand()%10+1;
	v1.z = rand()%10+1;
	
	v2.x = rand()%10+1;
	v2.y = rand()%10+1; 
	v2.z = rand()%10+1;
 
	printf("Vector v1. x = %.2f | y = %.2f | z = %.2f |\n", v1.x, v1.y, v1.z);
	printf("Vector v2. x = %.2f | y = %.2f | z = %.2f |\n\n", v2.x, v2.y, v2.z);
	
	printf("----- Vector result -----\n");
	
	Vector vp = Vec_plus(v1, v2);
	Vector vm = Vec_minus(v1, v2);
	Vector v_scalar = Scalar(v1, n);
	float in_v = in_product(v1, v2);
	Vector out_v = out_product(v1, v2);
	
	printf("PLUS | x = %.2f | y = %.2f | z = %.2f |\n", vp.x, vp.y, vp.z);
	printf("MINUS | x = %.2f | y = %.2f | z = %.2f |\n", vm.x, vm.y, vm.z);
	printf("SCALAR | x = %.2f | y = %.2f | z = %.2f |\n", v_scalar.x, v_scalar.y, v_scalar.z);
	printf("IN_Product : %.2f  |\n",in_v);
	printf("OUT_Product : x = %.2f | y = %.2f | z = %.2f |\n", out_v.x, out_v.y, out_v.z);
	
	return 0;
}
