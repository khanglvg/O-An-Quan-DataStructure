#include<iostream>
using namespace std;
#pragma region Prototype

#pragma region Terminology
//Mandarin square capturing: trò ô ăn quan
//Ricefields: ruộng
//Citizen squares: ô dân
//Citizen: dân
//Mandarin squares: ô quan
//Mandarin: quan
#pragma endregion

#pragma region Define Node and List

struct Node
{
	int dan, quan;
	int laquan; 
	Node *pre, *next;
};

struct List
{
	Node *pHead;
	Node *pTail;
};

#pragma endregion

#pragma region Create Node and List
void Init(List &l); // p.Head = null;
Node *GetNode(int _dan, int _quan, int _laquan); // tạo node, nhập thông tin vào, pre/next = null;
void AddHead(List &l, Node *pnew); // thêm vào Head;
void CreateList(List &l);
void OutPut(List l, int count1, int count2);
#pragma endregion



Node *SetNodeFromPos(List l, int pos);
void Distribute_AndScore(List &l, int &count, Node *p, int dir); // Rải hết quân thì dừng

// Trường hợp ăn
bool isScored_Pre(Node *p);
bool isScored_Next(Node *p);

#pragma region Check End Game
//Kiểm tra trạng thái dừng trò chơi
bool isStoped(List);// Cả 2 ô quan đều không có dân và không có quan
bool isContinued_P1(List, int);// Player 1 không còn đủ 5 dân và những ô dân trên bàn cờ của họ trống
bool isContinued_P2(List, int);// Player 2 không còn đủ 5 dân và những ô dân trên bàn cờ của họ trống
#pragma endregion


#pragma region Check Condition
bool isHaveCitizen_InBoard_P1(List);
bool isHaveCitizen_InBoard_P2(List);//Kiểm tra có còn dân trên bàn mình không;
bool isGreater_5Citizen(int);//Kiểm tra có còn hơn 5 dân không?
#pragma endregion


#pragma region Distribute After No Citizen
void Distribute_P1(List &l, int &count);// Rải quân sau khi bàn không còn quân cho P1
void Distribute_P1(List &l, int &count);// Rải quân sau khi bàn không còn quân cho P2
#pragma endregion


void ChangeVirtualPosition(int &pos);
void CheckToDistribute_P1(List l, int &count1, int, bool dung2);
void CheckToDistribute_P2(List l, int, int &count2, bool dung2);// Kiểm tra có hết quân trên bàn không và rải quân


int whoWin(List l, int &count1, int &count2); //So sánh ai nhiều quân hơn thì thắng. Trả về giá trị 1 người 1 thắng, giá trị 2 người 2 thắng, giá trị 0 hòa

#pragma endregion

