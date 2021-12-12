#include "function.h"
#include "math.h"
#include <iostream>
using namespace std;

FILE* save_node_dis;
FILE* save_unit_force;
FILE* save_node_force;
FILE* show_input_data;
FILE* input_data;
FILE* pic_data;
int node_num_each_unit;
int node_DOF;
int node_num;
int unit_num;
int constrained_DOF_num;
int unit_kind_num;
int DOF;
int stiffness_matrix_capacity;
double X[50], Y[50], Z[50];
double X3, X2, X1, Y3, Y2, Y1, Z3, Z2, Z1, b;
int node_id_of_unit[4][50];
int constrained_dx_id[30];
int PDE_id[50];
int unit_kind[100];
int NMN;
double unit_attribute[4][100];
double P[100], P1[100];
double PP[100];
double A;
double E;
double I;
double C;
double TK[7][7];
double T[7][7];
double TT[7][7];
double global_stiffness_matrix[100];
double partial_stiffness_matrix[7][7];
double s[7][7];
int partial_id_to_global_id[7];
double L;
double SG;
double dr[100];
double dr_result[100];
double dee[50][7];
double Fee[50][7];
double F[50][100];
double l[100][100], y[100];


void scan()
{
	int i;

	show_input_data = fopen("output/��ʾ��������.txt", "w");
	input_data = fopen("ԭʼ����.txt", "r");
	pic_data = fopen("output/��ͼ����.txt", "w");

	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");
	fscanf_s(input_data, "�밴��Ҫ������������ģ���������Ҫ������ӿ�����ո�");
	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "node_num_each_unit(��Ԫ�ڵ���)=%d\n", &node_num_each_unit);
	fprintf(show_input_data, "node_num_each_unit(��Ԫ�ڵ���)=%d\n", node_num_each_unit);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "node_DOF(�ڵ����ɶ���)=%d\n", &node_DOF);
	fprintf(show_input_data, "node_DOF(�ڵ����ɶ���)=%d\n", node_DOF);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");


	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "node_num(�ڵ�����)=%d\n", &node_num);
	fprintf(show_input_data, "node_num(�ڵ�����)=%d\n", node_num);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "unit_num(��Ԫ����)=%d\n", &unit_num);
	fprintf(show_input_data, "unit_num(��Ԫ����)=%d\n", unit_num);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "unit_kind_num(��Ԫ�������)=%d\n", &unit_kind_num);
	fprintf(show_input_data, "unit_kind_num(��Ԫ�������)=%d\n", unit_kind_num);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "constrained_DOF_num(��Լ�������ɶ�����)=%d\n", &constrained_DOF_num);
	fprintf(show_input_data, "constrained_DOF_num(��Լ�������ɶ�����)=%d\n", constrained_DOF_num);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "\n��Ԫ���(��i����Ԫ������һ�ֵ�Ԫ���ͣ������ǵĵ���ģ��������Ȳ���������:\n");
	for (i = 1; i <= unit_num; ++i)
		fscanf_s(input_data, "%d", &unit_kind[i]);
	for (i = 1; i <= unit_num; ++i)
		fprintf(show_input_data, "��%d����Ԫ���=%d\n", i, unit_kind[i]);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, 
		"\n��Ԫ���͵Ĳ���(ÿһ�зֱ���������ģ���������������Ծأ���β�޿ո�):\n");
	for (i = 1; i <= unit_kind_num; ++i)
		fscanf_s(input_data, "%le %le %le", &unit_attribute[1][i], &unit_attribute[2][i], &unit_attribute[3][i]);

	for (i = 1; i <= unit_kind_num; ++i)
		fprintf(show_input_data, "��%d����Ԫ����E=%.2e��A=%.2e��I=%.2e\n",
			i, unit_attribute[1][i], unit_attribute[2][i], unit_attribute[3][i]);


	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "�ڵ������ֵ(ÿһ�зֱ�����x y z��ֵ�����пո��������β�޿ո�):\n");

	for (i = 1; i <= node_num; ++i)
		fscanf_s(input_data, "%lf%lf", &X[i], &Y[i]);
	for (i = 1; i <= node_num; ++i)
		fprintf(show_input_data, "��%d���ڵ������ֵ:(%.2f, %.2f)\n", i, X[i], Y[i]);
	for (i = 1; i <= node_num; ++i)
		fprintf(pic_data, "%f %f ", X[i], Y[i]);
	fprintf(pic_data, "\n");



	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "\n��Ԫ�Ľڵ��(���յ�Ԫ˳��ɶ����룬���пո���߻س����������ܿո�ӻس�):\n");
	for (i = 1; i <= unit_num; ++i)
		fscanf_s(input_data, "%d%d", &node_id_of_unit[1][i], &node_id_of_unit[2][i]);
	for (i = 1; i <= unit_num; ++i)
		fprintf(show_input_data, "��%d����Ԫ�Ľڵ��:%d %d\n", i, node_id_of_unit[1][i], node_id_of_unit[2][i]);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");
	for (i = 1; i <= unit_num; ++i)
		fprintf(pic_data, "%d %d ", node_id_of_unit[1][i], node_id_of_unit[2][i]);
	fprintf(pic_data, "\n");


	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "\n��Լ����λ�ƺ�(���пո���߻س�����,���ܿո�ӻس�):\n");
	for (i = 1; i <= constrained_DOF_num; ++i)
		fscanf_s(input_data, "%d", &constrained_dx_id[i]);
	for (i = 1; i <= constrained_DOF_num; ++i)
		fprintf(show_input_data, "��Լ���ĵ�%d��λ�ƺ�:%d\n", i, constrained_dx_id[i]);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "\n�ڵ���غɴ�С(ÿ������,�ֱ���Px,Py,M,��i�д����i���ڵ��):\n");
	for (i = 1; i <= node_num * node_DOF; ++i)
		fscanf_s(input_data, "%lf", &P[i]);
	for (i = 1; i <= node_num * node_DOF; ++i) {
		P1[i] = P[i];
		fprintf(show_input_data, "��%d��λ�Ʒ����ϵ�����غ�:%.3f\n", i, P[i]);
	}

	fclose(show_input_data);
	fclose(input_data);
	fclose(pic_data);

}

