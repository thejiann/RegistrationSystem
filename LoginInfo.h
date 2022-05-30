#pragma once
#include "User/User.h"

class User;

//登录信息类
class LoginInfo {
public:
    static bool Status;
    static User *CurrentUser;

public:
    static bool getLoginStatus() {
        return Status;
    }

    static User *getCurrentUser() {
        return CurrentUser;
    }

    static void setLoginInfo(bool status, User *currentUser) {
        Status = status;
        CurrentUser = currentUser;
    }
};

 //初始化类的静态成员
User *LoginInfo::CurrentUser = NULL;
bool LoginInfo::Status = false;
