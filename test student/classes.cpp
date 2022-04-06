#include "classes.h"

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

// Hàm sẽ lấy giá trị năm của năm 2 ví dụ 20CLC10 thì lấy 20
// sau đó so sánh với các lớp trong dữ liệu rồi xuất ra lớp 21 
void OutputClasses (Year *curYear){
    Class *curClass = curYear -> pHeadClass;
    ifstream ifile ("datayear2.csv");
    string tmp;
    ifile >> tmp;
    ifile.close();
    int year2 = stoi (tmp.c_str()); 
    tmp = curClass -> className;
    int year1 = stoi (tmp.c_str());
    while (year1 <= year2){
        curClass = curClass -> pNext;
        tmp = curClass -> className;
        year1 = stoi (tmp.c_str());
    } 
    Student *curStu;
    ofstream ofile ("datayear1.csv"); 
    while (curClass != nullptr){
        curStu = curClass -> pHeadStu;
        ofile << curClass -> className << endl;
        for (int i = 0; i < curClass -> numberOfStu; i++){
            ofile << curStu -> No << ", "
                  << curStu -> IDStu << ", "
                  << curStu -> firstname << ", "
                  << curStu -> lastname << ", "
                  << curStu -> gender << ", "
                  << curStu -> date << ", "
                  << curStu -> IDSocial << endl; 
            curStu = curStu -> pNext;
        }
        curClass = curClass -> pNext;
    }
    ofile.close();
}


