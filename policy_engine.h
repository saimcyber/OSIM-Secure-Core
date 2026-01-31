#ifndef POLICY_ENGINE_H
#define POLICY_ENGINE_H

using namespace std;

// Clearance Levels
class ClearanceLevel {
public:
    static const int JUNIOR = 1;
    static const int MANAGER = 2;
    static const int DIRECTOR = 3;
    static const int EXECUTIVE = 4;
};

// Actions that require policy checks
class Action {
public:
    static const int VIEW_INBOX = 0;
    static const int SEND_MESSAGE = 1;
    static const int CREATE_TASK = 2;
    static const int MANAGE_TASK = 3;
    static const int VIEW_AUDIT_LOGS = 4;
    static const int VIEW_PERFORMANCE_REPORT = 5;
    static const int SEND_GLOBAL_NOTIFICATION = 6;
    static const int DELETE_GLOBAL_NOTIFICATION = 7;

private:
    Action() {}
};

// Interface for policy engines
class IPolicyEngine {
public:
    virtual bool hasAccess(int userLevel, int action) = 0;
    virtual ~IPolicyEngine() {}
};

// Singleton implementation of the policy engine
class DefaultPolicyEngine : public IPolicyEngine {
private:
    DefaultPolicyEngine();
    ~DefaultPolicyEngine();
    DefaultPolicyEngine(const DefaultPolicyEngine&) = delete;
    DefaultPolicyEngine& operator=(const DefaultPolicyEngine&) = delete;

public:
    static DefaultPolicyEngine& getInstance();
    bool hasAccess(int userLevel, int action) override;
};

// Static access wrapper
class PolicyEngine {
public:
    static bool hasAccess(int userLevel, int action);
};

#endif // POLICY_ENGINE_H