void Length(int i) {
	X2 = X[node_id_of_unit[2][i]];
	X1 = X[node_id_of_unit[1][i]];
	Y2 = Y[node_id_of_unit[2][i]];
	Y1 = Y[node_id_of_unit[1][i]];
	L = sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
}

void StiffnessMatrix_unit(int i) {
	Length(i);

	NMN = unit_kind[i];
	E = unit_attribute[1][NMN];
	A = unit_attribute[2][NMN];
	//G = unit_attribute[3][NMN];
	I = unit_attribute[3][NMN];
	C = E * A / L;
	TK[1][1] = TK[4][4] = C;
	TK[1][4] = TK[4][1] = -C;
	C = 12 * E * I / L / L / L;
	TK[2][2] = TK[5][5] = C;
	TK[2][5] = TK[5][2] = -C;
	C = 6 * E * I / L / L;
	TK[2][3] = TK[3][2]= TK[2][6] = TK[6][2] = C;
	TK[3][5] = TK[5][3] = TK[6][5] = TK[5][6] = -C;
	C = E * I / L;
	TK[3][3] = TK[6][6] = 4*C;
	TK[6][3] = TK[3][6] = 2 * C;

	//JY = unit_attribute[5][NMN];
	//JZ = unit_attribute[6][NMN];
	//TK[1][1] = E * A / L;
	//TK[7][7] = TK[1][1];
	//TK[7][1] = -TK[1][1];
	//C = 12 * E * JZ / L / L / L;
	//TK[2][2] = C;
	//TK[8][8] = C;
	//TK[8][2] = -C;
	//TK[8][6] = -C * L / 2;
	//C = 12 * E * JY / L / L / L;
	//TK[3][3] = C;
	//TK[9][9] = C;
	//TK[9][3] = -C;
	//TK[9][5] = C * L / 2;
	//TK[4][4] = G * JX / L;
	//TK[10][10] = TK[4][4];
	//TK[10][4] = -TK[4][4];
	//C = 4 * E * JY / L;
	//TK[5][5] = C;
	//TK[11][11] = C;
	//C = 6 * E * JY / L / L;
	//TK[5][3] = -C;
	//TK[11][9] = C;
	//TK[11][3] = -C;
	//TK[11][5] = 2 * E * JY / L;
	//C = 4 * E * JZ / L;
	//TK[6][6] = C;
	//TK[12][12] = C;
	//C = 6 * E * JZ / L / L;
	//TK[6][2] = C;
	//TK[12][2] = C;
	//TK[12][8] = -C;
	//TK[12][6] = 2 * E * JZ / L;
	//for (int m = 1; m <= 12; ++m)
	//	for (int n = 1; n <= (m - 1); ++n)
	//		TK[n][m] = TK[m][n];
}

