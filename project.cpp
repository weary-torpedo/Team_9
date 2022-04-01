#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
using namespace std;

#define xp 8 // xp (x pointer) means the place ( at console) we use gotoxy to print box

struct NodePass{
    string user,pass;
    NodePass* pNext;
};

struct Score {
    float Mid;
    float Final;
    float Other;
    float GPA;
} 

struct Student{
	int No, IDSocial;
	string firstname, lastname,gender, date, IDStu, course;
    Score *Inclass;
	Student* pNext;
};

struct Class{
	string className;
	int numberOfStu;
	Student* pHeadStu;
	Class* pNext;
};

struct Course{
	string nameCou;
	string IDCou;
	string teacher;
	int credits;
	int maxStu, enrolling;
	string day1, day2;
	string session1, session2; //bu?i h?c
	Student* Stu;
	Course* pNext;
};

struct Semester{
	Course* pHeadCou;	
	string  startDate;
	string endDate;
};

struct Year{
	Semester Sem1, Sem2, Sem3;
	Class* pHeadClass;
	Year* pNext;
	int start, end;
};

string startDateRegister, endDateRegister;
bool logOut;
int orderSem, soluong;

void staffSee(Year *&pcurYear, int orderSem);

void createCourseCSV(Year *&pCurYear, int orderSem);
void createCourse(Year *&pCurYear, int orderSem);
void editCourse(Year *&pCurYear, int orderSem, int orderCou);
void deleteCourse(Year *&pCurYear, int orderSem, int orderCou);
void exportCourse(Year *pCurYear, int orderSem);
void listCourse(Year *pCurYear, int orderSem, int &soluong);
void createCourseRegister(Year *&pCurYear, int &orderSem);
void PrintCourse (Year *pCurYear, int semester);
void RegisterCou(Year *pcurYear, int sem, Course *pHead, Student *curStu);
bool CheckScheduleCou (Student *curStu, Course *curEnroll, Course *pHead);
void Runtest (Year *pcurYear, int sem, Student *curStu);
void UpdateData (Year *pCurYear, int semester, bool yearCreated);

void createSemester(Year *&pCurYear, int &orderSem);

Class* CreateClass(Class*& phead, string tmp, Year*& curyear);
void ImportNewStu (string filename,  Class *curClass);
void ImportOldStu(string filename, Class*& cHead);
void ImportClasses (Class *&pheadClass, Year *&pcurYear);
void OutPutStu (Class *pheadClass);

void createYear(Year *&pcurYear);

void staffCreate(Year *&pcurYear, int &orderSem);

int checkTime();

void changePass(string username);
void viewProfile(Student *pStu);
void homePage(Student *pStu, string username);

void importData(Year *&pcurYear);

void gotoxy(int x, int y);
void printBox(string text, int x, int y, int size);
void box(int x, int y, int w, int h);
bool loginCheck(string username, string password);
string loginSystem();

int main(){

	Student *pStudent = new Student; 
	pStudent->IDStu = "21127637";
	pStudent->lastname = "Phan";
	pStudent->firstname = "Linh";	

	Year *pcurYear;
	importData(pcurYear);
	do{
		logOut = false;
		gotoxy(20,20);
		string username =  loginSystem();
		homePage(pStudent, username);
		int time = checkTime();
		
		if (!logOut && !('2' <= username[0] && username[0] <= '9') && time == 1)
				staffCreate(pcurYear, orderSem);
//		if (!logOut && ('2' <= username[0] && username[0] <= '9') && time == 2)
//				Runtest(pcurYear, 1, pStudent);
		if (!logOut && !('2' <= username[0] && username[0] <= '9') && time == 2)
				staffSee(pcurYear, orderSem);
//		if (!logOut && !('2' <= username[0] && username[0] <= '9') && time == 3)
//				staffScore(pcurYear, orderSem);	
	}
	while (logOut);
	
	gotoxy(30,30);
	return 0;
}

void importCourse (string filename, Year *&pcurYear, int orderSem){
	fstream FILE;
	FILE.open(filename, ios::in);
	
	string tmp;
	tmp = "";
	Course *pHeadCou = NULL, *pCurCou;

	while (!(FILE.eof())){
		Course *pNewCou = new Course;
		getline(FILE, tmp,',');
        if (tmp == "\0")
            break;		
        pNewCou -> IDCou = tmp;
		getline(FILE,pNewCou->nameCou,',');
		getline(FILE,tmp,',');
		pNewCou->credits = stoi(tmp); 
		tmp = "";
		getline(FILE,pNewCou->day1,',');
		getline(FILE,pNewCou->session1,',');
		getline(FILE,pNewCou->day2,',');
		getline(FILE,pNewCou->session2,',');
		getline(FILE,pNewCou->teacher,',');
		getline(FILE,tmp, ',');
        pNewCou -> maxStu = stoi (tmp);
        tmp = "";
        getline(FILE,tmp, '\n');
        pNewCou -> enrolling = stoi (tmp);
        tmp = "";
        
        pNewCou -> pHeadInclasstu = NULL;
        Student *pCurInclasstu = pNewCou -> pHeadInclasstu;
        
        for ( int i = 0; i < pNewCou->enrolling; i++){
        	Student *pNewStu = new Student;
        	getline (FILE, tmp, ',');
            pNewStu -> No = stoi (tmp); 
            tmp = "";
            getline (FILE, tmp, ',');
            pNewStu -> IDStu = tmp;
            getline (FILE, tmp,',');
            pNewStu -> firstname = tmp.erase(0,1);
            getline (FILE, tmp,',');
            pNewStu -> lastname = tmp.erase(0,1);
            getline (FILE, tmp,',');
            pNewStu -> gender = tmp.erase(0,1);
            getline (FILE, tmp,',');
            pNewStu -> date = tmp.erase(0,1);
            getline (FILE, tmp,'\n');
            pNewStu -> IDSocial = stoi (tmp);
            tmp = "";
     		if (pNewCou -> pHeadInclasstu == NULL)
			pNewCou -> pHeadInclasstu =  pNewStu;
			if (pCurInclasstu != NULL)
				pCurInclasstu->pNext =  pNewStu;
			pCurInclasstu =  pNewStu;	
		}
        if (pCurInclasstu != NULL)
			pCurInclasstu->pNext = NULL;
        
		if (pHeadCou == NULL)
			pHeadCou = pNewCou;
		if (pCurCou != NULL)
			pCurCou->pNext = pNewCou;
		pCurCou = pNewCou;
	}
	if (pCurCou != NULL)
		pCurCou->pNext = NULL;		
	
	switch (orderSem){
		case 1:{
			pcurYear->Sem1.pHeadCou = pHeadCou;
			break;
		}
		case 2:{
			pcurYear->Sem2.pHeadCou = pHeadCou;
			break;
		}
		case 3:
			pcurYear->Sem3.pHeadCou = pHeadCou;	
	}
	FILE.close();	
}

