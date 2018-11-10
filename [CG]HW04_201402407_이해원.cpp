#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
// 201402407 이해원
using namespace std;

// enum으로 mySetLight 함수 인자의 PROP 설정.
enum PROP { 
	AMB,
	DIFF,
	SPEC,
	EMI,
	POS
};

// Stack 안에 담기는 vector 구조체 정의.
struct Light {
	double ambient[3];
	double diffuse[3];
	double specular[3];
	double vector[3]; // 초기 color Vector 값
	double Phong[3]; // I 결과 값.
	PROP prop;
};

static double Ka_vector[3] = { 0.5, 0.5, 0.5 };
static double Kd_vector[3] = { 0.2, 0.2, 0.2 };
static double Ks_vector[3] = { 0.5, 0.3, 0.5 };

double Ia_vector[3] = { 0.5, 0.3, 0.5 };
double Id_vector[3] = { 0.2, 0.5, 0.3 };
double Is_vector[3] = { 0.5, 0.3, 0.3 };

double light_vector[3] = { 0.2, 0.3, 0.4 };
double normal_vector[3] = { 0.5, 0.5, 0.5 };
double view_vector[3] = { 0.4, 0.4, 0.4 };
double reflect_vector[3] = { 0.6, 0.6, 0.6 };

struct Light myLight[5]; // ID값에 따른 Light 구조체.

static double attenuation;
static double color[3] = { 1.0, 1.0, 1.0 };

enum LIGHT_ID { // enum으로 mySetLight 함수 인자의 PROP 설정.
	myLight0,
	myLight1,
	myLight2,
	myLight3,
	myLight4
};

void copy_vector(double (*start), double (*end)) {
	int i;

	for (i = 0; i < 3; i++) {
			end[i] = start[i];
		}
}

int select_LightID(LIGHT_ID ID) {
	switch (ID) {
	case myLight0:
		return 0;
		break;
	case myLight1:
		return 1;
		break;
	case myLight2:
		return 2;
		break;
	case myLight3:
		return 3;
		break;
	case myLight4:
		return 4;
		break;
	}
}

void mySetLight(LIGHT_ID ID, PROP prop, double *coef) {
	int id = select_LightID(ID); // ID값 분류해서 가져옴
	myLight[id].prop = prop; // PROP 설정
	
	// 초기화
	int i;
	for (i = 0; i < 3; i++) {
		myLight[id].ambient[i] = 0;
		myLight[id].diffuse[i] = 0;
		myLight[id].specular[i] = 0;
		myLight[id].vector[i] = coef[i];
	}
}

void myAmbient(LIGHT_ID ID,	double *coef) {
	int id = select_LightID(ID); // ID값 분류해서 가져옴

	int i;
	double temp[3];
	for (i = 0; i < 3; i++) {
		 temp[i] = Ka_vector[i] * coef[i];
	}
	copy_vector(temp, myLight[id].ambient);
}

void myDiffuse(LIGHT_ID ID, double *coef) {
	int id = select_LightID(ID); // ID값 분류해서 가져옴

	int i;
	double scalar = 0;
	double temp[3];

	for (i = 0; i < 3; i++) {
		temp[i] = Kd_vector[i] * coef[i];
		scalar += light_vector[i] * normal_vector[i];
	}

	for (i = 0; i < 3; i++) {
		temp[i] = temp[i] * scalar;
	}
	copy_vector(temp, myLight[id].diffuse);
}

void mySpecular(LIGHT_ID ID, double *coef, double attenuation) {
	int id = select_LightID(ID); // ID값 분류해서 가져옴

	int i;
	double scalar = 0;
	double temp[3];

	for (i = 0; i < 3; i++) {
		temp[i] = Ks_vector[i] * coef[i];
		scalar += view_vector[i] * reflect_vector[i];
	}

	scalar = pow(scalar, attenuation);
	for (i = 0; i < 3; i++) {
		temp[i] = temp[i] * scalar;
	}

	copy_vector(temp, myLight[id].specular);
}

double myAttenuation(double ka, double kb, double kc, double dist) {
	double a, b, c, d;
	a = ka;
	b = kb;
	c = kc;
	d = dist;
	return 1 / (a + (b * d) + (c * (d * d)));
}

void myLighting(LIGHT_ID ID) {
	int id = select_LightID(ID); // ID값 분류해서 가져옴

	double I[3];
	int i;
	for (i = 0; i < 3; i++) {
		I[i] = myLight[id].ambient[i] + myLight[id].diffuse[i] + myLight[id].specular[i];
		
	}

	copy_vector(I, myLight[id].Phong);
}

void print_vector(char* string, double *vector) {

	cout << string << " {  ";
	int i;
	for (i = 0; i < 3; i++) {
		printf("%.4f", vector[i]);
		if (i == 2) {
			cout << "  ";
			break;
		}
		cout << "   ";
	}
	cout << "}" << endl;
	cout << endl;
}

int main() {

	cout << "##### mySetLight() 호출 #####" << endl;
	cout << "-> { 1.0 , 1.0 , 1.0 } 을 인자로 호출" << endl;
	int ID = select_LightID(myLight0);
	mySetLight(myLight0, AMB, color);
	print_vector("-> coef :", myLight[ID].vector);

	cout << "##### myAmbient() 호출 #####" << endl;
	myAmbient(myLight0, Ia_vector);
	print_vector("-> Ka : ", Ka_vector);
	print_vector("-> Ia : ", Ia_vector);
	print_vector("-> result ambient : " ,myLight[ID].ambient);

	cout << "##### myDiffuse() 호출 #####" << endl;
	myDiffuse(myLight0, Id_vector);
	print_vector("-> Kd : ", Kd_vector);
	print_vector("-> Id : ", Id_vector);
	print_vector("-> result diffuse : ", myLight[ID].diffuse);


	cout << "##### myAttenuation() 호출 #####" << endl;
	double attenuation = myAttenuation(1.0, 1.0, 1.0, 2.0);
	cout << endl;
	cout << "-> { 1.0 , 1.0 , 1.0 , 2.0 } 을 myAttenuation()에 대입" << endl;
	cout << "-> Attenuation : " << attenuation << endl;
	cout << endl;

	cout << "##### mySpecular() 호출 #####" << endl;
	mySpecular(myLight0, Is_vector, attenuation);
	print_vector("-> Ks : ", Ks_vector);
	print_vector("-> Is : ", Is_vector);
	print_vector("-> result diffuse : ", myLight[ID].specular);

	cout << "##### myLighting() 호출 #####" << endl;
	myLighting(myLight0);
	print_vector("-> result Phong Model : ", myLight[ID].Phong);
	return 0;
}