void TransformMatrix(int i) {
	Length(i);

	//�ֲ�����ϵx'
	//d'=Td
	//T:[cos(x,x') cos(y,x') 0         0         0		   0
	//T:[cos(x,y') cos(y,y') 0         0         0		   0
	//T:[0		   0		 1         0         0		   0
	//T:[0		   0		 0		   cos(x,x') cos(y,x') 0
	//T:[0         0         0		   cos(x,y') cos(y,y') 0
	//T:[0         0         0		   0		 0		   1

	T[1][1] = T[4][4] = T[2][2] = T[5][5] = (X2 - X1) / L;
	T[1][2] =  T[4][5]  = (Y2 - Y1) / L;
	T[2][1] = T[5][4] = -(Y2 - Y1) / L;
	T[3][3] = T[6][6] = 1;



	//����Ԫ��Ĭ���Ѿ����㲻����
	for (int m = 1; m <= (node_DOF * node_num_each_unit); ++m)
		for (int n = 1; n <= (node_DOF * node_num_each_unit); ++n)//node_DOF�����ڵ�����ɶ���,node_num_each_unit��Ԫ�Ľڵ�����
			TT[n][m] = T[m][n];
}

void MultiplyMatrix(int i) {
	int j, m, n;
	double a;
	StiffnessMatrix_unit(i);
	TransformMatrix(i);

	for (n = 1; n <= (node_DOF * node_num_each_unit); ++n) {//��Ԫ���ɶ���*��Ԫ�ڵ���=3*2=6
		for (m = 1; m <= (node_DOF * node_num_each_unit); ++m) {
			a = 0.0;
			for (j = 1; j <= (node_DOF * node_num_each_unit); ++j)
				a += TT[n][j] * TK[j][m];
			s[n][m] = a;
		}
	}

	//s(6*2)*T(2*6)
	for (n = 1; n <= (node_DOF * node_num_each_unit); ++n) {//��Ԫ���ɶ���*��Ԫ�ڵ���=3*2=6
		for (m = 1; m <= (node_DOF * node_num_each_unit); ++m) {
			a = 0.0;
			for (j = 1; j <= (node_DOF * node_num_each_unit); ++j)
				a += s[n][j] * T[j][m];
			partial_stiffness_matrix[n][m] = a;
		}
	}
}

void creating_partial_id_to_global_id(int i) {
		partial_id_to_global_id[1] = (node_id_of_unit[1][i] - 1) * node_DOF + 1;
		partial_id_to_global_id[2] = (node_id_of_unit[1][i] - 1) * node_DOF + 2;
		partial_id_to_global_id[3] = (node_id_of_unit[1][i] - 1) * node_DOF + 3;

		partial_id_to_global_id[4] = (node_id_of_unit[2][i] - 1) * node_DOF + 1;
		partial_id_to_global_id[5] = (node_id_of_unit[2][i] - 1) * node_DOF + 2;
		partial_id_to_global_id[6] = (node_id_of_unit[2][i] - 1) * node_DOF + 3;
}