void listStuOfCou (Year *&pcurYear, int &orderSem, int orderCou, bool print){
	system("cls");
	gotoxy(xp,5);
	cout << "NO";
	gotoxy(xp + 5,5);
	cout << "STUDENT ID";
	gotoxy(xp + 17,5);
	cout << "NAME ";
	gotoxy(xp + 40,5);
	cout << "BIRTH DATE";
	gotoxy(xp + 55 ,5);
	cout << "SOCIAL ID";
	
	Course *pHeadCou;
	switch (orderSem){
		case 1:{
			pHeadCou = pcurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pcurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pcurYear->Sem3.pHeadCou;
		}
	}
		
	Course *pCurCou = pHeadCou;	
	for ( int i = 0; i < orderCou; i++){
		pCurCou = pCurCou->pNext; 
	}
	
	Student *pCurStu = pCurCou->pHeadInclasstu;
	for ( int i = 1; i <= pCurCou->enrolling; i++){
		gotoxy(xp,5+i);
		cout << pCurStu->No;
		gotoxy(xp + 5,5+i);
		cout << pCurStu->IDStu;
		gotoxy(xp + 17,5+i);
		cout << pCurStu->firstname << " " << pCurStu->lastname;
		gotoxy(xp + 40,5+i);
		cout << pCurStu->date;
		gotoxy(xp + 55 ,5+i);
		cout << pCurStu->IDSocial;
		pCurStu = pCurStu->pNext;		
	}
	
	if (print){
		string tmp = pCurCou->IDCou + ".csv";
		fstream FILE;
		FILE.open(tmp, ios::out);	
		FILE << pCurCou->IDCou << ",";
		FILE << pCurCou->nameCou << ",";
		FILE << pCurCou->credits << ",";
		FILE << pCurCou->day1 << ",";
		FILE << pCurCou->session1 << ",";
		FILE << pCurCou->day2 << ",";
		FILE << pCurCou->session2 << ",";
		FILE << pCurCou->teacher << ",";
		FILE << pCurCou->maxStu << ",";
		FILE << pCurCou->enrolling;
		pCurStu = pCurCou->pHeadInclasstu;
		for ( int i = 1; i <= pCurCou->enrolling; i++){
			FILE << "\n" << pCurStu->No << ",";
			FILE << pCurStu->IDStu << ",";
			FILE << pCurStu->firstname << ",";
			FILE << pCurStu->lastname << ",";
			FILE << pCurStu->date << ",";
			FILE << pCurStu->IDSocial;
			pCurStu = pCurStu->pNext;		
	}
		FILE.close();	
		gotoxy(xp, 5 + soluong + 12 );
		cout << "You printed successfully!";
		gotoxy(xp, 5 + soluong + 13 );
		cout << "PRESS ENTER TO GO BACK...";
		getch();
	}
}

void listClass(Year* pcurYear, int &soluong){
	system("cls");
	gotoxy(xp,5);
	cout << "(Move arrow keys and enter to choose a class if you want to see list of students in that class)";
	gotoxy(xp,6);
	cout << "LIST OF CLASSES:";

	Class* pHeadClass = pcurYear->pHeadClass;
	Class* pCurClass = pHeadClass;
	soluong = -1;
	int i = 0;
	while(pCurClass != NULL){
		soluong++;
		gotoxy(xp+5,7+i);
		cout << pCurClass->className;
		pCurClass = pCurClass->pNext;
		i++;
	}
}

void listStuofClass(Year* pcurYear, int orderClass){
	system("cls");
	gotoxy(xp,5);
	cout << "NO";
	gotoxy(xp + 5,5);
	cout << "STUDENT ID";
	gotoxy(xp + 17,5);
	cout << "NAME ";
	gotoxy(xp + 40,5);
	cout << "BIRTH DATE";
	gotoxy(xp + 55 ,5);
	cout << "SOCIAL ID";

	Class* pCurClass = pcurYear->pHeadClass;
	for(int i = 0; i < orderClass; i++){
		pCurClass = pCurClass->pNext;
	}
	Student* pCurStu = pCurClass->pHeadStu;
	for ( int i = 1; i <= pCurClass->numberOfStu; i++){
		gotoxy(xp,5+i);
		cout << pCurStu->No;
		gotoxy(xp + 5,5+i);
		cout << pCurStu->IDStu;
		gotoxy(xp + 17,5+i);
		cout << pCurStu->lastname << " " << pCurStu->firstname;
		gotoxy(xp + 40,5+i);
		cout << pCurStu->date;
		gotoxy(xp + 55 ,5+i);
		cout << pCurStu->IDSocial;
		pCurStu = pCurStu->pNext;		
	}
	getch();
}

