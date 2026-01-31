#include "otp.h"
#include "string_utils.h"
#include "policy_engine.h"
#include "notification.h"
#include "performance.h"
#include "employee.h"
#include "message.h"
#include "credential.h"
#include "audit.h"
#include "anomaly.h"
#include "ui.h"
#include "task.h"
using namespace std;

int main() 
{
    cout << "\n\n\t\t"<< "\033[48;5;196m"<< "************ Organizational Simulation and Internal Management System! ************"<< "\033[0m\n";
    cout << "\t\t"<< "\033[48;5;30m"<< "************* SIMULATE WORKFORCE, MANAGE RESOURCES, DRIVE SUCCESS! *********"<< "\033[0m\n\n";
    cout << "\t\t"<< "\033[38;5;46m"<< "* Multi-Factor Authentication Enabled *"<< "\033[0m\n";cout << "\t\t"<< "\033[38;5;226m"<< "* Anomaly Detection Activated *"<< "\033[0m\n\n";

    CredentialManager cm;
    Employee* user = cm.loginSystem();

    if (user) {
        cout << "\n Authentication complete. Welcome to the system!\n";
        UserInterface ui;
        ui.showMainMenu(user);

        delete user;
    } else {
        cout << "\n Authentication failed. System access denied.\n";
    }

    AnomalyDetection::saveUserProfiles();

    cout << "\n\033[38;5;196m===================================\033[0m\n";
    cout << "\033[48;5;196m\033[38;5;15mSYSTEM SESSION TERMINATED\033[0m\n";
    cout << "\033[38;5;196m===================================\033[0m\n";

    return 0;
}
