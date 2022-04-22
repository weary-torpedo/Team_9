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
    string CouName;
    string CouID;
    int semester;
    float Mid;
    float Final;
	float Total;
    float Other;
    float GPA;
    Score *pNext;

};

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
	Student** Stu;
    int semester;
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
bool logOut, isRegister, exitProgram, hasScore;
int orderSem, soluong;

void PrintCourse (Year *pCurYear, int semester);
void RegisterCou(Year *&pcurYear, int sem, Course *pHead, Student *curStu);
bool CheckScheduleCou (Student *curStu, Course *curEnroll, Course *pHead);
void Runtest (Year *pcurYear, int sem, Student *curStu);
//hàm update data được dùng để liên kết các dữ liệu lại
//dùng khi mới kết thúc đăng ký học phần và mới khởi động lại chương trình
//biến bool createdScore dùng để biết là có tạo điểm hay không, nếu là kết thúc
//đăng ký học phần thì biến này là true còn không thì là false
void UpdateData (Year *&pCurYear, int semester, bool yearCreated, bool createScore);
// Hàm export data dùng để lưu lại các data hiện có khi thoát chương trình
// Hàm cần truyền vào biến bool Course Register kết thúc chưa
void ExportData (Year *&curYear, bool couRegistEnd);
void readData (Year *&pcurYear, bool hasScore);

void staffScore(Year* pcurYear, int orderSem);

void staffSee(Year *&pcurYear, int orderSem);

void createCourseCSV(Year *&pCurYear, int orderSem);
void createCourse(Year *&pCurYear, int orderSem);
void editCourse(Year *&pCurYear, int orderSem, int orderCou);
void deleteCourse(Year *&pCurYear, int orderSem, int orderCou);
void exportCourse(Year *pCurYear, int orderSem);
void listCourse(Year *pCurYear, int orderSem, int &soluong);
void createCourseRegister(Year *&pCurYear, int &orderSem);
void createSemester(Year *&pCurYear, int &orderSem);
Class* CreateClass(Class*& phead, string tmp, Year*& curyear);
void ImportNewStu (string filename,  Class *curClass);
// hàm importOldStu import 1 file data.
void ImportOldStu(string filename, Class*& cHead);
// Hàm import old data import 3 file data dùng khi year mới được tạo
void ImportOldData (Year *&curYear);
void ImportClasses (Class *&pheadClass, Year *&pcurYear);
void OutPutStu (Class *pheadClass);
void createYear(Year *&pcurYear);
void staffCreate(Year *&pcurYear, int &orderSem);

int checkTime();
void changePass(string username);
void viewProfile(Student *pStu);
void homePage(Student *pStu, string username);

void gotoxy(int x, int y);
void printBox(string text, int x, int y, int size);
void box(int x, int y, int w, int h);
bool loginCheck(string username, string password);
Student* afterLog(string username, Year* pcurYear);
string loginSystem();