void staffSee(Year *&pcurYear, int orderSem){
	if (pcurYear->Sem1.pHeadCou != NULL)
			orderSem = 1;
	if (pcurYear->Sem2.pHeadCou != NULL)
			orderSem = 2;
	if (pcurYear->Sem3.pHeadCou != NULL)
			orderSem = 3;
			
	while (!logOut){
		system("cls");
		printBox("Press 1 to see list of classes",xp,5,50);
		printBox("Press 2 to see list of courses",xp,8,50);
		printBox("Press O to log out",xp,11,50);
		
		char c = getch();
		if (c == 'o' || c == 'O'){
			logOut = true;
			return;
		}
	else if ( c == '1')
			while(true){
				listClass(pcurYear, soluong);
				printBox("Press B to back",xp,5 + soluong + 9,25);
				char c1 = '1';
				int orderClass = 0;
				gotoxy(3,7);
				while(c1 != 13 && c1 != 'B' && c1 !='b'){
					c1 = getch();
					if(c1 == 72 && orderClass > 0){
						orderClass--;
						gotoxy(3,7 + orderClass);
					}
					else if (c1 == 80 && orderClass < soluong ){
						orderClass ++;
						gotoxy (3,7 + orderClass);	
					}	
				}

				if(c1 == 'B' || c1 =='b'){
					break;
				}
				else if(c1 == 13){
					listStuofClass(pcurYear,orderClass);
				}
			}
			
	else if ( c == '2')
			while ( true){
				switch (orderSem){
					case 1:{
						importCourse ("Sem1.csv", pcurYear, orderSem);
						break;
					}
					case 2:{
						importCourse ("Sem2.csv", pcurYear, orderSem);
						break;
					}
					case 3:{
						importCourse ("Sem3.csv", pcurYear, orderSem);
					}
				}
				listCourse(pcurYear,orderSem,soluong);
				gotoxy(xp,5 + soluong + 5);
				cout << "Move arrow keys and enter to choose a course,";
				gotoxy(xp,5 + soluong + 6);
				cout << "then you can see list of students in that course";
				printBox("Press B to back",xp,5 + soluong + 9,25);	
				
				char c1 = '1'; 
				int orderCou = 0;
				gotoxy(3,6);
				while (c1 != 13 && c1 != 'B' && c1 != 'b'){
					c1 = getch();
					if (c1 == 72 && orderCou > 0){ // len
						orderCou --;
						gotoxy (3,6 + orderCou);
					}
					else if (c1 == 80 && orderCou < soluong ){ // xuong
						orderCou ++;
						gotoxy (3,6 + orderCou);	
					}	
				}
				
				if (c1 == 'B' || c1 == 'b')
					break;
				else if (c1 == 13){
					listStuOfCou(pcurYear,orderSem,orderCou, 0);
					gotoxy(xp, 5 + soluong + 9 );
					cout << "Do you want to print list of students: ";
					gotoxy(xp, 5 + soluong + 10 );
					cout << "Y for Yes, N for No:  ";					
					cin >> c1;
					if ( c1 == 'Y' || c1 == 'y')
						listStuOfCou(pcurYear,orderSem,orderCou, 1);
				}	
			}
	}
}

void createCourseCSV(Year *&pCurYear, int orderSem){
	system("cls");
	Course *pHeadCou;
	string tmp;
	fstream FILE;
	FILE.open("ImportCourse.csv",ios::in);

	switch (orderSem){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
		}
	}

	Course *pCurCou = pHeadCou;
	while (pHeadCou != NULL && pCurCou->pNext != NULL)
		pCurCou = pCurCou->pNext;
	
	while (!(FILE.eof())){
		Course *pNew = new Course;
		getline(FILE, tmp,',');
        if (tmp == "\0")
            break;
        pNew -> IDCou = tmp;
		getline(FILE,pNew->nameCou,',');
		getline(FILE,tmp,',');
		pNew->credits = stoi(tmp); 
		tmp = "";
		getline(FILE,pNew->day1,',');
		getline(FILE,pNew->session1,',');
		getline(FILE,pNew->day2,',');
		getline(FILE,pNew->session2,',');
		getline(FILE,pNew->teacher,',');
		getline(FILE,tmp, '\n');
        pNew -> maxStu = stoi (tmp);
        pNew -> enrolling = 0;
        pNew -> pHeadInclasstu = NULL;
		tmp = "";
		if (pHeadCou == NULL)
			pHeadCou = pNew;
		if (pCurCou != NULL)
			pCurCou->pNext = pNew;
		pCurCou = pNew;
	}
		pCurCou->pNext = NULL;

	switch (orderSem){
		case 1:{
			pCurYear->Sem1.pHeadCou = pHeadCou;
			break;
		}
		case 2:{
			pCurYear->Sem2.pHeadCou = pHeadCou;
			break;
		}
		case 3:{
			pCurYear->Sem3.pHeadCou = pHeadCou;
		}
	}
	
//	pCurCou = pCurYear->Sem1.pHeadCou;
//	while (pCurCou != NULL){
//		cout << endl << pCurCou->IDCou << endl;
//		cout << pCurCou->nameCou << endl;
//		cout << pCurCou->credits << endl;
//		cout << pCurCou->day1 << endl;
//		cout << pCurCou->session1 << endl;
//		cout << pCurCou->day2 << endl;
//		cout << pCurCou->session2 << endl;
//		cout << pCurCou->teacher << endl;
//		cout << pCurCou->maxStu << endl;
//		pCurCou = pCurCou->pNext;
//	}	

	FILE.close();
}

void createCourse(Year *&pCurYear, int orderSem){
	Course *pHeadCou;
	switch (orderSem){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
		}
	}
	Course *pCurCou = pHeadCou;
	while (pHeadCou != NULL && pCurCou->pNext != NULL)
		pCurCou = pCurCou->pNext;
	
	Course *pNew = new Course;
	pNew->pHeadInclasstu= NULL;
	
	system ("cls");
	cout << "\n\n\n\n\n\n";
	cout << "        " << "Course ID:  ";
	cin.ignore();
	getline(cin,pNew->IDCou,'\n');
	cout << "\n\n        " << "Name course:  ";
	getline(cin,pNew->nameCou,'\n');	 
	cout << "\n\n        " << "Credit:  "; 
	cin >> pNew->credits;
	cout << "\n\n        " << "Day1:  "; 
	cin.ignore();
	getline(cin,pNew->day1,'\n');
	cout << "\n\n        " << "Session1:  "; 
	getline(cin,pNew->session1,'\n');
	cout << "\n\n        " << "Day2:  "; 
	getline(cin,pNew->day2,'\n');
	cout << "\n\n        " << "Session2:  "; 
	getline(cin,pNew->session2,'\n');
	cout << "\n\n        " << "Teacher:  "; 
	getline(cin,pNew->teacher,'\n');
	cout << "\n\n        " << "The max number of student:  "; 
	cin >> pNew->maxStu;
	cout << "\n\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";
	
	if ( pHeadCou == NULL)
		pHeadCou = pNew;
	else
		pCurCou->pNext = pNew;
	pCurCou = pNew;
	pCurCou->pNext = NULL;

	switch (orderSem){
		case 1:{
			pCurYear->Sem1.pHeadCou = pHeadCou;
			break;
		}
		case 2:{
			pCurYear->Sem2.pHeadCou = pHeadCou;
			break;
		}
		case 3:{
			pCurYear->Sem3.pHeadCou = pHeadCou;
		}
	}
							
	getch();	
}