int main()
{
	List l;
	int pos1 = 0, pos2 = 0;
	int clockwise_P1 = 0, clockwise_P2 = 0; //clockwise = 1 (right), clockwise = 2 (left)
	char way;
	int playagain = 1;
	cout << "Here is direction: \nInput your cell's position you want to move and the way.";
	cout << "\nPLAYER 1 Cell's position must be from 1 to 5.";
	cout << "\nPLAYER 2 Cell's position must be from 6 to 10.";
	cout << "\nThe way is RIGHT(R/r) or LEFT(L/l).";
	cout << "\n\t\t\t LET'S GO \n\n";

	while (playagain == 1)
	{
		int count1 = 0, count2 = 0;

		bool pauseP2 = false;
		bool pauseP1 = false;

		//Khởi tạo và xuất ra màn hình trạng thái trước khi chơi
		CreateList(l);
		OutPut(l, count1, count2);


		for (int turn = 1; !isStoped(l) && !pauseP2 && !pauseP1; turn++) //turn: lượt của mỗi người chơi 
		{
			if (turn % 2 != 0) // Lượt người thứ 1
			{
				cout << "\n\n--------------------PLAYER 1--------------------\n";
				Node *p = new Node;
				CheckToDistribute_P1(l, count1, count2, pauseP1);

				cout << "Cell's position:\t\t";
				cin >> pos1;
				p = SetNodeFromPos(l, pos1);
				while (pos1 > 5 || pos1 < 1 || p->dan == 0)
				{
					cout << "\nYour cell is INCORRECT or NO CITIZEN ---------- Please choose again :) \n ";
					cout << "Cell's position:\t\t";
					cin >> pos1;
					p = SetNodeFromPos(l, pos1);
				}

				cout << "\n(Right = R/r, Left = L/l):\t";
				cin >> way;
				while ((way != 'R' && way != 'r' && way != 'l' && way != 'L'))
				{
					cout << "\nYour way must be Right (R/r) or Left(L/r) ---------- Please choose again :)";
					cout << "\n(Right = R/r, Left = L/l):\t";
					cin >> way;
				}


				if (way == 'R' || way == 'r')	clockwise_P1 = 1;
				else clockwise_P1 = 2;
				Distribute_AndScore(l, count1, p, clockwise_P1);
				cout << "\n\n";
				OutPut(l, count1, count2);
				pauseP2 = isContinued_P2(l, count2);
				pauseP1 = isContinued_P1(l, count1);
			}

			else // Lượt người thứ 2
			{
				cout << "\n\n--------------------PLAYER 2--------------------\n";
				Node *p = new Node;
				CheckToDistribute_P2(l, count1, count2, pauseP2);

				cout << "Cell's position:\t\t";
				cin >> pos2;
				ChangeVirtualPosition(pos2); //6-7-8-9-10 (trong máy) -> 10-9-8-7-6(người dùng thấy)
				p = SetNodeFromPos(l, pos2); // gán p = Node đang chọn
				while (pos2 < 6 || pos2>10 || p->dan == 0)
				{
					cout << "\nYour cell is INCORRECT or NO CITIZEN ---------- Please choose again :) \n ";
					cout << "\nCell's position:\t\t";
					cin >> pos2;
					ChangeVirtualPosition(pos2);
					p = SetNodeFromPos(l, pos2);
				}

				cout << "\n(Right = R/r, Left = L/l):\t";
				cin >> way;
				while ((way != 'R' && way != 'r' && way != 'l' && way != 'L'))
				{
					cout << "\nYour way must be Right (R) or Left(L) ---------- Please choose again :)";
					cout << "\n(Right = R/r, Left = L/l):\t";
					cin >> way;
				}


				if (way == 'R' || way == 'r')	clockwise_P2 = 1;
				else clockwise_P2 = 2;
				Distribute_AndScore(l, count2, p, clockwise_P2);
				cout << "\n\n";
				OutPut(l, count1, count2);
				pauseP2 = isContinued_P2(l, count2);
				pauseP1 = isContinued_P1(l, count1);
			}
		}

		/*Node* temp = new Node;
		temp = l.pHead;
		for (int i = 1; i <= 12; i++, temp = temp->next)
		{
			if (i <= 5)
			{
				count1 += temp->dan;
			}
			if (i >= 7 && i <= 11)
			{
				count2 += temp->dan;
			}
		}*/

		int result = whoWin(l, count1, count2);
		cout << "\nPLAYER 1: " << count1 << " POINTS";
		cout << "\nPLAYER 2: " << count2 << " POINTS";
		if (result == 1)
			cout << "\nPLAYER 1 WIN";
		else
			if (result == 0)
				cout << "\nEQUAL";
			else
				cout << "\nPLAYER 2 WIN";
		cout << endl;

		cout << "Play again? (1-Yes / 0-No)";
		cin >> playagain;
	}
	system("pause");
}

#pragma region Definition
void Init(List &l)
{
	l.pHead = NULL;
	l.pTail = NULL;
}

