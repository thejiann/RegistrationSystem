#pragma once

#include <iostream>
#include <mysql/mysql.h> //mysql提供的函数接口头文件
#include "Database/Mysql.h"
#include "User/User.h"
#include "Department.h"
#include <unistd.h>
#include "Hao.h"

#define EXIT -1
#define LOGIN_IN 0
#define SIGN_UP 1

#define REGISTER 2 //挂号
#define VIEW_REGISTER_INFO 3 //查看挂号信息
#define CANCEL_REGISTER 4 //取消挂号
#define VIEW_USER_INFO 5 //查看用户信息
#define MODIFY_USER_INFO 6//修改用户信息
#define DELETE_USER 7 //注销账户

#define COMMON_USER 0
#define DOCTOR 1
#define ADMIN 2

//按回车键返回
void enterBack() {
    cout << "回车返回上级菜单...";
    getchar();
    while (getchar() != '\n') {
    }
    system("clear");
}

void showPatientPage();

void showDoctorPage();

void showAdminPage();

//登录界面
void showFirstPage() {
    int functionId;
    string phoneNumber;
    string password;
    cout << "登录 0" << endl;
    cout << "注册 1" << endl;
    cout << "退出 -1" << endl;
    cout << "功能选择: ";
    cin >> functionId;
    std::system("clear");
    switch (functionId) {
        case LOGIN_IN: {// 登录
            cout << "PhoneNumber:（请输入11位手机号码）";
            cin >> phoneNumber;
            cout << "password: ";
            cin >> password;
            bool haveLogin = User::Login(phoneNumber, password);
            if (!haveLogin) {
                cout << "用户名或密码错误！" << endl;
                enterBack();
                showFirstPage();
                return;
            }
            system("clear");
            cout << "Login success!" << endl;
            User *user = LoginInfo::getCurrentUser();
            switch (user->getUserType()) {
                case COMMON_USER:
                    showPatientPage();
                    break;
                case DOCTOR:
                    showDoctorPage();
                    break;
                case ADMIN:
                    showAdminPage();
                    break;
            }
            return;
        }
            break;
        case SIGN_UP:
            User::Register(); //注册
            cout << "注册成功，请重新登录!" << endl;
            enterBack();
            showFirstPage();
        case EXIT:
            exit(0);
        default:
            cout << "输入错误，请重新输入： ";
            break;
    }
}

//患者界面
void showPatientPage() {
    cout << "挂号 2" << endl;
    cout << "查询已挂号信息 3" << endl;
    cout << "取消挂号 4" << endl;
    cout << "查看用户信息 5" << endl;
    cout << "修改用户信息 6" << endl;
    cout << "注销账户 7" << endl;
    cout << "退出系统 -1" << endl;

    int functionId;
    cin >> functionId;
    system("clear");
    User *currentUser = LoginInfo::getCurrentUser();
    switch (functionId) {
        case REGISTER: {
            Hao::guahao(currentUser->getUserId());
            enterBack();
            showPatientPage();
            return;
        } //挂号
            break;
        case VIEW_REGISTER_INFO:  //查询已挂号信息
            Hao::showPatientHaoInfo(currentUser->getUserId());
            enterBack();
            showPatientPage();
            break;
        case CANCEL_REGISTER: {
            int havaHao = Hao::showPatientHaoInfo(currentUser->getUserId());
            if (havaHao) {
                cout << "请选择需要取消的号: ";
                int haoId;
                cin >> haoId;
                Hao::cancelHao(currentUser->getUserId(), haoId);
                enterBack();
                showPatientPage();
                return;
            }
        } //取消挂号
            break;
        case VIEW_USER_INFO:   //查看用户信息
            currentUser->showUserInfo();
            enterBack();
            showPatientPage();
            return;
        case MODIFY_USER_INFO: {
            bool updateStatus;
            while (true) {
                updateStatus = currentUser->updateUserInfo();
                if (updateStatus) {
                    cout << "信息修改成功！" << endl;
                    enterBack();
                    showPatientPage();
                    return;
                } else {
                    cout << "信息修改失败,请问是否继续修改？修改（0） 不修改（1）";
                    bool NotContinueModify;
                    cin >> NotContinueModify;
                    if (NotContinueModify) {
                        enterBack();
                        showPatientPage();
                        return;
                    } else {
                        system("clear");
                        continue;
                    }
                }
            }
        } //修改用户信息
            break;
        case DELETE_USER: {
            currentUser->deleteUser();
            cout << "成功注销账户!"<<endl;
            enterBack();
            showPatientPage();
            return;
        }//注销账户
            break;
        case EXIT:
            exit(0);
        default:
            cout << "输入错误，请重新输入： ";
            break;
    }
}