void editCourse(Year *&pCurYear, int orderSem, int orderCou){
	system("cls");
	
	Course *pCurCou;
	switch (orderSem){
		case 1:{
			pCurCou = pCurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pCurCou = pCurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pCurCou = pCurYear->Sem3.pHeadCou;
		}
	}
	
	int t = 0;
	while (pCurCou != NULL && orderCou != t){
		pCurCou = pCurCou->pNext;
		t++;		
	}
	
	gotoxy(xp,2);
	cout << "Move arrow keys and enter to choose the element you want to edit";
	gotoxy(xp,5);
	cout << "Course ID: " << pCurCou->IDCou << endl;
	gotoxy(xp,6);
	cout << "Name course: " << pCurCou->nameCou << endl;
	gotoxy(xp,7);
	cout << "Credit: " << pCurCou->credits << endl;
	gotoxy(xp,8);
	cout << "Day 1: " << pCurCou->day1 << endl;
	gotoxy(xp,9);
	cout << "Session 1: " << pCurCou->session1 << endl;
	gotoxy(xp,10);
	cout << "Day 2: " << pCurCou->day2 << endl;
	gotoxy(xp,11);
	cout << "Session 2: " << pCurCou->session2 << endl;
	gotoxy(xp,12);
	cout << "Teacher: " << pCurCou->teacher << endl;
	gotoxy(xp,13);
	cout << "The max number of student: " << pCurCou->maxStu;	
	
	char c = '1'; 
	int orderInfo = 0;
	gotoxy(50,5);
	while (c != 13 ){
		c = getch();
		if (c == 72 && orderInfo > 0){ // len
			orderInfo --;
			gotoxy (50,5 + orderInfo);
		}
		else if (c == 80 && orderInfo < 8 ){ // xuong
			orderInfo ++;
			gotoxy (50,5 + orderInfo);	
		}	
	}
	
	gotoxy(xp,25);
	cout << "Do you want change to : ";
	gotoxy (32,25);
	fflush(stdin);
	switch(orderInfo){
		case 0:{
			getline(cin, pCurCou->IDCou, '\n');
			break;
		}
		case 1:{
			getline(cin, pCurCou->nameCou, '\n');
			break;
		}
		case 2:{
			cin >> pCurCou->credits;
			break;
		}
		case 3:{
			getline(cin, pCurCou->day1, '\n');
			break;
		}
		case 4:{
			getline(cin, pCurCou->session1, '\n');
			break;
		}
		case 5:{
			getline(cin, pCurCou->day2, '\n');
			break;
		}	
		case 6:{
			getline(cin, pCurCou->session2, '\n');
			break;
		}	
		case 7:{
			getline(cin, pCurCou->teacher, '\n');
			break;
		}		
		case 8:{
			cin >> pCurCou->maxStu;
		}		
	}
	
	gotoxy(xp,27);
	cout << "Edit successfully";
	getch();	
}

void deleteCourse(Year *&pCurYear, int orderSem, int orderCou){
	system("cls");
	
	Course *pHeadCou, *pRemove;
	switch (orderSem){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
		}
	}
	
	if (orderCou == 0){
		pRemove = pHeadCou;
		pHeadCou = pHeadCou->pNext;	
		switch (orderSem){
		case 1:{
			pCurYear->Sem1.pHeadCou = pHeadCou;
			break;
		}
		case 2:{
			pCurYear->Sem2.pHeadCou = pHeadCou;
			break;
		}
		case 3:
			pCurYear->Sem3.pHeadCou = pHeadCou;
		}	
	}
	else{
		Course *pCurCou = pHeadCou;
		int t = 1;
		while (pCurCou->pNext != NULL && t != orderCou){
			pCurCou = pCurCou->pNext;
			t++;		
		}
		pRemove = pCurCou->pNext;
		pCurCou->pNext = pCurCou->pNext->pNext;		
	}
	delete pRemove;
	
	gotoxy(xp,5);
	cout << "Delete successfully";
	getch();
}

void exportCourse(Year *pCurYear, int orderSem){
	fstream FILE;
	
	Course *pHeadCou;
	switch (orderSem){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			FILE.open("Sem1.csv",ios::out);
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			FILE.open("Sem2.csv",ios::out);
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
			FILE.open("Sem3.csv",ios::out);
		}
	}
		
	Course *pCurCou = pHeadCou;
	while (pCurCou != NULL){
		FILE << pCurCou->IDCou << ",";
		FILE << pCurCou->nameCou << ",";
		FILE << pCurCou->credits << ",";
		FILE << pCurCou->day1 << ",";
		FILE << pCurCou->session1 << ",";
		FILE << pCurCou->day2 << ",";
		FILE << pCurCou->session2 << ",";
		FILE << pCurCou->teacher << ",";
		FILE << pCurCou->maxStu  << ",";
		FILE << pCurCou->enrolling << "\n";
		
		Student *pCurStu = pCurCou->pHeadInclasstu;
		for ( int i = 1; i <= pCurCou->enrolling; i++){
			FILE << pCurStu->No << "," ;
			FILE << pCurStu->IDStu << "," ;
			FILE << pCurStu->firstname << "," ;
			FILE << pCurStu->lastname << "," ;
			FILE << pCurStu->date << "," ;
			FILE << pCurStu->IDSocial << "\n" ;
			pCurStu = pCurStu->pNext;		
		}
		pCurCou = pCurCou->pNext;
	}		
	FILE.close();
}

