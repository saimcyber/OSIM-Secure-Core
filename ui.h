#ifndef UI_H
#define UI_H

#include "employee.h"
#include "message.h"
#include "audit.h"
#include "notification.h"
#include "policy_engine.h"
#include "anomaly.h"

class UserInterface {
public:
    void showMainMenu(Employee* user);
    void createTask(const char* username, int clearance, PerformanceReview* performance);
    void manageTask(const char* username, int clearance, PerformanceReview* performance);

private:
    void viewInbox(const char* username);
    void sendMessage(const char* username, int clearance, PerformanceReview* performance);
    void viewAuditLogs(const char* username);
    void viewPerformanceReport(PerformanceReview* performance);
    void viewAnomalyReport(const char* username);
    void pause();
};

#endif