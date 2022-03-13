#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct account {
    char username[50][20];
    char password[50][20];
    int locate;
    bool login;
    int numberofaccount;
};

bool checkAccount(char compareData[], account& user);
bool checkPassword(char compareData[], account user);
bool comparePassword(char newPass[], char confirmPass[]);
int findPasswordPosition(char compareData[], account user);
int findAccountPosition(char compareData[], account user);
void getDataFromFile (account &user);
bool isLogin (char accountname[], char password[], account& user);
void updatePass(account user);


int main (){
    account human;
    getDataFromFile(human);
    char pass[20];
    char account[20];
    cout << "Enter account: ";
    cin >> account;
    cout << "Enter password: ";
    cin >> pass;
    if (isLogin(account, pass, human)  == true)
        cout << "You have logined successfully" << endl;
    else cout << "Wrong password or account" << endl;

}

// Kiểm tra xem tài khoản có tồn tại không
bool checkAccount(char compareData[], account &user){
    // compareData là một chuỗi char nên sẽ được so sánh từng phần tử
    for (int k = 0; k < user.numberofaccount; k++){ 
        for (int j =0 ; j < 20; j++){
            /*vòng lặp sẽ thoát ra khi thỏa cả 2 điều kiện là dữ liệu không
            phải rỗng mà chữ cái trong chuỗi compareData khác chữ cái trong
            dữ liêu */
            if (user.username[k][j] != '\0' && compareData[j] != user.username[k][j]){
                break;
            }
            // khi chạy hết chuỗi không bị break thì sẽ return true
            else if (user.username[k][j] == '\0')
                user.locate = k;
                return true;
        }
    }
    return false;
}

// tương tự với kiểm tra mật khẩu
bool checkPassword(char compareData[], account user){
    for (int k = 0; k < user.numberofaccount; k++){ 
        for (int j =0 ; j < 20; j++){
            if (user.password[k][j] != '\0' && compareData[j] != user.password[k][j]){
                break;
            }
            else if (user.password[k][j] == '\0')
                return true;
        }
    }
    return false;
}

// Đây là chức năng dùng để kiểm tra xem 2 chuỗi có giống nhau không trong thay
// viêc đổi mật khẩu
bool comparePassword(char newPass[], char confirmPass[]){
    for (int k = 0; k < 3; k++){ 
        if (newPass[k] != confirmPass[k] && newPass[k] != '\0'){
            return false;
        }
    }
    return true;
}

int findPasswordPosition(char compareData[], account user){
    int k;
    for ( k = 0; k < user.numberofaccount; k++){ 
        for (int j =0 ; j < 20; j++){
            if (user.password[k][j] != '\0' && compareData[j] != user.password[k][j]){
                break;
            }
            else if (user.password[k][j] == '\0')
                return k;
        }
    }
    return k;
}

int findAccountPosition(char compareData[], account user){
    int k;
    for ( k = 0; k < user.numberofaccount; k++){ 
        for (int j =0 ; j < 20; j++){
            if (user.username[k][j] != '\0' && compareData[j] != user.username[k][j]){
                break;
            }
            else if (user.username[k][j] == '\0')
                return k;
        }
    }
    return k;
}

void getDataFromFile (account &user){
    ifstream inputFile;
        user.numberofaccount = 0;
        inputFile.open("account.txt");
        while (inputFile >> user.username[user.numberofaccount]){
            user.numberofaccount++;
        }
        inputFile.close();
        inputFile.open("password.txt");
        user.numberofaccount = 0;
        while (inputFile >> user.password[user.numberofaccount]){
            user.numberofaccount++;
        }
        inputFile.close();
}
// chức năng để cho biết người dùng đăng nhập thành công chưa
bool isLogin (char accountname[], char password[], account &user){
    if (checkAccount(accountname, user) == true && checkPassword(password, user) == true){
        return true;
    }
    return false;
}
// chức năng dùng để xuất password ra file sau khi đổi thành công
void updatePass(account user){
    ofstream outputFile;
    outputFile.open("password.txt");
    for (int k = 0; k < user.numberofaccount; k++){   
    outputFile << user.password[k] << '\n';
    }
    outputFile.close();
}
