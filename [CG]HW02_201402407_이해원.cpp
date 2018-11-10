#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

// Stack �ȿ� ���� matrix�� vertex ����ü ����.
typedef struct CG_Stack {
	double matrix[4][4];
	double vertex[4]; // MyRunMatrix(v)�� ����ϴ� vertex
}Stack;

enum MatrixMode { // enum���� MatrixMode ���� ����.
	ModelView_Mode,
	Projection_Mode
};

static Stack stack[10]; // ������ ���� ��ü ũ�⸦ 10���� ���� ������ ����.
static double PI = 3.141592; // ���̰� ����.
int size = 0; // ���� ���� ũ��. 
int top_point = -1; // ������ ž�� ����Ű�� ����Ʈ. -1�� ���ÿ� �ƹ� �͵� ���� ��
MatrixMode current_matrix_mode;

void // matrix �� �����ϴ� �Լ�
copy_matrix(double (*start)[4], double (*end)[4]) {
	int i;
	int j;
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			end[i][j] = start[i][j];
		}
	}
}

void // vertex �� �����ϴ� �Լ�
copy_vertex(double *start, double *end) {
	int i;

	for (i = 0; i < 4; i++) {
		end[i] = start[i];
	}

}


void // matrix ����ϴ� �Լ�. ù ��°�� matrix �迭 , �� ��°�� ���� char* ��.
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

void // matrix�� �Ҽ������� ����ϴ� �Լ�.
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

void // vertex ����ϴ� �Լ�. ù ��°�� matrix �迭 , �� ��°�� ���� char* ��.
print_vertex(double *vertex) {
	int i;

	std::cout << std::endl;

		for (i = 0; i < 4; i++) {
			std::cout << vertex[i] << "	";
		}
	std::cout << std::endl;
	std::cout << std::endl;
}

void // ���� Matrix�� Mode�� �������ִ� �Լ�. ���⼭�� ModelView �� Projection �� �� identify �� �ؾ� �ϴµ� 0���� ������. identity ������ �����Ƿ�.
mySetMatrixMode(MatrixMode mode) {
	current_matrix_mode = mode; // Mode ����.
	int i;
	int j;

	if (size == 0) { // stack�� �ƹ��͵� ���� ���. matrix�� vertex�� 0���� �ʱ�ȭ.
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				stack[size].matrix[i][j] = 0;	
			}
			// ���ÿ� �����ϴ� vertrx�� 0���� �ʱ�ȭ.
			stack[size].vertex[i] = 0;	
		}
	}

	// ���ÿ� �׿��ִ� �͵� ���� 0���� �ʱ�ȭ. 0���� �ʱ�ȭ ���ص� �� �� �ֳ� ?
	while (size > 0) {

		// ���ÿ� �����ϴ� matrix 0���� �ʱ�ȭ.
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				stack[size - 1].matrix[i][j] = 0;
			}
			// ���ÿ� �����ϴ� vertrx�� 0���� �ʱ�ȭ.
			stack[size - 1].vertex[i] = 0;
		}
		size--;
	}
	top_point = -1;
}

void // ���� ���ÿ� �ִ� matrix�� identity matrix�� ��ȯ�� �� load�ϴ� �Լ�.
myLoadIdentity() {
	int i;
	int j;
	// �ӽ��� identity matrix ����.
	double identity_matrix[4][4] = {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};
	double identity_vertex[4] = { 1, 1, 1, 1 }; // �ӽ÷� identity vertex�� �����Ѵٰ� ����(�׽�Ʈ�� ����)

	if (top_point == -1) {
		copy_matrix(identity_matrix, stack[++top_point].matrix); // ���ÿ� �ƹ��͵� ������ top�� ���ɷ� ����. ��ġ�� push�� �ƴϹǷ� size�� ������Ű�� �ʴ´�.
		copy_vertex(identity_vertex, stack[top_point].vertex);
	}
	else {
		copy_matrix(identity_matrix, stack[top_point].matrix); // ���� ������ ž�� �ִ� matrix�� identity�� ����.
		copy_vertex(identity_vertex, stack[top_point].vertex);
	}
}

void // ���� ���ÿ� matrix�� �ϳ� Ǫ���ϴ� �Լ�.
myPushMatrix() {
	size++; // ������ �߰�.
	top_point = size - 1; // top point �� ����.
	if (top_point > 0) { // ù push�� ���� top_point�� 0�� ����̹Ƿ� �� ���� �� ó�� �ʱ�ȭ �� ������ ������ ���ÿ� �ִ� ���� ��� �ѱ��.
		copy_matrix(stack[top_point - 1].matrix, stack[top_point].matrix); // ���� stack�� size 1 ������Ű�� ���� matrix�� top���� ����.	
		copy_vertex(stack[top_point - 1].vertex, stack[top_point].vertex);
	}
}

void // ���� ���ÿ��� matrix �ϳ� �� �ϴ� �Լ�.
myPopMatrix() {
	double init_matrix[4][4] = // �ӽ��� �ʱ� matrix ����.
	{ { 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 } };

	copy_matrix(init_matrix, stack[top_point--].matrix); // Push�̹Ƿ� top�� �� �ӽ� �ʱ� matrix �ְ� ����.
	size--; // ������ �߰�.
	
}

void // ���ڷ� ���� matrix�� stack�� top�� �ִ� �Ͱ� ���ϰ� ���� ���� top�� ��ü. (��, matrix�� ���� �� �ִ�.) 
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

	copy_matrix(replace, stack[top_point].matrix); // ������ top�� �ִ� matrix �� ���� matrix ������ �ٲٱ�.
}

void // ���ڷ� ���� vertex�� ���� ������ top ���� ���ϰ� vertex���� �ٲٴ� �Լ�. ( ��, matrix�� ���� �� �ִ�.)
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

void // ���� ������ top * translateX matrix�� �����ְ� top matrix ��ü.
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

void // ���� ������ top * rotateX matrix�� �����ְ� top matrix ��ü.
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

void // ���� ������ top * scaleX matrix�� �����ְ� top matrix ��ü.
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
