#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <string>
#include "task.h"
#include "performance.h"
#include "audit.h"
#include "anomaly.h"
#include "string_utils.h"
#include "ui.h"
#include "time_manager.h"
using namespace std;


void UserInterface::createTask(const char* username, int clearance, PerformanceReview* performance) {
    cout << "\n-- Create Task --\n";
    char name[100], task[200], deadline[30], priority[10], assignNow[10], assignee[50] = "Unassigned";
    char ttl[10] = ""; // TTL field

    time_t now = time(0);
    struct tm taskCreateTime;
    localtime_s(&taskCreateTime, &now);

    cout << "Enter task name: ";
    cin.getline(name, 100);

    cout << "Enter task description: ";
    cin.getline(task, 200);

    cout << "Enter deadline (e.g. 2025-05-10): ";
    cin.getline(deadline, 30);

    do {
        cout << "Enter priority (High/Medium/Low): ";
        cin.getline(priority, 10);
    } while (manual_strcmp(priority, "High") != 0 && manual_strcmp(priority, "Medium") != 0 && manual_strcmp(priority, "Low") != 0);

    // Add TTL input
    cout << "Enter Time-To-Live (e.g., 3d, 24h, 60m, leave empty for no expiration): ";
    cin.getline(ttl, 10);

    cout << "Assign task now? (yes/no): ";
    cin.getline(assignNow, 10);
    if (manual_strcmp(assignNow, "yes") == 0 || manual_strcmp(assignNow, "Yes") == 0) {
        cout << "Enter assignee username: ";
        cin.getline(assignee, 50);

        // Track task assignment in performance
        performance->assignTask();
    }

    // Store task creation time
    char creationTimeStr[30];
    strftime(creationTimeStr, sizeof(creationTimeStr), "%Y-%m-%d %H:%M:%S", &taskCreateTime);

    ofstream fout("task.txt", ios::app);
    if (!fout) {
        cout << "Error opening task.txt\n";
        return;
    }

    fout << username << "|" << assignee << "|" << name << "|" << task << "|"
        << deadline << "|Created|" << priority << "|" << creationTimeStr << "|" << ttl << "\n";
    fout.close();

    cout << "\033[38;5;46mTask created successfully!\033[0m\n";

    // If TTL is specified, show expiration time
    if (ttl[0] != '\0') {
        time_t expirationTime = TimeManager::calculateExpirationTime(creationTimeStr, ttl);
        string formattedExpiration = TimeManager::getFormattedTime(expirationTime);
        cout << "Task will expire on: " << formattedExpiration << "\n";
    }

    AuditLogger::log(username, "CREATE_TASK", "SUCCESS");
    pause();
}

