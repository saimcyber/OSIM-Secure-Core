#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "performance.h"
#include "string_utils.h"
using namespace std;

class Employee {
protected:
    char* username;
    char* hashedPassword;
    int clearanceLevel;
    PerformanceReview* performance;

public:
    Employee(const char* uname = "", const char* pass = "", int clearance = 0);
    virtual ~Employee();
    PerformanceReview* getPerformance() const;
    const char* getUsername() const;
    int getClearance() const;
    void savePerformanceData() const;
    void loadPerformanceData();
    virtual void viewDashboard() = 0;
};

class Junior : public Employee {
public:
    Junior(const char* uname = "", const char* pass = "");
    void viewDashboard() override;
};

class Manager : public Employee {
public:
    Manager(const char* uname = "", const char* pass = "");
    void viewDashboard() override;
};

class Director : public Employee {
public:
    Director(const char* uname = "", const char* pass = "");
    void viewDashboard() override;
};

class Executive : public Employee {
public:
    Executive(const char* uname = "", const char* pass = "");
    void viewDashboard() override;
};

#endif // EMPLOYEE_H
