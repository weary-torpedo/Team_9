#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

struct Student;
struct Class;
struct Semester;
struct Course;
struct Year;

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
    string day1, day2;
    string session1, session2;
    Student* pHeadInclassStu;
    Course* pNext;
};

Year* CreateYear(Year* phead) {
    Year* pcur = phead;
    while (pcur->pNext != nullptr)
        pcur = pcur->pNext;
    Year* tmp = new Year;
    tmp->pNext = nullptr;
    pcur->pNext = tmp;
    return tmp;
}

Class* CreateClass(Class*& phead, string tmp, Year*& test) {
    if (phead == nullptr) {
        phead = new Class;
        phead->className = tmp;
        phead->pNext = nullptr;
        test->pHeadClass = phead;
        return phead;
    }
    Class* pcur = phead;
    while (pcur->pNext != nullptr)
        pcur = pcur->pNext;
    pcur->pNext = new Class;
    pcur->pNext->className = tmp;
    pcur->pNext->pNext = nullptr;
    return pcur;
}

// Dùng để input dữ liệu của học sinh trong file lớp 
void ImportStuOfClass (string filename,  Class *pointer){
    ifstream ifile (filename.c_str());
    string tmp;
    getline (ifile, tmp, ',');
    if (tmp == "\0"){
        return;
    }
    Student *phead = new Student;
    phead -> No = stoi (tmp); 
    getline (ifile, tmp, ',');
    // stoi là hàm để tách số trong chuỗi
    phead -> IDStu = stoi (tmp);
    getline (ifile, tmp,',');
    // erase để xóa space ở đầu string
    phead -> firstname = tmp.erase(0,1);
    getline (ifile, tmp,',');
    phead -> lastname = tmp.erase(0,1);
    getline (ifile, tmp,',');
    phead -> gender = tmp.erase(0,1);
    getline (ifile, tmp,',');
    phead -> date = tmp.erase(0,1);
    getline (ifile, tmp,'\n');
    phead -> IDSocial = stoi (tmp);
    pointer -> pHeadStu = phead;
    Student *pcur = phead;
    pcur -> pNext = nullptr;
    while (!ifile.eof()){
        getline (ifile, tmp, ',');
        if (tmp != "\0"){
            pcur -> pNext = new Student;
            pcur = pcur -> pNext;
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
            pcur -> IDSocial = stoi (tmp);
            pcur -> pNext = nullptr;
        }
        else break;
    }
    ifile.close();
    //a[i-1].erase(remove(a[i-1].begin(), a[i-1].end(), '\n'), a[i-1].end();
    // hàm được comment là hàm dùng để loại enter khỏi chuôi
}

// Sau khi year được tạo thì class được tạo thông qua hàm này
// Hàm này giúp cập nhật tên cho lớp, nhập dữ liệu cho lớp hoc
void ImportClasses (Class *&pheadClass, Year *&curYear){
    int n;
    cout << "How many files are you going to import? ";
    cin >> n;
    string filename, classname, compare;
    bool outloop = false;
    for (int i = 0; i < n; i++){
        while (outloop == false){
            cout << "Enter class name: ";
            cin.ignore (100, '\n');
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
        ImportStuOfClass(filename, curClass);
    } 
}

// Hàm dùng để xuất kiểm tra dữ liệu trong các file có được input đúng không
void OutPutStu (Class *pheadClass){
    Class *pcurClass = pheadClass;
    while (pcurClass != nullptr){
        Student *pcurStu = pcurClass -> pHeadStu;
        cout << "Class: " << pcurClass -> className << endl;
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

int main(){  
    Year *pheadYear = new Year;
    pheadYear -> pNext = new Year; 
    Year *pcurYear = CreateYear (pheadYear);
    Class *pheadClass = nullptr;
    ImportClasses(pheadClass, pcurYear);
    OutPutStu (pheadClass);
    return 0; 
}
