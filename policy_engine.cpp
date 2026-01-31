#include "policy_engine.h"

DefaultPolicyEngine::DefaultPolicyEngine() {}
DefaultPolicyEngine::~DefaultPolicyEngine() {}

DefaultPolicyEngine& DefaultPolicyEngine::getInstance() {
    static DefaultPolicyEngine instance;
    return instance;
}

bool DefaultPolicyEngine::hasAccess(int userLevel, int action) {
    int required = ClearanceLevel::JUNIOR;
    switch (action) {
    case Action::VIEW_INBOX:
    case Action::SEND_MESSAGE:
    case Action::CREATE_TASK:
        required = ClearanceLevel::JUNIOR;
        break;
    case Action::MANAGE_TASK:
    case Action::VIEW_PERFORMANCE_REPORT:
        required = ClearanceLevel::MANAGER;
        break;
    case Action::VIEW_AUDIT_LOGS:
    case Action::SEND_GLOBAL_NOTIFICATION:
    case Action::DELETE_GLOBAL_NOTIFICATION:
        required = ClearanceLevel::DIRECTOR;
        break;
    default:
        required = ClearanceLevel::EXECUTIVE;
    }
    return userLevel >= required;
}

bool PolicyEngine::hasAccess(int userLevel, int action) {
    return DefaultPolicyEngine::getInstance().hasAccess(userLevel, action);
}