// Hàm dùng để xuất kiểm tra dữ liệu trong các file có được input đúng không
void OutPutStu (Class *pheadClass){
    Class *pcurClass = pheadClass;
    system ("clear");
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

void createCourseCSV(Year *&pCurYear, int orderSem){
	system("clear");
	Course *pHeadCou = NULL, *pCurCou = pHeadCou;
	string tmp;
	fstream FILE;
	FILE.open("Course.csv",ios::in);
	
	while (!(FILE.eof())){
		Course *pNew = new Course;
		getline(FILE, tmp,',');
        if (tmp == "\0")
            break;
        pNew -> IDCou = tmp;
		getline(FILE, tmp, ',');
        pNew -> nameCou = tmp.erase (0,1);
		getline(FILE, tmp, ',');
		pNew->credits = stoi(tmp.c_str()); 
		getline(FILE, tmp, ',');
        pNew -> day1 = tmp.erase (0,1);
		getline(FILE, tmp, ',');
        pNew -> session1 = tmp.erase (0,1);
		getline(FILE, tmp, ',');
        pNew -> day2 = tmp.erase (0,1);
		getline(FILE, tmp, ',');
        pNew -> session2 = tmp.erase (0,1);
		getline(FILE, tmp, ',');
        pNew -> teacher = tmp.erase (0,1);
		getline(FILE, tmp, '\n');
        pNew -> maxStu = stoi (tmp.c_str());
        pNew -> enrolling = 0;
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
    //getch();
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

void createSemester(Year *&pCurYear, int &orderSem){
	system("clear");
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

	/*cout << "\n        " << "You created successfully!";
	cout << "\n        " << "PRESS ENTER TO GO BACK...";*/
}

// trả về true nếu trùng giờ các khóa học trước hoặc đã đạt số lượng học sinh
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
void RegisterCou(Year *pcurYear, int sem, Course *pHead, Student *curStu){
    int maxCourse = 0;
    Course *pcur = pHead;
    while (pcur != nullptr){
        maxCourse++;
        pcur = pcur -> pNext;
    }  
    int no = 0;
    while (no <= 0 || no > maxCourse){
        system ("clear");
        PrintCourse (pcurYear, sem);
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
        PrintCourse (pcurYear, sem);
        cout << "You enroll this course sucessfully";
    } 
} 

void Runtest(Year *pcurYear, int sem){
    Student *stu = new Student;   
    stu -> course = "001002";
    RegisterCou (pcurYear, 1, pcurYear -> Sem1.pHeadCou, stu);
    
}

// Hàm dùng để nhập dữ liệu và nối liên kết giữa các biến
// dùng khi khởi động lại chương trình, khi kết thúc đăng ký học phần
// khi kết thúc đăng ký học phần cần kèm thêm điều kiện đã kết thúc học phần
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
    // length là độ dài được quy định của tên course
    // vòng lặp được chạy qua từng học sinh và dò course mà các học sinh đó đã
    // đăng ký, nếu tên course đăng ký trùng với tên course có sẵn thì pointer
    // của học sinh sẽ được cập nhật trong course
    while (pCurClass != nullptr){
        for (int j = 0; j < pCurClass -> numberOfStu; j++){
            pCurStu = pCurClass -> pHeadStu;
            int maxCou = strlen (pCurStu -> course.c_str()) / length;
            pCurStu -> Inclass = new Score [maxCou];
            for (int i = 0; i < maxCou; i++){
                pCurCou = pHeadCou;
                while (pCurCou != nullptr && pCurCou -> IDCou != pCurStu -> course.substr(i*length, length))
                    pCurCou = pCurCou -> pNext;
                if (pCurCou == nullptr)
                    continue;
                k = 0;
                while (pCurCou -> Stu[k] != '\0')
                    k++;
                pCurCou -> Stu[k] = pCurStu;
            }
            pCurStu = pCurStu -> pNext;  
        }
        pCurClass = pCurClass -> pNext;
    }             
}

void TestUpdateData (){
    // trong hàm của t cần có Year để chỉ đến pHeadClass, sem để chỉ đến
    // pHeadCourse mak ở đây test nên t cho sem = 1, cần có numberOfStu trong
    // class, cần enrolling trong course 
    Year *headYear = new Year;
    Class *headClass = new Class;
    headClass -> pNext = nullptr;
    headClass -> numberOfStu = 1;
    headYear -> pHeadClass = headClass;
    Student *curStu = new Student;
    headClass -> pHeadStu = curStu;
    curStu -> course = "001002";
    curStu -> No = 2112;
    createCourseCSV (headYear, 1);
    Course *curCou = headYear -> Sem1.pHeadCou;
    // đang chỉnh enrolling của các course là 1
    // xuất ra các id course cho dễ dò
    while (curCou != nullptr){
        cout << curCou -> IDCou << endl;
        curCou -> enrolling = 1;
        curCou = curCou -> pNext;
    } 
    UpdateData (headYear, 1, true); 
    cout << "Truoc khi thay doi " << curStu -> No << endl;
    curCou = headYear -> Sem1.pHeadCou;
    curCou -> Stu[0] -> No = 12345678;
    cout << "Sau khi thay doi: " << curStu -> No << endl;

} 

void ImportOldStu(string filename, Class*& cHead) {
    ifstream ifile(filename.c_str());
    string tmp;
    getline(ifile, tmp, ',');
    if (tmp == "\0") {
        return;
    }
    Class *curClass;
    if (cHead != nullptr) {
        curClass = cHead;
        while (curClass -> pNext != nullptr)
            curClass = curClass -> pNext;
    }
    else curClass = new Class;
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
            for (int i = curClass->numberOfStu; i > 0; i--) {
                Student* pcur = new Student;
                if (i != curClass->numberOfStu) {
                    pcur->pNext = new Student;
                    pcur = pcur->pNext;
                }
                else curClass->pHeadStu = pcur;
                pcur->No = stoi(tmp.c_str());
                getline(ifile, tmp, ',');
                pcur->IDStu = stoi(tmp.c_str());
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
        }
        else break;
    }
    ifile.close();
}

//Chỉ cần gọi hàm này để nhập dữ liệu từ các file data
//khi year chưa được tạo và sem chưa được tạo
void ImportOldData (Year *&curYear){
    string tmp = "datayear";
    for (int i = 4; i > 1; i--)
        ImportOldStu (tmp + to_string(i) + ".csv", curYear -> pHeadClass);
}

// hàm xuất data khi chương trình tắt
void SavingData (Year *pCurYear, int orderSem, bool yearCreated, bool semCreated){
    if (yearCreated == false && semCreated == false)
        return;
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
    Course *curCou = pHeadCou;
    int count = 0;
    while (curCou != nullptr){
        count ++;
        curCou = curCou -> pNext;
    } 
    
}