Node* GetNode(int _dan, int _quan, int _laquan)
{
	Node *p = new Node;
	if (p == NULL)
	{
		cout << "Error.The memory is full" << endl;
		return p;
	}

	p->dan = _dan;
	p->quan = _quan;
	p->laquan = _laquan;
	p->next = p->pre = NULL;
	return p;
}

void AddHead(List &l, Node *pnew)
{
	if (l.pHead == NULL)
		l.pHead = l.pTail = pnew;
	else
	{
		l.pHead->pre = pnew;
		pnew->next = l.pHead;
		l.pHead = pnew;
	}
}

// Khởi tạo cấu trúc ô ăn quan.
//Khởi tạo danh sách các node của ô ăn quan gồm 5 nút dân và 2 nút quan
void CreateList(List &l)
{
	Init(l);
	for (int i = 1; i <= 12; i++)
	{
		if (i == 1 || i == 7) // vị trí quan là 1,7
		{
			Node *p = new Node;
			p = GetNode(0, 10, 1);
			AddHead(l, p);
		}
		else //những vị trí không phải là quan
		{
			Node *p = new Node;
			p = GetNode(5, 0, 0);
			AddHead(l, p);
		}
	}
	l.pHead->pre = l.pTail;
	l.pTail->next = l.pHead;
}

void OutPut(List l, int count1, int count2)
{
	Node *p = new Node;
	int i;
	cout << "-    ";
	for (p = l.pHead, i = 1; i <= 12; i++, p = p->next)
	{
		if (i == 1 || i == 7)
			cout << "[" << p->dan << ", ";
		else if (i == 5 || i == 11)
			cout << p->dan << "]";
		else if (i == 6 || i == 12)
			cout << " <" << p->quan + p->dan << "> ";
		else
			cout << p->dan << ", ";
	}
	cout << "( " << count1 << "||" << count2 << " )" << endl;
}

Node* SetNodeFromPos(List l, int pos)
{
	Node *p = new Node;
	p = l.pHead;

	for (int i = 1; i <= pos - 1; i++, p = p->next)
	{
		if (i == 5) // nếu vị trí chọn là 5 thì chuyển qua vị trí là 7
		{
			p = p->next;
		}
	}
	return p;
}

void ChangeVirtualPosition(int &pos)
{
	if (pos == 7)	pos = 9;
	else
		if (pos == 6)	pos = 10;
		else
			if (pos == 9)	pos = 7;
			else
				if (pos == 10)	pos = 6;
}

bool isScored_Next(Node *p)
{
	if (p->dan == 0 && p->laquan == 0 && ((p->next->dan + p->next->quan)>0))
		return true;
	if (p->dan == 0 && p->laquan == 1 && p->quan == 0 && ((p->next->dan + p->next->quan)>0))
		return true;
	return false;
}

bool isScored_Pre(Node *p)
{
	if (p->dan == 0 && p->laquan == 0 && ((p->pre->dan + p->pre->quan)>0))
		return true;
	if (p->dan == 0 && p->laquan == 1 && p->quan == 0 && ((p->pre->dan + p->pre->quan)>0))
		return true;
	return false;
}

void Distribute_AndScore(List &l, int &count, Node *p, int dir)
{
	Node *temp = new Node;
	temp = p;
	if (dir == 1)
	{
		while (p->dan != 0) //for(int i = 1;p->dan ==0; i++)
		{
			p->dan--;
			temp = temp->next;
			temp->dan++;
		}
		temp = temp->next;
		while ((temp->dan != 0) && (temp->laquan == 0))
		{
			int t = temp->dan;
			temp->dan = 0;
			while (t != 0)
			{
				t--;
				temp = temp->next;
				temp->dan++;
			}
			temp = temp->next;
		}
		while (isScored_Next(temp))
		{
			count = count + temp->next->dan + temp->next->quan;
			temp->next->dan = 0; temp->next->quan = 0;
			temp = temp->next->next;
		}
	}

	else
	{
		while (p->dan != 0)
		{
			p->dan--;
			temp = temp->pre;
			temp->dan++;
		}
		temp = temp->pre;
		while ((temp->dan != 0) && temp->laquan == 0)
		{
			int t = temp->dan;
			temp->dan = 0;
			while (t != 0)
			{
				t--;
				temp = temp->pre;
				temp->dan++;
			}
			temp = temp->pre;
		}
		while (isScored_Pre(temp))
		{
			count = count + temp->pre->dan + temp->pre->quan;
			temp->pre->dan = 0; temp->pre->quan = 0;
			temp = temp->pre->pre;
		}
	}
}

