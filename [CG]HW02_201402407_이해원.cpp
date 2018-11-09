#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

// Stack 안에 담기는 matrix와 vertex 구조체 정의.
typedef struct CG_Stack {
	double matrix[4][4];
	double vertex[4]; // MyRunMatrix(v)에 사용하는 vertex
}Stack;

enum MatrixMode { // enum으로 MatrixMode 변수 설정.
	ModelView_Mode,
	Projection_Mode
};

static Stack stack[10]; // 임의의 스택 전체 크기를 10으로 정적 변수로 설정.
static double PI = 3.141592; // 파이값 설정.
int size = 0; // 현재 스택 크기. 
int top_point = -1; // 스택의 탑을 가리키는 포인트. -1은 스택에 아무 것도 없을 때
MatrixMode current_matrix_mode;

void // matrix 값 복사하는 함수
copy_matrix(double (*start)[4], double (*end)[4]) {
	int i;
	int j;
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			end[i][j] = start[i][j];
		}
	}
}

void // vertex 값 복사하는 함수
copy_vertex(double *start, double *end) {
	int i;

	for (i = 0; i < 4; i++) {
		end[i] = start[i];
	}

}


void // matrix 출력하는 함수. 첫 번째는 matrix 배열 , 두 번째는 제목 char* 값.
print_matrix(double(*matrix)[4]) {
	int i;
	int j;

	std::cout << std::endl;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++) {
			std::cout << matrix[i][j] << "	";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void // matrix의 소수점까지 출력하는 함수.
print_double_matrix(double(*matrix)[4]) {
	int i;
	int j;

	std::cout << std::endl;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++) {
			printf("%.2f	", matrix[i][j]);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void // vertex 출력하는 함수. 첫 번째는 matrix 배열 , 두 번째는 제목 char* 값.
print_vertex(double *vertex) {
	int i;

	std::cout << std::endl;

		for (i = 0; i < 4; i++) {
			std::cout << vertex[i] << "	";
		}
	std::cout << std::endl;
	std::cout << std::endl;
}

void // 현재 Matrix의 Mode를 설정해주는 함수. 여기서는 ModelView 와 Projection 둘 다 identify 로 해야 하는데 0으로 설정함. identity 설정이 있으므로.
mySetMatrixMode(MatrixMode mode) {
	current_matrix_mode = mode; // Mode 설정.
	int i;
	int j;

	if (size == 0) { // stack에 아무것도 없는 경우. matrix와 vertex를 0으로 초기화.
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				stack[size].matrix[i][j] = 0;	
			}
			// 스택에 존재하던 vertrx도 0으로 초기화.
			stack[size].vertex[i] = 0;	
		}
	}

	// 스택에 쌓여있는 것들 전부 0으로 초기화. 0으로 초기화 안해도 들어갈 수 있나 ?
	while (size > 0) {

		// 스택에 존재하던 matrix 0으로 초기화.
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				stack[size - 1].matrix[i][j] = 0;
			}
			// 스택에 존재하던 vertrx도 0으로 초기화.
			stack[size - 1].vertex[i] = 0;
		}
		size--;
	}
	top_point = -1;
}

