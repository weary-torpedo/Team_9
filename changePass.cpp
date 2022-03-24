#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <fstream>
using namespace std;



struct NodePass{
    string user,pass;
    NodePass* pNext;
};

void changePass(string username){
    ifstream fin;

    if(username[0] >= '0' && username[0] <= '9')
        fin.open("loginStu.txt");
    else
        fin.open("loginStaff.txt");
    
    string usercheck, passcheck, tmppass;
    NodePass* pHead = nullptr, *pCur;
    while(!fin.eof()){
        fin >> usercheck >> passcheck;
        if(usercheck == username){
			gotoxy(32,11);
            cout << "Current password: ";
			box(50,10,25,2);
			gotoxy(36,14);
			cout << "New password: ";
			box(50,13,25,2);
			gotoxy(52,11);
            cin >> tmppass;
            if(tmppass == passcheck){
                gotoxy(52,14);
                cin >> tmppass;
                passcheck = tmppass;
            }
            else{
                cout << "wrong password.";
                return;
            }
        }
        NodePass* pTmp = new NodePass;
        pTmp->user = usercheck;
        pTmp->pass = passcheck;
        pTmp->pNext = nullptr;
        if(pHead == NULL){
			pHead = pTmp;
			pCur = pHead;
		}
        else{
			pCur->pNext = pTmp;	
			pCur = pCur->pNext;
		}
    }
    fin.close();
    
    ofstream fout;
    if(username[0] >= '0' && username[0] <= '9')
        fout.open("loginStu.txt");
    else
        fout.open("loginStaff.txt");
    
    while(pHead){
        fout << pHead->user << " " << pHead->pass << endl;
        NodePass* pTmp = pHead;
        pHead = pHead->pNext;
        delete pTmp;
    }
}

