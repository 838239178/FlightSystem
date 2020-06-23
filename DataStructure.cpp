#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <iomanip>
#include <algorithm>
using namespace std;
#include "DataStructure.h"
/*邻接表*/
AVLPTR StationTable::Find(string start, string endd)
{
    STATIONNODEPTR sta;
    auto it = headlist.begin();
    while (it != headlist.end()) {
        if (it->StartName == start) {
            sta = it->edge.begin();
            while (sta != it->edge.end()) {
                if (sta->EndName == endd)
                    return sta->AirLineData;
                sta++;
            }
            break;
        }
        it++;
    }
    return NULL;
}
bool StationTable::Find(string start, string endd, vector<AVLPTR>& ptrs)
{
    STATIONNODEPTR sta;
    auto it = headlist.begin();
    while (it != headlist.end()) {
        if (it->StartName == start) {
            sta = it->edge.begin();
            while (sta != it->edge.end()) {
                if (sta->EndName == endd)
                    ptrs.push_back(sta->AirLineData);
                sta++;
            }
            break;
        }
        it++;
    }
    if (ptrs.empty())
        return false;
    else
        return true;
}
void StationTable::Insert(string start, string endd, AVLPTR data)
{
    auto it = headlist.begin();
    while (it != headlist.end()) {
        if (it->StartName == start) {
            it->edge.push_front(StationNode{ endd,data });
            data->head = it;
            data->node = it->edge.begin();
            break;
        }
        it++;
    }
    if (it == headlist.end()) {
        headlist.push_front(StationHead{ start });
        it = headlist.begin();
        it->edge.push_back(StationNode{ endd,data });
        data->head = it;
        data->node = it->edge.begin();
    }
}
bool StationTable::Remove(string start, string endd)
{
    auto it = headlist.begin();
    STATIONNODEPTR sta;
    bool flag = false;
    while (it != headlist.end()) {
        if (it->StartName == start) {
            while (sta != it->edge.end()) {
                if (sta->EndName == endd) {
                    it->edge.erase(sta);
                    flag = true;
                    break;
                }
            }
            break;
        }
        it++;
    }
    if (it->edge.size() == 0) {
        headlist.erase(it);
    }
    return flag;
}
bool StationTable::Remove(STATIONHEADPTR start, STATIONNODEPTR end)
{
    start->edge.erase(end);
    return true;
}
void StationTable::Clear() 
{
    headlist.clear();
}