#define SHOW_MY_PATIENT_GUAHAO 1

//医生界面
void showDoctorPage() {
    cout << "查看自己患者的挂号信息 1" << endl;
    cout << "查看用户信息 5" << endl;
    cout << "修改用户信息 6" << endl;
    cout << "退出系统 -1" << endl;

    int functionId;
    cin >> functionId;
    system("clear");
    User *currentUser = LoginInfo::getCurrentUser();
    Doctor *doctor = Doctor::getDoctorByUserId(currentUser->getUserId());
    switch (functionId) {
        case SHOW_MY_PATIENT_GUAHAO: {
            vector<int> haoTypeIds = HaoType::getHaoTypeIdsByDoctorId(doctor->getDoctorId());
            if (haoTypeIds.empty()) {
                cout << "您没有号！" << endl;
            } else {
                Hao::showMyPatientGuahao(haoTypeIds);
                enterBack();
                showDoctorPage();
                return;
            }
        }//查看自己患者的挂号信息
        case VIEW_USER_INFO: {
            currentUser->showUserInfo();
            enterBack();
            showDoctorPage();
            return;
        }//查看用户信息
        case MODIFY_USER_INFO: {
            bool updateStatus;
            while (true) {
                updateStatus = currentUser->updateUserInfo();
                if (updateStatus) {
                    cout << "信息修改成功！";
                    enterBack();
                    showDoctorPage();
                    return;
                } else {
                    cout << "信息修改失败,请问是否继续修改？修改（0） 不修改（1）";
                    bool NotContinueModify;
                    cin >> NotContinueModify;
                    if (NotContinueModify) {
                        //system("clear");
                        enterBack();
                        showDoctorPage();
                        return;
                    } else {
                        system("clear");
                        continue;
                    }
                }
            }
        }//修改用户信息
        case EXIT:
            exit(0);
        default:
            cout << "输入错误，请重新输入： ";
            break;
    }
}

#define ADD_HAO 1
#define SHOW_ALL_GUAHAO 2
#define ADD_DOCTOR 3
#define SHOW_ALL_USERS 4
#define DELETE_ANY_USER 5
#define ADD_HAO_TYPE 6

//管理员界面
void showAdminPage() {
    cout << "添加号 1" << endl;
    cout << "查询所有挂号信息 2" << endl;
    cout << "添加医生 3" << endl;
    cout << "查看所有用户信息 4" << endl;
    cout << "注销任意账户 5" << endl;
    cout << "添加号类型 6" << endl;
    cout << "退出系统 -1" << endl;

    int functionId;
    cin >> functionId;
    system("clear");
    User *currentUser = LoginInfo::getCurrentUser();
    switch (functionId) {
        case ADD_HAO: {
            int haoTypeId;
            int amount;
            cout << "请输入想要添加的号:" << endl;
            cout << "类型id: ";
            cin >> haoTypeId;
            cout << "数量: ";
            cin >> amount;
            Hao::addHao(haoTypeId, amount);
            enterBack();
            showAdminPage();
            return;
        } //添加号
        case SHOW_ALL_GUAHAO: {
            Hao::showAllGuahao();
            enterBack();
            showAdminPage();
            return;
        } //查询所有挂号信息
        case ADD_DOCTOR: {
            bool isSet = Doctor::setCommonUserToDoctor();
            if (isSet) {
                cout << "添加成功！" << endl;
            }
            enterBack();
            showAdminPage();
            return;
        }//添加医生
        case SHOW_ALL_USERS: {
            User::showAllUserInfo();
            enterBack();
            showAdminPage();
            return;
        }//查看所有用户信息
        case DELETE_ANY_USER: { //注销任意账户
            string phoneNumber;
            cout << "请输入想要删除用户的手机号码: ";
            cin >> phoneNumber;
            bool isDeleted = User::deleteUserByPhoneNumber(phoneNumber);
            if (isDeleted) {
                cout << "删除成功！" << endl;
            }
            enterBack();
            showAdminPage();
            return;
        }
        case ADD_HAO_TYPE: { //添加号类型
            HaoType::CreateHaoType();
            enterBack();
            showAdminPage();
            return;
        }
        case EXIT:  //退出系统
            exit(0);
        default:
            cout << "输入错误，请重新输入： ";
            break;
    }
}
