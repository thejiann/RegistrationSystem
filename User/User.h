#pragma once

#include <iostream>
#include <utility>
#include <random>
#include "../Database/Mysql.h"
#include "../LoginInfo.h"

#define MAX_SQL_LENGTH 300
#define COMMON_USER 0
#define DOCTOR 1
#define ADMIN 2

using namespace std;

//用户类
class User {
private:
    int id;
    string password;
    string name;
    int sex;
    string phoneNumber;
    string idCard;
    string address;
    int userType;

    friend class Doctor;

public:
    User(int id, string accountId, string name, bool sex, string phoneNumber, string idCard, string address,
         short userType); //构造函数实现
    User();

    static void Register(); //用户注册
    static User *findUserByPhoneNumber(const string &phoneNumber); //通过电话号码寻找用户
    static bool Login(const string &phoneNumber, const string &password);  //登录
    string getPhoneNumber() {
        return this->phoneNumber;
    }

    void showUserInfo(); //显示用户信息
    bool updateUserInfo();  //修改用户信息
    void deleteUser();  //删除用户信息
    static User *getUserById(int userId);

    static void setUserType(int userId, int userType);

    static void showAllUserInfo();

    static void deleteUserByUserId(int userId);

    string getName() {
        return this->name;
    }

    int getUserType() {
        return this->userType;
    }

    int getId() {
        return this->id;
    }
};

//医生类
class Doctor : public User {
private:
    int userId;
    int departmentId;
    string professionalTitle;
    string briefIntroduction;


public:
    static Doctor *getDoctorByUserId(int userId);

    string getProfessionalTitle() {
        return this->professionalTitle;
    }

    string getBriefIntroduction() {
        return this->briefIntroduction;
    }

    static void setCommonUserToDoctor();

    static void addDoctor(int userId);

    static void deleteDoctorByUserId(int userId);

    string getName() {
        return this->name;
    }
};

//构造函数实现
User::User(int id, string accountId, string name, bool sex, string phoneNumber, string idCard, string address,
           short int userType) {
    this->id = id;
    this->name = move(name);
    this->sex = sex;
    this->phoneNumber = move(phoneNumber);
    this->idCard = move(idCard);
    this->address = move(address);
    this->userType = userType;
}

User::User() {}

//用户注册
void User::Register() {
    MYSQL *Cur = connectDb();
//    // 随机生成11111111-88888888之间八位数的随机数作为账号
//    srand(time(NULL));
//    int accountIdInt = rand() % (88888888 - 11111111 + 1) + 11111111;
//    string accountId = to_string(accountIdInt);
    string password;
    string name;
    int sex;
    string phoneNumber;
    string idCard;
    string address;
    int userType = COMMON_USER;

    cout << "password: ";
    cin >> password;
    cout << "name: ";
    cin >> name;
    cout << "sex: 男(0) 女(1)" << endl;
    cin >> sex;
    cout << "PhoneNumber:";
    cin >> phoneNumber;
    cout << "idCard:";
    cin >> idCard;
    cout << "address:";
    cin >> address;

    char sql[MAX_SQL_LENGTH];
    sprintf(sql,
            "insert into user (password,name,sex,phone_number,id_card,address,user_type) values('%s','%s',%d,'%s','%s','%s',%d)",
            password.c_str(), name.c_str(), sex, phoneNumber.c_str(), idCard.c_str(),
            address.c_str(),
            userType);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
        string errorInfo = mysql_error(Cur);
        cout << errorInfo << endl;
        cout << "failed!" << endl;
    } else { // query succeeded, process any data returned by it
        if (mysql_field_count(Cur) == 0) { // when (update || insert || delete)
            // query does not return data
            // (it was not a SELECT)
            // num_rows = mysql_affected_rows(Cur);
        } else { // mysql_store_result() should have returned data
            fprintf(stderr, "Error: %s\n", mysql_error(Cur));
        }
    }
}

//通过电话号码寻找用户
User *User::findUserByPhoneNumber(const string &phoneNumber) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from user where phone_number= %s", phoneNumber.c_str());
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result)  // there are rows
        {
            MYSQL_ROW row = mysql_fetch_row(result);
            User *userTemp = new User();
            int i = 0;
            userTemp->id = atoi(row[i++]);
            userTemp->password = row[i++];
            userTemp->name = row[i++];
            userTemp->sex = atoi(row[i++]);
            userTemp->phoneNumber = row[i++];
            userTemp->idCard = row[i++];
            userTemp->address = row[i++];
            userTemp->userType = atoi(row[i++]);
            return userTemp;
        }
    }
    return NULL;
}

//登录
bool User::Login(const string &phoneNumber, const string &password) {
    User *user = findUserByPhoneNumber(phoneNumber); //创建一个user对象
    if (user == NULL) {
        cout << "invalid account or password!" << endl;
        cout << "按任意键重试..." << endl;
        system("clear");
        return false;
    } else {
        if (password == user->password) {
            LoginInfo::setLoginInfo(true, user);
            return true;
        } else {
            cout << "invalid account or password!" << endl;
            cout << "按任意键重试..." << endl;
            system("pause");
            system("cls");
            return false;
        }
    }
}

