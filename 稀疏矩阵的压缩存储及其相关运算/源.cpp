#define _CRT_SECURE_NO_WARNINGS 1
#define DATA M->tu
//#define MAXSIZE 125//��MAXSIZE����ʱ������ջ����
#define MAXCOUNT 52
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>	

/*------���ݽṹ��ϡ�����ġ���Ԫ��˳����洢��ʾ------*/

/*---�����б�---*/
//SMatrix:ϡ�����
//Triple:��Ԫ��
//TSqList:��Ԫ��˳���
//TSMatrix:�á���Ԫ��˳�����ʾ��ϡ�����

typedef int ElemType;//����Ԫ��
typedef struct
{
	int i, j;//��0Ԫ��λ��
	ElemType e;//��0Ԫ��ֵ
}Triple,*TSqList;
//Triple:Ԫ�ؽṹ�壬���ڴ洢���󵥸���0Ԫ�أ���Ԫ�飩
//TSqList:�ṹ��ָ�룬���ڡ������������з�0Ԫ�أ�����Ԫ��˳��������飬ʹ��ָ�������malloc�����ڶ��϶�̬�����ڴ棬ע�����׵ġ�����ʧ�ܼ�顱�͡�free����
//ָ��Թ���������������һ�����������á�
typedef struct
{
	TSqList data;//��Ԫ��˳���������Ԫ�飬�����±��1��ʼ��0�Ŵ��ã���Ԫ��˳����Ѿ����Խ�һ��ϡ������Ԫ�ش洢�꣬�Ǿ����������
	int mu, nu, tu;//������Ϣ��Ϊ���ڲ������ش������������ͱ��������ڴ洢���������������������������Ԫ������������data[0],��������int,Ӱ�첻��
	int* rpos;//λ�ñ�
}TSMatrix, * TSMs;
//TSMatrix����������Ϣ��ϡ�����
//TSMs�����ڴ洢��������Ա��ڡ���������������ļ���
//���߼�������Ϣ

//typedef struct
//{
//	Triple data[MAXSIZE+1];//��MAXSIZE����ʱ��ջ����	
//	int mu, nu, tu;//
//}TSMatrix, * TSMs;//TSMatrix����������Ϣ��ϡ�����
////TSMs�����ڴ洢��������Ա��ڡ���������������ļ���

/*------�����б�------*/
/*������������*/
void Create(TSMatrix* M);//���벢��������
void GetRpos(TSMatrix* M);//���������������߼���Ϣ������Ԫ��λ�ñ�
void MatrixPrint(TSMatrix M);//������ʽ��ӡ
void SqListPrint(TSMatrix M);//��Ԫ����ʽ��ӡ
int Add(TSMatrix M, TSMatrix N, TSMatrix* Q);//����ӷ�
int maxNum(int a, int b);//���ؽϴ��������ڼӷ�����Ŀռ����
int minPos(int a, int b);//���ؽ�С����λ�ã�ǰһ�����Ǻ�һ�������ڼӼ������߼��ж�
int Subt(TSMatrix M, TSMatrix N, TSMatrix* Q);//�������
int Mult(TSMatrix M,TSMatrix N,TSMatrix* Q);//����˷�
void FastTranspose(TSMatrix M, TSMatrix* T);//����Ŀ���ת��
void printmenu();//��ӡ�˵�
void printtip(TSMatrix* M);//��ӡ����Ҫ��
/*������װ����*/
void Menu();//�˵�ѡ����
TSMatrix addMatrix(TSMatrix M);//����ӷ�����
TSMatrix subMatrix(TSMatrix M);//�����������
TSMatrix multMatrix(TSMatrix M);//����˷�����
TSMatrix transMatrix(TSMatrix M);//����ת�ù���
TSMatrix printMatrix(TSMatrix M);//��������빦��
void close();//�����˳�����

//void TSMatrixSort(TSMatrix* M);
//void Modify(TSMatrix M);
//�ۼӱ��湦��,���������ڴ����ж������

