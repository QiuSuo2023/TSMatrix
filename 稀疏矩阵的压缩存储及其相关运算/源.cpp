#define _CRT_SECURE_NO_WARNINGS 1
#define DATA M->tu
//#define MAXSIZE 125//当MAXSIZE过大时，函数栈会满
#define MAXCOUNT 52
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>	

/*------数据结构：稀疏矩阵的“三元组顺序表”存储表示------*/

/*---命名列表---*/
//SMatrix:稀疏矩阵
//Triple:三元组
//TSqList:三元组顺序表
//TSMatrix:用“三元组顺序表”表示的稀疏矩阵

typedef int ElemType;//矩阵元素
typedef struct
{
	int i, j;//非0元素位置
	ElemType e;//非0元素值
}Triple,*TSqList;
//Triple:元素结构体，用于存储矩阵单个非0元素（三元组）
//TSqList:结构体指针，用于【管理】矩阵所有非0元素，（三元组顺序表，即数组，使用指针变量和malloc函数在堆上动态分配内存，注意配套的“分配失败检查”和“free”）
//指针对关联起来的数据起到一个【管理作用】
typedef struct
{
	TSqList data;//三元组顺序表，所有三元组，数组下标从1开始，0号待用，三元组顺序表已经可以将一个稀疏矩阵的元素存储完，是矩阵的主数据
	int mu, nu, tu;//矩阵信息，为便于操作，特此另设三个整型变量，用于存储矩阵的特征：行数，列数，非零元个数，可以用data[0],但多三个int,影响不大
	int* rpos;//位置表
}TSMatrix, * TSMs;
//TSMatrix：带行列信息的稀疏矩阵
//TSMs：用于存储多个矩阵，以便于“本矩阵计算器”的计算
//行逻辑连接信息

//typedef struct
//{
//	Triple data[MAXSIZE+1];//当MAXSIZE过大时，栈会满	
//	int mu, nu, tu;//
//}TSMatrix, * TSMs;//TSMatrix：带行列信息的稀疏矩阵
////TSMs：用于存储多个矩阵以便于“本矩阵计算器”的计算

/*------函数列表------*/
/*基本操作函数*/
void Create(TSMatrix* M);//输入并创建矩阵
void GetRpos(TSMatrix* M);//计算输入矩阵的行逻辑信息（行首元素位置表）
void MatrixPrint(TSMatrix M);//矩阵形式打印
void SqListPrint(TSMatrix M);//三元组形式打印
int Add(TSMatrix M, TSMatrix N, TSMatrix* Q);//矩阵加法
int maxNum(int a, int b);//返回较大数，用于加法结果的空间分配
int minPos(int a, int b);//返回较小数的位置，前一个还是后一个，用于加减法的逻辑判断
int Subt(TSMatrix M, TSMatrix N, TSMatrix* Q);//矩阵减法
int Mult(TSMatrix M,TSMatrix N,TSMatrix* Q);//矩阵乘法
void FastTranspose(TSMatrix M, TSMatrix* T);//矩阵的快速转置
void printmenu();//打印菜单
void printtip(TSMatrix* M);//打印输入要求
/*功能组装函数*/
void Menu();//菜单选择功能
TSMatrix addMatrix(TSMatrix M);//程序加法功能
TSMatrix subMatrix(TSMatrix M);//程序减法功能
TSMatrix multMatrix(TSMatrix M);//程序乘法功能
TSMatrix transMatrix(TSMatrix M);//程序转置功能
TSMatrix printMatrix(TSMatrix M);//程序矩阵翻译功能
void close();//程序退出功能

//void TSMatrixSort(TSMatrix* M);
//void Modify(TSMatrix M);
//累加保存功能,计算器，内存中有多个矩阵