void listCourse(Year *pCurYear, int orderSem, int &soluong){
	system("cls");
	gotoxy(xp,5);
	cout << "NO";
	gotoxy(xp + 5,5);
	cout << "COURSE ID";
	gotoxy(xp + 17,5);
	cout << "NAME COURSE ";
	gotoxy(xp + 40,5);
	cout << "CREDIT ";
	gotoxy(xp + 49 ,5);
	cout << "SESSIONS ";
	gotoxy(xp + 73,5);
	cout << "TEACHER ";
	gotoxy(xp + 95,5);
	cout << "STUDENT ";
	
	Course *pHeadCou;
	switch (orderSem){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
		}
	}
		
	Course *pCurCou = pHeadCou;
	int i = 0;
	soluong = -1;
	while (pCurCou != NULL){
		soluong ++;
		gotoxy(xp,6 + i);
		cout << soluong + 1;
		gotoxy(xp + 5, 6 + i);
		cout << pCurCou->IDCou;
		gotoxy(xp + 17,6 + i);
		cout << pCurCou->nameCou;
		gotoxy(xp + 40,6 + i);
		cout << pCurCou->credits;
		gotoxy(xp + 49,6 + i);
		cout << pCurCou->day1 << " " << pCurCou->session1 << ", " << pCurCou->day2 << " " << pCurCou->session2 ;
		gotoxy(xp + 73,6 + i);
		cout << pCurCou->teacher;
		gotoxy(xp + 95,6 + i);
		cout << pCurCou->enrolling << " / " << pCurCou->maxStu ;	
		pCurCou = pCurCou->pNext;
		i ++;
	}
}

void createCourseRegister(Year *&pCurYear, int &orderSem){
	system("cls");
	if (orderSem == 1 || orderSem == 2 || orderSem == 3){
		cout << "\n\n\n\n\n\n";
		cout << "        " << "When does the course registration start? (DD/MM/YYYY) ";
		cin >> startDateRegister;
		cout << "\n\n        " << "When does the course registration end? (DD/MM/YYYY)   ";
		cin >> endDateRegister;
		cout << "\n\n        " << "You created successfully!";
		cout << "\n        " << "PRESS ENTER TO GO BACK...";
		getch();		
	}
	else{
		string tmp;
		fstream FILE;
		FILE.open("ExportCourse.csv", ios::in);
		getline(FILE,tmp,'\n');
		orderSem = (int)tmp[3] - 48;
		FILE.close();	
	}
	
	int size = 35;
	while (true){
		system("cls");
		
		printBox("Press A to create a course",xp,5,size);
		printBox("Press C to see list of courses",xp,8,size);
		printBox("Press O to log out",xp,11,size);	
	
		char c = getch();
		if ( c == 'A' || c == 'a'){
			int TMP;
			system("cls");
			gotoxy(xp,5);
			cout << "Please press 1 or 2 to import course(s) ";
			gotoxy(xp,6);
			cout << "1. Import one by one. ";
			gotoxy(xp,7);
			cout << "2. Import CSV file. ";
			gotoxy(xp,8);
			cin >> TMP;
			if ( TMP == 1)
				createCourse(pCurYear, orderSem);
			else
				createCourseCSV(pCurYear,orderSem);
		}
			
		else if ( c == 'C' || c == 'c'){
			char c1 = '1', c2 = '1';
			while ( c1 != 'B' && c1 != 'b' && c2 != 'B' && c2 != 'b'){
				listCourse(pCurYear,orderSem,soluong);
				gotoxy(xp,5 + soluong + 5);
				cout << "Move arrow keys and enter to choose a course, then ...";
				printBox("Press E to edit the course",xp,5 + soluong + 8,size);
				printBox("Press D to delete the course",xp,5 + soluong + 11,size);
				printBox("Press B to back",xp,5 + soluong + 14,size);	
				
				c2 = '1'; 
				int orderCou = 0;
				gotoxy(3,6);
				while (c2 != 13 && c2 != 'B' && c2 != 'b'){
					c2 = getch();
					if (c2 == 72 && orderCou > 0){ // len
						orderCou --;
						gotoxy (3,6 + orderCou);
					}
					else if (c2 == 80 && orderCou < soluong ){ // xuong
						orderCou ++;
						gotoxy (3,6 + orderCou);	
					}	
				}
				if (c2 == 'B' || c2 == 'b')
					break;
				if (c2 == 13)
					c1 = getch();
				if ( c1 == 'E' || c1 == 'e')
					editCourse(pCurYear,orderSem,orderCou);
				else if (c1 == 'D' || c1 == 'd')
					deleteCourse(pCurYear,orderSem,orderCou);
			}
	}
	else if ( c == 'O' || c == 'o'){
		exportCourse(pCurYear, orderSem);
		logOut = true;
		return;
		}
	}
}

//Hàm dùng in danh sách các khóa học ra console cho học sinh chọn
//Sau khi chọn xong dùng lại hàm này sẽ cập nhập số lượng học sinh đang đăng ký
//khóa học
void PrintCourse (Year *pCurYear, int orderSem){
    Course *pHeadCou;
    switch (orderSem){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
            break;
		}
	}
    Course *pCurCou = pHeadCou;
    int count = 1;
    string tmp;
    cout.width(8);
    cout << left << "No"; cout.width (10);
    cout <<  "ID" ; cout.width(25);
    cout << "Name course" ; cout.width(10);
    cout << "Credit" ; cout.width(10);
    cout << "Day 1" ; cout.width(15); 
    cout << "Session 1" ; cout.width(10);
    cout << "Day 2" ; cout.width(15);
    cout << "Session 2"; cout.width(20);
    cout << "Teacher" ; cout.width(10);
    cout << "Student" << endl; 
	while (pCurCou != NULL){
        cout.width(8);
        cout << left << count;  cout.width(10);
		cout <<  pCurCou->IDCou; cout.width(25);
		cout << pCurCou->nameCou ; cout.width(10);
		cout << pCurCou->credits ; cout.width(10);
		cout << pCurCou->day1 ; cout.width(15);
		cout << pCurCou->session1 ; cout.width(10);
		cout << pCurCou->day2 ; cout.width(15);
		cout << pCurCou->session2 ; cout.width(20);
		cout << pCurCou->teacher; cout.width(10);
        tmp = to_string(pCurCou -> enrolling) + "/" + to_string(pCurCou->maxStu);
		cout << tmp << endl;
		pCurCou = pCurCou->pNext;
        count ++;
	}	
}