//void test2() {
//	TSMatrix M, N, Q;//�ֲ����������ڴ洢�û��������Ԫ��˳���,��Create�����г�ʼ��
//	Create(&M);
//	Create(&N);
//	SqListPrint(M);
//	MatrixPrint(M);
//	SqListPrint(N);
//	MatrixPrint(N);
//
//	Mult(M, N, &Q);
//	printf("\nM\t*\tN\t=\n");
//	SqListPrint(Q);
//	MatrixPrint(Q);
//}
//void test3() {
//	TSMatrix M, N, Q;//�ֲ����������ڴ洢�û��������Ԫ��˳���,��Create�����г�ʼ��
//	Create(&M);
//	Create(&N);
//	SqListPrint(M);
//	MatrixPrint(M);
//	SqListPrint(N);
//	MatrixPrint(N);
//
//	Subt(M, N, &Q);
//	printf("\nM\t-\tN\t=\n");
//	SqListPrint(Q);
//	MatrixPrint(Q);
//}
/*------������------*/
//ȫ�ֱ���
char m[MAXCOUNT];//�������
int x = 0;//��¼���������ÿ����һ�����󣬻��ߵõ�һ���¾��󣬾��������A������Z������a������y
int cnt = 0;//����������
int main() {
	char p = 'A', q = 'a';
	for (int i = 0; i < 26; i++) {
		m[i] = p;
		p++;
	}
	for (int i = 26; i < MAXCOUNT; i++) {
		m[i] = q;
		q++;
	}
	Menu();
	return 0;
}
/*------�Ӻ���------*/
/*������������*/
/*---����ϡ�������Ԫ��˳���1---*/
void Create(TSMatrix* M) {
	/*���������Ϣ*/
	printf("\t\t����������ϡ�������С��С�����Ԫ����");
	scanf("%d%d%d", &M->mu, &M->nu, &M->tu);
	/*�Ϸ��Լ��*/
	while (M->tu > M->mu * M->nu) {//M->tu > M->mu * M->nu��Ȼ���������ظ���ʹ�������޷���������
			printf("***>�Ƿ����룡Ԫ�ظ�����С�ڵ��ڡ�%d*%d=%d����������������ȷ��Ԫ�ظ�����\n", M->mu, M->nu, M->mu * M->nu);
			printf("\t\t�������Ԫ����");
			scanf("%d", &M->tu);
	}
	/*�������Ԫ��*/
	if (M->tu == 0) {//���Ϊ�������������Ԫ��
		M->data = NULL;//��ʼ��������ֵΪ�գ����Դ��ã���ʵҲ���Բ������if������ռ�����һ��Triple���׵�ַ
		printf("����������һ������󡣡�\n");
		return;//�����ת���У�pҲ�Ǵ�1��ʼ�����ᷢ������Խ�磬���ǿ���ʡ�Ը�������ǰ���׼��������Ϊ�ˣ�ֻ���ڸ������мӸ�������ж�ֱ�ӽ�����if
	}
	int p;//˳����ָ��
	M->data = (TSqList)malloc(sizeof(Triple) * (M->tu + 1));//�±��1��ʼ��0�Ŵ���
	if (!M->data) {
		printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
		return;//�������˵�
	}
	printtip(M);
	printf("�к� �к� ֵ��\n");
	/*������Ʊ��*/
	int** UniquePos;//Ψһ�Լ�飬ȷ���û������λ����ϢΨһ��ͬһ��λ��ֻ����һ��Ԫ�أ�λ���±��0��ʼ���������ռ�
	UniquePos = (int**)malloc(sizeof(int*) * M->mu);
	if (!UniquePos) {
		printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
		exit(1);//������������
		return;//�������˵�
	}
	for (int i = 0; i < M->mu; i++) {
		UniquePos[i] = (int*)malloc(sizeof(int) * M->nu);
		if (!UniquePos[i]) {
			printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
			exit(1);//������������
			return;//�������˵�
		}
	}
	/*
	* ���ڶ�ά����ָ�����⣺
	��ָ��������׵�ַ����ʼ������š�ÿһ�С���Ԫ�صĵ�ַ�������ڸ�Ԫ��ƫ����Ϊ��ά����һ��Ԫ�صĵ�ַ����
	ָ��������׵�ַ������ָ���ţ���һ��������ÿ����ָ�룬���䡰����*sizeof(int*)�����ڴ泤��
	��ÿ�е�ָ����������ڴ棬��ָ�룬���������ֱ���������е�ÿ��Ԫ�أ�ÿ�����䡰����*sizeof(int)�����ڴ泤��
	����ָ�붯̬�����ͷ��ڴ棡���ص㣺����ά���鶯̬�����ڴ棡һά�ĸ߽���ʽ����λ���
	�ȷ���һ��ռ䣬��СΪ������Ԫ���ֽ���*����������ٰ�����ڴ���׵�ַ(16�������У�
	��һ��ָ������Ԫ�ص�ָ�룬���ָ�롾ͨ���׵�ַ�͹̶�ƫ��������������Ԫ�ء�
	��ô�����������ָ�룬���൱�ڹ����������������Ԫ�أ�����ǡ����顿��ԭ��
	����⡿������ڴ���һά�ģ�������ͨ�����á���Ρ���ʹ�ڴ桾��ά����
	*/
	for (int i = 0; i < M->mu; i++) {//��Ʊ����ʼ�����±��0��ʼ
		for (int j = 0; j < M->nu; j++)
			UniquePos[i][j] = 0;
	}
	/*��ʼ����*/
	//int flag;//��Χ����Ψһ�Լ����źţ����ƺ����ķ���ֵ������������źŽ���ý�顿��������Ʊ������жϡ�
	bool repeat = false;
	for (p = 1; p <= M->tu; p++)
	{
		if (repeat) {//������ؼ�鲻ͨ������Ҫ������������
			for (int i = 1; i < p; i++)//��ӡ�ѽ��ܵ�����
			{
				printf("%d %d %d\n", M->data[i].i, M->data[i].j, M->data[i].e);
			}
			repeat = false;//��־ִ����Ҫ��ʱ�ָ����Ա��´��ж�
		}
		scanf("%d%d%d", &(M->data[p].i), &(M->data[p].j), &(M->data[p].e));
		/*�Ϸ��Լ�飬ʹ�û���ȷ����*/
		/*1.��Χ��顾�Ӳ����ܸ����ݡ�*/
		if (M->data[p].i<1 || M->data[p].i >M->mu || M->data[p].j<1 || M->data[p].j>M->nu)
		{//��δͨ���������²�������
			printf("***>�Ƿ����룡Ԫ�����к����ڡ�1-%d����1-%d���ڣ������������Ԫ�أ�\n",M->mu,M->nu);
			p--;//���ˣ��´���д��Triple
			//flag = 0;//���δͨ������Ψһ�Լ����ź�
			repeat = true;//��Ҫ����
		}
		else
		{//��ͨ���������²�������
			//flag = 1;//���ͨ������Ψһ�Լ����ź�
			++UniquePos[M->data[p].i - 1][M->data[p].j - 1];//����Χ���ͨ������Ʊ��ע���������ͨ����Ҫ��Ʊ�����򳬳���Χ��λ�ûᵼ�¼�Ʊ��Խ��
		}
		
		/*2.Ψһ�Լ��*/
		if (p >= 2) {//�ڶ���Ԫ���Ժ����ҪΨһ�Լ��,��ͨ����ǰ��ķ�Χ������ҪΨһ�Լ�顾����Ʊ������жϡ�
			if (UniquePos[M->data[p].i - 1][M->data[p].j - 1] >= 2) {//��0��  ˵��δͨ����Χ��飬�������Ψһ�Լ�飬��������䡿
																	 //��1��  ˵��ͨ����Χ��飬��ֻ����1�Σ�����Ψһ�ԣ���������䡿
																	 //��2~n��˵��ͨ����Χ��飬�����볬��1�Σ�������Ψһ�ԣ�ִ����ʾ�ͻ������������
				// Ʊ�� >= 2 ʱ��δͨ�����������²�����
				//printf("==>��λ���Ѵ��ڷ���Ԫ�أ��Ƿ���¸�λ��Ԫ�ء�y/!y����\n");
				printf("***>��λ������Ԫ�أ�������Ԫ�ص���ȷλ�ã�\n");
				p--;//���ˣ��´���д��Triple
				repeat = true;//��Ҫ����
			}
		}
	}
	/*ȷ������*/
	//��һ�û������˳���������ô�죿
	// ����ʱ���о������
	// �䵽����û�ˣ��Ǿ�ֻ�ܳ��������û����򣬻���ֱ������
	//int select;//����ѡ����
	//printf("===>�Ƿ���Ҫ�Դ���Ԫ��˳�����С��������ȡ������򣿡�1����Ҫ����2������Ҫ�����䣻3������Ҫ��ȷ������\n");
	//scanf("%d", &select);
	//if (select == 'y' || select == 'Y') {
	//	TSMatrixSort(M);//��Ԫ���ľ��������㷨
	//}
	/*���ټ�Ʊ��*/
	for (int i = 0; i < M->mu; i++)
		free(UniquePos[i]);//������+1��ָ����Ҫ�ͷţ���ô����Ķѿռ䣬����ô�ͷ�
	free(UniquePos);//���ͷŵͼ�ָ�룬����ͷŸ߼�ָ��
	/*����rpos*/
	GetRpos(M);
	printf("�������ɹ�����\n");
	return ;
}
/*
ɾ����printf("\t\t=====>");�����������������
*/
/*---������������Ϣ----*/
void GetRpos(TSMatrix* M) {
	/*����M��Ԫ��λ����Ϣ�������߼����ӡ�������������ϣ������*/
	//�������飺��Ʊ����λ����Ϣ��
	int row, p;
	int* num = (int*)malloc((M->mu + 1) * sizeof(int));//0��λ���ã��ռ�ΪM������һ��int��
	if (!num) {
		printf("\n ���ڴ治�㣬����ռ�ʧ�ܣ���");
		exit(0);
		return;
	}
	M->rpos = (int*)malloc((M->mu + 1) * sizeof(int));//��¼Mÿһ�еĸ�����ÿһ�е�һ��Ԫ�ص�λ��
	if (!M->rpos) {
		printf("\n ���ڴ治�㣬����ռ�ʧ�ܣ��� ");
		exit(0);
		return;
	}
	//��Ʊ����ʼ��
	for (row = 1; row <= M->mu; ++row)
		num[row] = 0;
	//ͳ��M����ÿ��Ԫ�ظ������� j �ĳ��ִ��� ��ÿ�� j ����һ������Ӧλ�ü�һƱ������Ʊ����
	for (p = 1; p <= M->tu; ++p)//p��1��tu!
		++num[M->data[p].i];
	//����M����ÿ�е�һ������Ԫ��ת�ú���T˳����е�λ��;���Ʒ�;ʹT�����ɸ����
	M->rpos[1] = 1;
	for (row = 2; row <= M->mu; ++row)
		M->rpos[row] = M->rpos[row - 1] + num[row - 1];
	free(num);//�ͷžֲ�����
	return;
}
/*---�������ת��---*/
void FastTranspose(TSMatrix M,TSMatrix* T){
	/*��������������*/
	if (M.tu == 0)//������ת��ֻ�轻������������������ҲҪ��ֵ����ʼ��
	{//�򻯲������Ż�
		T->data = NULL;
		T->mu = M.nu;
		T->nu = M.mu;
		T->tu = 0;
		return;
	}
	/*�ֲ������б�*/
	int p, q,col;//M��T��Ԫ�ز���ָ�룬M����ָ�룬pָM��qָT��colָM����
	/*����M��Ԫ��λ����Ϣ�������߼����ӡ�������������ϣ������*/
	/*ȷ��M��ÿ��Ԫ����T�е�λ�ã��ŵ�ʱ�򽻻�ij����*/
		//�������飺��Ʊ����λ����Ϣ��
	int* num = (int*)malloc((M.nu + 1) * sizeof(int));//0��λ���ã��ռ�ΪM������һ��int��
	if (!num) {//�ѷ������׼��
		printf("\n ���ڴ治�㣬����ռ�ʧ�ܣ���");
		exit(0);//������������
		return ;//�������˵�
	}
	int* cpot = (int*)malloc((M.nu + 1) * sizeof(int));//��¼Mÿһ�еĸ�����ÿһ�е�һ��Ԫ�ص�λ��
	if (!cpot) {
		printf("\n ���ڴ治�㣬����ռ�ʧ�ܣ��� ");
		exit(0);//������������
		return;//�������˵�
	}
		//��Ʊ����ʼ��
	for (col = 1; col <= M.nu; ++col)
		num[col] = 0;
		//ͳ��M����ÿ��Ԫ�ظ������� j �ĳ��ִ��� ��ÿ�� j ����һ������Ӧλ�ü�һƱ������Ʊ����
	for (p = 1; p <= M.tu; ++p)
		++num[M.data[p].j];
		//����M����ÿ�е�һ������Ԫ��ת�ú���T˳����е�λ��;���Ʒ�;ʹT�����ɸ����
	cpot[1] = 1;
	for (col = 2; col <= M.nu; ++col)
		cpot[col] = cpot[col - 1] + num[col - 1];
	/*����T���������ݣ��ֲ�������ʼ����*/
		/*д�������Ϣ*/
	T->mu = M.nu; T->nu = M.mu; T->tu = M.tu;
		/*д�����Ԫ�أ�˳���*/
			/*��˳����ָ�������ֵ��ʹ��ǰ�����ÿ��ָ�������һ��ȷ����ֵ����*/
	T->data= (TSqList)malloc(sizeof(Triple) * (M.tu + 1));
	if (!T->data) {
		printf("\n ����ռ�ʧ�ܣ� ");
		return;//�������˵�
	}
			/*��M˳����Ԫ�����η���T˳���Ķ�Ӧλ�ã�cpotλ����Ϣ������Ծʽ�洢����*/
	for (p = 1; p <= M.tu; ++p)
	{
		col = M.data[p].j;
		q = cpot[col];
		T->data[q].i = M.data[p].j;//T->data[cpot[M.data[p].j]].i=M.data[p].j;
		T->data[q].j = M.data[p].i;
		T->data[q].e = M.data[p].e;
		++cpot[col];
	}
	free(cpot);
	free(num);
	return ;
}
/*
�ҵ��㷨��⣺
�Ȱ�M��ÿ�е�һ���źã���Ӧ������T��ÿ�е�һ����0Ԫ��
M��һ��ɨ��Ĳ�һ����M��һ�еģ����Ը�Ԫ����T˳����оͲ�һ����T.data�ĵ�һ��Triple������T.data�ĵ�cpot[j]��Triple��
������T�еĴ洢����һ�֡���Ծʽ�Ĵ洢���������洢��Ͱ���򣿣���
��M��ÿ�е�һ����0Ԫ����T��ÿ�еĵ�һ����0Ԫλ�ú���M���ٴ�ɨ�赽ͬһ�е�Ԫ�غ󣬽����ϴ�֮���λ�ã�����++cpot[col];����
��ΪM����Ҳ������洢�ģ�ת��T�У�Tÿ��Ҳ���Ǹ��е��������Ը��㷨��ǰ���У�M˳���������Ҫ��������洢
*/
/*
��־��
-Add1-����:��Ҫȷ���û��������Ԫ��˳��������򣬼����뺯�������á��Ϸ��Լ�顱
����������->��������->�������룬��������->ð�ݻ��������->���ֹ��ܣ����û�������ɺ����û�ȷ�������Ƿ���Ҫ����
������ִ�����򡪡�����˳�򽻸��û�������ʱ���ǿ��к��к�����ģ����ȷ��һ�£��費��Ҫ����
ok-Add2-����:��Ҫȷ���û����ظ�����ͬһ��λ�ã����أ�ÿ��λ�ü�һ��Ʊ������Ʊ����
�������������Triple����д��λ�ã�p--
������������ظ�λ�ã���ô�ڶ��μ���֮�������Ԫ�ض����ᱻ��ӡ��˳����ָ�������ƶ�
ok-Add3-����:���û�����Tripleʱ����������кŴ����û�����ʵ������������Ҫ���û��������������
*/
/*---����ӷ�---*/
int Add(TSMatrix M, TSMatrix N, TSMatrix* Q)
{
	/*�Ϸ��Լ��*/
	if (M.mu != N.mu || M.nu != N.nu) {
		printf("�����ʧ�ܣ���ӵľ�����Ҫͬ�ͣ���\n");
		return 0;
	}
	if (M.tu == 0) {
		*Q = N;
		return 1;
		}
	if (N.tu == 0) {
		*Q = M;
		return 1;
	}//�����
	/*����ֲ�����*/
	int m, n, q;//����ָ��
	int count = 0;//����Q�ķ���Ԫ����
	int len = 0;//�ж��Ƿ���Ҫ���ӷ��䣬������Ҫ��̬����
	Triple* newbase = NULL;//��̬����˳�����
	Q->mu = M.mu;
	Q->nu = M.nu;
	Q->data = (Triple*)malloc(sizeof(Triple) * (maxNum(M.tu, N.tu)+1));//0��Ԫ��Ҫ���ǣ��������ݿռ�
	len = maxNum(M.tu, N.tu) + 1;//0��Ԫ��Ҫ���ǣ�ʵ��ʹ�ó���Ҫ��ȥ1��
	if (!Q->data) {
		printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
		exit(1);//������������
	}
	q = 1, m = 1, n = 1;//�ֱ�ָ��M,N,Q��1��λ��
	/*��ÿ��λ��Ϊ�о�������в���*/
	while (m <= M.tu && n <= N.tu) {//���ƹ鲢�����
		/*1.��̬���ӷ���*/
		if (q > len - 1) {//q��Q�к���һλ�󣬷��ֿռ䲻������len-1������q��ʵ�ʷ�Χ
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
				exit(1);//������������
			}
			Q->data = newbase;
		}
		/*2.������Ԫ��˳���ӷ��߼�*/
		if (M.data[m].i == N.data[n].i) {//iͬ
			if (M.data[m].j == N.data[n].j) {//i��j����ͬ����Ӻ�洢
				if (M.data[m].e + N.data[n].e != 0) {//��Ԫ�ز��洢!
					Q->data[q].e = M.data[m].e + N.data[n].e;//e
					Q->data[q].i = M.data[m].i;//i
					Q->data[q].j = M.data[m].j;//j
					m++, n++, count++;//ɨ���������
				}//if3
				else {
					m++, n++;//M,N����Ҫ����
					q--;//Qָ�����,�������ƣ�
				}//else3
			}//if2
			else {//iͬ��j��ͬ��ȡjС��
				Q->data[q].i = M.data[m].i;//i
				if (minPos(M.data[m].j, N.data[n].j) == 1) {
					Q->data[q].j = M.data[m].j;//j
					Q->data[q].e = M.data[m].e;//e
					m++, count++;
				}//if3
				else if (minPos(M.data[m].j, N.data[n].j) == 2) {
					Q->data[q].j = N.data[n].j;//j
					Q->data[q].e = N.data[n].e;//e
					n++, count++;
				}//else if3
			}//else2
		}//if1
		else {//i��ͬ
			if (minPos(M.data[m].i, N.data[n].i) == 1) {
				Q->data[q].i = M.data[m].i;//i
				Q->data[q].j = M.data[m].j;//j
				Q->data[q].e = M.data[m].e;//e
				m++, count++;
			}//if2
			else if (minPos(M.data[m].i, N.data[n].i) == 2) {
				Q->data[q].i = N.data[n].i;//i
				Q->data[q].j = N.data[n].j;//j
				Q->data[q].e = N.data[n].e;//e
				n++, count++;
			}//else if2
		}//else1
		/*3.Qÿ��һ��,ָ�����һλ*/
		q++;
	}
	/*���ƹ鲢�������ʣ��δ��ӵ�Ԫ�طŽ�Q�У�������еĶ�Ҫ��*/
	while (m <= M.tu) {
		/*1.��̬���ӷ���*/
		if (q > len - 1) {//q��Q�к���һλ�󣬷��ֿռ䲻������len-1������q��ʵ�ʷ�Χ
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
				exit(1);//������������
			}
			Q->data = newbase;
		}
		/*2.������Ԫ��˳���ӷ��߼�*/
		Q->data[q].i = M.data[m].i;//i
		Q->data[q].j = M.data[m].j;//j
		Q->data[q].e = M.data[m].e;//e
		m++, count++;
		/*3.Qÿ��һ��,ָ�����һλ*/
		q++;
	}
	while (n <= N.tu) {
		/*1.��̬���ӷ���*/
		if (q > len - 1) {//q��Q�к���һλ�󣬷��ֿռ䲻������len-1������q��ʵ�ʷ�Χ
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
				exit(1);//������������
			}
			Q->data = newbase;
		}
		/*2.������Ԫ��˳���ӷ��߼�*/
		Q->data[q].i = N.data[n].i;//i
		Q->data[q].j = N.data[n].j;//j
		Q->data[q].e = N.data[n].e;//e
		n++, count++;
		/*3.Qÿ��һ��,ָ�����һλ*/
		q++;
	}
	/*�����¾���ĸ���*/
	Q->tu = count;
	printf("����ӳɹ�����\n");
	return 1;
}
//���ؽϴ��������ڼӼ�������Ŀռ����
int maxNum(int a,int b) {
	return a > b ? a : b;
}
//���ؽ�С����λ�ã�ǰһ�����Ǻ�һ�������ڼӼ������߼��ж�
int minPos(int a, int b) {
	if (a < b) {
		return 1;
	}
	else if (a == b) {
		return -1;
	}
	else {
		return 2;
	}
}
/*---�������---*/
int Subt(TSMatrix M, TSMatrix N, TSMatrix* Q)
{
	/*�Ϸ��Լ��*/
	if (M.mu != N.mu || M.nu != N.nu) {
		printf("�����ʧ�ܣ���ӵľ�����Ҫͬ�ͣ���\n");
		return 0;
	}
	if (N.tu == 0) {//��ȡ����󲻱䣬���Ǳ�����������
		*Q = M;
		return 1;
	}//�����
	/*����ֲ�����*/
	int m, n, q;//����ָ��
	int count = 0;//����Q�ķ���Ԫ����
	int len = 0;//�ж��Ƿ���Ҫ���ӷ��䣬������Ҫ��̬����
	Triple* newbase = NULL;//��̬����˳�����
	Q->mu = M.mu;
	Q->nu = M.nu;
	Q->data = (Triple*)malloc(sizeof(Triple) * (maxNum(M.tu, N.tu) + 1));//0��Ԫ��Ҫ���ǣ��������ݿռ�
	len = maxNum(M.tu, N.tu) + 1;//0��Ԫ��Ҫ���ǣ�ʵ��ʹ�ó���Ҫ��ȥ1��
	if (!Q->data) {
		printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
		exit(1);//������������
	}
	q = 1, m = 1, n = 1;//�ֱ�ָ��M,N,Q��1��λ��
	/*��ÿ��λ��Ϊ�о�������в���*/
	while (m <= M.tu && n <= N.tu) {//���ƹ鲢�����
		/*1.��̬���ӷ���*/
		if (q > len - 1) {//q��Q�к���һλ�󣬷��ֿռ䲻������len-1������q��ʵ�ʷ�Χ
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
				exit(1);//������������
			}
			Q->data = newbase;
		}
		/*2.������Ԫ��˳�������߼�*/
		if (M.data[m].i == N.data[n].i) {//iͬ
			if (M.data[m].j == N.data[n].j) {//i��j����ͬ����Ӻ�洢
				if (M.data[m].e - N.data[n].e != 0) {//��Ԫ�ز��洢!
					Q->data[q].e = M.data[m].e - N.data[n].e;//e
					Q->data[q].i = M.data[m].i;//i
					Q->data[q].j = M.data[m].j;//j
					m++, n++, count++;//ɨ���������
				}//if3
				else {
					m++, n++;//M,N����Ҫ����
					q--;//Qָ�����,�������ƣ�
				}//else3
			}//if2
			else {//iͬ��j��ͬ��ȡjС��
				Q->data[q].i = M.data[m].i;//i
				if (minPos(M.data[m].j, N.data[n].j) == 1) {
					Q->data[q].j = M.data[m].j;//j
					Q->data[q].e = M.data[m].e;//e�����M�ĸ�λ�ø�ǰ��˵��N�Ķ�Ӧλ��Ϊ��,���λ��Ϊ����
					m++, count++;
				}//if3
				else if (minPos(M.data[m].j, N.data[n].j) == 2) {
					Q->data[q].j = N.data[n].j;//j
					Q->data[q].e = N.data[n].e * -1;//e�����N�ĸ�λ�ø�ǰ��˵��M�Ķ�Ӧλ��Ϊ��,���λ��Ϊ�෴��
					n++, count++;
				}//else if3
			}//else2
		}//if1
		else {//i��ͬ
			if (minPos(M.data[m].i, N.data[n].i) == 1) {
				Q->data[q].i = M.data[m].i;//i
				Q->data[q].j = M.data[m].j;//j
				Q->data[q].e = M.data[m].e;//e�����M�ĸ�λ�ø�ǰ��˵��N�Ķ�Ӧλ��Ϊ��,���λ��Ϊ����
				m++, count++;
			}//if2
			else if (minPos(M.data[m].i, N.data[n].i) == 2) {
				Q->data[q].i = N.data[n].i;//i
				Q->data[q].j = N.data[n].j;//j
				Q->data[q].e = N.data[n].e * -1;//e�����N�ĸ�λ�ø�ǰ��˵��M�Ķ�Ӧλ��Ϊ��,���λ��Ϊ�෴��
				n++, count++;
			}//else if2
		}//else1
		/*3.Qÿ��һ��,ָ�����һλ*/
		q++;
	}
	/*���ƹ鲢�������ʣ��δ��ӵ�Ԫ�طŽ�Q�У�������еĶ�Ҫ��*/
	while (m <= M.tu) {
		/*1.��̬���ӷ���*/
		if (q > len - 1) {//q��Q�к���һλ�󣬷��ֿռ䲻������len-1������q��ʵ�ʷ�Χ
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
				exit(1);//������������
			}
			Q->data = newbase;
		}
		/*2.Mֱ�ӷ�����*/
		Q->data[q].i = M.data[m].i;//i
		Q->data[q].j = M.data[m].j;//j
		Q->data[q].e = M.data[m].e;//e
		m++, count++;
		/*3.Qÿ��һ��,ָ�����һλ*/
		q++;
	}
	while (n <= N.tu) {
		/*1.��̬���ӷ���*/
		if (q > len - 1) {//q��Q�к���һλ�󣬷��ֿռ䲻������len-1������q��ʵ�ʷ�Χ
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
				exit(1);//������������
			}
			Q->data = newbase;
		}
		/*2.N����Ԫ����Ҫ*-1�ٷ���*/
		Q->data[q].i = N.data[n].i;//i
		Q->data[q].j = N.data[n].j;//j
		Q->data[q].e = N.data[n].e * -1;//e
		n++, count++;
		/*3.Qÿ��һ��,ָ�����һλ*/
		q++;
	}
	/*�����¾���ĸ���*/
	Q->tu = count;
	printf("������ɹ�����\n");
	return 1;
}
/*---����˷�----*/
int Mult(TSMatrix M, TSMatrix N, TSMatrix* Q)
{
	/*�Ϸ��Լ��*/
	if (M.nu != N.mu) {//���е�����
		printf("�����ʧ�ܣ�N�����������M����������\n");
		return 0;
	}
	if (M.tu == 0 || N.tu == 0) {//MΪ������NΪ����󣬼�û�з���Ԫ
		Q->tu = 0;
		Q->mu = M.mu;//��ȡ��
		Q->nu = N.nu;//��ȡ��
		Q->data = NULL;
		Q->rpos = NULL;
		return 1;
	}
	/*����ֲ�����*/
	//int m, n, q;//����ָ��
	//int count = 0;//ͳ��Q����Ԫ������Q->tu
	int len = 0;//�ж��Ƿ���Ҫ���ӷ��䣬������Ҫ��̬����
	Triple* newbase = NULL;//��̬����˳����ȵ��м����
	Q->mu = M.mu;//��ȡ��
	Q->nu = N.nu;//��ȡ��
	Q->tu = 0;//��д����
	Q->rpos = (int*)malloc(sizeof(int) * (M.mu + 1));//Q�ĳ�ʼ��
	len = M.tu + N.tu;//0��Ԫ��Ҫ���ǣ�ʵ��ʹ�ó���Ҫ��ȥ 1��
	len += len / 2 + 1;
	Q->data = (Triple*)malloc(sizeof(Triple) * len);//0��Ԫ��Ҫ���ǣ�����һ������Ԫ��ҲҪ�������ݿռ�
	int* ctemp = (int*)malloc(sizeof(Triple) * (N.tu + 1));//�ۼӺ�Ҫѹ��ȥ�㣬��ʱ�ǵ�free();
	if (!Q->data) {
		printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
		exit(1);//������������
	}
	/*�����ӳ˷��߼�*/
	//���ۼ���
	int ccol = 0;
	//��ǰ�����M���б� 
	int arow;
	//���д���,arow����ǰ�˾���M���к�Ҳ�ǽ������Q���к� 
	for (arow = 1; arow <= M.mu; ++arow) 
	{//3.���������е�ȫ�����
		//��Ԫ���ۼ�����0 
		for (int i = 0; i <= N.nu; i++)
			ctemp[i] = 0;
		//ÿ����һ�����õ�ǰ�еĵ�һ����0Ԫ��λ��Ϊ֮ǰ�����з�0����+1 ������Q��rpos
		Q->rpos[arow] =	Q->tu + 1;//��ʾQ��д���������ǰ������������
		//��ǰ�е���һ�е�һ����0Ԫ����M.data�е�λ�ã���������һ��ȡ���еķ�0Ԫ����֮��+1 ��rpos[row]>tu,��ʾ��һ��û��Ԫ��
		int tp = 0;//λ��
		if (arow < M.mu)
			tp = M.rpos[arow + 1];
		else
			tp = M.tu + 1;//���õ�ǰ����˳���ķ�Χ��M.rpos[arow]~tp-1����һ�еĻ�M.rpos[arow]==1
		int p = 0;//Mָ��
		int q = 0;//Nָ��
		int t = 0;//λ��
		//�Ե�ǰ�е�ÿһ����0Ԫ�ش����������Ӧ��λ�ó˻��� 
		int brow = 0;
		for (p = M.rpos[arow]; p < tp; ++p)//M�ĸ���˳���λ�ã�M.rpos[arow]~tp-1
		{//2.����һ�е�ȫ�����
			//�ҵ���Ԫ�ض�Ӧ��N���к� 
			brow = M.data[p].j;
			//ȡ�ҵ���N��Ӧ����һ�е�һ��Ԫ�ص�λ�ã���������һ��ȡ���еķ�0Ԫ��֮��+1  
			if (brow < N.mu)
				t = N.rpos[brow + 1];
			else
				t = N.tu + 1;//����Ŀ������N˳���ķ�Χ��N.rpos[brow]~t-1,
			//N��Ӧ��λ���ϵ�Ԫ����M��ǰλ��Ԫ����ˣ��ۼӽ���ctemp���ۼ�����Ӧλ�� 
			for (q = N.rpos[brow]; q < t; ++q)//N�Ķ�Ӧ��˳���λ�ã�N.rpos[brow]~t-1��M.data[p].e����ÿ��q��ָN��Ԫ�أ��ֱ�õ����������еĲ�������
			{//1.����һ�еĲ��ֽ��,��ctemp������г˻��������һ�����У���������λ���ϵ����ݣ����ֻ��ҪN.nu���ռ�
				//ȡNԪ�ص��кţ���Ӧ����������
				ccol = N.data[q].j;
				ctemp[ccol] += M.data[p].e * N.data[q].e;//�ֱ��Ӧ���������N��ÿһ�еĽ��������Mһ����һ��Ԫ�صĽ��
				//�����������Ҫÿ�ж�Ӧ�ۼӱ���M����ǰ�е�ÿһ���˻����У�M.data[p].e�˳����Ĳ���Ҫ�ۼӵģ��Ǹ��еģ���ν�����д���
				//����Ҫ�ֱ��Ӧ����������ǰ�е�ÿһ���ϵģ���ǰ�е�ÿһ������Ԫ�ĳ˻����ж����ۼӣ�
				//���ʱ��������һ�б��������
				//����֮��������֪��ÿ��Ԫ����˳����еľ��巶Χ������ÿ�ζ��Ǽ���һ�еĲ��ֽ���������д���
			}//for1	
		}//for2

		//�ۼ����϶�Ӧλ��ѹ���洢 
		for (ccol = 1; ccol <= Q->nu; ++ccol)//һ�����飬�ۼӺ��˵�ǰ�е����н����M�ӵ�һ�п�ʼ��Q�õ���Ҳ�ǵ�һ�п�ʼ
		{//�պ��ǰ����������У�ÿ��N.nu��
			if (ctemp[ccol]) {//���δ���ǰ�е�ÿһ��Ԫ�أ���������Ϊ�㣬��ѹ���洢
				//Q->tu��һ���ٴ���
				++Q->tu;
				if (Q->tu > len - 1) //�ռ䲻��
				 {//q��Q�к���һλ�󣬷��ֿռ䲻������len-1������q��ʵ�ʷ�Χ
					newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
					len += len / 2;
					if (!newbase) {
						printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
						exit(1);//������������
					}
					Q->data = newbase;
					free(newbase);
				}
				Q->data[Q->tu].i = arow;//��ǰ���ڴ������
				Q->data[Q->tu].j = ccol;//��ǰ���ڴ�����е�ÿһ��
				Q->data[Q->tu].e = ctemp[ccol];//��ǰ���ڴ�����е�ÿһ������Ӧ����ۼӵĽ��
			}//if��Ϊ0
		}//for2.2ѹ���洢
	}//for3
	//Q->rpos[arow] = Q->tu + 1;//Q���кŵ���M���к�
	free(ctemp);
	printf("����ɹ���\n");
	return 1;
}
/*---˳������---*/
void SqListPrint(TSMatrix M) {
	printf("%d %d %d\n", M.mu, M.nu, M.tu);
	for (int j = 1; j <= M.nu * 2 + 1; j++) {
		printf("-");
		if (j == M.nu * 2 + 1)printf("\n");
	}
	for (int i = 1; i <= M.tu; i++)
	{
		printf("%d %d %d", M.data[i].i, M.data[i].j, M.data[i].e);
		printf("\n");//ѡ���Ի���
	}
}
/*---������ʽ���---*/
void MatrixPrint(TSMatrix M) {//Ŀ�ģ��ҵ���Ԫ�����Ԫ�صĶ�Ӧλ�ã�ƥ������
	int p = 1;//ָʾ��Ԫ��˳����ָ��
	for (int i = 1; i <= M.mu; i++){
		printf("\t\t=====>");
		for (int j = 1; j <= M.nu; j++){//i��j��ָʾ��������ָ��
			if (M.tu != 0) {//�������������dataΪNULL!
				if (M.data[p].i == i && M.data[p].j == j && p <= M.tu) {//tu�������˺���ȫ���0����
					printf("%d ", M.data[p].e);
					p++;//!!!ƥ��ɹ���˳����ָ����ƣ�δƥ��ɹ���������ң�ǰ����p��ָԪ�ص�λ����ijָ�봦���Ժ��λ��
				}
				else printf("%d ", 0);
			}//�����ֱ��ȫ��0
			else printf("%d ", 0);
		}
		printf("\n");//���һ�в��û���
	}
}
/*
������־��
���㷨������ϡ��ȳ�ʼ��һ����ά���飬�ٸ��Ķ�Ӧλ�õķ�0Ԫ��,S=O(mu*nu)����ȣ�
ʹ�õĿռ临�Ӷ�ΪO(1),ʱ�临�Ӷȶ�ΪT=O(n^2)
  
���㷨��ǰ����p��ָԪ�ص�λ����ijָ�봦���Ժ��λ��
�����Ϊ�Լ���λ�ñ���һ��ռ�ˣ����Լ�����һ��ײλ���ˣ�����ijָ����Զ�Ҳ����Լ���λ�ã���ô�Լ�����
��Ԫ�ؽ����ᱻ�������Ϊpָ��Ҫ�Լ�������˲Ż����
�����Ը��㷨���������ظ�λ�õĳ��֡���ͬһλ�õĵڶ���Ԫ�ز���û��������ᣬ���ᵼ�º�������Ԫ�ض��޷������
����������桿���Ľ�������Ϊ���ټ�һ��ѭ��������i,j����p > M.tuΪ�˳���������ͬλ�ûᱻ����Ԫ�ض���
��pָ��ʹ����˳��������������������ӡԪ�أ���������ν�����ǻ��桿������Ҿ���Ľṹ
���Դ�ӡ����ǲ�����ͬһ��λ��������Ԫ�أ��������Ļ��һ��λ�þ���һ�����������滻��Ļ�ϵĽ����ֻ��
��ͷ����þ���
������ʽ�����һ���£�������ʽ�洢����ά���顿����һ���£��洢���ԡ����ǻ������¡���ͬһ��λ��ֻ�洢���һ����������
���ǵ��ǡ���Ԫ��˳��������ԣ��Ͳ��������ظ�λ�õĳ��֣�
�����㷨Ҳ����������ʵ�ʸ�������Χ��Ԫ�س��֡�
*/