bool isStoped(List l)
{
	Node *p = new Node;
	int i = 1, temp = 0;
	for (p = l.pHead; i <= 12; i++, p = p->next)
	{
		if (p->laquan == 1 && (p->quan + p->dan) == 0)
			temp++;
	}
	return temp == 2 ? true : false; // if temp = 2 return true; else return false
}

bool isHaveCitizen_InBoard_P1(List l) //Kiểm tra xem còn quân trên bàn hay không;
{
	Node *p = new Node;
	int i = 1;
	for (p = l.pHead; i <= 5; i++, p = p->next)
	{
		if (p->dan>0)
			return true;
	}
	return false;
}

bool isHaveCitizen_InBoard_P2(List l) //Kiểm tra có còn dân trên bàn mình không?
{
	Node *p = new Node;
	int i = 1;
	for (p = l.pTail->pre; i <= 5; i++, p = p->pre)
	{
		if (p->dan>0)
			return true;
	}
	return false;
}

bool isGreater_5Citizen(int count)  //Kiểm tra có còn hơn 5 dân không?
{
	return count >= 5 ? true : false;
}

bool isContinued_P1(List l, int count) // người 1 không còn đủ 5 dân và những ô dân trên bàn cờ của họ trống
{
	if (!isHaveCitizen_InBoard_P1(l) && isGreater_5Citizen(count))
		return true;
	else
		return false;
}

bool isContinued_P2(List l, int count) // người 2 không còn đủ 5 dân và những ô dân trên bàn cờ của họ trống
{
	if (!isHaveCitizen_InBoard_P2(l) && isGreater_5Citizen(count))
		return true;
	else
		return false;
}

void Distribute_P1(List &l, int &count)
{
	Node *p = new Node;
	int i = 1;
	for (p = l.pHead; i<=5; i++, p = p->next)
	{
		p->dan++;
	}
	count -= 5;
}

void Distribute_P2(List &l, int &count)
{
	Node *p = new Node;
	int i = 1;
	for (p = l.pTail->pre; i<=5; i++, p = p->pre)
	{
		p->dan++;
	}
	count -= 5;
}

void CheckToDistribute_P1(List l, int &count1, int count2, bool pause)
{
	if (!pause && !isHaveCitizen_InBoard_P1(l) )
	{
		cout << "\nPLAYER 111 het quan tren ban. Rai Quan: \n" << endl;
		Distribute_P1(l, count1);
		OutPut(l, count1, count2);
	}
}

void CheckToDistribute_P2(List l, int count1, int &count2, bool pause)
{
	if (!pause && !isHaveCitizen_InBoard_P2(l) )
	{
		cout << "\nPLAYER 222 het quan tren ban. Rai Quan: \n" << endl;
		Distribute_P2(l, count2);
		OutPut(l, count1, count2);
	}
}

int whoWin(List l, int &count1, int &count2) // so sánh điểm của P1 vs P2; return 1 nếu P1 win, else return 2;
{
	Node *p = new Node;
	Node *t = new Node;
	int i = 1, j = 1;
	for (p = l.pHead; i<=5; i++, p = p->next)
	{
		count1 += p->dan;
	}
	for (t = l.pTail->pre; j<=5; j++, t = t->pre)
	{
		count2 += t->dan;
	}
	if (count1>count2) return 1;
	else
		if (count1 == count2) return 0;
		else return 2;
}
#pragma endregion