int main(){
	Year *pcurYear;
	readData(pcurYear,0);
	exitProgram = false;
	do{
		logOut = false;
		gotoxy(20,20);
		string username =  loginSystem();
		Student* pStudent;
		if ('2' <= username[0] && username[0] <= '9')
			pStudent = afterLog(username,pcurYear);
		homePage(pStudent, username);
		int time;
		
		if (!logOut && !exitProgram && !('2' <= username[0] && username[0] <= '9'))
			time = checkTime();
		
		if (!logOut && !exitProgram && !('2' <= username[0] && username[0] <= '9') && time == 1)
				staffCreate(pcurYear, orderSem);
		if (!logOut && !exitProgram && !('2' <= username[0] && username[0] <= '9') && time == 2)
				staffSee(pcurYear, orderSem);
		if (!logOut && !exitProgram && !('2' <= username[0] && username[0] <= '9') && time == 3)
				staffScore(pcurYear, orderSem);	
		if (!logOut && !exitProgram && ('2' <= username[0] && username[0] <= '9') && isRegister)
				Runtest(pcurYear, orderSem, pStudent);
	}
	while (logOut && !exitProgram);
	//ExportData(pcurYear, hasScore);
	
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
	
	for ( int i = 0; i < pCurCou->enrolling; i++){
		gotoxy(xp,6+i);
		cout << pCurCou->Stu[i]->No;
		gotoxy(xp + 5,6+i);
		cout << pCurCou->Stu[i]->IDStu;
		gotoxy(xp + 17,6+i);
		cout << pCurCou->Stu[i]->firstname << " " << pCurCou->Stu[i]->lastname;
		gotoxy(xp + 40,6+i);
		cout << pCurCou->Stu[i]->date;
		gotoxy(xp + 55,6+i);
		cout << pCurCou->Stu[i]->IDSocial;		
	}
	
	// if (print){
	// 	string tmp = pCurCou->IDCou + ".csv";
	// 	ofstream fout;
	// 	fout.open(tmp);	
	// 	fout << "BANG DIEM MON " << pCurCou->nameCou << endl;
	// 	fout << "NO,ID,NAME,MIDTERM MARK,FINAL MARK,TOTAL MARK,OTHER MARK" << endl;
	// 	for ( int i = 0; i < pCurCou->enrolling; i++){
	// 		Score* pCur = pCurCou->Stu[i]->Inclass;
	// 		while(pCur->CouID != pCurCou->IDCou)
	// 			pCur = pCur->pNext;
	// 		fout << i + 1 << ",";
	// 		fout << pCurCou->Stu[i]->IDStu << ",";
	// 		fout << pCurCou->Stu[i]->lastname << " ";
	// 		fout << pCurCou->Stu[i]->firstname << ",";
	// 		if(pCur->Mid != -1)
	// 			fout << pCur->Mid << ",";
	// 		else 
	// 			fout << "-" << ",";
	// 		if(pCur->Final != -1)
	// 			fout << pCur->Final << ",";
	// 		else 
	// 			fout << "-" << ",";
	// 		if(pCur->Total != -1)
	// 			fout << pCur->Total << ",";
	// 		else 
	// 			fout << "-" << ",";
	// 		if(pCur->Other != -1)
	// 			fout << pCur->Other << ",";
	// 		else 
	// 			fout << "-" << ",";
	// 		fout << endl;
	// 	}
	// 	fout.close();
	// 	gotoxy(xp, 5 + soluong + 12 );
	// 	cout << "You printed successfully!";
	// 	gotoxy(xp, 5 + soluong + 13 );
	// 	cout << "PRESS ENTER TO GO BACK...";
	// 	getch();
	// }
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

void exportCourseToTeacher(Year* pcurYear, int orderSem, int orderCou){
	Course* pCurCou;
	switch(orderSem){
		case 1:
			pCurCou = pcurYear->Sem1.pHeadCou;
			break;
		case 2:
			pCurCou = pcurYear->Sem2.pHeadCou;
			break;
		case 3:
			pCurCou = pcurYear->Sem3.pHeadCou;
			break;
		
	}
	for(int i = 1; i < orderCou; i++){
		pCurCou = pCurCou->pNext;
	}

	string tmp = pCurCou->IDCou + ".csv";
	ofstream fout;
	fout.open(tmp.c_str());	
	fout << "BANG DIEM MON " << pCurCou->nameCou << endl;
	fout << "NO,ID,NAME,MIDTERM MARK,FINAL MARK,TOTAL MARK,OTHER MARK" << endl;
	for ( int i = 0; i < pCurCou->enrolling; i++){
		Score* pCur = pCurCou->Stu[i]->Inclass;
		while(pCur->CouID != pCurCou->IDCou)
			pCur = pCur->pNext;
		fout << i + 1 << ",";
		fout << pCurCou->Stu[i]->IDStu << ",";
		fout << pCurCou->Stu[i]->lastname << " ";
		fout << pCurCou->Stu[i]->firstname << ",";
		if(pCur->Mid != -1)
			fout << pCur->Mid << ",";
		else 
			fout << "-" << ",";
		if(pCur->Final != -1)
			fout << pCur->Final << ",";
		else 
			fout << "-" << ",";
		if(pCur->Total != -1)
			fout << pCur->Total << ",";
		else 
			fout << "-" << ",";
		if(pCur->Other != -1)
			fout << pCur->Other << ",";
		else 
			fout << "-" << ",";
		fout << endl;
	}
	fout.close();
	gotoxy(xp, 5 + soluong + 12 );
	cout << "You printed successfully!";
	gotoxy(xp, 5 + soluong + 13 );
	cout << "PRESS ENTER TO GO BACK...";
	getch();
	return;
}

void importCourseScore(Year* pcurYear,int orderSem,string filename){
	Course* pCurCou;
	switch(orderSem){
		case 1:
			pCurCou = pcurYear->Sem1.pHeadCou;
			break;
		case 2:
			pCurCou = pcurYear->Sem2.pHeadCou;
			break;
		case 3:
			pCurCou = pcurYear->Sem3.pHeadCou;
			break;
		
	}
	string tmp = "";
	int i = 0;
	while(filename[i] != '.')
		tmp += filename[i];
	while(pCurCou->IDCou != tmp)
		pCurCou = pCurCou->pNext;
	ifstream fin;
	fin.open(filename);
	getline(fin,tmp);
	getline(fin,tmp);
	for ( int i = 0; i < pCurCou->enrolling; i++){
		getline(fin,tmp,',');
		getline(fin,tmp,',');
		getline(fin,tmp,',');
		Score* pCur = pCurCou->Stu[i]->Inclass;
		while(pCur->CouID != pCurCou->IDCou)
			pCur = pCur->pNext;
		getline(fin,tmp,',');
		pCur->Mid = stof(tmp);
		getline(fin,tmp,',');
		pCur->Final = stof(tmp);
		getline(fin,tmp,',');
		pCur->Total = stof(tmp);
		getline(fin,tmp,',');
		pCur->Other = stof(tmp);
		if(pCur->Total >= 0 && pCur->Total <= 2.9)
			pCur->GPA = 0.0;
		if(pCur->Total >= 3.0 && pCur->Total <= 3.9)
			pCur->GPA = 0.5;
		if(pCur->Total >= 4.0 && pCur->Total <= 4.7)
			pCur->GPA = 1.0;
		if(pCur->Total >= 4.8 && pCur->Total <= 5.4)
			pCur->GPA = 1.5;
		if(pCur->Total >= 5.5 && pCur->Total <= 6.2)
			pCur->GPA = 2.0;
		if(pCur->Total >= 6.3 && pCur->Total <= 6.9)
			pCur->GPA = 2.5;
		if(pCur->Total >= 7.0 && pCur->Total <= 7.7)
			pCur->GPA = 3.0;
		if(pCur->Total >= 7.8 && pCur->Total <= 8.4)
			pCur->GPA = 3.5;
		if(pCur->Total >= 8.5 && pCur->Total <= 10.0)
			pCur->GPA = 4.0;
	}
	fin.close();
	cout << "The score board has been updated!!!" << endl;
	cout << "Press any key to back...";
	getch();
	return;
}

void staffScore(Year* pcurYear, int orderSem){
	Course* pHeadCou;
	switch(orderSem){
		case 1:
			pHeadCou = pcurYear->Sem1.pHeadCou;
			break;
		case 2:
			pHeadCou = pcurYear->Sem2.pHeadCou;
			break;
		case 3:
			pHeadCou = pcurYear->Sem3.pHeadCou;
			break;
		
	}
	while (!logOut){
		system("cls");
		printBox("Press E to export list students of course to update the scoreboard.",xp,5,70);
		printBox("Press I to import the updated scoreboard of course.",xp,8,70);
		printBox("Press O to log out",xp,11,33);
		printBox("Press ESC to exit",xp + 37,11,33);
		
		char c = getch();
		if (c == 'o' || c == 'O'){
			logOut = true;
			return;
		}
		else if (c == 27){
			exitProgram = true;
			return;
		}
		else if (c == 'e' || c == 'E'){
			system("cls");
			Course* pCur = pHeadCou;
			int orderCou;
			int No = 0;
			while(pCur != NULL){
				No++;
				cout << No << ") " << pCur->IDCou << ". " << pCur->nameCou << " " << pCur->teacher << endl;
				pCur = pCur->pNext;
			}
			cout << "Which course do you want to export? Please enter the course's No (Ex: 1): ";
			cin >> orderCou;
			exportCourseToTeacher(pcurYear,orderSem,orderCou);
		}
		else if (c == 'i' || c == 'I'){
			system("cls");
			Course* pCur = pHeadCou;
			int No = 0;
			string filename;
			cout << "Here all courses in this semester:" << endl;
			while(pCur != NULL){
				No++;
				cout << No << ") " << pCur->IDCou << ". " << pCur->nameCou << " " << pCur->teacher << endl;
				pCur = pCur->pNext;
			}
			cout << "Which course do you want to update the scoreboard? Please enter the course's file (Ex: 001.csv): ";
			cin >> filename;
			importCourseScore(pcurYear,orderSem,filename);
		}
	}
}

void staffSee(Year *&pcurYear, int orderSem){
			
	while (!logOut){
		system("cls");
		printBox("Press 1 to see list of classes",xp,5,50);
		printBox("Press 2 to see list of courses",xp,8,50);
		printBox("Press O to log out",xp,11,50);
		printBox("Press ESC to exit",xp,14,50);
		
		char c = getch();
		if ( c == 'o' || c == 'O'){
			logOut = true;
			return;
		}
	else if ( c == 27){
			exitProgram = true;
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
						importCourse (to_string(pcurYear->start) + "Sem1.csv", pcurYear, orderSem);
						break;
					}
					case 2:{
						importCourse (to_string(pcurYear->start) + "Sem2.csv", pcurYear, orderSem);
						break;
					}
					case 3:{
						importCourse (to_string(pcurYear->start) + "Sem3.csv", pcurYear, orderSem);
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
					// gotoxy(xp, 5 + soluong + 9 );
					// cout << "Do you want to print list of students: ";
					// gotoxy(xp, 5 + soluong + 10 );
					// cout << "Y for Yes, N for No:  ";					
					// cin >> c1;
					// if ( c1 == 'Y' || c1 == 'y')
					// 	listStuOfCou(pcurYear,orderSem,orderCou, 1);
				}	
			}
	}
}

