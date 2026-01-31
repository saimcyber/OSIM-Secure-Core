#ifndef ANOMALY_H
#define ANOMALY_H

#include <iostream>
using namespace std;

class AnomalyDetection {
public:
    static void saveUserProfiles();
    static void trackLoginActivity(const char* username, bool success, const char* timestamp);
    static void trackTaskReassignment(const char* username, const char* taskName, const char* oldAssignee, const char* newAssignee);
    static void generateAnomalyReport(char* report, int size);
    static void analyzeAuditLog();
};

#endif // ANOMALY_H