//void test2() {
//	TSMatrix M, N, Q;//局部变量，用于存储用户输入的三元组顺序表,在Create函数中初始化
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
//	TSMatrix M, N, Q;//局部变量，用于存储用户输入的三元组顺序表,在Create函数中初始化
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
/*------主函数------*/
//全局变量
char m[MAXCOUNT];//矩阵代号
int x = 0;//记录矩阵个数，每输入一个矩阵，或者得到一个新矩阵，矩阵代号由A递增到Z，再由a递增到y
int cnt = 0;//保存计算次数
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
/*------子函数------*/
/*基本操作函数*/
/*---创建稀疏矩阵三元组顺序表1---*/
void Create(TSMatrix* M) {
	/*输入矩阵信息*/
	printf("\t\t请依次输入稀疏矩阵的行、列、非零元数：");
	scanf("%d%d%d", &M->mu, &M->nu, &M->tu);
	/*合法性检查*/
	while (M->tu > M->mu * M->nu) {//M->tu > M->mu * M->nu必然造成输入的重复，使得输入无法正常结束
			printf("***>非法输入！元素个数须小于等于【%d*%d=%d】！请重新输入正确的元素个数：\n", M->mu, M->nu, M->mu * M->nu);
			printf("\t\t矩阵非零元数：");
			scanf("%d", &M->tu);
	}
	/*输入矩阵元素*/
	if (M->tu == 0) {//如果为零矩阵，则不需输入元素
		M->data = NULL;//初始化，并赋值为空，可以待用，其实也可以不加这个if，分配空间会分配一个Triple的首地址
		printf("“您创建了一个零矩阵。”\n");
		return;//输出和转置中，p也是从1开始，不会发生数组越界，但是可以省略各函数内前面的准备工作，为此，只需在各函数中加个零矩阵判读直接结束的if
	}
	int p;//顺序表的指针
	M->data = (TSqList)malloc(sizeof(Triple) * (M->tu + 1));//下标从1开始，0号待用
	if (!M->data) {
		printf("***>内存不足，申请空间失败！\n ");
		return;//返回主菜单
	}
	printtip(M);
	printf("行号 列号 值：\n");
	/*创建计票器*/
	int** UniquePos;//唯一性检查，确保用户输入的位置信息唯一，同一个位置只能有一个元素，位置下标从0开始，不多分配空间
	UniquePos = (int**)malloc(sizeof(int*) * M->mu);
	if (!UniquePos) {
		printf("***>内存不足，申请空间失败！\n ");
		exit(1);//后续操作待定
		return;//返回主菜单
	}
	for (int i = 0; i < M->mu; i++) {
		UniquePos[i] = (int*)malloc(sizeof(int) * M->nu);
		if (!UniquePos[i]) {
			printf("***>内存不足，申请空间失败！\n ");
			exit(1);//后续操作待定
			return;//返回主菜单
		}
	}
	/*
	* 关于二维数组指针的理解：
	“指针数组的首地址”初始化，存放“每一行”首元素的地址，数组内各元素偏移量为二维数组一行元素的地址步长
	指针数组的首地址，用行指针存放，就一个，管理每行列指针，分配“行数*sizeof(int*)”的内存长度
	对每行的指针变量分配内存，列指针，行数个，分别管理所在行的每个元素，每个分配“列数*sizeof(int)”的内存长度
	二级指针动态分配释放内存！【重点：给二维数组动态分配内存！一维的高阶形式，层次化】
	先分配一块空间，大小为待管理元素字节数*管理个数，再把这块内存的首地址(16进制序列）
	给一个指向这种元素的指针，这个指针【通过首地址和固定偏移量来管理所有元素】
	那么，管理了这个指针，就相当于管理了他管理的所有元素，这就是【数组】的原理
	【理解】计算机内存是一维的，但可以通过设置【层次】来使内存【多维化】
	*/
	for (int i = 0; i < M->mu; i++) {//计票器初始化，下标从0开始
		for (int j = 0; j < M->nu; j++)
			UniquePos[i][j] = 0;
	}
	/*开始输入*/
	//int flag;//范围检查给唯一性检查的信号，类似函数的返回值【各函数间的信号交流媒介】【但仅看票数便可判断】
	bool repeat = false;
	for (p = 1; p <= M->tu; p++)
	{
		if (repeat) {//如果两重检查不通过，需要重输上条数据
			for (int i = 1; i < p; i++)//打印已接受的数据
			{
				printf("%d %d %d\n", M->data[i].i, M->data[i].j, M->data[i].e);
			}
			repeat = false;//标志执行完要及时恢复，以便下次判定
		}
		scanf("%d%d%d", &(M->data[p].i), &(M->data[p].j), &(M->data[p].e));
		/*合法性检查，使用户正确输入*/
		/*1.范围检查【接不接受该数据】*/
		if (M->data[p].i<1 || M->data[p].i >M->mu || M->data[p].j<1 || M->data[p].j>M->nu)
		{//【未通过进行以下操作：】
			printf("***>非法输入！元素行列号须在【1-%d】【1-%d】内！请重新输入该元素：\n",M->mu,M->nu);
			p--;//回退，下次重写此Triple
			//flag = 0;//标记未通过，给唯一性检查的信号
			repeat = true;//需要重输
		}
		else
		{//【通过进行以下操作：】
			//flag = 1;//标记通过，给唯一性检查的信号
			++UniquePos[M->data[p].i - 1][M->data[p].j - 1];//【范围检查通过：计票】注意这里必须通过才要计票，否则超出范围的位置会导致计票器越界
		}
		
		/*2.唯一性检查*/
		if (p >= 2) {//第二个元素以后才需要唯一性检查,且通过了前面的范围检查才需要唯一性检查【仅看票数便可判断】
			if (UniquePos[M->data[p].i - 1][M->data[p].j - 1] >= 2) {//【0：  说明未通过范围检查，无需进行唯一性检查，跳过本语句】
																	 //【1：  说明通过范围检查，且只出现1次，符合唯一性，跳过本语句】
																	 //【2~n：说明通过范围检查，但输入超过1次，不符合唯一性，执行提示和回退重输操作】
				// 票数 >= 2 时【未通过，进行以下操作】
				//printf("==>该位置已存在非零元素，是否更新该位置元素【y/!y】：\n");
				printf("***>该位置已有元素；请输入元素的正确位置：\n");
				p--;//回退，下次重写此Triple
				repeat = true;//需要重输
			}
		}
	}
	/*确认输入*/
	//万一用户最后发现顺序出错了怎么办？
	// 输入时就有纠错机会
	// 输到最后就没了，那就只能程序来帮用户排序，或者直接重输
	//int select;//创建选择器
	//printf("===>是否需要对此三元组顺序表进行“行序优先”的排序？【1：需要排序；2：不需要，重输；3：不需要，确认有序】\n");
	//scanf("%d", &select);
	//if (select == 'y' || select == 'Y') {
	//	TSMatrixSort(M);//三元组表的矩阵排序算法
	//}
	/*销毁计票器*/
	for (int i = 0; i < M->mu; i++)
		free(UniquePos[i]);//有行数+1个指针需要释放，怎么分配的堆空间，就怎么释放
	free(UniquePos);//先释放低级指针，最后释放高级指针
	/*计算rpos*/
	GetRpos(M);
	printf("“创建成功！”\n");
	return ;
}
/*
删除了printf("\t\t=====>");方便整体的数据输入
*/
/*---计算行链接信息----*/
void GetRpos(TSMatrix* M) {
	/*制作M列元素位置信息表：“列逻辑链接”（“索引表？哈希表？”）*/
	//声明数组：计票器和位置信息表
	int row, p;
	int* num = (int*)malloc((M->mu + 1) * sizeof(int));//0号位不用，空间为M行数多一个int型
	if (!num) {
		printf("\n “内存不足，申请空间失败！”");
		exit(0);
		return;
	}
	M->rpos = (int*)malloc((M->mu + 1) * sizeof(int));//记录M每一行的个数、每一行第一个元素的位置
	if (!M->rpos) {
		printf("\n “内存不足，申请空间失败！” ");
		exit(0);
		return;
	}
	//计票器初始化
	for (row = 1; row <= M->mu; ++row)
		num[row] = 0;
	//统计M矩阵每列元素个数，即 j 的出现次数 ，每个 j 出现一次在相应位置记一票，“计票法”
	for (p = 1; p <= M->tu; ++p)//p从1到tu!
		++num[M->data[p].i];
	//计算M矩阵每列第一个非零元，转置后，在T顺序表中的位置;递推法;使T的生成更便捷
	M->rpos[1] = 1;
	for (row = 2; row <= M->mu; ++row)
		M->rpos[row] = M->rpos[row - 1] + num[row - 1];
	free(num);//释放局部变量
	return;
}
/*---矩阵快速转置---*/
void FastTranspose(TSMatrix M,TSMatrix* T){
	/*特殊情况：零矩阵*/
	if (M.tu == 0)//零矩阵的转置只需交换行列数，其他变量也要赋值即初始化
	{//简化操作，优化
		T->data = NULL;
		T->mu = M.nu;
		T->nu = M.mu;
		T->tu = 0;
		return;
	}
	/*局部变量列表*/
	int p, q,col;//M和T的元素操作指针，M的列指针，p指M，q指T，col指M的列
	/*制作M列元素位置信息表：“列逻辑链接”（“索引表？哈希表？”）*/
	/*确定M中每个元素在T中的位置，放的时候交换ij即可*/
		//声明数组：计票器和位置信息表
	int* num = (int*)malloc((M.nu + 1) * sizeof(int));//0号位不用，空间为M列数多一个int型
	if (!num) {//堆分配配套检查
		printf("\n “内存不足，申请空间失败！”");
		exit(0);//后续操作待定
		return ;//返回主菜单
	}
	int* cpot = (int*)malloc((M.nu + 1) * sizeof(int));//记录M每一列的个数、每一列第一个元素的位置
	if (!cpot) {
		printf("\n “内存不足，申请空间失败！” ");
		exit(0);//后续操作待定
		return;//返回主菜单
	}
		//计票器初始化
	for (col = 1; col <= M.nu; ++col)
		num[col] = 0;
		//统计M矩阵每列元素个数，即 j 的出现次数 ，每个 j 出现一次在相应位置记一票，“计票法”
	for (p = 1; p <= M.tu; ++p)
		++num[M.data[p].j];
		//计算M矩阵每列第一个非零元，转置后，在T顺序表中的位置;递推法;使T的生成更便捷
	cpot[1] = 1;
	for (col = 2; col <= M.nu; ++col)
		cpot[col] = cpot[col - 1] + num[col - 1];
	/*向结果T中输入数据（局部变量初始化）*/
		/*写入矩阵信息*/
	T->mu = M.nu; T->nu = M.mu; T->tu = M.tu;
		/*写入矩阵元素（顺序表）*/
			/*给顺序表的指针变量赋值（使用前必须给每个指针变量赋一个确定的值！）*/
	T->data= (TSqList)malloc(sizeof(Triple) * (M.tu + 1));
	if (!T->data) {
		printf("\n 申请空间失败！ ");
		return;//返回主菜单
	}
			/*把M顺序表的元素依次放入T顺序表的对应位置，cpot位置信息表，“跳跃式存储！”*/
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
我的算法理解：
先把M的每列第一个放好，对应的是在T的每行第一个非0元，
M第一个扫描的不一定是M第一列的，所以该元素在T顺序表中就不一定是T.data的第一个Triple，而是T.data的第cpot[j]个Triple，
体现在T中的存储，是一种“跳跃式的存储”（索引存储？桶排序？），
当M中每列第一个非0元放在T中每行的第一个非0元位置后，在M中再次扫描到同一列的元素后，紧随上次之后的位置，即【++cpot[col];】，
因为M的行也是有序存储的，转到T中，T每行也就是该有的列序，所以该算法的前提有：M顺序表自身需要按行列序存储
*/
/*
日志：
-Add1-排序:需要确保用户输入的三元组顺序表本身有序，即输入函数需设置“合法性检查”
无序则排序->快速排序法->无序输入，有序输入->冒泡或插入排序法->两种功能，在用户输入完成后，让用户确认数据是否需要排序
无序则执行排序——输入顺序交给用户，输入时就是看行号列号输入的，最后确认一下，需不需要排序
ok-Add2-查重:还要确保用户不重复输入同一个位置，查重，每个位置记一次票，“计票法”
否则重新输入该Triple，重写该位置，p--
如果允许输入重复位置，那么第二次及这之后输入的元素都不会被打印且顺序表的指针永不移动
ok-Add3-限制:当用户输入Triple时，会出现行列号大于用户给的实际行列数，需要对用户的输入进行限制
*/
/*---矩阵加法---*/
int Add(TSMatrix M, TSMatrix N, TSMatrix* Q)
{
	/*合法性检查*/
	if (M.mu != N.mu || M.nu != N.nu) {
		printf("“相加失败！相加的矩阵需要同型！”\n");
		return 0;
	}
	if (M.tu == 0) {
		*Q = N;
		return 1;
		}
	if (N.tu == 0) {
		*Q = M;
		return 1;
	}//零矩阵
	/*定义局部变量*/
	int m, n, q;//操作指针
	int count = 0;//计算Q的非零元个数
	int len = 0;//判断是否需要增加分配，长度需要动态更新
	Triple* newbase = NULL;//动态增加顺序表长度
	Q->mu = M.mu;
	Q->nu = M.nu;
	Q->data = (Triple*)malloc(sizeof(Triple) * (maxNum(M.tu, N.tu)+1));//0号元素要考虑，至少两份空间
	len = maxNum(M.tu, N.tu) + 1;//0号元素要考虑，实际使用长度要减去1！
	if (!Q->data) {
		printf("***>内存不足，申请空间失败！\n ");
		exit(1);//后续操作待定
	}
	q = 1, m = 1, n = 1;//分别指向M,N,Q的1号位置
	/*以每个位置为研究对象进行操作*/
	while (m <= M.tu && n <= N.tu) {//类似归并有序表
		/*1.动态增加分配*/
		if (q > len - 1) {//q在Q中后移一位后，发现空间不够，【len-1】才是q的实际范围
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>内存不足，申请空间失败！\n ");
				exit(1);//后续操作待定
			}
			Q->data = newbase;
		}
		/*2.矩阵三元组顺序表加法逻辑*/
		if (M.data[m].i == N.data[n].i) {//i同
			if (M.data[m].j == N.data[n].j) {//i、j都相同，相加后存储
				if (M.data[m].e + N.data[n].e != 0) {//零元素不存储!
					Q->data[q].e = M.data[m].e + N.data[n].e;//e
					Q->data[q].i = M.data[m].i;//i
					Q->data[q].j = M.data[m].j;//j
					m++, n++, count++;//扫描过，后移
				}//if3
				else {
					m++, n++;//M,N还是要后移
					q--;//Q指针回退,抵消后移！
				}//else3
			}//if2
			else {//i同，j不同，取j小的
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
		else {//i不同
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
		/*3.Q每多一个,指针后移一位*/
		q++;
	}
	/*类似归并有序表，把剩余未相加的元素放进Q中，上面该有的都要有*/
	while (m <= M.tu) {
		/*1.动态增加分配*/
		if (q > len - 1) {//q在Q中后移一位后，发现空间不够，【len-1】才是q的实际范围
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>内存不足，申请空间失败！\n ");
				exit(1);//后续操作待定
			}
			Q->data = newbase;
		}
		/*2.矩阵三元组顺序表加法逻辑*/
		Q->data[q].i = M.data[m].i;//i
		Q->data[q].j = M.data[m].j;//j
		Q->data[q].e = M.data[m].e;//e
		m++, count++;
		/*3.Q每多一个,指针后移一位*/
		q++;
	}
	while (n <= N.tu) {
		/*1.动态增加分配*/
		if (q > len - 1) {//q在Q中后移一位后，发现空间不够，【len-1】才是q的实际范围
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>内存不足，申请空间失败！\n ");
				exit(1);//后续操作待定
			}
			Q->data = newbase;
		}
		/*2.矩阵三元组顺序表加法逻辑*/
		Q->data[q].i = N.data[n].i;//i
		Q->data[q].j = N.data[n].j;//j
		Q->data[q].e = N.data[n].e;//e
		n++, count++;
		/*3.Q每多一个,指针后移一位*/
		q++;
	}
	/*保存新矩阵的个数*/
	Q->tu = count;
	printf("“相加成功！”\n");
	return 1;
}
//返回较大数，用于加减法结果的空间分配
int maxNum(int a,int b) {
	return a > b ? a : b;
}
//返回较小数的位置，前一个还是后一个，用于加减法的逻辑判断
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
/*---矩阵减法---*/
int Subt(TSMatrix M, TSMatrix N, TSMatrix* Q)
{
	/*合法性检查*/
	if (M.mu != N.mu || M.nu != N.nu) {
		printf("“相减失败！相加的矩阵需要同型！”\n");
		return 0;
	}
	if (N.tu == 0) {//减取零矩阵不变，但是被零矩阵减得算
		*Q = M;
		return 1;
	}//零矩阵
	/*定义局部变量*/
	int m, n, q;//操作指针
	int count = 0;//计算Q的非零元个数
	int len = 0;//判断是否需要增加分配，长度需要动态更新
	Triple* newbase = NULL;//动态增加顺序表长度
	Q->mu = M.mu;
	Q->nu = M.nu;
	Q->data = (Triple*)malloc(sizeof(Triple) * (maxNum(M.tu, N.tu) + 1));//0号元素要考虑，至少两份空间
	len = maxNum(M.tu, N.tu) + 1;//0号元素要考虑，实际使用长度要减去1！
	if (!Q->data) {
		printf("***>内存不足，申请空间失败！\n ");
		exit(1);//后续操作待定
	}
	q = 1, m = 1, n = 1;//分别指向M,N,Q的1号位置
	/*以每个位置为研究对象进行操作*/
	while (m <= M.tu && n <= N.tu) {//类似归并有序表
		/*1.动态增加分配*/
		if (q > len - 1) {//q在Q中后移一位后，发现空间不够，【len-1】才是q的实际范围
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>内存不足，申请空间失败！\n ");
				exit(1);//后续操作待定
			}
			Q->data = newbase;
		}
		/*2.矩阵三元组顺序表减法逻辑*/
		if (M.data[m].i == N.data[n].i) {//i同
			if (M.data[m].j == N.data[n].j) {//i、j都相同，相加后存储
				if (M.data[m].e - N.data[n].e != 0) {//零元素不存储!
					Q->data[q].e = M.data[m].e - N.data[n].e;//e
					Q->data[q].i = M.data[m].i;//i
					Q->data[q].j = M.data[m].j;//j
					m++, n++, count++;//扫描过，后移
				}//if3
				else {
					m++, n++;//M,N还是要后移
					q--;//Q指针回退,抵消后移！
				}//else3
			}//if2
			else {//i同，j不同，取j小的
				Q->data[q].i = M.data[m].i;//i
				if (minPos(M.data[m].j, N.data[n].j) == 1) {
					Q->data[q].j = M.data[m].j;//j
					Q->data[q].e = M.data[m].e;//e，如果M的该位置更前，说明N的对应位置为零,结果位置为本身
					m++, count++;
				}//if3
				else if (minPos(M.data[m].j, N.data[n].j) == 2) {
					Q->data[q].j = N.data[n].j;//j
					Q->data[q].e = N.data[n].e * -1;//e，如果N的该位置更前，说明M的对应位置为零,结果位置为相反数
					n++, count++;
				}//else if3
			}//else2
		}//if1
		else {//i不同
			if (minPos(M.data[m].i, N.data[n].i) == 1) {
				Q->data[q].i = M.data[m].i;//i
				Q->data[q].j = M.data[m].j;//j
				Q->data[q].e = M.data[m].e;//e，如果M的该位置更前，说明N的对应位置为零,结果位置为本身
				m++, count++;
			}//if2
			else if (minPos(M.data[m].i, N.data[n].i) == 2) {
				Q->data[q].i = N.data[n].i;//i
				Q->data[q].j = N.data[n].j;//j
				Q->data[q].e = N.data[n].e * -1;//e，如果N的该位置更前，说明M的对应位置为零,结果位置为相反数
				n++, count++;
			}//else if2
		}//else1
		/*3.Q每多一个,指针后移一位*/
		q++;
	}
	/*类似归并有序表，把剩余未相加的元素放进Q中，上面该有的都要有*/
	while (m <= M.tu) {
		/*1.动态增加分配*/
		if (q > len - 1) {//q在Q中后移一位后，发现空间不够，【len-1】才是q的实际范围
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>内存不足，申请空间失败！\n ");
				exit(1);//后续操作待定
			}
			Q->data = newbase;
		}
		/*2.M直接放入结果*/
		Q->data[q].i = M.data[m].i;//i
		Q->data[q].j = M.data[m].j;//j
		Q->data[q].e = M.data[m].e;//e
		m++, count++;
		/*3.Q每多一个,指针后移一位*/
		q++;
	}
	while (n <= N.tu) {
		/*1.动态增加分配*/
		if (q > len - 1) {//q在Q中后移一位后，发现空间不够，【len-1】才是q的实际范围
			newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
			len += len / 2;
			if (!newbase) {
				printf("***>内存不足，申请空间失败！\n ");
				exit(1);//后续操作待定
			}
			Q->data = newbase;
		}
		/*2.N还有元素则要*-1再放入*/
		Q->data[q].i = N.data[n].i;//i
		Q->data[q].j = N.data[n].j;//j
		Q->data[q].e = N.data[n].e * -1;//e
		n++, count++;
		/*3.Q每多一个,指针后移一位*/
		q++;
	}
	/*保存新矩阵的个数*/
	Q->tu = count;
	printf("“相减成功！”\n");
	return 1;
}
/*---矩阵乘法----*/
int Mult(TSMatrix M, TSMatrix N, TSMatrix* Q)
{
	/*合法性检查*/
	if (M.nu != N.mu) {//左列等右行
		printf("“相乘失败！N的行数需等于M的列数！”\n");
		return 0;
	}
	if (M.tu == 0 || N.tu == 0) {//M为零矩阵或N为零矩阵，即没有非零元
		Q->tu = 0;
		Q->mu = M.mu;//行取左
		Q->nu = N.nu;//列取右
		Q->data = NULL;
		Q->rpos = NULL;
		return 1;
	}
	/*定义局部变量*/
	//int m, n, q;//操作指针
	//int count = 0;//统计Q非零元个数，Q->tu
	int len = 0;//判断是否需要增加分配，长度需要动态更新
	Triple* newbase = NULL;//动态增加顺序表长度的中间变量
	Q->mu = M.mu;//行取左
	Q->nu = N.nu;//列取右
	Q->tu = 0;//边写边算
	Q->rpos = (int*)malloc(sizeof(int) * (M.mu + 1));//Q的初始化
	len = M.tu + N.tu;//0号元素要考虑，实际使用长度要减去 1！
	len += len / 2 + 1;
	Q->data = (Triple*)malloc(sizeof(Triple) * len);//0号元素要考虑，就算一个非零元素也要至少两份空间
	int* ctemp = (int*)malloc(sizeof(Triple) * (N.tu + 1));//累加后要压缩去零，临时记得free();
	if (!Q->data) {
		printf("***>内存不足，申请空间失败！\n ");
		exit(1);//后续操作待定
	}
	/*行链接乘法逻辑*/
	//行累加器
	int ccol = 0;
	//当前处理的M的行标 
	int arow;
	//按行处理,arow即是前乘矩阵M的行号也是结果矩阵Q的行号 
	for (arow = 1; arow <= M.mu; ++arow) 
	{//3.算完所有行的全部结果
		//行元素累加器清0 
		for (int i = 0; i <= N.nu; i++)
			ctemp[i] = 0;
		//每计算一行设置当前行的第一个非0元素位置为之前行所有非0个数+1 ：计算Q的rpos
		Q->rpos[arow] =	Q->tu + 1;//表示Q在写入此行数据前，此行无数据
		//当前行的下一行第一个非0元素在M.data中的位置，如果是最后一行取所有的非0元个数之和+1 ：rpos[row]>tu,表示这一行没有元素
		int tp = 0;//位置
		if (arow < M.mu)
			tp = M.rpos[arow + 1];
		else
			tp = M.tu + 1;//定好当前行在顺序表的范围：M.rpos[arow]~tp-1，第一行的话M.rpos[arow]==1
		int p = 0;//M指针
		int q = 0;//N指针
		int t = 0;//位置
		//对当前行的每一个非0元素处理，计算其对应的位置乘积和 
		int brow = 0;
		for (p = M.rpos[arow]; p < tp; ++p)//M的该行顺序表位置：M.rpos[arow]~tp-1
		{//2.算完一行的全部结果
			//找到该元素对应的N的行号 
			brow = M.data[p].j;
			//取找到的N对应行下一行第一个元素的位置，如果是最后一行取所有的非0元素之和+1  
			if (brow < N.mu)
				t = N.rpos[brow + 1];
			else
				t = N.tu + 1;//定好目标行在N顺序表的范围：N.rpos[brow]~t-1,
			//N对应行位置上的元素与M当前位置元素相乘，累加进入ctemp行累加器对应位置 
			for (q = N.rpos[brow]; q < t; ++q)//N的对应行顺序表位置：N.rpos[brow]~t-1，M.data[p].e乘以每个q所指N的元素，分别得到结果矩阵各列的部分数据
			{//1.算完一行的部分结果,用ctemp保存各列乘积，结果是一个序列，是列数个位置上的数据，最大只需要N.nu个空间
				//取N元素的列号，对应结果矩阵的列
				ccol = N.data[q].j;
				ctemp[ccol] += M.data[p].e * N.data[q].e;//分别对应保存好所乘N的每一列的结果，这是M一行里一个元素的结果
				//结果矩阵中需要每列对应累加保存M矩阵当前行的每一个乘积序列，M.data[p].e乘出来的不是要累加的，是各列的，所谓“整行处理”
				//而是要分别对应保存结果到当前行的每一列上的，当前行的每一个非零元的乘积序列都被累加，
				//则此时结果矩阵的一行被计算完成
				//巧妙之处，在于知道每行元素在顺序表中的具体范围，而且每次都是计算一行的部分结果，“整行处理”
			}//for1	
		}//for2

		//累加器上对应位置压缩存储 
		for (ccol = 1; ccol <= Q->nu; ++ccol)//一个数组，累加好了当前行的所有结果，M从第一行开始，Q得到的也是第一行开始
		{//刚好是按行列序排列，每行N.nu个
			if (ctemp[ccol]) {//依次处理当前行的每一列元素，如果结果不为零，则压缩存储
				//Q->tu进一步再存结果
				++Q->tu;
				if (Q->tu > len - 1) //空间不足
				 {//q在Q中后移一位后，发现空间不够，【len-1】才是q的实际范围
					newbase = (Triple*)realloc(Q->data, sizeof(Triple) * (len + len / 2));
					len += len / 2;
					if (!newbase) {
						printf("***>内存不足，申请空间失败！\n ");
						exit(1);//后续操作待定
					}
					Q->data = newbase;
					free(newbase);
				}
				Q->data[Q->tu].i = arow;//当前正在处理的行
				Q->data[Q->tu].j = ccol;//当前正在处理的行的每一列
				Q->data[Q->tu].e = ctemp[ccol];//当前正在处理的行的每一列所对应相乘累加的结果
			}//if不为0
		}//for2.2压缩存储
	}//for3
	//Q->rpos[arow] = Q->tu + 1;//Q的行号等于M的列号
	free(ctemp);
	printf("“相乘成功！”\n");
	return 1;
}
/*---顺序表输出---*/
void SqListPrint(TSMatrix M) {
	printf("%d %d %d\n", M.mu, M.nu, M.tu);
	for (int j = 1; j <= M.nu * 2 + 1; j++) {
		printf("-");
		if (j == M.nu * 2 + 1)printf("\n");
	}
	for (int i = 1; i <= M.tu; i++)
	{
		printf("%d %d %d", M.data[i].i, M.data[i].j, M.data[i].e);
		printf("\n");//选择性换行
	}
}
/*---矩阵形式输出---*/
void MatrixPrint(TSMatrix M) {//目的：找到三元组非零元素的对应位置，匹配问题
	int p = 1;//指示三元组顺序表的指针
	for (int i = 1; i <= M.mu; i++){
		printf("\t\t=====>");
		for (int j = 1; j <= M.nu; j++){//i，j是指示输出矩阵的指针
			if (M.tu != 0) {//非零矩阵，零矩阵的data为NULL!
				if (M.data[p].i == i && M.data[p].j == j && p <= M.tu) {//tu个输完了后面全输出0就行
					printf("%d ", M.data[p].e);
					p++;//!!!匹配成功则顺序表的指针后移，未匹配成功，则继续找；前提是p所指元素的位置在ij指针处及以后的位置
				}
				else printf("%d ", 0);
			}//零矩阵直接全输0
			else printf("%d ", 0);
		}
		printf("\n");//最后一行不用换行
	}
}
/*
感悟日志：
该算法相比网上“先初始化一个二维数组，再更改对应位置的非0元素,S=O(mu*nu)”相比，
使用的空间复杂度为O(1),时间复杂度都为T=O(n^2)
  
该算法的前提是p所指元素的位置在ij指针处及以后的位置
如果因为自己的位置被上一个占了，即自己跟上一个撞位置了，导致ij指针永远找不到自己的位置，那么自己后面
的元素将不会被输出，因为p指针要自己被输出了才会后移
【所以该算法不能允许重复位置的出现】，同一位置的第二个元素不仅没有输出机会，还会导致后面所有元素都无法输出！
如果允许【顶替】，改进方法可为：再加一层循环，重置i,j，以p > M.tu为退出条件，相同位置会被后需元素顶替
且p指针使所有顺序表输出，但是这样会多打印元素，并不是所谓【覆盖或顶替】，会打乱矩阵的结构
所以打印输出是不允许同一个位置输出多个元素，输出到屏幕，一个位置就是一个，不可能替换屏幕上的结果，只能
从头输出该矩阵
矩阵形式输出是一回事，矩阵形式存储【二维数组】是另一回事，存储可以【覆盖或顶替或更新】，同一个位置只存储最后一个，但这里
考虑的是【三元组顺序表】，所以，就不能允许重复位置的出现！
【该算法也不能允许超出实际给出矩阵范围的元素出现】
*/