void creating_PDE_id() {
	int k, i, j, L, IG, J;
	PDE_id[1] = 1;
	//����ÿһ���ڵ㣬ȷ����ýڵ���ص���С�Ľڵ�ţ�������õ��Ӧ��PDE_id
	for (k = 1; k <= node_num; ++k) {
		IG = 100000;
		for (i = 1; i <= unit_num; ++i)//����ÿһ����Ԫ
			for (j = 1; j <= node_num_each_unit; ++j) {//�ҵ�һ����Ԫ�еĽڵ��k��
				if (node_id_of_unit[j][i] != k)continue;

				for (L = 1; L <= node_num_each_unit; ++L) {//���õ�Ԫ�еĽڵ�����Сֵ��¼��������ΪIG
					if (node_id_of_unit[L][i] >= IG)continue;
					IG = node_id_of_unit[L][i];
				}
			}
		for (i = 1; i <= node_DOF; ++i) {
			J = node_DOF * (k - 1) + i;//�ڵ�k��Ӧ�ĸն�ϵ�����ܸնȾ�������ռ���к�(�ڵ�kʵ����ռ��һ��node_DOF*node_DOF�ķֿ�)
			if (J == 1)continue;//��1�е����Խ�Ԫλ�þ���1��Ϊ�˱���J-1Ϊ0
			PDE_id[J] = PDE_id[J - 1] + node_DOF * (k - IG) + i;//node_DOF * (k - IG) + i�������
		}
		DOF = node_num * node_DOF;
		stiffness_matrix_capacity = PDE_id[DOF];//��ʾ�ṹ�նȾ����һά���������
	}
}

void StructureMatrix() {
	int i, j, m, NI, NJ, IJ;
	creating_PDE_id();
	//N = node_num * node_DOF;
	//stiffness_matrix_capacity = PDE_id[N];//��ʾ��һά����PDE_id������
	for (m = 1; m <= unit_num; ++m) {
		MultiplyMatrix(m);//���㵥Ԫ�նȾ���
		creating_partial_id_to_global_id(m);//����partial_id_to_global_id,partial_id_to_global_id(i)��ʾ�ڸ�m��Ԫ�еĵ�i��λ�ƶ�Ӧ������ܽṹ�е�λ�Ʊ��
		for (i = 1; i <= (node_DOF * node_num_each_unit); ++i)
			for (j = 1; j <= (node_DOF * node_num_each_unit); ++j) {
				if (partial_id_to_global_id[i] - partial_id_to_global_id[j] >= 0) {//ֻ���Ǵ洢�����Ǽ�i>j
					NI = partial_id_to_global_id[i];
					IJ = PDE_id[NI] - (NI - partial_id_to_global_id[j]);//ԭ�����(k,k)λ������(i-j)��
					global_stiffness_matrix[IJ] += partial_stiffness_matrix[i][j];
				}
			}
	}
	//Լ�������ô�������
	for (i = 1; i <= constrained_DOF_num; ++i) {
		NI = constrained_dx_id[i];
		NJ = PDE_id[NI];
		global_stiffness_matrix[NJ] = 1e65;
	}
}

