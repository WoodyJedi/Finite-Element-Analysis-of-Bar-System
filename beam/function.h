
#ifndef INCFILE_H_
#define INCFILE_H_


#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 

#include "stdio.h"


//�洢�ṹ�ڵ�λ��
extern FILE* save_node_dis;
//�洢��Ԫ����
extern FILE* save_unit_force;
//�洢�ڵ���
extern FILE* save_node_force;
//��ʾ��������
extern FILE* show_input_data;
//ԭʼ����
extern FILE* input_data;
//��Ԫ�Ľڵ�����
extern int node_num_each_unit;
//�����ڵ�����ɶ���
extern int node_DOF;
//�ڵ�����
extern int node_num;
//��Ԫ����
extern int unit_num;
//��Լ�������ɶ�����
extern int constrained_DOF_num;
//��Ԫ�����������Ԫ�������
extern int unit_kind_num;
//N=node_num*node_DOF
extern int DOF;
//һά�洢global_stiffness_matrix��������
extern int stiffness_matrix_capacity;
//�����ڵ����ά����
extern double X[50], Y[50], Z[50];
extern double X2, X1, Y2, Y1, Z2, Z1, b;
//ÿ����Ԫ�Ľڵ�ţ�node_id_of_unit[1][i]��ŵ�i����Ԫ��һ���ڵ�����ţ�node_id_of_unit[2][i]��ŵ�i����Ԫ�ڶ����ڵ������
extern int node_id_of_unit[4][50];
//Լ����λ�ƺ�
extern int constrained_dx_id[30];
//PDE_id[i]��¼��i�����Խ�Ԫglobal_stiffness_matrix[i][i]��һά�洢�еı��
extern int PDE_id[50];
//ÿ����Ԫ�����
extern int unit_kind[100];
extern int NMN;
//unit_attribute[1][i]��ŵ�i�����͵ĵ�Ԫ������ģ����unit_attribute[2][i]��ŵ�i�����͵ĵ�Ԫ�ĺ�����
extern double unit_attribute[4][100];
//�ڵ��غ�
extern double P[100], P1[100];
//����ṹ�ڵ���
extern double PP[100];
//��Ԫ�����
extern double A;
//��Ԫ����ģ��
extern double E;
//��Ԫ���Ծ�
extern double I;
//��նȾ�����м����
extern double C;
//��Ԫ�նȾ���
extern double TK[7][7];
//����ת������
extern double T[7][7];
//����ת�������ת��
extern double TT[7][7];
//����նȾ���
extern double global_stiffness_matrix[100];
//��������ϵ�µĵ�Ԫ�ն���
extern double partial_stiffness_matrix[7][7];
//������˷�ʱ���м����
extern double s[7][7];
extern int partial_id_to_global_id[7];
//�˵�Ԫ�ĳ���
extern double L;
//��ԪӦ��
extern double SG;
//�ṹλ�ƾ���
extern double dr[100];
//��Ԫλ�ƾ���
extern double dr_result[100];
//�ֲ������µĵ�Ԫλ�ƾ���
extern double dee[50][7];
//��Ԫ�������ֲ�����ϵ�µĵ�Ԫ�ڵ�����
extern double Fee[50][7];
//��Ԫ��������ϵ�еĽڵ���
extern double F[50][100];
//�ⷽ��ʱ�õ���L,Y����
extern double l[100][100], y[100];


//���������������Ҫ�Ĳ���
void scan();

//��ÿi���˵ĳ���(i��ʾ��Ԫ��)
void Length(int i);

//��ÿi���˵�Ԫ�ĵ�Ԫ�նȾ���(��λ����ϵ��)
void StiffnessMatrix_unit(int i);

//�γɿռ���ܵ�Ԫ������ת������
void TransformMatrix(int i);

//�����Ԫ��������ϵ�еĸնȾ���
void MultiplyMatrix(int i);

//����partial_id_to_global_id���飬partial_id_to_global_id(i)��ʾ�ڵ�Ԫ�еĵ�i��λ�ƶ�Ӧ������ܽṹ�е�λ�Ʊ��
void creating_partial_id_to_global_id(int i);

//����PDE_id���飬��node_DOF*unit_num��һ���ڵ��Ӧ�����ɶ������Ե�Ԫ�Ľڵ�����PDE_id[i]��¼��i�����Խ�Ԫglobal_stiffness_matrix[i][i]��һά�洢�еı��
void creating_PDE_id(int i);

//��ÿ����Ԫ�նȾ����鼯Ϊ�ṹ�նȾ��󣬸ýṹ�նȾ���һά�洢������ֻ�洢�����ǵİ����
void StructureMatrix();

//��������Է�����AX=B,����A��n�״���ϡ��Գ���������X��B��n*1�׾���

void colve_equation(int n, double a[100], double x[100]);

//��ⵥԪ�����Լ�Լ�����������ƽ��
void OutputInternalForce();


//�����ú���
void test();


#endif  /* INCFILE_H_ */