/*菜单选择功能-*/
void Menu()
{
	printmenu();//菜单界面打印
	int select;//功能选择
	int continu = 0;//是否继续
	TSMatrix M, T;
	while (1)
	{
		if (cnt >= 1) {//使用过一次以上
			printf("是否继续进行迭代计算【1】继续 【其他】重新输入：");
			scanf("%d", &continu);
			if (continu == 1)//如果需要迭代，上一步结果T赋值给初始矩阵M
				M = T;//后面就不需要在输入M了【这对M的赋值分支有两个，一个T,一个用户输入，这个后需可以改进！】
			else x++;//新输入的矩阵编号
		}
		//调整为先输入选择
		printf("请选择你想要进行的运算【0-5】：");
		scanf("%d", &select);
		//再输入矩阵
		if (select != 0) {//用户选择【0】退出时,不用输入矩阵，直接退出
			if (cnt == 0 || continu != 1) {//第一次使用或不是第一次使用且不需要迭代时，就需要输入初始矩阵M
				printf("\t\t请输入一个矩阵 %c ：\n", m[x]);
				Create(&M);
			}
		}
		/*功能选择*/
		switch (select)
		{
		case 1:T = addMatrix(M); break;
		case 2:T = subMatrix(M); break;
		case 3:T = multMatrix(M); break;
		case 4:T = transMatrix(M); break;
		case 5:T = printMatrix(M); break;
		case 0:close(); break;
		default:printf("“请在【0-5】之间选择你想要的功能。”\n");
		}
	}//while
}
/*加法功能*/
TSMatrix addMatrix(TSMatrix M) {
	TSMatrix N,Q;
	printf("\t\t请输入需要相加的矩阵 %c ：\n", m[x+1]);
	Create(&N);
	int flag = Add(M, N, &Q);
	SqListPrint(M);
	MatrixPrint(M);
	SqListPrint(N);
	MatrixPrint(N);
	if (flag==0) {
		return M;
	}
	x += 2;
	printf("\n%c + %c = %c\n", m[x - 2], m[x - 1], m[x]);
	SqListPrint(Q);
	MatrixPrint(Q);
	cnt++;
	return Q;
}
/*减法功能*/
TSMatrix subMatrix(TSMatrix M) {
	TSMatrix N, Q;
	printf("\t\t请输入需要相减的矩阵 %c ：\n", m[x+1]);
	Create(&N);
	int flag = Subt(M, N, &Q);
	SqListPrint(M);
	MatrixPrint(M);
	SqListPrint(N);
	MatrixPrint(N);
	if (flag == 0) {
		return M;
	}
	x += 2;
	printf("\n%c - %c = %c\n", m[x - 2], m[x - 1], m[x]);
	SqListPrint(Q);
	MatrixPrint(Q);
	cnt++;
	return Q;
}
/*乘法功能*/
TSMatrix multMatrix(TSMatrix M) {
	TSMatrix N, Q;
	printf("\t\t请输入需要相乘的矩阵 %c ：\n", m[x+1]);
	Create(&N);
	int flag=Mult(M, N, &Q);
	SqListPrint(M);
	MatrixPrint(M);
	SqListPrint(N);
	MatrixPrint(N);
	if (flag == 0) {
		return M;
	}
	x += 2;
	printf("\n%c * %c = %c\n", m[x - 2], m[x - 1], m[x]);
	SqListPrint(Q);
	MatrixPrint(Q);
	cnt++;
	return Q;
}
/*转置功能*/
TSMatrix transMatrix(TSMatrix M) {
	TSMatrix T;
	FastTranspose(M, &T);
	SqListPrint(M);
	MatrixPrint(M);
	x++;
	printf("\n%c 转置后= %c\n", m[x - 1], m[x]);
	SqListPrint(T);
	MatrixPrint(T);
	cnt++;
	return T;
}
/*翻译功能*/
TSMatrix printMatrix(TSMatrix M) {
	printf("你输入的矩阵 %c 为：\n", m[x]);
	SqListPrint(M);
	MatrixPrint(M);
	cnt++;
	return M;
}
/*退出功能*/
void close() {
	printf("\n\t\t\t\t   │────────感谢您的使用！──────│\n");
	exit(0);
}

