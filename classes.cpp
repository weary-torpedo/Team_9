#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

struct Semester;
struct Year;
struct Student;
struct Class;
struct Course;
struct Semester {
    Course* pHeadCou;
    string  startDate;
    string endDate;
};

struct Year {
    Semester Sem1, Sem2, Sem3;
    Class* pHeadClass;
    Year* pNext;
};

struct Student {
    int No, IDStu, IDSocial;
    string firstname, lastname, gender, date;
    float Final;
    float Mid;
    float Other;
    float Total;
    Student* pNext;
};

struct Class {
    string className;
    int numberOfStu;
    Student* pHeadStu;
    Class* pNext;
};

struct Course {
    string nameCou;
    string IDCou;
    string teacher;
    int credits;
    int maxStu;
    int enrolling;
    string day1, day2;
    string session1, session2;
    Student* pHeadInclassStu;
    Course* pNext;
};

Year* CreateYear (Year* phead);
Class * CreateClass (Class*& phead, string tmp, Year*& curyear);
void ImportNewStu (string filename, Class *curClass);
void ImportOldStu(string filename, Class*& cHead);
void ImportClasses (Class *&pheadClass, Year *&curYear);
void OutPutStu (Class *pheadClass);
bool CheckScheduleCou (Student *curStu, Course *curEnroll, Course *pHead);
void Runtest ();
void PrintCourse (Year *pCurYear, int semester);
int main (){
    Year *pheadYear = new Year;
    pheadYear -> pNext = new Year; 
    Year *pcurYear = CreateYear (pheadYear);
    Class *pheadClass = nullptr;
    ImportClasses(pheadClass, pcurYear);
    OutPutStu (pheadClass);
    return 0; 
}

Year* CreateYear(Year* phead) {
    Year* pcur = phead;
    while (pcur->pNext != nullptr)
        pcur = pcur->pNext;
    Year* tmp = new Year;
    tmp->pNext = nullptr;
    pcur->pNext = tmp;
    return tmp;
}

Class* CreateClass(Class*& phead, string tmp, Year*& curyear) {
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

// Dùng để input dữ liệu của học sinh trong file lớp 
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
            pcur -> IDStu = stoi (tmp);
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
    //a[i-1].erase(remove(a[i-1].begin(), a[i-1].end(), '\n'), a[i-1].end();
    // hàm được comment là hàm dùng để loại enter khỏi chuôi
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
                    pcur->IDStu = stoi(tmp);
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

/*void ImportOldData (Class *&pheadClass, Year *&curYear){
    while (
}*/
// Sau khi year được tạo thì class được tạo thông qua hàm này
// Hàm này giúp cập nhật tên cho lớp, nhập dữ liệu cho lớp hoc
void ImportClasses (Class *&pheadClass, Year *&curYear){
    int n;
    cout << "How many files are you going to import? ";
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
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
        Class *curClass = CreateClass (pheadClass, classname, curYear);
        ImportNewStu(filename, curClass);
    } 
}

// Hàm dùng để xuất kiểm tra dữ liệu trong các file có được input đúng không
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

// trả về true nếu trùng giờ các khóa học trươc
bool CheckScheduleCou (Student *curStu, Course *curEnroll, Course *pHead){
    string *Enrolled = new string [5];
    int count = 0, j = 0;
    int length = strlen (pHead -> IDCou.c_str());
    cout << "Length: " <<length << endl;
    string tmp = curStu -> course.substr(j, length);
    cout << tmp << endl;
    if (tmp == "\0") 
        return false;
    while (tmp != "\0"){
        Enrolled[count] = tmp;
        j += length;
        tmp = curStu -> course.substr(j, length);
        cout << tmp << endl;
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

void PrintCourse (Year *pCurYear, int semester){
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

void Runtest(){
    Student *stu = new Student;
    Course *phead = new Course;
    phead -> day1 = "MON";
    phead -> day2 = "THU";
    phead -> IDCou = "CSC1001";
    stu -> course += phead -> IDCou;
    phead -> session1 = "7:30";
    phead -> session2 = "9:30";
    phead -> pNext = new Course;
    phead -> pNext -> day1 = "TUE";
    phead -> pNext -> day2 = "WED";
    phead -> pNext -> IDCou = "CSC1002";
    stu -> course += phead -> pNext -> IDCou;
    phead -> pNext -> session1 = "7:30";
    phead -> pNext -> session2 = "9:30";
    Course *check = new Course;
    check -> day1 = "TUE";
    check -> session1 = "9:30";
    check -> day2 = "THU";
    check -> session2 = "9:30";
    bool result = CheckScheduleCou(stu, check, phead);
    if (result == true)
       cout << "This course cannot be enrolled" << endl;
    else cout << "You can enroll this course" << endl; 
}