/*�˵�ѡ����-*/
void Menu()
{
	printmenu();//�˵������ӡ
	int select;//����ѡ��
	int continu = 0;//�Ƿ����
	TSMatrix M, T;
	while (1)
	{
		if (cnt >= 1) {//ʹ�ù�һ������
			printf("�Ƿ�������е������㡾1������ ���������������룺");
			scanf("%d", &continu);
			if (continu == 1)//�����Ҫ��������һ�����T��ֵ����ʼ����M
				M = T;//����Ͳ���Ҫ������M�ˡ����M�ĸ�ֵ��֧��������һ��T,һ���û����룬���������ԸĽ�����
		}
		//����Ϊ������ѡ��
		printf("��ѡ������Ҫ���е����㡾0-5����");
		scanf("%d", &select);
		//���������
		if (select != 0) {//�û�ѡ��0���˳�ʱ,�����������ֱ���˳�
			if (cnt == 0 || continu != 1) {//��һ��ʹ�û��ǵ�һ��ʹ���Ҳ���Ҫ����ʱ������Ҫ�����ʼ����M
				printf("������һ������ %c ��\n", m[x]);
				Create(&M);
			}
		}
		/*����ѡ��*/
		switch (select)
		{
		case 1:T = addMatrix(M); break;
		case 2:T = subMatrix(M); break;
		case 3:T = multMatrix(M); break;
		case 4:T = transMatrix(M); break;
		case 5:T = printMatrix(M); break;
		case 0:close(); break;
		default:printf("�����ڡ�0-5��֮��ѡ������Ҫ�Ĺ��ܡ���\n");
		}
	}//while
}
/*�ӷ�����*/
TSMatrix addMatrix(TSMatrix M) {
	TSMatrix N,Q;
	printf("��������Ҫ��ӵľ��� %c ��\n", m[x+1]);
	int y = x;
	x++;
	Create(&N);
	int flag = Add(M, N, &Q);
	SqListPrint(M);
	MatrixPrint(M);
	SqListPrint(N);
	MatrixPrint(N);
	if (flag==0) {
		return M;
	}
	printf("\n%c\t+\t%c\t=\n",m[y],m[x]);
	SqListPrint(Q);
	MatrixPrint(Q);
	cnt++;
	return Q;
}
/*��������*/
TSMatrix subMatrix(TSMatrix M) {
	TSMatrix N, Q;
	printf("��������Ҫ����ľ��� %c ��\n", m[x+1]);
	int y = x;
	x++;
	Create(&N);
	int flag = Subt(M, N, &Q);
	SqListPrint(M);
	MatrixPrint(M);
	SqListPrint(N);
	MatrixPrint(N);
	if (flag == 0) {
		return M;
	}
	printf("\n%c\t-\t%c\t=\n", m[y], m[x]);
	SqListPrint(Q);
	MatrixPrint(Q);
	cnt++;
	return Q;
}
/*�˷�����*/
TSMatrix multMatrix(TSMatrix M) {
	TSMatrix N, Q;
	printf("��������Ҫ��˵ľ��� %c ��\n", m[x+1]);
	int y = x;
	x++;
	Create(&N);
	int flag=Mult(M, N, &Q);
	SqListPrint(M);
	MatrixPrint(M);
	SqListPrint(N);
	MatrixPrint(N);
	if (flag == 0) {
		return M;
	}
	printf("\n%c\t*\t%c\t=\n", m[y], m[x]);
	SqListPrint(Q);
	MatrixPrint(Q);
	cnt++;
	return Q;
}
/*ת�ù���*/
TSMatrix transMatrix(TSMatrix M) {
	TSMatrix T;
	int y = x;
	x++;
	FastTranspose(M, &T);
	SqListPrint(M);
	MatrixPrint(M);
	printf("\n%c\tת�ú�=\t%c\t\n", m[y], m[x]);
	SqListPrint(T);
	MatrixPrint(T);
	cnt++;
	return T;
}
/*���빦��*/
TSMatrix printMatrix(TSMatrix M) {
	printf("������ľ��� %c Ϊ��\n", m[x]);
	SqListPrint(M);
	MatrixPrint(M);
	cnt++;
	return M;
}
/*�˳�����*/
void close() {
	printf("\n\t\t\t\t   ��������������������л����ʹ�ã���������������\n");
	exit(0);
}

