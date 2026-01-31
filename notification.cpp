#include "notification.h"
#include "limits"

NotificationManager::NotificationManager() {}

NotificationManager& NotificationManager::getInstance() {
    static NotificationManager instance;
    return instance;
}

void NotificationManager::persist(const char* line) {
    ofstream fout("global_notifications.txt", ios::app);
    if (fout) {
        fout << line << "\n";
        fout.close();
    }
}

void NotificationManager::getCurrentTimestamp(char* buffer, int size) {
    time_t now = time(0);
    struct tm ltm;
#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    ltm = *localtime(&now);
#endif
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", &ltm);
}

void NotificationManager::broadcast(const char* sender, const char* message) {
    char timestamp[30];
    getCurrentTimestamp(timestamp, sizeof(timestamp));

    char line[512];
    manual_strcpy(line, "[");
    manual_strcat(line, timestamp);
    manual_strcat(line, "]|");
    manual_strcat(line, sender);
    manual_strcat(line, "|");
    manual_strcat(line, message);

    persist(line);
}

void NotificationManager::displayAllFor(const char* username) {
    ifstream fin("global_notifications.txt");
    if (!fin) return;

    cout << "\n\033[5m\033[48;5;196m\033[38;5;15m--- GLOBAL NOTIFICATIONS ---\033[0m\n";
    char line[512];
    while (fin.getline(line, sizeof(line))) {
        cout << line << "\n";
    }
    fin.close();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\033[38;5;226mPress Enter to continue...\033[0m";;
    cin.get();
}

bool NotificationManager::deleteEntry(int lineNumber) {
    ifstream fin("global_notifications.txt");
    if (!fin) return false;

    char lines[100][512];
    int totalLines = 0;
    char line[512];

    while (fin.getline(line, sizeof(line)) && totalLines < 100) {
        manual_strcpy(lines[totalLines++], line);
    }
    fin.close();

    if (lineNumber <= 0 || lineNumber > totalLines) return false;

    ofstream fout("global_notifications.txt");
    if (!fout) return false;

    for (int i = 0; i < totalLines; ++i) {
        if (i != lineNumber - 1) fout << lines[i] << "\n";
    }
    fout.close();
    return true;
}
