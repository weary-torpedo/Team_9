#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <windows.h>
using namespace std;

#define xp 8

struct Student{
	int No, IDSocial;
	string firstname, lastname,gender, date, IDStu;
	float Final;
	float Mid;
	float Other;
	float Total;
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
	string session1, session2;
	Student* pHeadInclasstu;
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

Year *pheadYear = NULL, *pCurYear; 
string startDateRegister, endDateRegister;
bool logOut;

void gotoxy(int x, int y) 
{
    COORD c;
    c.X = x - 1;
    c.Y = y - 1;
    SetConsoleCursorPosition (GetStdHandle(STD_OUTPUT_HANDLE), c);
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

	FILE.close();
	getch();
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
	FILE.open("ExportCourse.csv",ios::out);

	Course *pHeadCou;
	switch (orderSem){
		case 1:{
			pHeadCou = pCurYear->Sem1.pHeadCou;
			FILE << "Sem1\n";
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			FILE << "Sem2\n";
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
			FILE << "Sem3\n";
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
		pCurCou = pCurCou->pNext;
	}		

	FILE.close();
}

void listCourse(Year *&pCurYear, int orderSem, int &soluong){
	system("cls");
	gotoxy(xp,5);
	cout << "COURSE ID";
	gotoxy(xp + 12,5);
	cout << "NAME COURSE ";
	gotoxy(xp + 35,5);
	cout << "CREDIT ";
	gotoxy(xp + 44 ,5);
	cout << "SESSIONS ";
	gotoxy(xp + 68,5);
	cout << "TEACHER ";
	gotoxy(xp + 90,5);
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
	while (pCurCou != NULL){
		soluong ++;
		gotoxy(xp,6 + i);
		cout << pCurCou->IDCou;
		gotoxy(xp + 12,6 + i);
		cout << pCurCou->nameCou;
		gotoxy(xp + 35,6 + i);
		cout << pCurCou->credits;
		gotoxy(xp + 44,6 + i);
		cout << pCurCou->day1 << " " << pCurCou->session1 << ", " << pCurCou->day2 << " " << pCurCou->session2 ;
		gotoxy(xp + 68,6 + i);
		cout << pCurCou->teacher;
		gotoxy(xp + 90,6 + i);
		cout << pCurCou->enrolling << " / " << pCurCou->maxStu ;	
		pCurCou = pCurCou->pNext;
		i ++;
	}
}

void createCourseRegister(Year *&pCurYear, int &orderSem){
	system("cls");
	if (orderSem == 1 && orderSem == 2 && orderSem == 3){
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
				int soluong = -1;
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

void createSemester(Year *&pCurYear, int &orderSem){
	system("cls");
	cout << "\n\n\n\n\n\n";
	cout << "        " << "Which sem do you want to create? ";
	cout << "\n        " << "1 for Sem 1";
	cout << "\n        " << "2 for Sem 2";
	cout << "\n        " << "3 for Sem 3";
	cout << "\n\n        ";
	cin >> orderSem;
	
	switch (orderSem){
		case 1:{
			pCurYear->Sem1.pHeadCou = NULL;
			break;
		};
		case 2:{
			pCurYear->Sem2.pHeadCou = NULL;
			break;
		}
		case 3:
			pCurYear->Sem3.pHeadCou = NULL;
	}

	cout << "\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";
	getch();	
}

void createClass(Year *&pCurYear){
}

void createYear(Year *&pCurYear){
}

void staffCreate(Year *&pYear, Year *&pCurYear){
	pCurYear = pYear;
	int orderSem;
	while (!logOut){
		system("cls");
		printBox("Press 1 to create a school year",xp,5,50);
		printBox("Press 2 to create a class",xp,8,50);
		printBox("Press 3 to create a semester",xp,11,50);
		printBox("Press 4 to create a course registration session",xp,14,50);
		printBox("Press O to log out",xp,17,50);
		
		char c = getch();
		if (c == 'o' || c == 'O'){
			logOut = true;
			return;
		}
	else if ( c == '1')
			createYear(pCurYear);
	else if ( c == '2')
			createClass(pCurYear);
	else if ( c == '3')
			createSemester(pCurYear,orderSem);
	else if ( c == '4')
			createCourseRegister(pCurYear, orderSem);		
	}
}

void changePassword(){	
}

void viewProfile(Student *pStu){
	int size = 13;
	system("cls");
	cout << "\n\n\n\n";
	cout << "    " << char (218);
	for ( int j = 1; j <= size; j++)
		cout << char(196);
	cout << char (191) << "\n    ";
	for ( int j = 1; j <= 8; j++)
	{
		cout << char (179);
		if ( j == 4)
		{
			cout << "  photo 3x4  ";
		}
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
		printBox("Press O to log out",xp,11,35);
		printBox("Press Enter to do other operations",xp,14,35);

		char c = getch();
		if ( c == 'C' || c == 'c')
		changePassword();		
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

int main(){	
	Student *pStu = new Student; // get info
	pStu->IDStu = "21127637";
	pStu->lastname = "Phan";
	pStu->firstname = "Linh";
	
	Year *pheadYear = new Year;
	pheadYear->start=2021;
	pheadYear->end = 2022;	
	
	do{
		logOut = false;
		gotoxy(20,20);
		string username =  loginSystem();
		homePage(pStu, username);
		if (!logOut)
			if (!('2' <= username[0] && username[0] <= '9'))
				staffCreate(pheadYear, pCurYear);
			// else student
			
	}
	while (logOut);
	
	gotoxy(30,30);
	return 0;
}