/*��ӡ�˻�����ķ�װ*/
/*---��ӡ�˵�---*/
void printmenu() {
	system("cls");
	//system("color 0A");
	printf("\t\t\t\t   ******************************\n");
	printf("\t\t\t\t   *       �� �� �� �� ��       *\n");
	printf("\t\t\t\t   ******************************\n");
	printf("\t\t\t\t   ������������������������������������������������������������\n");
	printf("\t\t\t\t   �� 1. ��          2. ��       ��\n");
	printf("\t\t\t\t   �� 3. ��          4. ת��     ��\n");
	printf("\t\t\t\t   �� 5. ��ʾ        0. �˳�     ��\n");
	printf("\t\t\t\t   ������������������������������������������������������������\n");
	printf("\t\t\t\t   ��������������������ӭ����ʹ�ã���������������\n");
}
/*---��ӡ������ʾҪ��---*/
void printtip(TSMatrix* M) {
	printf("\t\t����������������������������������������������������������������������������������������������������������������������������������������������������\n");
	printf("\t\t��  �밴�����к��С�˳���������� %d ������Ԫ���кż�Ԫ��ֵ |���кŴ�1��ʼ| ��\n", DATA);//����
	printf("\t\t����������������������������������������������������������������������������������������������������������������������������������������������������\n");
}