void colve_equation(int n, double a[100], double x[100]) {
	//���ڷ���Ax=b
	//���ﴫ���x�����Ǳ�ʾ����������b��Ϊʡ��ռ��bֱ���޸���������x,a�����ʾA��һά��ʾ
	int i, j, k, position_ii, position_kk, position_jk, position_ik, position_ij, mij, leftmost_column_of_rowi, leftmost_column_of_rowj;
	//�����ǲ��Դ��������
	//PDE_id[1] = 1;
	//PDE_id[2] = 3;
	//PDE_id[3] = 6;
	//PDE_id[4] = 8;
	//PDE_id[5] = 9;
	//PDE_id[6] = 13;
	//a[1] = 4.5;
	//a[2] = 0.2;
	//a[3] = 5.3;
	//a[4] = -1.3;
	//a[5] = 0;
	//a[6] = 10.2;
	//a[7] = 5.1;
	//a[8] = 8.4;
	//a[9] = 0.6;
	//a[10] = -1.7;
	//a[11] = 0;
	//a[12] = 0;
	//a[13] = 3.1;
	//x[1] = 3.4;
	//x[2] = 5.5;
	//x[3] = 12.3;
	//x[4] = 13.5;
	//x[5] = 0.6;
	//x[6] = 1.4;
	pic_data = fopen("output/��ͼ����.txt", "a");

	for (i = 1; i <= n; ++i) {
		// i = 1 ʱ������ֽ�
		if (i != 1) {
			leftmost_column_of_rowi = i - (PDE_id[i] - PDE_id[i - 1]) + 1;//��i���������Ԫ���к�mi
			if (leftmost_column_of_rowi != i) {//�ڵ�i�в�ֹ��һ���Խ�Ԫ�������
				for (j = leftmost_column_of_rowi; j <= i - 1; ++j) {//��j�д�����Ԫ�أ������Խ�Ԫ��ѭ��
					if (j != leftmost_column_of_rowi) {
						leftmost_column_of_rowj = j - (PDE_id[j] - PDE_id[j - 1]) + 1;//��j���������Ԫ���к�mj
						position_ij = PDE_id[i] - (i - j);//��ǰ���ֽ�Ԫ��Kij��һά��ַ
						//mij=max{mi,mj}
						mij = (leftmost_column_of_rowj < leftmost_column_of_rowi) ? leftmost_column_of_rowi : leftmost_column_of_rowj;
						if (mij <= j - 1) {
							//li1=ki1/d11
							//lij=(kij)-sum(t=mij,j-1){lit*dtt*ljt},i=2,3,....,n,j=2,3,...,i-1
							for (k = mij; k <= j - 1; ++k) {
								//�ֱ����Lik��Ljk��dkk��һά��ַ
								position_ik = PDE_id[i] - i + k;
								position_jk = PDE_id[j] - j + k;
								position_kk = PDE_id[k];
								a[position_ij] -= a[position_ik] * a[position_kk] * a[position_jk];
							}
						}
					}
					if (j == leftmost_column_of_rowi)//������ֽ�Ԫ���ڸ��еĵ�һ��ͬ�����÷ֽ�
						position_ij = PDE_id[i - 1] + 1;//�������Ԫ�ص�һά��ַ
					position_ii = PDE_id[j];
					a[position_ij] = a[position_ij] / a[position_ii];//���º��lij
					x[i] -= a[position_ij] * a[position_ii] * x[j];
				}
				for (k = leftmost_column_of_rowi; k <= i - 1; ++k) {
					position_ik = PDE_id[i] - i + k;//lik�ĵ�ַ
					position_kk = PDE_id[k];//dkk�ĵ�ַ
					a[PDE_id[i]] -= a[position_ik] * a[position_ik] * a[position_kk];//���º��dii
				}
			}
		}
		//��i��ֻ�жԽ�Ԫ��������⴦��
		x[i] = x[i] / a[PDE_id[i]];
	}
	//�ش�ѭ��
	for (i = n; i >= 2; --i) {
		leftmost_column_of_rowi = i - (PDE_id[i] - PDE_id[i - 1]) + 1;//��i���������Ԫ���к�

		if (leftmost_column_of_rowi == i)
			continue;
		for (k = leftmost_column_of_rowi; k <= i - 1; ++k) {//k��ʾ�к�
			position_ik = PDE_id[i] - i + k;//Lik��һά��ַ
			x[k] -= a[position_ik] * x[i];
		}
	}
	save_node_dis = fopen("output/����ڵ�λ��.txt", "w");
	for (i = 1; i <= n; ++i) {
		if (abs(x[i]) < 1e-9)
			x[i] = 0;
		fprintf(save_node_dis, "%d %.2le\n", i, x[i]);
		if(i%3<2)
			fprintf(pic_data, "%f ", x[i]);
	}
	fprintf(pic_data, "\n");

	fclose(save_node_dis);
	fclose(pic_data);
}