/*打印人机界面的封装*/
/*---打印菜单---*/
void printmenu() {
	system("cls");
	//system("color 0A");
	printf("\t\t\t\t   ******************************\n");
	printf("\t\t\t\t   *       矩 阵 计 算 器       *\n");
	printf("\t\t\t\t   ******************************\n");
	printf("\t\t\t\t   ┌────────────────────────────┐\n");
	printf("\t\t\t\t   │ 1. 加          2. 减       │\n");
	printf("\t\t\t\t   │ 3. 乘          4. 转置     │\n");
	printf("\t\t\t\t   │ 5. 显示        0. 退出     │\n");
	printf("\t\t\t\t   └────────────────────────────┘\n");
	printf("\t\t\t\t   │────────欢迎您的使用！──────│\n");
}
/*---打印输入提示要求---*/
void printtip(TSMatrix* M) {
	printf("\t\t┌────────────────────────────────────────────────────────────────────────┐\n");
	printf("\t\t│  请按“先行后列”顺序输入矩阵的 %d 个非零元行列号及元素值 |行列号从1开始| │\n", DATA);//交互
	printf("\t\t└────────────────────────────────────────────────────────────────────────┘\n");
}

/*
bug日志：
int* num;//vs特点：必须先初始化，否则地址未知，但是devc++却允许
int* cpot;

bug日志：
Q：分配空间后会，转置会改变M.data[3]中的j !?，只能采用定长数组的方式分配空间？
A：莫名奇妙又好了？？？

功能日志：
printf("==>该位置已存在非零元素，是否更新该位置元素【y/!y】：\n");
scanf("%c", &select);
if (select == 'y' || select == 'Y')
pass_数据量大，中途输错了怎么办？【功能：查找和排序】涉及到顺序表的查找和指定元素的更新，暂时放下。
先插入排序（基本有序）再二分查找（在顺序表中的位置），先二分查找行下标，再二分查找列下标（大可不必：2024/1/5）
每次行列一起匹配，存储结构是顺序表！
大量数据先提前准备好，直接导入复制就行

修改日志：2023/12/24
ok_基于用户输入错误需要更改时，对自己正确输入的信息不好直观地呈现出来的问题的改进：
重输提示后换行打印已输入的正确数据，用户紧接着在后面输入，用户可直观地看到自己被顺序表接受的输入数据
修改函数：void Create(TSMatrix* M);

修改日志：2023/12/27
1、当tu大于mu*nu时，会因为唯一性检查而无法输入
修改函数：修改函数：void Create(TSMatrix* M);
2、顺序表打印加上mu\nu\tu
3、忽略了0号元素：if (q > len) 
4、for (q = 1, m = 1, n = 1; m <= M.tu && n <= N.tu; ) {
	//这里不需要m++,n++，放到分支结构中了，与while的区别	
	}//for
5、下标问题，没有对应！N.data[m]=>N.data[n]
6、加法的结果可能是零元素！选择性存储！

bug日志：2024/1/5
乘法输入1 1 1；1 1 1；时发生刚访问冲突
又正常了？？？
*/