void UserInterface::manageTask(const char* username, int clearance, PerformanceReview* performance) {
    ifstream fin("task.txt");
    if (!fin) {
        cout << "Could not open task.txt\n";
        return;
    }

    const int MAX_TASKS = 100;
    char tasks[MAX_TASKS][9][200];  // 9 fields per task (including TTL)
    int totalTasks = 0;

    cout << "\n\033[48;5;19m\033[38;5;15m--- All Tasks ---\033[0m\n";
    char line[1000];
    int index = 1;

    while (fin.getline(line, sizeof(line)) && totalTasks < MAX_TASKS) {
        int field = 0, j = 0;
        for (int i = 0; line[i] != '\0'; ++i) {
            if (line[i] == '|') {
                tasks[totalTasks][field][j] = '\0';
                field++;
                j = 0;
            }
            else {
                tasks[totalTasks][field][j++] = line[i];
            }
        }
        tasks[totalTasks][field][j] = '\0';

        // Check if task has expired (if it has TTL)
        bool isExpired = false;
        char status[20];
        manual_strcpy(status, tasks[totalTasks][5]);

        // Handle TTL field (field 8 is TTL)
        if (field >= 8 && tasks[totalTasks][8][0] != '\0') {
            isExpired = TimeManager::isTaskExpired(tasks[totalTasks][7], tasks[totalTasks][8]);
            if (isExpired && manual_strcmp(status, "Completed") != 0 && manual_strcmp(status, "Expired") != 0) {
                manual_strcpy(tasks[totalTasks][5], "Expired");
            }
        }

        // Display task information
        if (field >= 6) {
            cout << "\033[38;5;46m" << index++ << ". Creator: " << tasks[totalTasks][0]
                << ", Assignee: " << tasks[totalTasks][1]
                << ", Name: " << tasks[totalTasks][2]
                << ", Task: " << tasks[totalTasks][3]
                << ", Deadline: " << tasks[totalTasks][4]
                << ", Status: " << tasks[totalTasks][5] << "\033[0m";
            if (field >= 8 && tasks[totalTasks][8][0] != '\0') {
                cout << "\033[38;5;226m, TTL: " << tasks[totalTasks][8] << "\033[0m";
                if (isExpired) {
                    cout << "\033[38;5;196m (EXPIRED)\033[0m";
                }
                else {
                    time_t expirationTime = TimeManager::calculateExpirationTime(tasks[totalTasks][7], tasks[totalTasks][8]);
                    string formattedExpiration = TimeManager::getFormattedTime(expirationTime);
                    cout << "\033[38;5;226m (Expires: " << formattedExpiration << ")\033[0m";
                }
            }
            cout << "\n";
            totalTasks++;
        }
    }
    fin.close();

    // Write back all tasks to update any expired statuses
    ofstream foutUpdate("task.txt");
    if (!foutUpdate) {
        cout << "Failed to update task statuses\n";
        return;
    }

    for (int t = 0; t < totalTasks; t++) {
        foutUpdate << tasks[t][0] << "|" << tasks[t][1] << "|" << tasks[t][2] << "|"
            << tasks[t][3] << "|" << tasks[t][4] << "|" << tasks[t][5] << "|" << tasks[t][6] << "|" << tasks[t][7];

        if (tasks[t][8][0] != '\0') {
            foutUpdate << "|" << tasks[t][8];
        }

        foutUpdate << "\n";
    }
    foutUpdate.close();

    if (totalTasks == 0) {
        cout << "No tasks available.\n";
        return;
    }

    int taskIndex;
    cout << "\nSelect a task to manage (0 to cancel): ";
    cin >> taskIndex;
    cin.ignore();

    if (taskIndex <= 0 || taskIndex > totalTasks) {
        cout << "Invalid selection. Returning to main menu...\n";
        return;
    }

    // Check if selected task is expired
    bool isSelectedTaskExpired = false;
    if (tasks[taskIndex - 1][8][0] != '\0') {
        isSelectedTaskExpired = TimeManager::isTaskExpired(tasks[taskIndex - 1][7], tasks[taskIndex - 1][8]);
        if (isSelectedTaskExpired && manual_strcmp(tasks[taskIndex - 1][5], "Completed") != 0) {
            cout << "This task has expired and cannot be modified.\n";
            pause();
            return;
        }
    }

    char* selectedTask[9];
    for (int i = 0; i < 9; i++) {
        selectedTask[i] = tasks[taskIndex - 1][i];
    }

    cout << "\n--- Manage Task: " << selectedTask[2] << " ---\n";
    cout << "1. Change Status\n";
    cout << "2. Change Deadline\n";
    cout << "3. Change Assignee\n";
    cout << "4. Mark as Completed\n";
    cout << "5. Change TTL\n";
    cout << "Enter your choice: ";

    int opt;
    cin >> opt;
    cin.ignore();

    performance->manageTask();

    char oldAssignee[50];
    manual_strcpy(oldAssignee, selectedTask[1]);

    switch (opt) {
    case 1:
        cout << "Enter new status: ";
        cin.getline(selectedTask[5], 200);
        break;
    case 2:
        cout << "Enter new deadline: ";
        cin.getline(selectedTask[4], 200);
        break;
    case 3:
        cout << "Enter new assignee username: ";
        cin.getline(selectedTask[1], 200);

        if (manual_strcmp(oldAssignee, selectedTask[1]) != 0) {
            AnomalyDetection::trackTaskReassignment(username, selectedTask[2], oldAssignee, selectedTask[1]);
        }
        break;
    case 4:
        manual_strcpy(selectedTask[5], "Completed");

        if (manual_strcmp(selectedTask[1], username) == 0) {
            time_t now = time(0);
            struct tm taskDeadline = { 0 };
            int year = 0, month = 0, day = 0;

            // Alternative parsing method without using sscanf
            std::string deadlineStr(selectedTask[4]);
            if (deadlineStr.length() >= 10) {
                year = atoi(deadlineStr.substr(0, 4).c_str());
                month = atoi(deadlineStr.substr(5, 2).c_str());
                day = atoi(deadlineStr.substr(8, 2).c_str());
            }

            taskDeadline.tm_year = year - 1900;
            taskDeadline.tm_mon = month - 1;
            taskDeadline.tm_mday = day;

            time_t deadlineTime = mktime(&taskDeadline);
            bool onTime = (now <= deadlineTime);

            performance->completeTask(onTime);

            cout << "Task marked as completed " << (onTime ? "on time!" : "late.") << "\n";
        }
        break;
    case 5:
        cout << "Enter new TTL (e.g., 3d, 24h, 60m, leave empty for no expiration): ";
        cin.getline(selectedTask[8], 10);

        if (selectedTask[8][0] != '\0') {
            time_t expirationTime = TimeManager::calculateExpirationTime(selectedTask[7], selectedTask[8]);
            string formattedExpiration = TimeManager::getFormattedTime(expirationTime);
            cout << "Task will now expire on: " << formattedExpiration << "\n";
        }
        else {
            cout << "Task will not expire (TTL removed)\n";
        }
        break;
    default:
        cout << "Invalid option.\n";
        return;
    }

    ofstream fout("task.txt");
    if (!fout) {
        cout << "Failed to write to task.txt\n";
        return;
    }

    for (int t = 0; t < totalTasks; t++) {
        fout << tasks[t][0] << "|" << tasks[t][1] << "|" << tasks[t][2] << "|"
            << tasks[t][3] << "|" << tasks[t][4] << "|" << tasks[t][5] << "|" << tasks[t][6] << "|" << tasks[t][7];

        if (tasks[t][8][0] != '\0') {
            fout << "|" << tasks[t][8];
        }

        fout << "\n";
    }
    fout.close();

    cout << "Task updated successfully.\n";
    pause();
}