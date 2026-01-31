#include "ui.h"
#include <iostream>
#include <fstream>
using namespace std;

void UserInterface::showMainMenu(Employee* user) {
    int choice = -1;
    char uname[50];
    manual_strcpy(uname, user->getUsername());
    int userClearance = user->getClearance();

  
    NotificationManager::getInstance().displayAllFor(uname);
    while (choice != 0) {
        system("cls");

        cout << "\033[38;5;196m=============================================\033[0m\n";
        cout << "\033[48;5;19m\033[38;5;15m         SECURE EMPLOYEE MANAGEMENT SYSTEM    \033[0m\n";
        cout << "\033[38;5;196m=============================================\033[0m\n";
        cout << "\033[38;5;226mLogged in as: \033[38;5;82m" << uname << "  \033[38;5;226m|  Level: \033[38;5;82m" << userClearance << "\033[0m\n";
        cout << "\033[38;5;196m---------------------------------------------\033[0m\n";
        cout << "\033[48;5;19m\033[38;5;15m               MAIN MENU                     \033[0m\n";
        cout << "\033[38;5;196m---------------------------------------------\033[0m\n";
        cout << "\033[38;5;46m 1. View Inbox\033[0m\n";
        cout << "\033[38;5;46m 2. Send Message\033[0m\n";
        cout << "\033[38;5;46m 3. Create a Task\033[0m\n";
        cout << "\033[38;5;46m 4. Manage Task\033[0m\n";
        cout << "\033[38;5;46m 5. View Audit Logs\033[0m\n";
        cout << "\033[38;5;46m 6. View Performance Report\033[0m\n";
        cout << "\033[38;5;46m 7. View Anomaly Report\033[0m\n";
        if (PolicyEngine::hasAccess(userClearance, Action::SEND_GLOBAL_NOTIFICATION))
            cout << "\033[38;5;46m 8. Send Global Notification\033[0m\n";
        if (PolicyEngine::hasAccess(userClearance, Action::DELETE_GLOBAL_NOTIFICATION))
            cout << "\033[38;5;46m 9. Delete Global Notification\033[0m\n";
        cout << "\033[38;5;196m---------------------------------------------\033[0m\n";
        cout << "\033[38;5;196m 0. Logout\033[0m\n";
        cout << "\033[38;5;196m=============================================\033[0m\n";
        cout << "\033[38;5;226mEnter your choice: \033[0m";

        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\033[38;5;196m\nInvalid input. Please enter a number.\033[0m\n";
            pause();
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice) {
        case 1:
            if (PolicyEngine::hasAccess(userClearance, Action::VIEW_INBOX))
                viewInbox(uname);
            else
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            break;
        case 2:
            if (PolicyEngine::hasAccess(userClearance, Action::SEND_MESSAGE))
                sendMessage(uname, userClearance, user->getPerformance());
            else
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            break;
        case 3:
            if (PolicyEngine::hasAccess(userClearance, Action::CREATE_TASK))
                createTask(uname, userClearance, user->getPerformance());
            else
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            break;
        case 4:
            if (PolicyEngine::hasAccess(userClearance, Action::MANAGE_TASK))
                manageTask(uname, userClearance, user->getPerformance());
            else
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            break;
        case 5:
            if (PolicyEngine::hasAccess(userClearance, Action::VIEW_AUDIT_LOGS))
                viewAuditLogs(uname);
            else
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            break;
        case 6:
            if (PolicyEngine::hasAccess(userClearance, Action::VIEW_PERFORMANCE_REPORT))
                viewPerformanceReport(user->getPerformance());
            else
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            break;
        case 7:
            if (userClearance >= ClearanceLevel::EXECUTIVE)
                viewAnomalyReport(uname);
            else
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            break;
        case 8:
            if (PolicyEngine::hasAccess(userClearance, Action::SEND_GLOBAL_NOTIFICATION)) {
                char message[500];
                cout << "Enter global notification message: ";
                cin.getline(message, 500);
                NotificationManager::getInstance().broadcast(uname, message);
                cout << "Global notification sent successfully.\n";
            }
            else {
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            }
            break;
        case 9:
            if (PolicyEngine::hasAccess(userClearance, Action::DELETE_GLOBAL_NOTIFICATION)) {
                int lineNumber;
                cout << "Enter the line number of the notification to delete: ";
                cin >> lineNumber;
                if (NotificationManager::getInstance().deleteEntry(lineNumber)) {
                    cout << "Notification deleted successfully.\n";
                }
                else {
                    cout << "Failed to delete notification.\n";
                }
            }
            else {
                cout << "\033[38;5;196mAccess Denied: Insufficient clearance level.\033[0m\n";
            }
            break;
        case 0:
            cout << "Logging out...\n";
            AuditLogger::log(uname, "LOGOUT", "SUCCESS");
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }

        if (choice != 0) pause();
    }
}


void UserInterface::viewInbox(const char* username) {
    char filename[64] = "inbox_";
    manual_strcat(filename, username);
    manual_strcat(filename, ".txt");

    ifstream fin(filename);
    if (!fin) {
        cout << "No inbox found.\n";
        pause();
        return;
    }

    cout << "\n\033[48;5;19m\033[38;5;15m--- Inbox for " << username << " ---\033[0m\n";
    char line[300];
    while (fin.getline(line, sizeof(line))) {
        cout << line << "\n";
    }

    fin.close();
    pause();
}

void UserInterface::sendMessage(const char* username, int clearance, PerformanceReview* performance) {
    char receiver[50], msgContent[500], msgType[10];
    Message* msg = nullptr;

    cout << "\n\033[48;5;19m\033[38;5;15m-- Send Message --\033[0m\n";
    cout << "Enter recipient username: ";
    cin.getline(receiver, 50);

    cout << "Enter message type (INFO / PRIVATE / ALERT): ";
    cin.getline(msgType, 10);

    cout << "Enter message content: ";
    cin.getline(msgContent, 500);

    int type;
    if (manual_strcmp(msgType, "INFO") == 0) type = MessageType::INFO;
    else if (manual_strcmp(msgType, "PRIVATE") == 0) type = MessageType::PRIVATE;
    else if (manual_strcmp(msgType, "ALERT") == 0) type = MessageType::ALERT;
    else {
        cout << "Invalid message type!\n";
        return;
    }

    int requiredLevel = (type == MessageType::ALERT) ? ClearanceLevel::MANAGER : ClearanceLevel::JUNIOR;
    if (!PolicyEngine::hasAccess(clearance, requiredLevel)) {
        cout << "\033[38;5;196mAccess Denied: You don't have clearance to send this message type.\033[0m\n";
        return;
    }

    if (type == MessageType::PRIVATE)
        msg = new PrivateMessage(username, receiver, msgContent);
    else if (type == MessageType::INFO)
        msg = new InfoMessage(username, receiver, msgContent);
    else
        msg = new AlertMessage(username, receiver, msgContent);

    msg->saveToFile();
    cout << "\033[38;5;46mMessage sent successfully.\033[0m\n";
    AuditLogger::log(username, "SEND_MESSAGE", "SUCCESS");
    performance->sendMessage();

    delete msg;
    pause();
}

void UserInterface::viewAuditLogs(const char* username) {
    ifstream fin("audit.txt");
    if (!fin) {
        cout << "No audit logs found.\n";
        pause();
        return;
    }

    cout << "\n--- Audit Logs ---\n";
    char line[300];
    while (fin.getline(line, sizeof(line))) {
        cout << line << "\n";
    }

    fin.close();
    AuditLogger::log(username, "VIEW_AUDIT_LOGS", "SUCCESS");
    pause();
}

void UserInterface::viewPerformanceReport(PerformanceReview* performance) {
    cout << *performance;
    pause();
}

void UserInterface::viewAnomalyReport(const char* username) {
    cout << "\n\033[48;5;201m\033[38;5;15m--- Anomaly Detection Report ---\033[0m\n";
    AnomalyDetection::analyzeAuditLog();

    char report[4096];
    AnomalyDetection::generateAnomalyReport(report, sizeof(report));

    cout << report;
    AuditLogger::log(username, "VIEW_ANOMALY_REPORT", "SUCCESS");
    pause();
}

void UserInterface::pause() {
    cout << "\n\033[38;5;226mPress Enter to return to the menu..\033[0m";
    cin.get();
}