//hàm dùng để check xem lịch của môn đang đăng ký có bị trùng với lịch của các
//môn đã đăng ký không 
bool CheckScheduleCou (Student *curStu, Course *curEnroll, Course *pHead){
    string *Enrolled = new string [5];
    int count = 0, j = 0;
    int length = strlen (pHead -> IDCou.c_str());
    string tmp = curStu -> course.substr(j, length);
    if (tmp == "\0") 
        return false;
    while (tmp != "\0"){
        Enrolled[count] = tmp;
        if (tmp == curEnroll -> IDCou)
            return true;
        j += length;
        tmp = curStu -> course.substr(j, length);
        count ++;
    }
    Course *pcur; 
    for (int i = 0; i < count; i++){
        pcur = pHead;
        while (pcur != nullptr && pcur -> IDCou != Enrolled[i])
            pcur = pcur -> pNext;
        if (pcur -> day1 == curEnroll -> day1 && pcur -> session1 == curEnroll -> session1) 
            return true;
        else if (pcur -> day1 == curEnroll -> day2 && pcur -> session1 == curEnroll -> session2)
            return true;
        else if (pcur -> day2 == curEnroll -> day1 && pcur -> session2 == curEnroll -> session1)
            return true;
        else if (pcur -> day2 == curEnroll -> day2 && pcur -> session2 == curEnroll -> session2)
            return true;
    }
    return false;
}

// hàm dùng để lấy nhận từ bàn phím số No của course và trả về course
void RegisterCou(Year *pcurYear, int orderSem, Course *pHead, Student *curStu){
    int maxCourse = 0;
    Course *pcur = pHead;
    while (pcur != nullptr){
        maxCourse++;
        pcur = pcur -> pNext;
    }  
    int no = 0;
    while (no <= 0 || no > maxCourse){
        system ("clear");
        PrintCourse (pcurYear, orderSem);
        cout << "Enter the No of the course: ";
        cin >> no;
    }
    int count = 1; 
    pcur = pHead;
    while (count != no){
        pcur = pcur -> pNext;
        count ++;
    }
    if (pcur -> enrolling >= pcur -> maxStu || CheckScheduleCou (curStu, pcur, pHead) == true)
        cout << "You can not enroll this course" << endl;
    else {
        system ("clear");
        curStu -> course += pcur -> IDCou;
        pcur -> enrolling += 1;
        PrintCourse (pcurYear, orderSem);
        cout << "You enroll this course sucessfully";
    } 
} 

void Runtest(Year *pcurYear, int orderSem, Student *curStu){
    Course *pHeadCou;
    switch (orderSem){
		case 1:{
			pHeadCou = pcurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pcurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pcurYear->Sem3.pHeadCou;
            break;
		}
	}
    RegisterCou (pcurYear, orderSem, pHeadCou, curStu); 
}

void UpdateData (Year *pCurYear, int semester, bool yearCreated){
    if (yearCreated == false)
        return;
    Course *pHeadCou;
    switch (semester){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
            break;
		}
	} 
    Course *pCurCou = pHeadCou;
    while (pCurCou != nullptr){
        pCurCou -> Stu = new Student *[pCurCou -> enrolling];
        pCurCou = pCurCou -> pNext;
    }
    *pCurCou = pHeadCou;
    Class *pCurClass = pCurYear -> pHeadClass;
    Student *pCurStu;
    int k;
    int lenth = strlen (pCurCou -> IDCou.c_str());
    while (pCurClass != nullptr){
        pCurStu = pCurClass -> pHeadStu;
        int maxCou = strlen (pCurStu -> IDCou.c_str()) / length;
        for (int i = 0; i < pCurClass -> numberOfStu; i++){
            pCurStu -> Inclass = new Score [maxCou];
            for (int j = 0; j < maxCou; j++){
                for (k  = 0; k < pCurCou -> enrolling; k++)
                    if (pCurCou -> Stu[k] == '\0')
                       break;
                Stu[k] = pCurStu;
            }
            pCurStu = pCurStu -> pNext;
        }
        pCurClass = pCurClass -> pNext;
    }             
}

void createSemester(Year *&pcurYear, int &orderSem){
	system("cls");
	cout << "\n\n\n\n\n\n";
	cout << "        " << "Which sem do you want to create? ";
	cout << "\n        " << "1 for Sem 1";
	cout << "\n        " << "2 for Sem 2";
	cout << "\n        " << "3 for Sem 3";
	cout << "\n\n        ";
	cin >> orderSem;
	
	cout << "\n\n        " << "When does the semester start? ";
	cin.ignore();
	switch (orderSem){
		case 1:{
			cin >> pcurYear->Sem1.startDate;
			break;
		};
		case 2:{
			cin >> pcurYear->Sem2.startDate;
			break;
		}
		case 3:
			cin >> pcurYear->Sem3.startDate;
	}
	cout << "\n\n        " << "When does the semester end? ";
	cin.ignore();
	switch (orderSem){
		case 1:{
			cin >> pcurYear->Sem1.endDate;
			break;
		};
		case 2:{
			cin >> pcurYear->Sem2.endDate;
			break;
		}
		case 3:
			cin >> pcurYear->Sem3.endDate;
	}
	
	cout << "\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";
	getch();	
}

Class* CreateClass(Class*& phead, string tmp, Year*& curyear){
    if (phead == nullptr) {
        phead = new Class;
        phead->className = tmp;
        phead->pNext = nullptr;
        curyear->pHeadClass = phead;
        return phead;
    }
    Class* pcur = phead;
    while (pcur->pNext != nullptr)
        pcur = pcur->pNext;
    pcur->pNext = new Class;
    pcur->pNext->className = tmp;
    pcur->pNext->pNext = nullptr;
    return pcur -> pNext;
}