void OutputInternalForce() {
	int n, m, i, k, j;
	save_unit_force = fopen("output/��Ԫ����.txt", "w");
	save_node_force = fopen("output/Լ������.txt", "w");
	pic_data = fopen("output/��ͼ����.txt", "a");

	for (i = 1; i <= unit_num; ++i) {
		//�õ�Ԫ�Ľڵ�λ�ƣ���������ϵ��
		TransformMatrix(i);
		StiffnessMatrix_unit(i);
		creating_partial_id_to_global_id(i);
		for (k = 1; k <= (node_DOF * node_num_each_unit); ++k)
			dr_result[k] = P[partial_id_to_global_id[k]];


		//�õ�Ԫ�ֲ�����ϵ�½ڵ�λ��
		for (j = 1; j <= (node_num_each_unit * node_DOF); ++j)
			for (k = 1; k <= (node_num_each_unit * node_DOF); ++k)
				dee[i][j] += T[j][k] * dr_result[k];
		//�õ�Ԫ����
		for (m = 1; m <= node_num_each_unit* node_DOF; ++m)
			for (j = 1; j <= node_num_each_unit*node_DOF; ++j)
				Fee[i][m] += TK[m][j] * dee[i][j];




		fprintf(save_unit_force, "��%d����Ԫ�ֲ�����ϵ�µ�����\n", i);
		fprintf(save_unit_force, "%.2le %.2le %.2le %.2le %.2le %.2le\n", Fee[i][1], Fee[i][2], Fee[i][3], Fee[i][4], Fee[i][5], Fee[i][6]);
		fprintf(save_unit_force, "\n\n");


		//�õ�Ԫ��������ϵ�µĽڵ���
		for (k = 1; k <= (node_num_each_unit * node_DOF); ++k)
			for (j = 1; j <= (node_num_each_unit * node_DOF); ++j)
				F[i][k] += TT[k][j] * Fee[i][j];


		//�������ṹ�ڵ���
		for (k = 1; k <= (node_num_each_unit * node_DOF); ++k)
			PP[partial_id_to_global_id[k]] += F[i][k];
	}
	fprintf(pic_data, "\n");

	//���Լ������
	for (k = 1; k <= node_DOF * node_num; ++k)
		P1[k] = PP[k] - P1[k];
	for (m = 1; m <= node_num; ++m) {
		for (n = 1; n <= node_DOF; ++n)
			fprintf(save_node_force, "��%d���ڵ�ĵ�%d��Լ����%f\n", m, n, P1[(m - 1) * node_DOF + n]);
		fprintf(save_node_force, "\n\n");
	}
	fclose(save_unit_force);
	fclose(save_node_force);
	fclose(pic_data);
}