//显示用户信息
void User::showUserInfo() {
    cout << "name:"<< this->name << endl;
    if(this->sex==0){
        cout << "sex:man" << endl;
    } else{
        cout << "sex:woman" << endl;
    }
    cout << "idCard:" << this->idCard << endl;
    cout << "phoneNumber:"<< this->phoneNumber << endl;
    cout << "address:"<< this->address << endl;
    cout << "password:"<< this->password << endl;
}

//修改用户信息
bool User::updateUserInfo() {
    MYSQL *Cur = connectDb();
    cout << "请输入新密码";
    cin >> this->password;
    cout << "请输入新名字";
    cin >> this->name;
    cout << "请输入新电话号码";
    cin >> this->phoneNumber;
    cout << "请输入新地址";
    cin >> this->address;
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "update user set password=%s,name=%s,phone_number=%s,address=%s where id_card=%s",
            this->password.c_str(),
            this->name.c_str(),
            this->phoneNumber.c_str(),
            this->address.c_str(),
            this->idCard.c_str()
    );
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
        string errorInfo = mysql_error(Cur);
        cout << errorInfo << endl;
        cout << "failed!" << endl;
    } else { // query succeeded, process any data returned by it
        if (mysql_field_count(Cur) == 0) { // when (update || insert || delete)
            return true;
            // query does not return data
            // (it was not a SELECT)
            // num_rows = mysql_affected_rows(Cur);
        } else { // mysql_store_result() should have returned data
            fprintf(stderr, "Error: %s\n", mysql_error(Cur));
        }
    }
    return false;
}

//删除用户信息
void User::deleteUser() {
    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "delete from user where id_card=%s", this->idCard.c_str());
    mysql_query(Cur, sql);
}

void User::setUserType(int userId, int userType) {
    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "update user set user_type=%d where id=%d", userType, userId);
    mysql_query(Cur, sql);    //执行sql语句
}

void User::showAllUserInfo() {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from user");
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result)  // there are rows
        {
            MYSQL_ROW row;
            unsigned int num_fields;
            num_fields = mysql_num_fields(result);
            int userId;
            int userType;
            Doctor *doctor;
            while ((row = mysql_fetch_row(result))) {
                userId = atoi(row[0]);
                userType = atoi(row[7]);
                for (int i = 0; i < num_fields; i++) {
                    cout << row[i];
                    cout << "\t";
                }
                if (userType == DOCTOR) {
                    doctor = Doctor::getDoctorByUserId(userId);
                    cout << doctor->getProfessionalTitle() + "\t";
                    cout << doctor->getBriefIntroduction() + "\t";

                }
                cout << "\n";
            }
        }
    }
}

void User::deleteUserByUserId(int userId) {
    User *user = getUserById(userId);
    if (user->userType == DOCTOR) {
        Doctor::deleteDoctorByUserId(userId);
    }

    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "delete from user where id=%d", userId);
    mysql_query(Cur, sql);    //执行sql语句
}

User *User::getUserById(int userId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from user where id= %d", userId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result)  // there are rows
        {
            MYSQL_ROW row = mysql_fetch_row(result);
            User *userTemp = new User();
            int i = 0;
            userTemp->id = atoi(row[i++]);
            userTemp->password = row[i++];
            userTemp->name = row[i++];
            userTemp->sex = atoi(row[i++]);
            userTemp->phoneNumber = row[i++];
            userTemp->idCard = row[i++];
            userTemp->address = row[i++];
            userTemp->userType = atoi(row[i++]);
            return userTemp;
        }
    }
    return NULL;
}

Doctor *Doctor::getDoctorByUserId(int userId) {
    User *user = User::getUserById(userId);
    Doctor *doctor = new Doctor();
    doctor->userId = user->id;
    doctor->password = user->password;
    doctor->name = user->name;
    doctor->sex = user->sex;
    doctor->phoneNumber = user->phoneNumber;
    doctor->idCard = user->idCard;
    doctor->address = user->address;
    doctor->userType = user->userType;
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from doctor where user_id=%d", userId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            MYSQL_ROW row = mysql_fetch_row(result);
            doctor->id = atoi(row[0]);
            // row[1]是user_id
            doctor->departmentId = atoi(row[2]);
            doctor->professionalTitle = row[3];
            doctor->briefIntroduction = row[4];
            return doctor;
        }
    }
    return NULL;
}

void Doctor::setCommonUserToDoctor() {
    string phoneNumber;
    cout << "请输入需要修改为医生的用户的手机号: ";
    cin >> phoneNumber;
    User *user = User::findUserByPhoneNumber(phoneNumber);
    User::setUserType(user->getId(), DOCTOR);
    addDoctor(user->getId());
}

void Doctor::addDoctor(int userId) {
    int departmentId;
    string professionalTitle;
    string briefIntroduction;
    cout << "科室id: ";
    cin >> departmentId;
    cout << "职称: ";
    cin >> professionalTitle;
    cout << "简介: ";
    cin >> briefIntroduction;

    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql,
            "insert into doctor (user_id, department_id,professional_title, brief_introduction) values(%d,%d,'%s','%s')",
            userId,
            departmentId,
            professionalTitle.c_str(),
            briefIntroduction.c_str());
    mysql_query(Cur, sql);
}

void Doctor::deleteDoctorByUserId(int userId) {
    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "delete from doctor where user_id=%d", userId);
    mysql_query(Cur, sql);
}