/*������*/

/*
bug��־��
int* num;//vs�ص㣺�����ȳ�ʼ���������ַδ֪������devc++ȴ����
int* cpot;

bug��־��
Q������ռ��ᣬת�û�ı�M.data[3]�е�j !?��ֻ�ܲ��ö�������ķ�ʽ����ռ䣿
A��Ī�������ֺ��ˣ�����

������־��
printf("==>��λ���Ѵ��ڷ���Ԫ�أ��Ƿ���¸�λ��Ԫ�ء�y/!y����\n");
scanf("%c", &select);
if (select == 'y' || select == 'Y')
pass_����������;�������ô�죿�����ܣ����Һ������漰��˳���Ĳ��Һ�ָ��Ԫ�صĸ��£���ʱ���¡�
�Ȳ������򣨻��������ٶ��ֲ��ң��ȶ��ֲ������±꣬�ٶ��ֲ������±�
������������ǰ׼���ã�ֱ�ӵ��븴�ƾ���

�޸���־��2023/12/24
ok_�����û����������Ҫ����ʱ�����Լ���ȷ�������Ϣ����ֱ�۵س��ֳ���������ĸĽ���
������ʾ���д�ӡ���������ȷ���ݣ��û��������ں������룬�û���ֱ�۵ؿ����Լ���˳�����ܵ���������
�޸ĺ�����void Create(TSMatrix* M);

�޸���־��2023/12/27
1����tu����mu*nuʱ������ΪΨһ�Լ����޷�����
�޸ĺ������޸ĺ�����void Create(TSMatrix* M);
2��˳����ӡ����mu\nu\tu
3��������0��Ԫ�أ�if (q > len) 
4��for (q = 1, m = 1, n = 1; m <= M.tu && n <= N.tu; ) {
	//���ﲻ��Ҫm++,n++���ŵ���֧�ṹ���ˣ���while������	
	}//for
5���±����⣬û�ж�Ӧ��N.data[m]=>N.data[n]
6���ӷ��Ľ����������Ԫ�أ�ѡ���Դ洢��
*/