/*AVL树*/ 
//把a的值赋给b
void AirLineAVLTree::Deliver(AVLTree a, AVLTree b)             
{
    //把a对应的站点的指针指向b
    a->node->AirLineData = b;
    //a的其他值覆盖b
    b->Code = a->Code;
    b->EndName = a->EndName;
    b->StartName = a->StartName;
    b->head = a->head;
    b->node = a->node;
    b->Height = a->Height;
    b->PlaneNo = a->PlaneNo;
    b->RemainTickets = a->RemainTickets;
    b->FlightDate = a->FlightDate;
}
int AirLineAVLTree::GetHeight(AVLTree A) {
    if (A == NULL)
        return 0;
    return A->Height;
}
//返回子树最大结点的父节点，需要注意的是无子节点时返回它本身
AVLTree  AirLineAVLTree::FindMaxParent(AVLTree T)
{
    while (T && T->Right && T->Right->Right)
    {
        T = T->Right;
    }
    return T;
}
//返回子树最小结点的父节点，需要注意的是无子节点时返回它本身
AVLTree  AirLineAVLTree::FindMinParent(AVLTree T)
{
    while (T && T->Left && T->Left->Left)
    {
        T = T->Left;
    }
    return T;
}
AVLTree AirLineAVLTree::LeftRotation(AVLTree T)
{
    AVLTree TR = T->Right;
    T->Right = TR->Left;
    TR->Left = T;
    T->Height = max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
    TR->Height = max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
    return TR;
}
AVLTree  AirLineAVLTree::RightRotation(AVLTree T)
{
    AVLTree TL = T->Left;
    T->Left = TL->Right;
    TL->Right = T;
    T->Height = max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
    TL->Height = max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
    return TL;
}
AVLTree  AirLineAVLTree::LeftRightRotation(AVLTree T)
{
    if (T->Left)
        T->Left = LeftRotation(T->Left);
    return RightRotation(T);
}
AVLTree  AirLineAVLTree::RightLeftRotation(AVLTree T)
{
    if (T->Right)
        T->Right = RightRotation(T->Right);
    return LeftRotation(T);
}
//将传入指针所指的数据插入到树中,指针不能为空
AVLTree  AirLineAVLTree::Insert(AVLTree Key, AVLTree& T)
{
    if (!T) {
        T = Key;
        if (!T) return NULL;
        T->Height = 1;
        T->Left = T->Right = NULL;
        return T;
    }
    if (T->Code > Key->Code) {
        T->Left = Insert(Key, T->Left);
        if (GetHeight(T->Left) - GetHeight(T->Right) > 1) {
            if (Key->Code < T->Left->Code)
                T = RightRotation(T);
            else
                T = LeftRightRotation(T);
        }
    }
    else if (T->Code < Key->Code) {
        T->Right = Insert(Key, T->Right);
        if (GetHeight(T->Right) - GetHeight(T->Left) > 1) {
            if (Key->Code > T->Right->Code)
                T = LeftRotation(T);
            else
                T = RightLeftRotation(T);
        }
    }
    T->Height = max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
    return T;
}
AVLTree AirLineAVLTree::Delete(string Key, AVLTree T)
{
    if (T->Code > Key) {
        T->Left = Delete(Key, T->Left);
        if (GetHeight(T->Right) - GetHeight(T->Left) > 1) {
            if (GetHeight(T->Right->Right) > GetHeight(T->Right->Left))
                T = LeftRotation(T);
            else
                T = RightLeftRotation(T);
        }
        T->Height = max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
        return T;
    }
    else if (T->Code < Key) {
        T->Right = Delete(Key, T->Right);
        if (GetHeight(T->Left) - GetHeight(T->Right) > 1) {
            if (GetHeight(T->Left->Right) < GetHeight(T->Left->Left))
                T = RightRotation(T);
            else
                T = LeftRightRotation(T);
        }
        T->Height = max(GetHeight(T->Left), GetHeight(T->Right)) + 1;
        return T;
    }
    else {
        if (T->Left && T->Right) {
            AVLTree temp = FindMaxParent(T->Left);
            AVLTree pre = temp->Right? temp->Right : temp;
            Deliver(pre, T);
            delete pre;
            temp->Right = NULL;
        }
        else if (T->Left && !T->Right) {
            Deliver(T->Left, T);
            AVLTree d = T->Left;
            delete d;
            T->Left = NULL;
        }
        else if (!T->Left && T->Right) {
            Deliver(T->Right, T);
            AVLTree d = T->Right;
            delete d;
            T->Right = NULL;
        }
        else {
            delete T;
            T = NULL;
        }
        return T;
    }
    return T;
}
AVLTree  AirLineAVLTree::FindByCode(string key, AVLTree T)
{
    AVLTree m = T;
    while (m) {
        if (key > m->Code)
            m = m->Right;
        else if (key < m->Code)
            m = m->Left;
        else
            return m;
    }
    return NULL;
}
bool  AirLineAVLTree::ClearByDate(int date)
{
    stack<AVLPTR> s;
    AVLPTR T = root;
    map<AVLTree, bool> isFrist;             //判断是否第一次出现
    /*后序遍历*/
    while (!s.empty() || T) {
        while (T)
        {
            isFrist[T] = true;
            s.push(T);
            T = T->Left;
        }
        if (!s.empty()) {
            T = s.top();
            if (isFrist[T]) {
                isFrist[T] = false;
                T = T->Right;
            }
            else {
                if (T->FlightDate < date) {
                    /*删除邻接表结点*/
                    auto head = T->head;
                    auto node = T->node;
                    head->edge.erase(node);
                    /*删除树中结点*/
                    root = Delete(T->Code, root);
                    return true;
                }
                s.pop();
                T = NULL;
            }
        }
    }
    return false;
}