void ImportNewStu (string filename,  Class *curClass){
    ifstream ifile (filename.c_str());
    string tmp;
    getline (ifile, tmp, ',');
    if (tmp == "\0"){
        return;
    }
    Student *pcur;
    curClass -> numberOfStu = 0;
    while (!ifile.eof()){
        if (tmp != "\0"){
            curClass -> numberOfStu += 1;
            if (curClass -> numberOfStu != 1) {
                pcur -> pNext = new Student;
                pcur = pcur -> pNext;
            }
            else {
                pcur = new Student;
                curClass -> pHeadStu = pcur;
            }
            pcur -> No = stoi (tmp); 
            getline (ifile, tmp, ',');
            pcur -> IDStu = tmp;
            getline (ifile, tmp,',');
            pcur -> firstname = tmp.erase(0,1);
            getline (ifile, tmp,',');
            pcur -> lastname = tmp.erase(0,1);
            getline (ifile, tmp,',');
            pcur -> gender = tmp.erase(0,1);
            getline (ifile, tmp,',');
            pcur -> date = tmp.erase(0,1);
            getline (ifile, tmp,'\n');
            pcur -> IDSocial = stoi (tmp.c_str());
            pcur -> pNext = nullptr;
            getline (ifile,tmp, ',');
        }
        else break;
    }
    ifile.close();
}

void ImportOldStu(string filename, Class*& cHead) {
    ifstream ifile(filename.c_str());
    string tmp;
    Class *curClass = new Class;
    getline(ifile, tmp, ',');
    if (tmp == "\0") {
        return;
    }
    while (!ifile.eof()) {
            if (tmp != "\0") {
                int j = 0;
                if (j != 0) {
                    curClass->pNext = new Class;
                    curClass = curClass->pNext;
                }
                else curClass = cHead;
                curClass->className = tmp;
                getline(ifile, tmp, '\n');
                curClass->numberOfStu = stoi(tmp.c_str());
                for (int i = curClass->numberOfStu; i > 0; i--) {
                    Student* pcur = new Student;
                    if (i != curClass->numberOfStu) {
                        pcur->pNext = new Student;
                        pcur = pcur->pNext;
                    }
                    else curClass->pHeadStu = pcur;
                    pcur->No = stoi(tmp);
                    getline(ifile, tmp, ',');
                    pcur->IDStu = tmp;
                    getline(ifile, tmp, ',');
                    pcur->firstname = tmp.erase(0, 1);
                    getline(ifile, tmp, ',');
                    pcur->lastname = tmp.erase(0, 1);
                    getline(ifile, tmp, ',');
                    pcur->gender = tmp.erase(0, 1);
                    getline(ifile, tmp, ',');
                    pcur->date = tmp.erase(0, 1);
                    getline(ifile, tmp, '\n');
                    pcur->IDSocial = stoi(tmp.c_str());
                    pcur->pNext = nullptr;
                    getline(ifile, tmp, ',');
                }
                j++;
            }
            else break;
        }
    ifile.close();
}

void ImportClasses (Class *&pheadClass, Year *&pcurYear){
	system("cls");
    int n;
    cout << "How many files are you going to import? ";
    cin >> n;
    cin.ignore(100,'\n');
    string filename, classname, compare;
    bool outloop = false;
    for (int i = 0; i < n; i++){
        while (outloop == false){
            cout << "Enter class name: ";
            getline (cin, classname, '\n');
            cout << "Enter file name to input class: ";
            getline (cin, filename, '\n');
            compare = filename.substr (0, filename.find(".csv"));
            if (strcmp (classname.c_str(), compare.c_str()) == 0)
                break; 
            else {
                cout << "Your filename and class name is not equal" << endl;
                cout << "Do you wish to continue? (Y or y to continue, N or n to set input again" << endl;
                char tmp;
                cin >> tmp;
                if (tmp == 'y' || tmp == 'Y')
                    outloop = true;
            }  
        }
        Class *pcurClass = CreateClass (pheadClass, classname, pcurYear);
        ImportNewStu(filename, pcurClass);
    } 
}

void OutPutStu (Class *pheadClass){
    Class *pcurClass = pheadClass;
    while (pcurClass != nullptr){
        Student *pcurStu = pcurClass -> pHeadStu;
        cout << "Class: " << pcurClass -> className << endl;
        cout << "Number of student: " << pcurClass -> numberOfStu << endl;
        while (pcurStu != nullptr){
            cout << "No: " << pcurStu -> No << endl;
            cout << "ID: " << pcurStu -> IDStu << endl;
            cout << "First name: " << pcurStu -> firstname << endl;
            cout << "Last name: " << pcurStu -> lastname << endl;
            cout << "Gender: " << pcurStu -> gender << endl;
            cout << "Date of birth: " << pcurStu -> date << endl;
            cout << "ID Social: " << pcurStu -> IDSocial << endl;
            cout << endl;
            pcurStu = pcurStu -> pNext;
        }
        pcurClass = pcurClass -> pNext;
        cout << endl;
    }
}

void createYear(Year *&pcurYear){	
	system ("cls");
	if (pcurYear == NULL)
		pcurYear = new Year;
	pcurYear->pHeadClass = NULL;
	pcurYear->Sem1.pHeadCou = NULL;
	pcurYear->Sem2.pHeadCou = NULL;
	pcurYear->Sem3.pHeadCou = NULL;
	fstream FILE;
	FILE.open("Sem1.csv",ios::out);
	FILE.close();
	FILE.open("Sem2.csv",ios::out);
	FILE.close();
	FILE.open("Sem3.csv",ios::out);
	FILE.close();
	
	cout << "\n\n\n\n\n\n";
	cout << "        " << "When does the school year start?  ";
	cin >> pcurYear->start;
	cout << "\n\n        " << "When does the school year start?  "; 
	cin >> pcurYear->end;
	cout << "\n\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";
	
	FILE.open ("Year.txt",ios::out);
	FILE << pcurYear->start << " " << pcurYear->end;
	FILE.close();
	
	getch();
}

