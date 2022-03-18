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
    return pcur;
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