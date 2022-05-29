#pragma once

#include "Department.h"
#include "Database/Mysql.h"

#define MAX_SQL_LENGTH 300

#include <string>

using namespace std;

class Department {
private:
    int id;
    string name;
    string address;

public:
    Department();

    static Department *getDepartmentById(int departmentId);

    static void showAllDepartments();

    string getName() {
        return this->name;
    }

    string getAddress() {
        return this->address;
    }
};

Department *Department::getDepartmentById(int departmentId) {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from department where id=%d", departmentId);
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            MYSQL_ROW row = mysql_fetch_row(result);
            Department *department;
            department->id = atoi(row[0]);
            department->name = row[1];
            department->address = row[2];
            return department;
        }
    }
    return NULL;
}

void Department::showAllDepartments() {
    MYSQL *Cur = connectDb();
    MYSQL_RES *result;  //Defines the result of the select
    char sql[MAX_SQL_LENGTH];
    sprintf(sql, "select * from department");
    int queryRes = mysql_query(Cur, sql);    //执行sql语句
    if (queryRes) {
        // error
    } else { // query succeeded, process any data returned by it
        result = mysql_store_result(Cur);
        if (result) { // there are rows
            MYSQL_ROW row;
            Department *department = new Department();
            while ((row = mysql_fetch_row(result))) {
                department->id = atoi(row[0]);
                department->name = row[1];
                department->address = row[2];
                cout << to_string(department->id) + "\t";
                cout << department->name + "\t";
                cout << department->address + "\n";
            }
        }
    }
}

Department::Department() {

}