//void test() {
//	TSMatrix M, N, Q;//�ֲ����������ڴ洢�û��������Ԫ��˳���,��Create�����г�ʼ��
//	Create(&M);
//	GetRpos(&M);
//	for (int i = 1; i <= M.mu; i++) {
//		printf("%d ", M.rpos[i]);
//	}
//}
//void TSMAdd() {
//	TSMatrix M, N, Q;//�ֲ����������ڴ洢�û��������Ԫ��˳���,��Create�����г�ʼ��
//	Create(&M);
//	Create(&N);
//	SqListPrint(M);
//	MatrixPrint(M);
//	SqListPrint(N);
//	MatrixPrint(N);
//
//	Add(M, N, &Q);
//	printf("\nM\t+\tN\t=\n");
//	SqListPrint(Q);
//	MatrixPrint(Q);
//}

//void Transpose() {
//	TSMatrix M, T;//�ֲ����������ڴ洢�û��������Ԫ��˳���,��Create�����г�ʼ��
//	Create(&M);
//	printf("\nԭʼ����˳���:\n");
//	SqListPrint(M);
//	printf("\nԭʼ����:\n");
//	MatrixPrint(M);
//
//	FastTranspose(M, &T);
//	printf("\nת�þ���˳���:\n");
//	SqListPrint(T);
//	printf("\nת�þ���:\n");
//	MatrixPrint(T);
//	printf("\n");
//}

