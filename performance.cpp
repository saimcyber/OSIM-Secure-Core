#include "performance.h"

PerformanceReview::PerformanceReview(const char* name)
    : tasksCompleted(0), deadlinesMissed(0), messagesSent(0),
    tasksAssigned(0), tasksManaged(0), averageResponseTime(0), totalResponseTimeEvents(0) {
    manual_strcpy(employeeName, name);
}

void PerformanceReview::completeTask(bool onTime) {
    tasksCompleted++;
    if (!onTime) deadlinesMissed++;
}

void PerformanceReview::sendMessage() {
    messagesSent++;
}

void PerformanceReview::assignTask() {
    tasksAssigned++;
}

void PerformanceReview::manageTask() {
    tasksManaged++;
}

void PerformanceReview::recordResponseTime(float hours) {
    averageResponseTime = (averageResponseTime * totalResponseTimeEvents + hours) /
        (totalResponseTimeEvents + 1);
    totalResponseTimeEvents++;
}

float PerformanceReview::getScore() const {
    float taskScore = tasksCompleted * 10 - deadlinesMissed * 5;
    float communicationScore = messagesSent * 2;
    float managementScore = tasksAssigned * 3 + tasksManaged * 4;

    float responseBonus = 0;
    if (totalResponseTimeEvents > 0) {
        responseBonus = (24 - averageResponseTime) * 0.5;
        if (responseBonus < 0) responseBonus = 0;
    }

    float totalScore = taskScore + communicationScore + managementScore + responseBonus;
    return (totalScore < 0) ? 0 : totalScore;
}

const char* PerformanceReview::getPerformanceLevel() const {
    float score = getScore();
    if (score >= 100) return "Outstanding";
    if (score >= 75) return "Excellent";
    if (score >= 50) return "Satisfactory";
    if (score >= 25) return "Needs Improvement";
    return "Unsatisfactory";
}

ostream& operator<<(ostream& out, const PerformanceReview& pr) {
    out << "\033[38;5;196m\n=========================================\n\033[0m";
    out << "\033[48;5;19m\033[38;5;15mPERFORMANCE REPORT FOR: " << pr.employeeName << "\033[0m\n";
    out << "\033[38;5;196m=========================================\n\033[0m";
    out << "\033[38;5;46mTasks Completed: " << pr.tasksCompleted << "\033[0m\n";
    out << "\033[38;5;196mDeadlines Missed: " << pr.deadlinesMissed << "\033[0m\n";
    out << "\033[38;5;226mTask Completion Rate: "
        << (pr.tasksCompleted > 0 ? (100 - (pr.deadlinesMissed * 100.0f / pr.tasksCompleted)) : 0)
        << "%\033[0m\n";
    out << "\033[38;5;46mMessages Sent: " << pr.messagesSent << "\033[0m\n";
    out << "\033[38;5;46mTasks Assigned: " << pr.tasksAssigned << "\033[0m\n";
    out << "\033[38;5;46mTasks Managed: " << pr.tasksManaged << "\033[0m\n";
    if (pr.totalResponseTimeEvents > 0)
        out << "\033[38;5;226mAverage Response Time: " << pr.averageResponseTime << " hours\033[0m\n";
    out << "\033[38;5;196m-----------------------------------------\n\033[0m";
    out << "\033[48;5;226m\033[38;5;16mOVERALL PERFORMANCE SCORE: " << pr.getScore() << "\033[0m\n";
    out << "\033[48;5;46m\033[38;5;16mPERFORMANCE LEVEL: " << pr.getPerformanceLevel() << "\033[0m\n";
    out << "\033[38;5;196m=========================================\n\033[0m";
    return out;
}

