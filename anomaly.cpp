#include "anomaly.h"
#include <fstream>
#include <cstring>
#include <ctime>

void AnomalyDetection::saveUserProfiles() {
    ofstream fout("profiles.txt", ios::app);
    fout << "[PROFILE] End of Session Snapshot\n";
    fout.close();
}

void AnomalyDetection::trackLoginActivity(const char* username, bool success, const char* timestamp) {
    ofstream fout("login_activity.txt", ios::app);
    fout << "[" << timestamp << "] " << username << (success ? " LOGIN_SUCCESS" : " LOGIN_FAIL") << "\n";
    fout.close();
}

void AnomalyDetection::trackTaskReassignment(const char* username, const char* taskName, const char* oldAssignee, const char* newAssignee) {
    ofstream fout("reassign_log.txt", ios::app);
    fout << "[" << username << "] changed task '" << taskName << "' from " << oldAssignee << " to " << newAssignee << "\n";
    fout.close();
}

void AnomalyDetection::generateAnomalyReport(char* report, int size) {
    ifstream fin("audit.txt");
    int loginFails = 0;
    char line[256];
    while (fin.getline(line, sizeof(line))) {
        bool failedCreds = false;
        bool failedOtp = false;

        // Manual strstr for "FAILED_CREDENTIALS"
        const char* keyword1 = "FAILED_CREDENTIALS";
        for (int i = 0; line[i] != '\0'; ++i) {
            int j = 0;
            while (line[i + j] != '\0' && keyword1[j] != '\0' && line[i + j] == keyword1[j]) {
                ++j;
            }
            if (keyword1[j] == '\0') {
                failedCreds = true;
                break;
            }
        }

        // Manual strstr for "FAILED_OTP"
        const char* keyword2 = "FAILED_OTP";
        for (int i = 0; line[i] != '\0'; ++i) {
            int j = 0;
            while (line[i + j] != '\0' && keyword2[j] != '\0' && line[i + j] == keyword2[j]) {
                ++j;
            }
            if (keyword2[j] == '\0') {
                failedOtp = true;
                break;
            }
        }

        if (failedCreds || failedOtp) {
            loginFails++;
        }
    }
    fin.close();

    // Manual strncpy replacement
    const char* resultMsg = (loginFails >= 3)
        ? "Multiple failed login attempts detected. Possible brute force.\n"
        : "No significant anomalies detected.\n";

    // Copy resultMsg manually to report
    int i = 0;
    while (resultMsg[i] != '\0' && i < size - 1) {
        report[i] = resultMsg[i];
        ++i;
    }
    report[i] = '\0';  // Null-terminate

}

void AnomalyDetection::analyzeAuditLog() {
    char report[256];
    generateAnomalyReport(report, sizeof(report));
    cout << "\n--- Anomaly Report ---\n" << report << "\n";
}