//for (int i = 0; i < M->mu; i++) {//��Ʊ������ֵ
//	for (int j = 0; j < M->nu; j++)
//		printf("%d ", UniquePos[i][j]);
//	printf("\n");//ÿ�л�һ��
//}

//for (int i = 0; i < M->mu; i++) {//��Ʊ����ʼֵ
//	for (int j = 0; j < M->nu; j++)
//		printf("%d ", UniquePos[i][j]);
//	printf("\n");//ÿ�л�һ��
//}

///*---����ϡ�������Ԫ��˳���2---*/
//void Create(TSMatrix* M) {
//	int p;
//	printf("\n���ʸ�ϡ�����������������Լ���0Ԫ����(��ͬ�������ÿո�ָ�):");
//	scanf("%d %d %d", &M->mu, &M->nu, &M->tu);
//	printf("\n������������%d����0Ԫ�ص����кż�Ԫ��ֵ:\n", M->tu);
//	printf("�к�\t�к�\tֵ\n");
//	for (p = 1; p <= M->tu; p++)
//		scanf("%d%d%d", &M->data[p].i, &M->data[p].j, &M->data[p].e);
//	return;
//}

///*---����ת��2---*/
//TSMatrix FastTranspose(TSMatrix M) {
//	TSMatrix T = M;
//	int p, q;//M��T��Ԫ�ز���ָ�룬pָM��qָT
//	int col;//M����ָ�룬colָ��
//	
//	/*������Ԫ����Ϣ��---��λ����������ϣ����*/
//		//��������
//	int num[MAXSIZE] = { 0 };//��MAXSIZE����ʱ������ջ����
//	int cpot[MAXSIZE];
//		//ͳ��M����ÿһ�У�j���ĸ��� ��ÿ�г��ּ�һƱ������Ʊ����
//	for (p = 1; p <= M.tu; ++p)
//		++num[M.data[p].j];
//		//����M����ÿһ�е�һ������Ԫ����ת�ú󡱣���T��˳����е�λ��;���Ʒ�;
//	cpot[1] = 1;
//	for (col = 2; col <= M.nu; ++col)
//		cpot[col] = cpot[col - 1] + num[col - 1];
//
//	/*����ɨ��M˳��������Ԫ�أ�����cpot��������ӦT˳����е�λ�ã�����Ծʽ�洢����*/
//	T.mu = M.nu; T.nu = M.mu; T.tu = M.tu;//��Ԫ��˳�����Ϣ
//	for (p = 1; p <= M.tu; ++p)
//	{
//		col = M.data[p].j;
//		q = cpot[col];
//		T.data[q].i = M.data[p].j;//T.data[cpot[M.data[p].j]].i=M.data[p].j;
//		T.data[q].j = M.data[p].i;
//		T.data[q].e = M.data[p].e;
//		++cpot[col];
//		/*
//		�ҵ��㷨��⣺����Ծ�ԵĴ洢��
//		�Ȱ�M��ÿ�е�һ���źã���Ӧ������T��ÿ�е�һ����0Ԫ
//		M��һ��ɨ��Ĳ�һ����M��һ�еģ����Ը�Ԫ����T˳����оͲ�һ���ǵ�һ��"T.data[1]"������cpot[j]�����Ӧλ��"T.data[cpot[j]]"
//		������T�еĴ洢��һ�֡���Ծ�ԵĴ洢���������洢����
//		��T�е�ÿ�е�һ����0Ԫ�źú���M���ٴ�ɨ�赽���е�Ԫ�غ󣬽������ͺá�++cpot[col];������ΪM����Ҳ�ǰ�˳��洢�ģ�
//		ת��T�У�Tÿ��Ҳ���Ǹ��е�����
//		*/
//	}
//	return T;
//}

