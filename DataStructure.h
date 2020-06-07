#pragma once
struct AirLineAVLNode;
typedef AirLineAVLNode* AVLPTR;
typedef AVLPTR AVLTree;
/*客户信息*/
typedef struct Customer
{
	string Name;
	string AirLineCode;
	int TicketsNum;
	int PlaneNum;

	Customer(string n, string a, int num, int code) {
		Name = n;
		AirLineCode = a;
		TicketsNum = num;
		PlaneNum = code;
	}
}Customer;
/*终点结点*/
typedef struct StationNode
{
	string EndName;
	list<Customer> CustomerList;
	queue<Customer> WatingQueue;
	AVLPTR AirLineData;

	StationNode(string e = "", AVLPTR p = NULL) {
		EndName = e;
		AirLineData = p;
	}
}StationNode;
/*起点结点*/
typedef struct StationHead
{
	string StartName;
	list<StationNode> edge;

	StationHead(string s = "") { StartName = s; }
}StationHead;
typedef list<StationNode>::iterator STATIONNODEPTR;
typedef list<StationHead>::iterator STATIONHEADPTR;
/*邻接表类*/
class StationTable
{
public:
	list<StationHead> headlist;

	STATIONNODEPTR Find(string start, string end);
	void Insert(string start, string end, AVLPTR data);
	bool Remove(string start, string end);
	bool Remove(STATIONHEADPTR start, STATIONNODEPTR end);
};
/*AVL结点*/
typedef struct AirLineAVLNode
{
    string Code;
    string EndName;
    string StartName;
    int FlightDate;
    int PlaneNumber;
    int RemainTickets;
    int Height;
    AirLineAVLNode* Left;
    AirLineAVLNode* Right;
    STATIONNODEPTR  node;
    STATIONHEADPTR  head;

    AirLineAVLNode() {
        Left = Right = NULL;
        Height = 1;
    }
}AirLineAVLNode;
/*AVL类*/
class AirLineAVLTree
{
public:
    AirLineAVLNode* root;

    AirLineAVLTree() { root = NULL; }
    int GetHeight(AVLTree A);
    AVLTree FindMax(AVLTree T);
    AVLTree FindMin(AVLTree T);
    AVLTree LeftRotation(AVLTree T);
    AVLTree RightRotation(AVLTree T);
    AVLTree LeftRightRotation(AVLTree T);
    AVLTree RightLeftRotation(AVLTree T);
    AVLTree Insert(AVLTree Key, AVLTree T);
    AVLTree Delete(string Key, AVLTree T);
    AVLTree FindByCode(string key, AVLTree T);
    void ClearByDate(int date);
private:
    void GetPTR(AVLTree T, int date, vector<AVLPTR>& code);
};