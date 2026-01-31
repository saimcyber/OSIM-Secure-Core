#include "time_manager.h"
#include "string_utils.h"
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;

bool TimeManager::isTaskExpired(const char* taskCreationTime, const char* ttl) {
    if (!ttl || strlen(ttl) == 0) {
        return false; // No TTL specified means task doesn't expire
    }

    time_t expirationTime = calculateExpirationTime(taskCreationTime, ttl);
    time_t currentTime = time(0);

    return currentTime > expirationTime;
}

int TimeManager::convertTTLToSeconds(const char* ttl) {
    if (!ttl || strlen(ttl) == 0) {
        return 0; // Default: no expiration
    }

    int value = 0;
    char unit = '\0';

    // Parse the TTL string (e.g., "3d" for 3 days, "2h" for 2 hours)
    if (strlen(ttl) >= 2) {
        // Get the numerical value
        value = atoi(ttl);

        // Get the unit character (last character)
        unit = ttl[strlen(ttl) - 1];
    }
    else {
        // If format is incorrect, default to seconds
        return atoi(ttl);
    }

    switch (unit) {
    case 'd': // days
        return value * 24 * 60 * 60;
    case 'h': // hours
        return value * 60 * 60;
    case 'm': // minutes
        return value * 60;
    case 's': // seconds
        return value;
    default:  // Default to seconds if unit is not recognized
        return value;
    }
}

time_t TimeManager::calculateExpirationTime(const char* creationTime, const char* ttl) {
    struct tm timeinfo = { 0 };
    int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;

    // Alternative parsing method without using sscanf
    // Format: YYYY-MM-DD HH:MM:SS
    string timeStr(creationTime);

    // Parse date part (YYYY-MM-DD)
    if (timeStr.length() >= 10) {
        year = manual_atoi(timeStr.substr(0, 4).c_str());
        month = manual_atoi(timeStr.substr(5, 2).c_str());
        day = manual_atoi(timeStr.substr(8, 2).c_str());
    }

    // Parse time part (HH:MM:SS) if available
    if (timeStr.length() >= 19) {
        hour = manual_atoi(timeStr.substr(11, 2).c_str());
        min = manual_atoi(timeStr.substr(14, 2).c_str());
        sec = manual_atoi(timeStr.substr(17, 2).c_str());
    }

    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = min;
    timeinfo.tm_sec = sec;

    time_t creationTimeT = mktime(&timeinfo);
    int ttlSeconds = convertTTLToSeconds(ttl);

    return creationTimeT + ttlSeconds;
}

string TimeManager::getFormattedTime(time_t timeValue) {
    struct tm timeinfo;
    localtime_s(&timeinfo, &timeValue);

    stringstream ss;
    ss << setfill('0')
        << setw(4) << (timeinfo.tm_year + 1900) << "-"
        << setw(2) << (timeinfo.tm_mon + 1) << "-"
        << setw(2) << timeinfo.tm_mday << " "
        << setw(2) << timeinfo.tm_hour << ":"
        << setw(2) << timeinfo.tm_min << ":"
        << setw(2) << timeinfo.tm_sec;

    return ss.str();
}

string TimeManager::getCurrentTimeString() {
    time_t now = time(0);
    return getFormattedTime(now);
}