void // 현재 스택에 있는 matrix를 identity matrix로 변환한 뒤 load하는 함수.
myLoadIdentity() {
	int i;
	int j;
	// 임시의 identity matrix 생성.
	double identity_matrix[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double identity_vertex[4] = { 1, 1, 1, 1 }; // 임시로 identity vertex가 존재한다고 가정(테스트를 위해)

	if (top_point == -1) {
		copy_matrix(identity_matrix, stack[++top_point].matrix); // 스택에 아무것도 없으면 top을 저걸로 변경. 그치만 push는 아니므로 size는 증가시키지 않는다.
		copy_vertex(identity_vertex, stack[top_point].vertex);
	}
	else {
		copy_matrix(identity_matrix, stack[top_point].matrix); // 현재 스택의 탑에 있는 matrix를 identity로 변경.
		copy_vertex(identity_vertex, stack[top_point].vertex);
	}
}

void // 현재 스택에 matrix를 하나 푸시하는 함수.
myPushMatrix() {
	size++; // 사이즈 추가.
	top_point = size - 1; // top point 값 변경.
	if (top_point > 0) { // 첫 push인 경우는 top_point가 0인 경우이므로 그 때는 맨 처음 초기화 한 것으로 이전에 스택에 있는 것이 없어서 넘긴다.
		copy_matrix(stack[top_point - 1].matrix, stack[top_point].matrix); // 현재 stack의 size 1 증가시키고 이전 matrix를 top으로 복사.	
		copy_vertex(stack[top_point - 1].vertex, stack[top_point].vertex);
	}
}

void // 현재 스택에서 matrix 하나 팝 하는 함수.
myPopMatrix() {
	double init_matrix[4][4] = // 임시의 초기 matrix 생성.
	{ { 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 } };

	copy_matrix(init_matrix, stack[top_point--].matrix); // Push이므로 top에 위 임시 초기 matrix 넣고 설정.
	size--; // 사이즈 추가.
	
}

void // 인자로 받은 matrix를 stack의 top에 있는 것과 곱하고 곱한 값을 top과 대체. (단, matrix만 곱할 수 있다.) 
myMatrixMul(double matrix[4][4]) {
	int i;
	int j;
	int k;
	double sum;
	double replace[4][4];

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			sum = 0;
			for (k = 0; k < 4; k++) {
				
				sum += stack[top_point].matrix[i][k] * matrix[k][j];
				
			}
			replace[i][j] = sum;
		}
	}

	copy_matrix(replace, stack[top_point].matrix); // 스택의 top에 있는 matrix 를 곱한 matrix 값으로 바꾸기.
}

void // 인자로 받은 vertex를 현재 스택의 top 값과 곱하고 vertex값을 바꾸는 함수. ( 단, matrix만 곱할 수 있다.)
myRunMatrix(double vertex[4]) {
	int i;
	int j;
	int sum;
	double replace[4];
	for (i = 0; i < 4; i++)
	{
		sum = 0;
		for (j = 0; j < 4; j++) {
			sum += stack[top_point].matrix[i][j] * vertex[j];
		}
		replace[i] = sum;
	}

	copy_vertex(replace, stack[top_point].vertex);
}

void // 현재 스택의 top * translateX matrix를 곱해주고 top matrix 교체.
myTranslateX(int size) {
	double translateX[4][4] =
	{ { 1, 0, 0, size },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 } };

	myMatrixMul(translateX);
}

void
myTranslateY(int size) {
	double translateY[4][4] =
	{ { 1, 0, 0, 0 },
	{ 0, 1, 0, size },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 } };

	myMatrixMul(translateY);
}

void
myTranslateZ(int size) {
	double translateZ[4][4] =
	{ { 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, size },
	{ 0, 0, 0, 1 } };

	myMatrixMul(translateZ);
}

void // 현재 스택의 top * rotateX matrix를 곱해주고 top matrix 교체.
myRotateX(double degree) {
	double radian = degree * (PI / 180);
	
	double rotateX[4][4] = { 
		{ 1, 0, 0, 0 },
	{ 0, cos(radian), -sin(radian), 0 },
	{ 0, sin(radian), cos(radian), 0 },
	{ 0, 0, 0, 1 } 
	};
	myMatrixMul(rotateX);
}

void
myRotateY(double degree) {
	double radian = degree * (PI / 180);
	double rotateY[4][4] =
	{ { cos(radian), 0, sin(radian), 0 },
	{ 0, 1, 0, 0 },
	{ -sin(radian), 0, cos(radian), 0 },
	{ 0, 0, 0, 1 } };
	
	myMatrixMul(rotateY);
}

void
myRotateZ(double degree) {
	double radian = degree * (PI / 180);
	double rotateZ[4][4] =
	{ { cos(radian), -sin(radian), 0, 0 },
	{ sin(radian), cos(radian), 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 } };

	myMatrixMul(rotateZ);
}

