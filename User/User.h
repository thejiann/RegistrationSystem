#pragma once

#include <iostream>
#include <utility>
#include <random>
#include "../Database/Mysql.h"
#include "../LoginInfo.h"
#include "../pages.h"

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
    string getPhoneNumber() {return this->phoneNumber;}
    void showUserInfo(); //显示用户信息
    bool updateUserInfo();  //修改用户信息
    void deleteUser();  //删除用户信息
    static User *getUserById(int userId);
    static void setUserType(int userId, int userType);
    static void showAllUserInfo();
    static void deleteUserByUserId(int userId);
    static bool deleteUserByPhoneNumber(string phoneNumber);
    string getName() {return this->name;}
    int getUserType() {return this->userType;}
    int getUserId() {return this->id;}
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
static Doctor *getDoctorByDoctorId(int doctorId);
static int getUserIdByDoctorId(int doctorId);
string getProfessionalTitle() {return this->professionalTitle;}
string getBriefIntroduction() {return this->briefIntroduction;}
static bool setCommonUserToDoctor();
static void addDoctor(int userId);
static void deleteDoctorByUserId(int userId);
string getName() {return this->name;}
int getDoctorId() {return this->id;}
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
        return false;
    } else {
        if (password == user->password) {
            LoginInfo::setLoginInfo(true, user);
            return true;
        } else {
            return false;
        }
    }
}

//显示用户信息
void User::showUserInfo() {
    cout << "name: " << this->name << endl;
    if (this->sex == 0) {
        cout << "sex: man" << endl;
    } else {
        cout << "sex: woman" << endl;
    }
    cout << "idCard: " << this->idCard << endl;
    cout << "phoneNumber: " << this->phoneNumber << endl;
    cout << "address: " << this->address << endl;
}

//修改用户信息
bool User::updateUserInfo() {
    MYSQL *Cur = connectDb();
    cout << "请输入新密码: ";
    cin >> this->password;
    cout << "请输入新名字: ";
    cin >> this->name;
    cout << "请输入新电话号码: ";
    cin >> this->phoneNumber;
    cout << "请输入新地址: ";
    cin >> this->address;
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "update user set password='%s',name='%s',phone_number='%s',address='%s' where id_card='%s'",
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

//设置用户类型
void User::setUserType(int userId, int userType) {
    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "update user set user_type=%d where id=%d", userType, userId);
    mysql_query(Cur, sql);    //执行sql语句
}

//展示所有用户信息
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
            cout.width(20);
            cout << std::left << "id";
            cout.width(23);
            cout << std::left << "密码";
            cout.width(22);
            cout << std::left << "姓名";
            cout.width(25);
            cout << std::left << "性别";
            cout.width(25);
            cout << std::left << "手机号码";
            cout.width(24);
            cout << std::left << "身份证号";
            cout.width(20);
            cout << std::left << "地址";
            cout.width(29);
            cout << std::left << "用户类型";
            cout.width(18);
            cout << std::left << "职称";
            cout.width(18);
            cout << std::left << "简介" << endl;
            while ((row = mysql_fetch_row(result))) {
                userId = atoi(row[0]);
                userType = atoi(row[7]);

                for (int i = 0; i < num_fields; i++) {
                    cout.width(19);
                    cout << std::left << row[i];
                    cout << "\t";
                }
                if (userType == DOCTOR) {
                    doctor = Doctor::getDoctorByUserId(userId);
                    cout.width(15);
                    cout << std::left << doctor->getProfessionalTitle() << '\t';
                    cout.width(15);
                    cout << std::left << doctor->getBriefIntroduction() << '\t';
                }
                cout << "\n";
            }
        }
    }
}

//通过user_id删除用户
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

//通过user_id得到用户信息
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

//通过手机号删除用户
bool User::deleteUserByPhoneNumber(string phoneNumber) {
    User *user = findUserByPhoneNumber(phoneNumber);
    if (user == NULL) {
        cout << "该手机号不存在！" << endl;
        return false;
    }
    if (user->userType == DOCTOR) {
        Doctor::deleteDoctorByUserId(user->getUserId());
    }

    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "delete from user where id=%d", user->getUserId());
    mysql_query(Cur, sql);    //执行sql语句
    return true;
}

//将普通用户设置成医生
bool Doctor::setCommonUserToDoctor() {
    string phoneNumber;
    cout << "请输入需要修改为医生的用户的手机号: ";
    cin >> phoneNumber;
    User *user = User::findUserByPhoneNumber(phoneNumber);
    if (user == NULL) {
        cout << "该手机号不存在!" << endl;
        return false;
    }
    User::setUserType(user->getUserId(), DOCTOR);
    addDoctor(user->getUserId());
    return true;
}

//在doctor表中添加一个医生账户
void Doctor::addDoctor(int userId) {
    int departmentId;
    string professionalTitle;
    string briefIntroduction;
    cout << "科室id: ";
    cin >> departmentId;
    cout << "职称: ";
    getchar();
    getline(cin, professionalTitle);
    cout << "简介: ";
    getline(cin, briefIntroduction);

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

//通过user_id删除医生账户
void Doctor::deleteDoctorByUserId(int userId) {
    MYSQL *Cur = connectDb();
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "delete from doctor where user_id=%d", userId);
    mysql_query(Cur, sql);
}

//通过医生id得到user_id
int Doctor::getUserIdByDoctorId(int doctorId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select user_id from doctor where id=%d", doctorId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            MYSQL_ROW row = mysql_fetch_row(result);
            return atoi(row[0]);
        }
    }
    return -1;
}

//通过user_id得到医生信息
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

//通过医生id得到医生信息
Doctor *Doctor::getDoctorByDoctorId(int doctorId) {
    int userId = getUserIdByDoctorId(doctorId);
    if (userId == -1) {
        return NULL;
    }
    Doctor *doctor = getDoctorByUserId(userId);
    return doctor;
}


