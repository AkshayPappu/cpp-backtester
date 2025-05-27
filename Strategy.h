#pragma once

#include <string>
#include <iostream>
#include "Candle.h"

enum class Action { BUY, SELL, HOLD};

inline std::ostream& operator<<(std::ostream& os, Action action) {
    switch(action) {
        case Action::BUY: return os << "BUY";
        case Action::SELL: return os << "SELL";
        case Action::HOLD: return os << "HOLD";
        default: return os << "UNKNOWN";
    }
}

struct Signal {
    Action action;
    double amount;
};

class Strategy {
    private:
    public:
        virtual Signal onCandle(const Candle& candle) {
            // Default implementation: no action
            return Signal{Action::HOLD, 0};
        };
        virtual ~Strategy() = default;
};