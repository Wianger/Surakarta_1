#ifndef SURAKARTAAGENTBASE_H
#define SURAKARTAAGENTBASE_H

#include <memory>
#include "surakartacommon.h"
#include "surakartarulemanager.h"

class SurakartaAgentBase {
public:
    SurakartaAgentBase(std::shared_ptr<SurakartaBoard> board,
                       std::shared_ptr<SurakartaGameInfo> game_info,
                       std::shared_ptr<SurakartaRuleManager> rule_manager)
        :board_(std::const_pointer_cast<const SurakartaBoard>(board)),
        game_info_(std::const_pointer_cast<const SurakartaGameInfo>(game_info)),
        rule_manager_(rule_manager) {}

    virtual SurakartaMove CalculateMove() {
        throw SurakartaException("SurakartaAgentBase::CalculateMove() is not implemented. Are you using SurakartaAgentBase directly?");
    }

    void SetRuleManager(std::shared_ptr<SurakartaRuleManager> rule_manager) {  // For testing
        rule_manager_ = rule_manager;
    }

protected:
    std::shared_ptr<const SurakartaBoard> board_;
    std::shared_ptr<const SurakartaGameInfo> game_info_;
    std::shared_ptr<SurakartaRuleManager> rule_manager_;
};

#endif // SURAKARTAAGENTBASE_H
