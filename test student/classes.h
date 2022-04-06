#ifndef _CLASSES_H
#define _CLASSES_H
#include <iostream>
#include <cstring>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

struct Semester;
struct Year;
struct Student;
struct Class;
struct Score;
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

struct Score {
    float Mid;
    float Final;
    float Other;
    float GPA;
};

struct Student {
    int No, IDStu, IDSocial;
    string firstname, lastname, gender, date, course;
    Score *Inclass; 
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
    Student** Stu;
    Course* pNext;
};

Year* CreateYear (Year* phead);
Class * CreateClass (Class*& phead, string tmp, Year*& curyear);
void ImportNewStu (string filename, Class *curClass);
void ImportClasses (Class *&pheadClass, Year *&curYear);
void OutPutStu (Class *pheadClass);
bool CheckScheduleCou (Student *curStu, Course *curEnroll, Course *pHead);
void Runtest (Year *pcurYear, int sem);
void createCourseCSV(Year *&pCurYear, int orderSem);
void createSemester(Year *&pCurYear, int &orderSem);
void PrintCourse (Year *pCurYear, int semester);
void RegisterCou(Year *pcurYear, int sem, Course *pHead, Student *curStu);
void OutputClasses (Year *curYear);
void UpdateData (Year *pCurYear, int semester, bool yearCreated);
void ImportOldData (Year *&curYear);
void ImportOldStu(string filename, Class*& cHead);
void TestUpdateData ();
#endif