//void AKETTFMatrix_unitbeam(int i) {
//	X1 = X[node_id_of_unit[1][i]];
//	X2 = X[node_id_of_unit[2][i]];
//	X3 = X[node_id_of_unit[3][i]];
//	Y1 = Y[node_id_of_unit[1][i]];
//	Y2 = Y[node_id_of_unit[2][i]];
//	Y3 = Y[node_id_of_unit[3][i]];
//	Z1 = Z[node_id_of_unit[1][i]];
//	Z2 = Z[node_id_of_unit[2][i]];
//	Z3 = Z[node_id_of_unit[3][i]];
//
//	//NM = NAPB[i];
//	//A1 = APB[1][NM];
//	//B1 = APB[2][NM];
//	//C1 = APB[3][NM];
//	//A2 = APB[4][NM];
//	//B2 = APB[5][NM];
//	//C2 = APB[6][NM];
//
//	////ƫ������
//	//L = (X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1) + (Z2 - Z1) * (Z2 - Z1) - (B2 - B1) * (B2 - B1) + (C2 - C1) * (C2 - C1);
//	//L = sqrt(L) + A1 - A2;
//
//	L = sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1) + (Z2 - Z1) * (Z2 - Z1));
//	NMN = unit_kind[i];
//	E = AEB[1][NMN];
//	A = AEB[2][NMN];
//	G = AEB[3][NMN];
//	AZ = AEB[4][NMN];
//	AY = AEB[5][NMN];
//	J = AEB[6][NMN];
//	JY = AEB[7][NMN];
//	JZ = AEB[8][NMN];
//	K = AEB[9][NMN];
//
//	AKE[1][1] = E * A / L;
//	AKE[7][7] = AKE[1][1];
//	AKE[7][1] = -AKE[1][1];
//	BZ = 12 * K * E * JY / G / AZ / L / L;
//	BY = 12 * K * E * JZ / G / AY / L / L;
//
//	C = 12 * E * JZ / (1 + BY) / L / L / L;
//	AKE[2][2] = C;
//	AKE[8][8] = C;
//	AKE[8][2] = -C;
//	AKE[8][6] = -C * L / 2;
//	C = 12 * E * JY / (1 + BZ) / L / L / L;
//	AKE[3][3] = C;
//	AKE[9][9] = C;
//	AKE[9][3] = -C;
//	AKE[9][5] = C * L / 2;
//	AKE[4][4] = G * J / L;
//	AKE[10][10] = AKE[4][4];
//	AKE[10][4] = -AKE[4][4];
//	C = (4 + BZ) * E * JY / (1 + BZ) / L;
//	AKE[5][5] = C;
//	AKE[11][11] = C;
//	C = 6 * E * JY / (1 + BZ) / L / L;
//	AKE[5][3] = C;
//	AKE[11][9] = C;
//	AKE[11][3] = -C;
//	AKE[11][5] = (2 - BZ) * E * JY / (1 + BZ) / L;
//	C = (4 + BY) * E * JZ / (1 + BY) / L;
//	AKE[6][6] = C;
//	AKE[12][12] = C;
//	C = 6 * E * JZ / (1 + BY) / L / L;
//	AKE[6][2] = C;
//	AKE[12][2] = C;
//	AKE[12][8] = C;
//	AKE[12][6] = (2 - BZ) * E * JZ / (1 + BZ) / L;
//
//	for (m = 1; m <= 12; ++m)
//		for (n = 1; n <= (m - 1); ++n)
//			AKE[n][m] = AKE[m][n];
//
//	////�γ���Ԫλ��ƫ������������������TTM��
//	//for (m = 1; m <= 12; ++m)
//	//	TTM[m][m] = 1;
//	//TTM[1][5] = -C1;
//	//TTM[1][6] = B1;
//	//TTM[2][4] = -C1;
//	//TTM[2][6] = -A1;
//	//TTM[3][4] = -B1;
//	//TTM[3][5] = A1;
//	//TTM[7][11] = -C2;
//	//TTM[7][12] = B2;
//	//TTM[8][10] = -C2;
//	//TTM[8][12] = -A2;
//	//TTM[9][10] = -B2;
//	//TTM[9][11] = A2;
//	////�γ���Ԫ����ת��������������T��
//	//XX = X2 - X1;
//	//YY = Y2 - Y1;
//	//ZZ = Z2 - Z1;
//	//L = sqrt(XX * XX + YY * YY + ZZ * ZZ);
//	//C = XX / L;
//	//T[1][1] = T[4][4] = T[7][7] = T[10][10] = C;
//	//C = YY / L;
//	//T[1][2] = T[4][5] = T[7][8] = T[10][11] = C;
//	//C = ZZ / L;
//	//T[1][3] = T[4][6] = T[7][9] = T[10][12] = C;
//	//XX = X3 - X1;
//	//YY = Y3 - Y1;
//	//ZZ = Z3 - Z1;
//	//L = sqrt(XX * XX + YY * YY + ZZ * ZZ);
//	//C = XX / L;
//	//T[2][1] = T[5][4] = T[8][7] = T[11][10] = C;
//	//C = YY / L;
//	//T[2][2] = T[5][5] = T[8][8] = T[11][11] = C;
//	//C = ZZ / L;
//	//T[2][3] = T[5][6] = T[8][9] = T[11][12] = C;
//	//C = T[1][2] * T[2][3] - T[1][3] * T[2][2];
//	//T[3][1] = T[6][4] = T[9][7] = T[12][10] = C;
//	//C = T[1][3] * T[2][1] - T[1][1] * T[2][3];
//	//T[3][2] = T[6][5] = T[9][8] = T[12][11] = C;
//	//C = T[1][1] * T[2][2] - T[1][2] * T[2][1];
//	//T[3][3] = T[6][6] = T[9][9] = T[12][12] = C;
//	////�γ���Ԫ����ƫ������������������TTF��
//	//for (m = 1; m <= 12; ++m)
//	//	TTF[m][m] = 1;
//	//TTF[4][2] = -C1;
//	//TTF[4][3] = B1;
//	//TTF[5][1] = C1;
//	//TTF[5][3] = -A1;
//	//TTF[10][8] = -C2;
//	//TTF[6][1] = -B1;
//	//TTF[10][9] = -B2;
//	//TTF[6][2] = -A1;
//	//TTF[11][7] = C2;
//	//TTF[11][9] = -A2;
//	//TTF[12][7] = -B2;
//	//TTF[12][8] = A2;
//
//
//}


