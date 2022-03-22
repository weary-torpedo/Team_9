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
	int maxStu;
	string day;
	string session;
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

void printBox(string text, int x, int y, int size)
{	
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

void createCourseRegister(Year *&pCurYear, int n)
{
	system("cls");
	cout << "\n\n\n\n\n\n";
	cout << "        " << "When does the course registration start? (DD/MM/YYYY) ";
	cin >> startDateRegister;
	cout << "\n\n        " << "When does the course registration end? (DD/MM/YYYY)   ";
	cin >> endDateRegister;
	cout << "\n\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";
	getch();
	
	int size = 35;
	
//	while (true)
//	{
//		system("cls");
//		
//		printBox("Press A to create a course",xp,5,size);
//		printBox("Press C to see list of courses",xp,8,size);
//		printBox("Press O to log out",xp,11,size);	
//	
//		char c = getch();
//		if ( c == 'A' || c == 'a')
//			createCourse(pCurYear,n);
//	else if ( c == 'C' || c == 'c')
//			listCourse(pCurYear,n);
//	 if ( c == 'O' || c == 'o')
//			exit(0);
//	}
}

void staffCreate(Year *&pYear, Year *&pCurYear)
{
	pCurYear = pYear;
	int n;
	while (true)
	{
		system("cls");
		printBox("Press 1 to create a school year",xp,5,50);
		printBox("Press 2 to create a class",xp,8,50);
		printBox("Press 3 to create a semester",xp,11,50);
		printBox("Press 4 to create a course registration session",xp,14,50);
		printBox("Press O to log out",xp,17,50);
		
		char c = getch();
		if (c == 'o' || c == 'O')
			exit(0);
//	else if ( c == '1')
//			createYear(pCurYear);
//	else if ( c == '2')
//			createClass(pCurYear);
//	else if ( c == '3')
//		createSemester(pCurYear,n);
	else if ( c == '4')
			createCourseRegister(pCurYear, n);		
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

void homePage(Student *pStu){
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