bool activeCourseRegister(Year *&pcurYear){
	fstream FILE;
	FILE.open ("Year.txt",ios::out);
	FILE << pcurYear->start << " " << pcurYear->end << " 1";
	FILE.close();
	
	system("cls");
	cout << "\n\n\n\n\n\n";
	cout << "        " << "The course registration for the latest semester is active successfully";
	cout << "\n\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";
	getch();
		
	return true;
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
        pNew -> Stu = NULL;
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
	pNew->Stu= NULL;
	
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
			FILE.open(to_string(pCurYear->start) + "Sem1.csv",ios::out);
			break;
		}
		case 2:{
			pHeadCou = pCurYear->Sem2.pHeadCou;
			FILE.open(to_string(pCurYear->start) + "Sem2.csv",ios::out);
			break;
		}
		case 3:{
			pHeadCou = pCurYear->Sem3.pHeadCou;
			FILE.open(to_string(pCurYear->start) + "Sem3.csv",ios::out);
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
		
        for (int i = 1; i <= pCurCou->enrolling; i++){
            FILE << i << "," ;
            FILE << pCurCou->Stu[i]->IDStu << "," ;
            FILE << pCurCou->Stu[i]->firstname << "," ;
        	FILE << pCurCou->Stu[i]->lastname << "," ;
            FILE << pCurCou->Stu[i]->date << "," ;
            FILE << pCurCou->Stu[i]->IDSocial << "\n" ;		
            Score *pcurScore = pCurCou -> Stu[i] -> Inclass;
            while (strcmp(pcurScore -> CouID.c_str(), pCurCou -> IDCou.c_str()) != 0)
                pcurScore = pcurScore -> pNext;
            if(pcurScore->Mid != -1)
                FILE << pcurScore->Mid << ",";
            else 
                FILE << "-" << ",";
            if(pcurScore->Final != -1)
                FILE << pcurScore->Final << ",";
            else 
                FILE << "-" << ",";
            if(pcurScore->Total != -1)
                FILE << pcurScore->Total << ",";
            else 
                FILE << "-" << ",";
            if(pcurScore->Other != -1)
                FILE << pcurScore->Other << ",";
            else 
                FILE << "-" << ",";
            if(pcurScore->GPA != -1)
                FILE << pcurScore->GPA << "," << endl;
            else 
                FILE << "-" << "," << endl;
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

void createCourseRegister(Year *&pcurYear, int &orderSem){
	system("cls");
	if ((orderSem == 1 && pcurYear->Sem1.pHeadCou == NULL)|| (orderSem == 2 && pcurYear->Sem2.pHeadCou == NULL) || (orderSem == 3 && pcurYear->Sem3.pHeadCou == NULL)){
		cout << "\n\n\n\n\n\n";
		cout << "        " << "When does the course registration start? (DD/MM/YYYY) ";
		cin >> startDateRegister;
		cout << "\n\n        " << "When does the course registration end? (DD/MM/YYYY)   ";
		cin >> endDateRegister;
		cout << "\n\n        " << "You created successfully!";
		cout << "\n        " << "PRESS ENTER TO GO BACK...";
		getch();		
	}

	int size = 50;
	while (true){
		system("cls");
		
		printBox("Press 1 to create a course",xp,5,size);
		printBox("Press 2 to see list of courses",xp,8,size);
		printBox("Press 3 to active course registration session",xp,11,size);
		printBox("Press O to log out",xp,14,size);	
		printBox("Press ESC to exit",xp,17,size);
		
		char c = getch();
		if ( c == '1' ){
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
				createCourse(pcurYear, orderSem);
			else
				createCourseCSV(pcurYear,orderSem);
		}
			
		else if ( c == '2'){
			char c1 = '1', c2 = '1';
			while ( c1 != 'B' && c1 != 'b' && c2 != 'B' && c2 != 'b'){
				listCourse(pcurYear,orderSem,soluong);
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
					editCourse(pcurYear,orderSem,orderCou);
				else if (c1 == 'D' || c1 == 'd')
					deleteCourse(pcurYear,orderSem,orderCou);
			}
		}
		else if ( c == '3')
			activeCourseRegister(pcurYear);
		else if ( c == 'O' || c == 'o'){
			logOut = true;
			return;
		}
		else if ( c == 27){
			exitProgram = true;
			return;
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
void RegisterCou(Year *&pcurYear, int orderSem, Course *pHead, Student *curStu){
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
        system ("cls");
        curStu -> course += pcur -> IDCou;
        pcur -> enrolling += 1;
        PrintCourse (pcurYear, orderSem);
        cout << "You enroll this course sucessfully";
    } 
} 

// Hàm dùng để nhập dữ liệu và nối liên kết giữa các biến
// dùng khi khởi động lại chương trình, khi kết thúc đăng ký học phần
// khi kết thúc đăng ký học phần cần kèm thêm điều kiện đã kết thúc học phần
void UpdateData (Year *&pCurYear, int semester, bool yearCreated, bool createScore){
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
    if (pHeadCou == nullptr){
        return;
    }
    Course *pCurCou = pHeadCou;
    // tạo dynamic array của student trong course dựa trên số lượng enrolling
    while (pCurCou != nullptr){
        pCurCou -> Stu = new Student *[pCurCou -> enrolling];
        pCurCou = pCurCou -> pNext;
    }
    Class *pCurClass = pCurYear -> pHeadClass;
    Student *pCurStu;
    // k dùng để dò xem course đã cập nhật được bao nhiêu học sinh rồi
    int k;
    pCurCou = pHeadCou;
    int length = strlen (pCurCou -> IDCou.c_str());
    Score *curScore;
    // length là độ dài được quy định của tên course
    // vòng lặp được chạy qua từng học sinh và dò course mà các học sinh đó đã
    // đăng ký, nếu tên course đăng ký trùng với tên course có sẵn thì pointer
    // của học sinh sẽ được cập nhật trong course
    while (pCurClass != nullptr){
        pCurStu = pCurClass -> pHeadStu;
        for (int j = 0; j < pCurClass -> numberOfStu; j++){
            int maxCou = strlen (pCurStu -> course.c_str()) / length;
            curScore = pCurStu -> Inclass;
            if (pCurStu -> Inclass != nullptr)
                while (curScore -> pNext != nullptr)
                    curScore = curScore -> pNext; 
            for (int i = 0; i < maxCou; i++){
                pCurCou = pHeadCou;
                while (pCurCou != nullptr && pCurCou -> IDCou != pCurStu -> course.substr(i*length, length)){
                    pCurCou = pCurCou -> pNext;
                }
                if (pCurCou == nullptr)
                    continue;

                // tạo score
                if (createScore == true){
                    if (pCurStu -> Inclass == nullptr) {
                        curScore = new Score;
                        pCurStu -> Inclass = curScore;
                    }
                    else{
                        curScore -> pNext = new Score;
                        curScore = curScore -> pNext;
                        curScore -> pNext = nullptr;
                    }               
                    curScore -> CouName = pCurCou -> nameCou;
                    curScore -> CouID = pCurCou -> IDCou;
                    curScore -> GPA = -1;
                    curScore -> Final = -1;
                    curScore -> Mid = -1;
                    curScore -> Other = -1;
                    curScore -> Total = -1;
                    curScore -> semester = semester;
                }
                // chạy tới cuối danh sách học sinh trong courses
                k = 0;
                while (pCurCou -> Stu[k] != '\0')
                    k++;
                pCurCou -> Stu[k] = pCurStu;
            }
            curScore = nullptr;
            pCurStu = pCurStu -> pNext;  
        }
        pCurClass = pCurClass -> pNext;
    }             
}             

void ExportData (Year *&curYear, bool couRegistEnd){
    Course *pHeadCou[3];
    pHeadCou[0] = curYear -> Sem1.pHeadCou;
    pHeadCou[1] = curYear -> Sem2.pHeadCou;
    pHeadCou[2] = curYear -> Sem3.pHeadCou;      
    string filename = to_string (curYear -> start) + ".csv";
    ofstream ofile (filename.c_str());
    for (int i = 0; i < 3; i++){
        Course *pcurCou = pHeadCou[i];
        int count = 0;
        while (pcurCou != nullptr){
            pcurCou = pcurCou -> pNext;
            count ++;
        }
        pcurCou = pHeadCou[i];
        // Xuất danh học kì và số lượng course trước rồi mới xuất danh sách các course
        ofile << i+1 << "," << count << endl;
        while (pcurCou != nullptr){
            ofile << pcurCou -> IDCou << ",";
            ofile << pcurCou -> nameCou << ",";
            ofile << pcurCou -> credits << ",";
            ofile << pcurCou -> day1 << ",";
            ofile << pcurCou -> session1 << ",";
            ofile << pcurCou -> day2 << ",";
            ofile << pcurCou -> session2 << ",";
            ofile << pcurCou -> teacher << ",";
            ofile << pcurCou -> enrolling << ",";
            ofile << pcurCou -> maxStu << endl;
            pcurCou = pcurCou -> pNext;
        } 
    }        
    Class *pHeadClass = curYear -> pHeadClass;
    Class *pcurClass = pHeadClass;
    int length = strlen (pHeadCou[0] -> IDCou.c_str());
    Student *pcurStu;
    Score *pcurScore;
    // Xuất tên class và số lượng học sinh trong class trước
    // Trong mỗi class sẽ xuất các thông tin cụ thể của mỗi học sinh
    // Các thông tin đó gồm STT, MSSV, Tên, Họ, Giới Tính, Ngày Sinh, CMND,
    // Course và Score nếu có
    while (pcurClass != nullptr){
        ofile << pcurClass -> className << "," << pcurClass -> numberOfStu << endl;
        pcurStu = pcurClass -> pHeadStu;
        for (int i = 0; i < pcurClass -> numberOfStu; i++){
            ofile << pcurStu -> No << ",";
            ofile << pcurStu -> IDStu << ",";
            ofile << pcurStu -> firstname << ",";
            ofile << pcurStu -> lastname << ",";
            ofile << pcurStu -> gender << ",";
            ofile << pcurStu -> date << ",";
            ofile << pcurStu -> IDSocial << ",";
            ofile << pcurStu -> course << endl;
            if (couRegistEnd == true){
                pcurScore = pcurStu -> Inclass;
                for (int i = 0; i < strlen(pcurStu -> course.c_str())/length; i++){
                    ofile << pcurScore -> semester << ",";
                    ofile << pcurScore -> CouID << ",";
                    ofile << pcurScore -> Mid << ",";
                    ofile << pcurScore -> Final << ",";
                    ofile << pcurScore -> Total << ",";
                    ofile << pcurScore -> Other << ",";
                    if (i != strlen(pcurStu -> course.c_str())/length - 1)
                        ofile << pcurScore -> GPA << ",";     
                    else 
                        ofile << pcurScore -> GPA << endl;
                    pcurScore = pcurScore -> pNext;
                    }
            }
            pcurStu = pcurStu -> pNext;
        }
        pcurClass = pcurClass -> pNext;
    } 
    ofile.close();
}

void readData (Year *&pcurYear, bool hasScore){
	fstream FILE;
	FILE.open("status.txt",ios::in);
	int start_year, end_year;
	while (!FILE.eof())
		FILE >> start_year >> end_year >> isRegister >> hasScore;
	FILE.close();
	
	if (start_year && end_year){
		pcurYear = new Year;
		pcurYear->start = start_year;
		pcurYear->end = end_year;
		
		string filename = to_string(pcurYear -> start) + ".csv";
	    ifstream ifile (filename.c_str());
	    Semester *sem[3];
	    sem[0] = &pcurYear -> Sem1;
	    sem[1] = &pcurYear -> Sem2;
	    sem[2] = &pcurYear -> Sem3; 
	    Course *pcurCou;
	    string tmp;
	    for (int i = 0; i < 3; i++){
	        getline (ifile, tmp, ',');
	        getline (ifile, tmp, '\n'); 
	        int count = stoi (tmp);
	        sem[i]->pHeadCou = NULL;
	        pcurCou = nullptr;
	        for (int j = 0; j < count; j++){
	            Course *pNew = new Course;
	            getline(ifile, tmp,',');
	            if (tmp == "\0")
	                break;
	            pNew -> IDCou = tmp;
	            getline(ifile, tmp, ',');
	            pNew -> nameCou = tmp;
	            getline(ifile, tmp, ',');
	            pNew->credits = stoi(tmp.c_str()); 
	            getline(ifile, tmp, ',');
	            pNew -> day1 = tmp;
	            getline(ifile, tmp, ',');
	            pNew -> session1 = tmp;
	            getline(ifile, tmp, ',');
	            pNew -> day2 = tmp;
	            getline(ifile, tmp, ',');
	            pNew -> session2 = tmp;
	            getline(ifile, tmp, ',');
	            pNew -> teacher = tmp;
	            getline(ifile, tmp, ',');
	            pNew -> enrolling = stoi (tmp.c_str());
	            getline (ifile, tmp, '\n'); 
	            pNew -> maxStu = stoi (tmp);
	            tmp = "";
	            if (sem[i] -> pHeadCou == nullptr)
	                sem[i] -> pHeadCou  = pNew;
	            if (pcurCou != nullptr)
	                pcurCou->pNext = pNew;
	            pcurCou = pNew;
	        }
	    }
	    getline (ifile, tmp, ',');
	    Class *curClass;
	    pcurYear->pHeadClass = NULL;
	    Student *curStu;
	    Score *curScore;
	    int length = strlen(sem[0] -> pHeadCou -> IDCou.c_str());
	    while (!ifile.eof()) {
	        if (tmp != "\0") {
	            if ( pcurYear -> pHeadClass != nullptr) {
	                curClass->pNext = new Class;
	                curClass = curClass->pNext;
	                curClass -> pNext = nullptr;
	            }
	            else {
	                curClass = new Class;
	                pcurYear -> pHeadClass = curClass; 
	            }
	            curClass->className = tmp;
	            getline(ifile, tmp, '\n');
	            curClass->numberOfStu = stoi(tmp.c_str());
	            for (int i = curClass->numberOfStu; i > 0; i--) {
	                if (i != curClass->numberOfStu) {
	                    curStu->pNext = new Student;
	                    curStu = curStu->pNext;
	                }
	                else { 
	                    curStu = new Student;
	                    curClass -> pHeadStu = curStu;
	                }
	                getline (ifile, tmp, ',');
	                curStu->No = stoi(tmp.c_str());
	                getline(ifile, tmp, ',');
	                curStu->IDStu = stoi(tmp.c_str());
	                getline(ifile, tmp, ',');
	
	                curStu->firstname = tmp;
	                getline(ifile, tmp, ',');
	                curStu->lastname = tmp;
	                getline(ifile, tmp, ',');
	                curStu->gender = tmp;
	                getline(ifile, tmp, ',');
	                curStu->date = tmp;         
	                getline(ifile, tmp, ',');
	                curStu->IDSocial = stoi(tmp.c_str());
	                getline (ifile, tmp, '\n');
	                curStu -> course = tmp;
	                curStu->pNext = nullptr;
	                if (hasScore == true){
	                    int end = strlen(curStu -> course.c_str()) / length;
	                    for (int j = 0; j < end; j++){
	                        if (curStu -> Inclass == nullptr){
	                            curScore = new Score;
	                            curStu -> Inclass = curScore;
	                        }
	                        else {
	                            curScore -> pNext = new Score;
	                            curScore = curScore -> pNext;
	                            curScore -> pNext = nullptr;
	                        }
	                        getline(ifile, tmp, ',');
	                        curScore -> semester = stoi (tmp);
	                        getline (ifile, tmp, ',');
	                        curScore -> CouID = tmp;
	                        getline (ifile, tmp, ',');
	                        curScore -> Mid = stoi (tmp);
	                        getline (ifile, tmp, ',');
	                        curScore -> Final = stoi (tmp);
	                        getline (ifile, tmp, ',');
	                        curScore -> Total = stoi (tmp);
	                        getline (ifile, tmp, ',');
	                        curScore -> Other = stoi (tmp);
	                        if (j != end -1)
	                            getline (ifile, tmp, ',');
	                        else getline (ifile, tmp, '\n');
	                        curScore -> GPA = stoi (tmp);
	                    }
	                    curScore = nullptr;
	                }
	            }
	        }
	        else break;
	        getline (ifile, tmp, ',');
	    }
	    for (int i = 1; i < 4; i++)
	        UpdateData(pcurYear, i, true, false);
		if (pcurYear->Sem1.pHeadCou != NULL)
			orderSem = 1;
		if (pcurYear->Sem2.pHeadCou != NULL)
			orderSem = 2;
		if (pcurYear->Sem3.pHeadCou != NULL)
			orderSem = 3;	
	}
	else
		pcurYear = NULL;	
		
     
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

void createSemester(Year *&pcurYear, int &orderSem){
	if (pcurYear->Sem1.pHeadCou != NULL)
		exportCourse(pcurYear, 1);
	if (pcurYear->Sem2.pHeadCou != NULL)
		exportCourse(pcurYear, 2);
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
    getline(ifile, tmp, ',');
    if (tmp == "\0") {
        return;
    }
    Class *curClass = new Class;
    while (!ifile.eof()) {
        if (tmp != "\0") {
            if ( cHead != nullptr) {
                curClass->pNext = new Class;
                curClass = curClass->pNext;
            }
            else cHead = curClass;
            
            curClass->className = tmp;
            getline(ifile, tmp, '\n');
            curClass->numberOfStu = stoi(tmp.c_str());
            getline (ifile, tmp, ',');
            Student* pcur = new Student;
            for (int i = curClass->numberOfStu; i > 0; i--) {
                if (i != curClass->numberOfStu) {
                    pcur->pNext = new Student;
                    pcur = pcur->pNext;
                }
                else curClass->pHeadStu = pcur;
                pcur->No = stoi(tmp.c_str());
                getline(ifile, tmp, ',');
                pcur->IDStu = tmp.erase(0, 1);
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
                getline(ifile, tmp, ',');
            }
            pcur->pNext = nullptr;  
        }
        else break;
    }
    curClass->pNext = NULL;
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

void ImportOldData (Year *&curYear){
    string tmp = "datayear";
    curYear->pHeadClass = NULL;
    for (int i = 4; i > 1; i--)
        ImportOldStu (tmp + to_string(i) + ".csv", curYear -> pHeadClass);
}

void createYear(Year *&pcurYear){	
	system ("cls");
	if (pcurYear == NULL)
		pcurYear = new Year;
	pcurYear->Sem1.pHeadCou = NULL;
	pcurYear->Sem2.pHeadCou = NULL;
	pcurYear->Sem3.pHeadCou = NULL;
	ImportOldData(pcurYear);
	
	cout << "\n\n\n\n\n\n";
	cout << "        " << "When does the school year start?  ";
	cin >> pcurYear->start;
	cout << "\n\n        " << "When does the school year start?  "; 
	cin >> pcurYear->end;
	cout << "\n\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";
	
	fstream FILE;
	FILE.open ("status.txt",ios::out);
	FILE << pcurYear->start << " " << pcurYear->end << " " << isRegister;
	FILE.close();
	
	getch();
}

void staffCreate(Year *&pcurYear, int &orderSem){
	while (!logOut && !exitProgram){
		system("cls");
		gotoxy(xp,3);
		cout << "At the beginning of school year...";
		printBox("Press 1 to create a school year",xp,5,50);
		printBox("Press 2 to create a class",xp,8,50);
		printBox("Press 3 to create a semester",xp,11,50);
		printBox("Press 4 to create a course registration session",xp,14,50);
		printBox("Press O to log out",xp,17,50);
		printBox("Press ESC to exit",xp,20,50);
		
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
	else if ( c == 27){
		exitProgram = true;
		return;
		}
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

Student* afterLog(string username, Year* pcurYear){
	Class* pClass = pcurYear->pHeadClass;
	while(pClass != NULL){
		if(pClass->className[0] == username[0] && pClass->className[1] == username[1]){
			Student* pStu = pClass->pHeadStu;
			while(pStu != NULL){
				if(pStu->IDStu == username)
					return pStu;
				pStu = pStu->pNext;
			}
		}
		pClass = pClass->pNext;
	}
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