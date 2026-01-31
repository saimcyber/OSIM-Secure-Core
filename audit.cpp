#include "audit.h"
#include "notification.h"
#include "string_utils.h"
#include <fstream>
#include <ctime>

void AuditLogger::log(const char* username, const char* action, const char* status) {
    ofstream fout("audit.txt", ios::app);
    if (!fout) {
        cout << "[ERROR] Could not open audit.txt" << endl;
        return;
    }

    char timestamp[30];
    NotificationManager::getInstance().getCurrentTimestamp(timestamp, sizeof(timestamp));

    fout << "[" << timestamp << "] " << username << " " << action << " " << status;
    if (manual_strcmp(action, "LOGIN") == 0) {
        fout << ((manual_strcmp(status, "SUCCESS") == 0) ? " +" : " -");
    }
    fout << endl;
    fout.close();
}