void // 현재 스택의 top * scaleX matrix를 곱해주고 top matrix 교체.
myScaleX(int scale) {
	double scaleX[4][4] =
	{ { scale, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 } };

	myMatrixMul(scaleX);

}

void
myScaleY(int scale) {
	double scaleY[4][4] =
	{ { 1, 0, 0, 0 },
	{ 0, scale, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 } };

	myMatrixMul(scaleY);

}

void
myScaleZ(int scale) {
	double scaleZ[4][4] =
	{ { 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, scale, 0 },
	{ 0, 0, 0, 1 } };

	myMatrixMul(scaleZ);

}

int main() {
	
	printf("########## Lab03 test start ##########\n");
	printf("# SetMatrixMode ModelView setting... \n");
	mySetMatrixMode(ModelView_Mode);
	printf("-> Success!! \n");
	printf("# Current Stack Matrix \n");
	print_matrix(stack[top_point].matrix);

	printf("# Identity Matrix loading ... \n");
	myLoadIdentity();
	printf("-> Success!! \n");
	printf("# Current Stack Matrix \n");
	print_matrix(stack[top_point].matrix);

	printf("# Push \n");
	printf("-> Success!! \n");
	printf("# Current Stack Matrix \n");
	myPushMatrix();
	print_matrix(stack[top_point].matrix);

	printf("# Pop \n");
	printf("-> Success!! \n");
	printf("# Current Stack Matrix \n");
	myPopMatrix();
	print_matrix(stack[top_point].matrix);

	printf("# Identity Matrix loading ... \n");
	myLoadIdentity();
	myPushMatrix();
	printf("# MatrixMul :: \n");
	double temp_matrix[4][4] = {
		{ 1, 2, 2, 1 },
		{ 2, 1, 1, 2 },
		{ 2, 1, 1, 2 },
		{ 1, 2, 2, 1 }
	};
	print_matrix(temp_matrix);
	printf("# Current Stack Matrix \n");
	print_matrix(stack[top_point].matrix);
	myMatrixMul(temp_matrix);
	printf("-> Success !! \n");
	print_matrix(stack[top_point].matrix);

	printf("# Identity Vertex loading ... \n");
	myLoadIdentity();
	myPushMatrix();
	double temp_vertex[4] = { 1, 2, 3, 4 };
	print_vertex(temp_vertex);
	myRunMatrix(temp_vertex);
	printf("-> Success!! \n");
	printf("# Current Stack Vertex \n");
	print_vertex(stack[top_point].vertex);
	
	printf("# TranslateX -> 10 \n");
	myLoadIdentity();
	myTranslateX(10);
	print_matrix(stack[top_point].matrix);
	printf("# TranslateY -> 20 \n");
	myLoadIdentity();
	myTranslateY(20);
	print_matrix(stack[top_point].matrix);
	printf("# TranslateZ -> 25 \n");
	myLoadIdentity();
	myTranslateZ(25);
	print_matrix(stack[top_point].matrix);
	
	printf("# RotateX -> 180 \n");
	myLoadIdentity();
	myRotateX(180);
	print_double_matrix(stack[top_point].matrix);
	printf("# RotateY -> 45 \n");
	myLoadIdentity();
	myRotateY(45);
	print_double_matrix(stack[top_point].matrix);
	printf("# RotateZ -> 90 \n");
	myLoadIdentity();
	myRotateZ(90);
	print_double_matrix(stack[top_point].matrix);
	
	printf("# ScaleX -> 20 \n");
	myLoadIdentity();
	myScaleX(20);
	print_matrix(stack[top_point].matrix);
	printf("# ScaleY -> 40 \n");
	myLoadIdentity();
	myScaleY(40);
	print_matrix(stack[top_point].matrix);

	printf("# ScaleZ -> 30 \n");
	myLoadIdentity();
	myScaleZ(30);
	print_matrix(stack[top_point].matrix);
	
	return 0;
}
