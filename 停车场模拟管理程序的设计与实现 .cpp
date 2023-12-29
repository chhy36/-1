
#include<iostream>
#include<string>
using namespace std;

#define MAX 5
#define price 0.05 
typedef struct time
{
	int hour, min;
}Time;
typedef struct node
{
	string num;
	Time reach, leave;
}CarNode;//结构体：定义每辆车的牌号，车辆状态定义为：进入时刻与离开时刻
typedef struct NODE
{
	CarNode* stack[MAX + 1];
	int top;
}SeqStackCar;//用栈定义，构造停车场
typedef struct car
{
	CarNode* data;
	struct car* next;
}QueueNode;//用队列结点定义，构造停车场外的单个等候车辆
typedef struct Node
{
	QueueNode* head, * rear;
}LinkQueueCar;//用队列定义，构造停车场外的等候便道

void InitStack(SeqStackCar*);
int InitQueue(LinkQueueCar*);
int Reach(SeqStackCar*, LinkQueueCar*);
void Leave(SeqStackCar*, SeqStackCar*, LinkQueueCar*);
void List(SeqStackCar, LinkQueueCar);
void PRINT(CarNode* p, int room);
int main()
{
	SeqStackCar Enter, Temp;//初始栈，中转栈
	LinkQueueCar Wait;//便道队列
	int a;
	InitStack(&Enter);
	InitStack(&Temp);
	InitQueue(&Wait);//初始化
	while (1)
	{
		cout << "            欢迎光临停车场!             " << endl;
		cout << "           1.车辆到达登记" << endl;
		cout << "           2.车辆离开登记" << endl;
		cout << "           3.车辆停靠查询" << endl;
		cout << "           4.退出系统" << endl;//系统设置
		while (1)
		{
			cin >> a;
			if (a >= 1 && a <= 4)break;
			else cout << endl << "请选择：1~4";
		}
		switch (a)
		{
		case 1:Reach(&Enter, &Wait); break;
		case 2:Leave(&Enter, &Temp, &Wait); break;
		case 3:List(Enter, Wait); break;
		case 4:exit(0);
		default:break;
		}

	}

}
void InitStack(SeqStackCar* s)//堆栈初始化
{
	s->top = 0;
	s->stack[s->top] = NULL;
}
int InitQueue(LinkQueueCar* Q)//队列初始化
{
	Q->head = new QueueNode;
	if (Q->head != NULL)
	{
		Q->head->next = NULL;
		Q->rear = Q->head;
		return 1;
	}
	else return -1;
}
void PRINT(CarNode* p, int room)//输出离开停车场的车辆情况
{
	int A, B, C, D;
	cout << "\n车辆离开的时间:" << endl;
	cout << "输入小时：";
	cin >> p->leave.hour;
	cout << "输入分钟:";
	cin >> p->leave.min;
	cout << "离开车辆的车牌号为：";
	cout << p->num;
	cout << endl << "其到达时间为：" << p->reach.hour << ":" << p->reach.min;
	cout << "离开时间为：" << p->leave.hour << ":" << p->leave.min;
	A = p->reach.hour;
	B = p->reach.min;
	C = p->leave.hour;
	D = p->leave.min;
	cout << endl << "应交费用为：" << ((C - A) * 60 + (D - B)) * price << "元" << endl;
	cout << "车辆离开登记完毕!" << endl;
	cout << "------------------------------------------------------" << endl;
	delete p;
}
int Reach(SeqStackCar* Enter, LinkQueueCar* W)
{
	CarNode* p;
	QueueNode* t;
	p = new CarNode;
	cout << "-------------------------------------------------------" << endl;
	cout << "车辆到达登记开始：" << endl;
	cout << endl << "请输入车牌号：";
	cin >> p->num;
	if (Enter->top < MAX)//如果车位未满则进入停车场
	{
		Enter->top++;
		cout << endl << "车辆在车场第" << Enter->top << "位置.";
		cout << endl << "车辆到达时间：" << endl;
		cout << "输入小时：";
		cin >> p->reach.hour;
		cout << "输入分钟:";
		cin >> p->reach.min;
		cout << endl << "车辆到达登记完毕!" << endl;
		cout << "---------------------------------------------------" << endl;
		Enter->stack[Enter->top] = p;
		return 1;
	}
	else//如果车位已满，则停在便道上
	{
		cout << "----------------------------------------------------" << endl;
		cout << endl << "该车须停靠在便道上，有车位时进入停车场" << endl;
		t = new QueueNode;
		t->data = p;
		t->next = NULL;
		W->rear->next = t;
		W->rear = t;
		return 1;
	}
}
void Leave(SeqStackCar* Enter, SeqStackCar* Temp, LinkQueueCar* W)
{
	int room;
	CarNode* p, * t;
	QueueNode* q;

	if (Enter->top > 0)
	{
		while (1)
		{
			cout << "----------------------------------------------" << endl;
			cout << "车辆离开登记开始：" << endl;
			cout << endl << "请输入车在车场的位置/1--" << Enter->top << "/:";
			cin >> room;//离开车的号码
			if (room >= 1 && room <= Enter->top)
				break;
		}
		while (Enter->top > room)//从停车场堆栈向中转堆栈移动车辆，直到要离开车辆的位置停止
		{
			Temp->top++;
			Temp->stack[Temp->top] = Enter->stack[Enter->top];
			Enter->stack[Enter->top] = NULL;
			Enter->top--;
		}
		p = Enter->stack[Enter->top];
		Enter->stack[Enter->top] = NULL;
		Enter->top--;
		while (Temp->top >= 1)//将中转堆栈中的车辆移到停车场堆栈
		{
			Enter->top++;
			Enter->stack[Enter->top] = Temp->stack[Temp->top];
			Temp->stack[Temp->top] = NULL;
			Temp->top--;
		}
		cout << "-----------------------------------------------------" << endl;
		cout << "车辆离开登记结算：" << endl;
		PRINT(p, room);
		if ((W->head != W->rear) && Enter->top < MAX)
		{
			q = W->head->next;
			t = q->data;
			Enter->top++;//
			cout << "停车场中的" << room + 1 << "," << room + 2 << "号车离开停车场为" << room << "号车让位。" << endl;
			cout << "停车场中" << room << "号车离开。" << endl;
			cout << "便道的" << t->num << "号车进入车场第" << Enter->top << "位置." << endl;
			cout << "请输入" << t->num << "号车进入车场时间：" << endl;
			cout << "输入小时：";
			cin >> t->reach.hour;
			cout << "输入分钟:";
			cin >> t->reach.min;
			W->head->next = q->next;
			if (q == W->rear)W->rear = W->head;
			Enter->stack[Enter->top] = t;
			delete q;
		}
		else cout << endl << "便道里没有车!" << endl;
	}
	else cout << endl << "车场里没有车" << endl;
}
void List1(SeqStackCar* S)//查看停车场内车辆情况的函数定义
{
	int i;
	if (S->top > 0)
	{
		cout << "-----------------------------------------------------" << endl;
		cout << endl << "车场内部车辆停靠情况：" << endl;
		cout << endl << "位置       到达时间         车牌号" << endl;
		for (i = 1; i <= S->top; i++)
		{
			cout << i;
			cout << "             " << S->stack[i]->reach.hour << ":" << S->stack[i]->reach.min;
			cout << "             " << S->stack[i]->num << endl;
		}
	}
	else cout << endl << "车场里没有车" << endl;
	cout << "--------------------------------------------------------" << endl;
}
void List2(LinkQueueCar* W)//查看便道上停靠车辆情况的函数定义
{
	QueueNode* p;
	p = W->head->next;
	if (W->head != W->rear)
	{
		cout << "---------------------------------------------------" << endl;
		cout << endl << "便道停靠车辆情况：" << endl;
		while (p != NULL)
		{
			cout << endl << "车辆牌号：";
			cout << p->data->num << endl;
			p = p->next;
		}
	}
	else cout << endl << "便道里没有车。" << endl;
	cout << "--------------------------------------------------------" << endl;
}
void List(SeqStackCar S, LinkQueueCar W)//车辆列表显示函数
{
	int flag, tag;
	flag = 1;
	while (flag)
	{
		cout << "----------------------------------------------------" << endl;
		cout << "车辆停靠查询开始：" << endl;
		cout << endl << "请选择 1|2|3： " << endl;
		cout << "1.车场列表" << endl << "2.便道列表" << endl << "3.返回主菜单" << endl;
		while (1)
		{
			cin >> tag;
			if (tag >= 1 || tag <= 3)break;
			else cout << endl;
			cout << "请选择 1~3：";
		}
		switch (tag)
		{
		case 1:List1(&S);
			cout << "车辆停靠查询结束!" << endl; break;
		case 2:List2(&W);
			cout << "车辆停靠查询结束!" << endl; break;
		case 3:flag = 0; break;
		default:break;
		}
	}
	cout << "-----------------------------------------------------" << endl;
}