//void Create(TSMatrix* M) {
//	/*���������Ϣ*/
//	printHead();
//	printf("\t\t����������ϡ�������С��С�����Ԫ����");
//	scanf("%d%d%d", &M->mu, &M->nu, &M->tu);
//
//	/*�Ϸ��Լ��*/
//	for (; M->tu > M->mu * M->nu;) {
//		if (M->tu > M->mu * M->nu) {
//			printf("***>�Ƿ����룡Ԫ�ظ�����С�ڵ��ڡ�%d*%d=%d���������������Ԫ�أ�\n", M->mu, M->nu, M->mu * M->nu);
//			printf("\t\t�������Ԫ����");
//			scanf("%d", &M->tu);
//		}
//	}
//	/*�������Ԫ��*/
//	if (M->tu == 0) {//���Ϊ�������������Ԫ��
//		M->data = NULL;//��ʼ��������ֵΪ�գ����Դ��ã���ʵҲ���Բ������if������ռ�����һ��Triple���׵�ַ
//		printf("\t\t=====>����������һ������󡣡�\n");
//		return;//�����ת���У�pҲ�Ǵ�1��ʼ�����ᷢ������Խ�磬���ǿ���ʡ�Ը�������ǰ���׼��������Ϊ�ˣ�ֻ���ڸ������мӸ�������ж�ֱ�ӽ�����if
//	}
//	int p;//˳����ָ��
//	M->data = (TSqList)malloc(sizeof(Triple) * (M->tu + 1));//�±��1��ʼ��0�Ŵ���
//	if (!M->data) {
//		printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
//		return;//�������˵�
//	}
//
//	print1(M);
//	printf("\t\t=====>�к� �к� ֵ��\n");
//	/*������Ʊ��*/
//	int** UniquePos;//Ψһ�Լ�飬ȷ���û������λ����ϢΨһ��ͬһ��λ��ֻ����һ����λ���±��0��ʼ���������ռ�
//	UniquePos = (int**)malloc(sizeof(int*) * M->mu);
//	if (!UniquePos) {
//		printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
//		exit(1);//������������
//		return;//�������˵�
//	}
//	for (int i = 0; i < M->mu; i++) {
//		UniquePos[i] = (int*)malloc(sizeof(int) * M->nu);
//		if (!UniquePos[i]) {
//			printf("***>�ڴ治�㣬����ռ�ʧ�ܣ�\n ");
//			exit(1);//������������
//			return;//�������˵�
//		}
//	}
//	/*
//	��ָ��������׵�ַ����ʼ������š�ÿһ�С���Ԫ�صĵ�ַ�������ڸ�Ԫ��ƫ����Ϊ��ά����һ��Ԫ�صĵ�ַ����
//	ָ��������׵�ַ������ָ���ţ���һ��������ÿ����ָ�룬���䡰����*sizeof(int*)�����ڴ泤��
//	��ÿ�е�ָ����������ڴ棬��ָ�룬���������ֱ���������е�ÿ��Ԫ�أ�ÿ�����䡰����*sizeof(int)�����ڴ泤��
//	����ָ�붯̬�����ͷ��ڴ棡���ص㣺����ά���鶯̬�����ڴ棡һά�ĸ߽���ʽ����λ���
//	�ȷ���һ��ռ䣬��СΪ������Ԫ���ֽ���*����������ٰ�����ڴ���׵�ַ(16�������У�
//	��һ��ָ������Ԫ�ص�ָ�룬���ָ�롾ͨ���׵�ַ�͹̶�ƫ��������������Ԫ�ء�
//	��ô�����������ָ�룬���൱�ڹ����������������Ԫ�أ�����ǡ����顿��ԭ��
//	����⡿������ڴ���һά�ģ�������ͨ�����á���Ρ���ʹ�ڴ桾��ά����
//	*/
//	for (int i = 0; i < M->mu; i++) {//��Ʊ����ʼ�����±��0��ʼ
//		for (int j = 0; j < M->nu; j++)
//			UniquePos[i][j] = 0;
//	}
//	//for (int i = 0; i < M->mu; i++) {//��Ʊ����ʼֵ
//	//	for (int j = 0; j < M->nu; j++)
//	//		printf("%d ", UniquePos[i][j]);
//	//	printf("\n");//ÿ�л�һ��
//	//}
//	/*��ʼ����*/
//	//int flag;//��Χ����Ψһ�Լ����źţ����ƺ����ķ���ֵ������������źŽ���ý�顿��������Ʊ������жϡ�
//	bool repeat = false;
//	for (p = 1; p <= M->tu; p++)
//	{
//		if (repeat) {//������ؼ�鲻ͨ������Ҫ������������
//			for (int i = 1; i < p; i++)//��ӡ�ѽ��ܵ�����
//			{
//				printf("\t\t=====>%d %d %d\n", M->data[i].i, M->data[i].j, M->data[i].e);
//			}
//			repeat = false;//��־ִ����Ҫ��ʱ�ָ����Ա��´��ж�
//		}
//		printf("\t\t=====>");
//		scanf("%d%d%d", &(M->data[p].i), &(M->data[p].j), &(M->data[p].e));
//		/*�Ϸ��Լ�飬ʹ�û���ȷ����*/
//		/*1.��Χ��顾�Ӳ����ܸ����ݡ�*/
//		if (M->data[p].i<1 || M->data[p].i >M->mu || M->data[p].j<1 || M->data[p].j>M->nu)
//		{//��δͨ���������²�������
//			printf("***>�Ƿ����룡Ԫ�����к����ڡ�1-%d����1-%d���ڣ������������Ԫ�أ�\n", M->mu, M->nu);
//			p--;//���ˣ��´���д��Triple
//			//flag = 0;//���δͨ������Ψһ�Լ����ź�
//			repeat = true;//��Ҫ����
//		}
//		else
//		{//��ͨ���������²�������
//			//flag = 1;//���ͨ������Ψһ�Լ����ź�
//			++UniquePos[M->data[p].i - 1][M->data[p].j - 1];//����Χ���ͨ������Ʊ��ע���������ͨ����Ҫ��Ʊ�����򳬳���Χ��λ�ûᵼ�¼�Ʊ��Խ��
//		}
//
//		/*2.Ψһ�Լ��*/
//		if (p >= 2) {//�ڶ���Ԫ���Ժ����ҪΨһ�Լ��,��ͨ����ǰ��ķ�Χ������ҪΨһ�Լ�顾����Ʊ������жϡ�
//			if (UniquePos[M->data[p].i - 1][M->data[p].j - 1] >= 2) {//��0��  ˵��δͨ����Χ��飬�������Ψһ�Լ�飬��������䡿
//																	 //��1��  ˵��ͨ����Χ��飬��ֻ����1�Σ�����Ψһ�ԣ���������䡿
//																	 //��2~n��˵��ͨ����Χ��飬�����볬��1�Σ�������Ψһ�ԣ�ִ����ʾ�ͻ������������
//				// ����  Ʊ�� >= 2 ʱ������δͨ�����������²�����
//				//printf("==>��λ���Ѵ��ڷ���Ԫ�أ��Ƿ���¸�λ��Ԫ�ء�y/!y����\n");
//				printf("***>��λ������Ԫ�أ�������Ԫ�ص���ȷλ�ã�\n");
//				p--;//���ˣ��´���д��Triple
//				repeat = true;//��Ҫ����
//			}
//		}
//	}
//	/*ȷ������*/
//	//��һ�û������˳���������ô�죿
//	// ����ʱ���о������
//	// �䵽����û�ˣ��Ǿ�ֻ�ܳ��������û����򣬻���ֱ������
//	//int select;//����ѡ����
//	//printf("===>�Ƿ���Ҫ�Դ���Ԫ��˳�����С��������ȡ������򣿡�1����Ҫ����2������Ҫ�����䣻3������Ҫ��ȷ������\n");
//	//scanf("%d", &select);
//	//if (select == 'y' || select == 'Y') {
//	//	TSMatrixSort(M);//��Ԫ���ľ��������㷨
//	//}
//	//for (int i = 0; i < M->mu; i++) {//��Ʊ������ֵ
//	//	for (int j = 0; j < M->nu; j++)
//	//		printf("%d ", UniquePos[i][j]);
//	//	printf("\n");//ÿ�л�һ��
//	//}
//	/*���ټ�Ʊ��*/
//	for (int i = 0; i < M->mu; i++)
//		free(UniquePos[i]);//������+1��ָ����Ҫ�ͷţ���ô����Ķѿռ䣬����ô�ͷ�
//	free(UniquePos);//���ͷŵͼ�ָ�룬����ͷŸ߼�ָ��
//
//	printf("�������ɹ�����\n");
//	return;
//}
