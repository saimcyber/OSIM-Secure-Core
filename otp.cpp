#include "otp.h"

int generateOTP() {
    srand(static_cast<unsigned int>(time(0)));
    return rand() % 900000 + 100000;
}

bool verifyOTP(int generatedOTP, int timeoutSeconds) {
    time_t startTime, endTime;
    int enteredOTP;

    cout << "\n\033[38;5;82m[SECURE] Your OTP is: " << generatedOTP << "\033[0m\n";
    cout << "\033[5m\033[38;5;226mEnter OTP within " << timeoutSeconds << " seconds: \033[0m";

    time(&startTime);
    cin.clear();
    cin >> enteredOTP;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\033[38;5;196m\n Invalid OTP format. Login denied.\033[0m\n";

        return false;
    }

    time(&endTime);
    double elapsed = difftime(endTime, startTime);

    if (elapsed > timeoutSeconds) {
        cout << "\033[38;5;196m\nOTP expired. Login denied.\033[0m\n";
        return false;
    }

    if (enteredOTP == generatedOTP) {
        cout << "\033[38;5;46m\n OTP verified successfully!\033[0m\n";
        return true;
    }
    else {
        cout << "\033[38;5;196m\n Incorrect OTP. Login denied.\033[0m\n";
        return false;
    }
}
