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
double X2, X1, Y2, Y1, Z2, Z1, b;
int node_id_of_unit[3][50];
int constrained_dx_id[30];
int PDE_id[50];
int unit_kind[100];
double unit_attribute[3][100];
double P[100], P1[100];
double PP[100];
double A;
double E;
double TK[3][3];
double T[3][7];
double TT[7][3];
double global_stiffness_matrix[100];
double partial_stiffness_matrix[7][7];
double s[7][3];
int partial_id_to_global_id[7];
double L;
double SG;
double dr[100];
double dr_result[100];
double dee[50][3];
double Fee[50][7];
double F[50][100];
double l[100][100], y[100];


void scan()
{
	int i;

	show_input_data = fopen("output/��ʾ��������.txt", "w");
	input_data = fopen("ԭʼ����.txt", "r");
	pic_data= fopen("output/��ͼ����.txt", "w");
	
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");
	fscanf_s(input_data, "�밴��Ҫ������������ģ���������Ҫ������ӿ�����ո�");
	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "node_num_each_unit(��Ԫ�ڵ���)=%d\n", &node_num_each_unit);
	fprintf(show_input_data, "node_num_each_unit(��Ԫ�ڵ���)=%d\n", node_num_each_unit);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "node_DOF(��Ԫ���ɶ���)=%d\n", &node_DOF);
	fprintf(show_input_data, "node_DOF(��Ԫ���ɶ���)=%d\n", node_DOF);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");


	fprintf(pic_data, "%d\n", node_DOF);

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
	fscanf_s(input_data, "\n��Ԫ���͵Ĳ���(ÿһ�зֱ����뵯�Գ��������������ո��������β�޿ո�):\n");
	for (i = 1; i <= unit_kind_num; ++i)
		fscanf_s(input_data, "%le%le", &unit_attribute[1][i], &unit_attribute[2][i]);//%leΪ��ָ����ʽ��� double ����
	for (i = 1; i <= unit_kind_num; ++i)
		fprintf(show_input_data, "��%d����Ԫ����E=%.2e��A=%.2e\n", i, unit_attribute[1][i], unit_attribute[2][i]);
	fprintf(show_input_data, "\n---------------------------------------------------------------\n\n");

	fscanf_s(input_data, "\n---------------------------------------------------------------\n");
	fscanf_s(input_data, "�ڵ������ֵ(ÿһ�зֱ�����x y z��ֵ�����пո��������β�޿ո�):\n");



	if (node_DOF == 3) {
		for (i = 1; i <= node_num; ++i)
			fscanf_s(input_data, "%lf%lf%lf", &X[i], &Y[i], &Z[i]);//%lf����˫���ȸ��������ݣ�double��
		for (i = 1; i <= node_num; ++i)
			fprintf(show_input_data, "��%d���ڵ������ֵ:(%.2f, %.2f, %.2f)\n", i, X[i], Y[i], Z[i]);//%f�������ȸ��������ݣ�float��
		for (i = 1; i <= node_num; ++i)
			fprintf(pic_data, "%f %f %f ", X[i], Y[i], Z[i]);
		fprintf(pic_data, "\n");
	}

	else if (node_DOF == 2) {
		for (i = 1; i <= node_num; ++i)
			fscanf_s(input_data, "%lf%lf", &X[i], &Y[i]);//%lf����˫���ȸ��������ݣ�double��
		for (i = 1; i <= node_num; ++i)
			fprintf(show_input_data, "��%d���ڵ������ֵ:(%.2f, %.2f)\n", i, X[i], Y[i]);//%f�������ȸ��������ݣ�float��
		for (i = 1; i <= node_num; ++i)
			fprintf(pic_data, "%f %f ", X[i], Y[i]);
		fprintf(pic_data, "\n");
	}
	else
		cout << "node_DOF ERROR";


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
	fscanf_s(input_data, "\n�ڵ���غɴ�С(ÿ������,�ֱ���Px,Py,Pz,��i�д����i���ڵ��):\n");
	for (i = 1; i <= node_num * node_DOF; ++i)
		fscanf_s(input_data, "%lf", &P[i]);
	for (i = 1; i <= node_num * node_DOF; ++i) {
		P1[i] = P[i];
		fprintf(show_input_data, "��%d��λ�Ʒ����ϵ�����غ�:%.3f %.3f\n", i, P[i], P1[i]);
	}

	fclose(show_input_data);
	fclose(input_data);
	fclose(pic_data);

}

