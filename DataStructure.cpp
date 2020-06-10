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
/*ÁÚ½Ó±í*/
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

/*AVLÊ÷*/
int AirLineAVLTree::GetHeight(AVLTree A) {
    if (A == NULL)
        return 0;
    return A->Height;
}
AVLTree  AirLineAVLTree::FindMax(AVLTree T)
{
    while (T && T->Right)
    {
        T = T->Right;
    }
    return T;
}
AVLTree  AirLineAVLTree::FindMin(AVLTree T)
{
    while (T && T->Left)
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
AVLTree  AirLineAVLTree::Insert(AVLTree Key, AVLTree& T)
{
    if (!T) {
        T = Key;
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
        AVLTree temp = T;
        while (temp->Left || temp->Right)
        {
            temp = FindMax(temp->Left);
            T->Code = temp->Code;
            T->FlightDate = temp->FlightDate;
            T->Height = temp->Height;
            T->PlaneNo = temp->PlaneNo;
            T->RemainTickets = temp->RemainTickets;
            T->EndName = temp->EndName;
            T->StartName = temp->StartName;
            T = temp;
        }
        delete temp;
        T = NULL;
        return NULL;
    }
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
void  AirLineAVLTree::ClearByDate(int date)
{
    vector<AVLPTR> keys;
    GetPTR(root, date, keys);
    for (int i = 0; i < keys.size(); i++)
    {
        auto head = keys[i]->head;
        auto node = keys[i]->node;
        head->edge.erase(node);
        Delete(keys[i]->Code, root);
    }
}
void  AirLineAVLTree::GetPTR(AVLTree T, int date, vector<AVLPTR>& code) {
    if (T) {
        if (T->FlightDate < date)
            code.push_back(T);
        GetPTR(T->Left, date, code);
        GetPTR(T->Right, date, code);
    }
}