void test() {
	cout << "��������Ԫ�ĳ��Ȳ��ԣ�\n";
	for (int i = 1; i <= unit_num; ++i) {
		Length(i);
		cout << "��" << i << "����Ԫ��\n";
		cout << L << '\n';
	}
	cout << "------------------------------\n";


	cout << "��������Ԫ�ĵ�Ԫ�նȾ�����ԣ�\n";
	for (int k = 1; k <= unit_num; ++k) {
		StiffnessMatrix_unit(k);
		cout << "��" << k << "����Ԫ��\n";
		for (int i = 1; i <= 6; ++i) {
			for (int j = 1; j <= 6; ++j) {
				cout << TK[i][j] << "  \t";
			}
			cout << '\n';
		}
	}
	cout << "------------------------------\n";

	cout << "��������Ԫ������ת��������ԣ�\n";
	for (int k = 1; k <= unit_num; ++k) {
		TransformMatrix(k);
		cout << "��" << k << "����Ԫ��\n";
		for (int i = 1; i <= 6; ++i) {
			for (int j = 1; j <= 6; ++j) {
				cout << T[i][j] << '\t';
			}
			cout << '\n';
		}
	}
	cout << "------------------------------\n";

	cout << "��������Ԫ����������ϵ�նȾ�����ԣ�\n";
	for (int k = 1; k <= unit_num; ++k) {
		MultiplyMatrix(k);
		cout << "��" << k << "����Ԫ��\n";
		for (int i = 1; i <= (node_DOF * node_num_each_unit); ++i) {
			for (int j = 1; j <= (node_DOF * node_num_each_unit); ++j) {
				cout << partial_stiffness_matrix[i][j] << '\t';
			}
			cout << '\n';
		}
	}

	cout << "------------------------------\n";

	cout << "PDE_id������ԣ�\n";
	creating_PDE_id();
	for (int k = 1; k <= DOF; ++k)
		cout << PDE_id[k] << '\t';
	cout << '\n';

	cout << "------------------------------\n";

	cout << "һά�ṹ�նȾ�����ԣ�\n";
	StructureMatrix();
	for (int k = 1; k <= stiffness_matrix_capacity; ++k)
		cout << global_stiffness_matrix[k] << '\t';
	cout << '\n';

	cout << "------------------------------\n";
}