void Length(int i) {
	//node_id_of_unit[1][i]�����i����Ԫ�ĵ�һ���ڵ�Ľڵ��
	//node_id_of_unit[2][i]�����i����Ԫ�ĵڶ����ڵ�Ľڵ��
	if (node_DOF == 3) {
		X2 = X[node_id_of_unit[2][i]];
		X1 = X[node_id_of_unit[1][i]];
		Y2 = Y[node_id_of_unit[2][i]];
		Y1 = Y[node_id_of_unit[1][i]];
		Z2 = Z[node_id_of_unit[2][i]];
		Z1 = Z[node_id_of_unit[1][i]];
		L = sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1) + (Z2 - Z1) * (Z2 - Z1));
	}

	else if (node_DOF == 2) {
		X2 = X[node_id_of_unit[2][i]];
		X1 = X[node_id_of_unit[1][i]];
		Y2 = Y[node_id_of_unit[2][i]];
		Y1 = Y[node_id_of_unit[1][i]];
		L = sqrt((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
	}
	else
		cout << "node_DOF ERROR";
}

void StiffnessMatrix_unit(int i) {
	Length(i);
	
	E = unit_attribute[1][unit_kind[i]];
	A = unit_attribute[2][unit_kind[i]];

	TK[1][1] = E * A / L;
	TK[1][2] = -E * A / L;
	TK[2][1] = -E * A / L;
	TK[2][2] = E * A / L;
}

void TransformMatrix(int i) {
	int m, n;
	Length(i);
	//�ֲ�����ϵx'
	//d'=Td
	//T:[cos(x,x') cos(y,x') cos(z,x') 0         0         0
	//   0         0         0         cos(x,x') cos(y,x') cos(z,x')]

	if (node_DOF == 3) {
		T[1][1] = T[2][4] = (X2 - X1) / L;
		T[1][2] = T[2][5] = (Y2 - Y1) / L;
		T[1][3] = T[2][6] = (Z2 - Z1) / L;
	}

	else if (node_DOF == 2) {
		T[1][1] = T[2][3] = (X2 - X1) / L;
		T[1][2] = T[2][4] = (Y2 - Y1) / L;
	}
	else
		cout << "node_DOF ERROR";

	//����Ԫ��Ĭ���Ѿ����㲻����
	for (m = 1; m <= 2; ++m)
		for (n = 1; n <= (node_DOF * node_num_each_unit); ++n)//node_DOF�����ڵ�����ɶ���,node_num_each_unit��Ԫ�Ľڵ�����
			TT[n][m] = T[m][n];
}

void MultiplyMatrix(int i) {
	int j, m, n;
	double a;
	StiffnessMatrix_unit(i);
	TransformMatrix(i);
	//T'(6*2)*K(2*2)*T(2*6)

	//s(6*2)=T'(6*2)*K(2*2)
	for (n = 1; n <= (node_DOF * node_num_each_unit); ++n) {//��Ԫ���ɶ���*��Ԫ�ڵ���=3*2=6
		for (m = 1; m <= 2; ++m) {
			a = 0.0;
			for (j = 1; j <= 2; ++j) 
				a += TT[n][j] * TK[j][m];
			s[n][m] = a;
		}
	}

	//s(6*2)*T(2*6)
	for (n = 1; n <= (node_DOF * node_num_each_unit); ++n) {//��Ԫ���ɶ���*��Ԫ�ڵ���=3*2=6
		for (m = 1; m <= (node_DOF * node_num_each_unit); ++m) {
			a = 0.0;
			for (j = 1; j <= 2; ++j)
				a += s[n][j] * T[j][m];
			partial_stiffness_matrix[n][m] = a;
		}
	}
}

void creating_partial_id_to_global_id(int i) {
	if (node_DOF == 3) {
		partial_id_to_global_id[1] = (node_id_of_unit[1][i] - 1) * node_DOF + 1;
		partial_id_to_global_id[2] = (node_id_of_unit[1][i] - 1) * node_DOF + 2;
		partial_id_to_global_id[3] = (node_id_of_unit[1][i] - 1) * node_DOF + 3;
		partial_id_to_global_id[4] = (node_id_of_unit[2][i] - 1) * node_DOF + 1;
		partial_id_to_global_id[5] = (node_id_of_unit[2][i] - 1) * node_DOF + 2;
		partial_id_to_global_id[6] = (node_id_of_unit[2][i] - 1) * node_DOF + 3;
	}
	else if (node_DOF == 2) {
		partial_id_to_global_id[1] = (node_id_of_unit[1][i] - 1) * node_DOF + 1;
		partial_id_to_global_id[2] = (node_id_of_unit[1][i] - 1) * node_DOF + 2;
		partial_id_to_global_id[3] = (node_id_of_unit[2][i] - 1) * node_DOF + 1;
		partial_id_to_global_id[4] = (node_id_of_unit[2][i] - 1) * node_DOF + 2;
	}
	else
		cout << "node_DOF ERROR";
} 

void creating_PDE_id() {
	int k, i, j, L, IG, J;
	PDE_id[1] = 1;
	//����ÿһ���ڵ㣬ȷ����ýڵ���ص���С�Ľڵ�ţ�������õ��Ӧ��PDE_id
	for (k = 1; k <= node_num; ++k) {
		IG = 100000;
		for(i=1;i<=unit_num;++i)//����ÿһ����Ԫ
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
		global_stiffness_matrix[NJ] = 1e25;
	}
}


void colve_equation(int n, double a[100], double x[100]) {
	//���ڷ���Ax=b
	//���ﴫ���x�����Ǳ�ʾ����������b��Ϊʡ��ռ��bֱ���޸���������x,a�����ʾA��һά��ʾ
	int i, j, k, position_ii, position_kk, position_jk,position_ik, position_ij, mij, leftmost_column_of_rowi, leftmost_column_of_rowj;
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
						mij=(leftmost_column_of_rowj < leftmost_column_of_rowi) ? leftmost_column_of_rowi : leftmost_column_of_rowj;
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
					position_ik = PDE_id[i]-i + k;//lik�ĵ�ַ
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
		for (j = 1; j <= 2; ++j) 
			for (k = 1; k <= (node_num_each_unit * node_DOF); ++k)
				dee[i][j] += T[j][k] * dr_result[k];

		//�õ�Ԫ����
		for (m = 1; m <= node_num_each_unit; ++m)
			for (j = 1; j <= node_num_each_unit; ++j)
				Fee[i][m] += TK[m][j] * dee[i][j];

		//�õ�ԪӦ��
		SG = Fee[i][2] / A;
		fprintf(save_unit_force, "��%d����Ԫ�ֲ�����ϵ�µ�������Ӧ��\n", i);
		fprintf(save_unit_force, "%le %le\n", Fee[i][2], SG);
		fprintf(pic_data, "%f ", SG);

		fprintf(save_unit_force, "\n\n");

		//�õ�Ԫ��������ϵ�µĽڵ���
		for (k = 1; k <= (node_num_each_unit*node_DOF); ++k)
			for(j=1;j<=node_num_each_unit;++j)
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

void test() {
	cout << "�����˵�Ԫ�ĳ��Ȳ��ԣ�\n";
	for (int i = 1; i <= unit_num; ++i) {
		Length(i);
		cout << "��" << i << "����Ԫ��\n";
		cout << L << '\n';
	}
	cout << "------------------------------\n";

	cout << "�����˵�Ԫ�ĵ�Ԫ�նȾ�����ԣ�\n";
	for (int k = 1; k <= unit_num; ++k) {
		StiffnessMatrix_unit(k);
		cout << "��"  << k << "����Ԫ��\n";
		for (int i = 1; i <= 2; ++i) {
			for (int j = 1; j <= 2; ++j) {
				cout << TK[i][j] << '\t';
			}
			cout << '\n';
		}
	}
	cout << "------------------------------\n";

	cout << "�����˵�Ԫ������ת��������ԣ�\n";
	for (int k = 1; k <= unit_num; ++k) {
		TransformMatrix(k);
		cout << "��" << k << "����Ԫ��\n";
		for (int i = 1; i <= 2; ++i) {
			for (int j = 1; j <= (node_DOF * node_num_each_unit); ++j) {
				cout << T[i][j] << '\t';
			}
			cout << '\n';
		}
	}
	cout << "------------------------------\n";

	cout << "�����˵�Ԫ����������ϵ�նȾ�����ԣ�\n";
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