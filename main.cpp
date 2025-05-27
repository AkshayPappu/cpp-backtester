#include <iostream>
#include <memory>
#include <deque>
#include "Backtester.h"
#include "Strategy.h"
#include "Candle.h"

// Moving Average Crossover Strategy
class MovingAverageCrossoverStrategy : public Strategy {
private:
    int shortWindow;
    int longWindow;
    std::deque<double> shortPrices;
    std::deque<double> longPrices;

public:
    MovingAverageCrossoverStrategy(int shortW, int longW)
        : shortWindow(shortW), longWindow(longW) {}

    Signal onCandle(const Candle& candle) override {
        double close = candle.getClose();

        shortPrices.push_back(close);
        longPrices.push_back(close);

        if (shortPrices.size() > shortWindow) shortPrices.pop_front();
        if (longPrices.size() > longWindow) longPrices.pop_front();

        if (shortPrices.size() < shortWindow || longPrices.size() < longWindow)
            return {Action::HOLD, 0};  // not enough data yet

        double shortSum = 0, longSum = 0;
        for (double p : shortPrices) shortSum += p;
        for (double p : longPrices) longSum += p;

        double shortMA = shortSum / shortPrices.size();
        double longMA = longSum / longPrices.size();

        if (shortMA > longMA) {
            return {Action::BUY, 1};
        } else if (shortMA < longMA) {
            return {Action::SELL, 1};
        }
        return {Action::HOLD, 0};
    }
};

int main() {
    std::string csv_file = "test_data.csv";  // make sure this file exists!

    // Create moving average crossover strategy: short=2, long=3
    auto strategy = std::make_unique<MovingAverageCrossoverStrategy>(2, 3);

    // Initialize backtester
    Backtester backtester(
        std::move(strategy),
        csv_file,
        100000.0,  // starting cash
        0.5,       // threshold (unused for now)
        100,       // long limit
        100        // short limit
    );

    // Run backtest
    backtester.run();

    return 0;
}