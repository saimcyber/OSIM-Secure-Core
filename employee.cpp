#include "employee.h"
#include "cstring"
#include "string"
#include <fstream>

Employee::Employee(const char* uname, const char* pass, int clearance) {
    username = new char[strlen(uname) + 1];
    manual_strcpy(username, uname);
    hashedPassword = new char[strlen(pass) + 1];
    manual_strcpy(hashedPassword, pass);
    clearanceLevel = clearance;
    performance = new PerformanceReview(uname);
    loadPerformanceData();
}

PerformanceReview* Employee::getPerformance() const {
    return performance;
}

const char* Employee::getUsername() const {
    return username;
}

int Employee::getClearance() const {
    return clearanceLevel;
}

void Employee::savePerformanceData() const {
    char filename[64] = "perf_";
    manual_strcat(filename, username);
    manual_strcat(filename, ".txt");

    ofstream fout(filename);
    if (fout) {
        fout << *performance;
        fout.close();
    }
}

void Employee::loadPerformanceData() {
    char filename[64] = "perf_";
    manual_strcat(filename, username);
    manual_strcat(filename, ".txt");

    ifstream fin(filename);
    if (fin) {
        fin.close(); // Parsing skipped as noted
    }
}

Employee::~Employee() {
    savePerformanceData();
    delete[] username;
    delete[] hashedPassword;
    delete performance;
}

Junior::Junior(const char* uname, const char* pass)
    : Employee(uname, pass, 1) {
}

void Junior::viewDashboard() {
    cout << "Junior Dashboard\n";
}

Manager::Manager(const char* uname, const char* pass)
    : Employee(uname, pass, 2) {
}

void Manager::viewDashboard() {
    cout << "Manager Dashboard\n";
}

Director::Director(const char* uname, const char* pass)
    : Employee(uname, pass, 3) {
}

void Director::viewDashboard() {
    cout << "Director Dashboard\n";
}

Executive::Executive(const char* uname, const char* pass)
    : Employee(uname, pass, 4) {
}

void Executive::viewDashboard() {
    cout << "Executive Dashboard\n";
}
