#ifndef SURAKARTAAGENT_H
#define SURAKARTAAGENT_H

#include <memory>
#include "surakartaagentbase.h"

class SurakartaAgent : public SurakartaAgentBase {
public:
    using SurakartaAgentBase::SurakartaAgentBase;
    SurakartaMove CalculateMove() override;
};

#endif // SURAKARTAAGENT_H