void staffCreate(Year *&pcurYear, int &orderSem){
	while (!logOut){
		system("cls");
		gotoxy(xp,3);
		cout << "At the beginning of school year...";
		printBox("Press 1 to create a school year",xp,5,50);
		printBox("Press 2 to create a class",xp,8,50);
		printBox("Press 3 to create a semester",xp,11,50);
		printBox("Press 4 to create a course registration session",xp,14,50);
		printBox("Press O to log out",xp,17,50);
		printBox("Press Enter to do other operations",xp,20,50);
		
		char c = getch();
		if (c == 'o' || c == 'O'){
			logOut = true;
			return;
		}
	else if ( c == '1')
			createYear(pcurYear);
	else if ( c == '2')
		    ImportClasses(pcurYear->pHeadClass, pcurYear);		
	else if ( c == '3')
			createSemester(pcurYear,orderSem);
	else if ( c == '4')
			createCourseRegister(pcurYear, orderSem);	
	else if ( c == 13)
			return;
	}
}

void changePass(string username){
	system("cls");
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

void viewProfile(Student *pStu){
	int size = 13;
	system("cls");
	cout << "\n\n\n\n";
	cout << "    " << char (218);
	for ( int j = 1; j <= size; j++)
		cout << char(196);
	cout << char (191) << "\n    ";
	for ( int j = 1; j <= 8; j++){
		cout << char (179);
		if ( j == 4)
			cout << "  photo 3x4  ";
		else 
			cout << "             ";
		cout << char (179) << "\n    ";	
	}
	cout << char (192);
	for ( int j = 1; j <= size; j++)
		cout << char(196);
	cout << char (217);

	cout << "\n\n";
	cout << "     Fullname: " << pStu->lastname << " " << pStu->firstname << "\n\n";
	cout << "     ID: " << pStu->IDStu << "\n\n";
	cout << "     Social ID: " << "\n\n";
	cout << "     Gender: " << "\n\n";
	cout << "     Date of birth: " << "\n\n";
	cout << "\n\n\n\n";
	cout << "     PRESS ENTER TO GO BACK...";
	getch();
}

void homePage(Student *pStu, string username){
	while (true){
		system("cls");
		if ('2' <= username[0] && username[0] <= '9')
			printBox("Press V to view profile",xp,5,35);
		printBox("Press C to change password",xp,8,35);
		printBox("Press Enter to continue",xp,11,35);
		printBox("Press O to log out",xp,14,35);
		

		char c = getch();
		if ( c == 'C' || c == 'c')
		changePass(username);		
	else if ( c == 'V' || c == 'v' ) 
		viewProfile(pStu);
	else if ( c == 'O' || c == 'o'){
		logOut = true;
		return;
		}
	else if ( c == 13)
		return;
	}
}

int checkTime(){
	system("cls");
	gotoxy(20,xp);
	cout << "I want to know when you logged in system";
	gotoxy(20,xp+1);
	cout << "1 for at beginning the semester";
	gotoxy(20,xp+2);
	cout << "2 for the course registration is active";
	gotoxy(20,xp+3);
	cout << "3 for at the end of the semester";
	gotoxy(20,xp+5);
	int t;
	cin >> t;
	return t;
}

void importData(Year *&pcurYear){
	fstream FILE;
	FILE.open("Year.txt",ios::in);
	int start_year, end_year;
	FILE >> start_year >> end_year;
	FILE.close();
	if (start_year && end_year){
		pcurYear = new Year;
		pcurYear->start = start_year;
		pcurYear->end = end_year;
		
		pcurYear->pHeadClass = NULL; // import class

		importCourse("Sem1.csv", pcurYear, 1);
		importCourse("Sem2.csv", pcurYear, 2);
		importCourse("Sem3.csv", pcurYear, 3);
	}
	else
		pcurYear = NULL;	
}

void box(int x, int y, int w, int h){
	for(int i = x; i <= x + w; i++){
		gotoxy(i, y);
		cout << char(196);
		gotoxy(i, y + h);
		cout << char(196);
	}
	for(int i = y; i <= y + h; i++){
		gotoxy(x, i);
		cout << char(179);
		gotoxy(x + w, i);
		cout << char(179);
	}
	gotoxy(x, y); cout << char(218);
	gotoxy(x, y + h); cout << char(192);
	gotoxy(x + w, y); cout << char(191);
	gotoxy(x + w, y + h); cout << char(217);
}

bool loginCheck(string username, string password){
	ifstream fin;
	if(username[0] >= '0' && username[0] <= '9'){
		fin.open("loginStu.txt");
		string userCheck, passCheck;
		while(!fin.eof()){
			fin >> userCheck >> passCheck;
			if(username == userCheck && password == passCheck)
				return true;
		}
		return false;
	}
	else if(username[0] == 's'){
			fin.open("loginStaff.txt");
			string userCheck, passCheck;
			while(!fin.eof()){
				fin >> userCheck >> passCheck;
				if(username == userCheck && password == passCheck)
					return true;
			}
			return false;
		 }
		 else return false;
}

string loginSystem(){
	system("cls");
	string username, password;
	gotoxy(41,11);
	cout << "USERNAME:";
	box(50,10,25,2);
	gotoxy(41,14);
	cout << "PASSWORD:";
	box(50,13,25,2);
	int again = 0;
	while(again < 5){
		gotoxy(52,11);
		cin >> username;
		gotoxy(52,14);
		cin >> password;
		if(loginCheck(username,password)){
			return username;
		}
		else{
			again++;
			gotoxy(52,11);
			for(int i = 1; i <= username.length(); i++)
				cout << " ";
			gotoxy(52,14);
			for(int i = 1; i <= password.length(); i++)
				cout << " ";
			gotoxy(48,17);
			cout << "INVALID USERNAME OR PASSWORD. PLEASE TRY AGAIN.";	
		}
	}
	username = "invalid";
	return username;
}

void gotoxy(int x, int y){
    COORD c;
    c.X = x - 1;
    c.Y = y - 1;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
} 

void printBox(string text, int x, int y, int size){	
	gotoxy(x,y);
	cout << char (218);
	for ( int j = 1; j <= size; j++)
		cout << char(196);
	cout << char (191);
	
	gotoxy(x,y+1);
	cout << char (179);		
	gotoxy(x + size/2 + 1 - text.length()/2, y+1 );
	cout << text;
	gotoxy(x+size + 1,y+1);
	cout << char (179);
		
	gotoxy(x,y+2);
	cout << char (192);
	for ( int j = 1; j <= size; j++)
		cout << char(196);
	cout << char (217);		
}
