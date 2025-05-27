#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <memory>

#include "Strategy.h"
#include "Candle.h"
#include "Portfolio.h"

class Backtester {
    private:
        std::unique_ptr<Strategy>strategy;
        std::vector<Candle> candles;
        std::unique_ptr<Portfolio> portfolio;
    public:
        // constructor: take in strategy, data as a csv file, portfolio amount, and threshold
        Backtester(std::unique_ptr<Strategy> strategy, const std::string& data_file, double portfolio_amount, double threshold, double long_limit, double short_limit) 
            : strategy(std::move(strategy)), portfolio(std::make_unique<Portfolio>(portfolio_amount, threshold, long_limit, short_limit)) {
            // read data from csv file and initialize candles
            std::ifstream file(data_file);
            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string token;
                std::getline(ss, token, ',');
                double open = std::stod(token);
                std::getline(ss, token, ',');
                double close = std::stod(token);
                std::getline(ss, token, ',');
                double high = std::stod(token);
                std::getline(ss, token, ',');
                double low = std::stod(token);
                std::getline(ss, token, ',');
                double volume = std::stod(token);
                std::getline(ss, token, ',');
                double timestamp = std::stod(token);
                candles.push_back(Candle(open, close, high, low, volume, timestamp));
            }
        };

        // destructor
        ~Backtester() = default;
        

        // run the backtest
        void run() {
            // iterate through candles and apply strategy
            for (const auto& candle: candles) {
                Signal signal = strategy->onCandle(candle);
                portfolio->applySignal(candle, signal);
            }

            // print trades and portfolio
            for (const auto& trade: portfolio->getTrades()) {
                std::cout << "Trade: " << trade.action << " " << trade.amount << " " << trade.price << " " << trade.timestamp << std::endl;
            }
            std::cout << "Portfolio: " << portfolio->getCash() << " " << portfolio->getLongPosition() << " " << portfolio->getShortPosition() << std::endl;
            return;
        }
};