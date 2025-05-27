#pragma once
#include <cstddef>
#include <algorithm>
#include <vector>

#include "Strategy.h"
#include "Candle.h"

struct Trade {
    Action action;
    double amount;
    double price;
    double timestamp;
};

class Portfolio {
    private:
        double cash;
        double longPosition;
        double shortPosition;
        double longLimit;
        double shortLimit;
        double threshold;
        std::vector<Trade> trades;

    public:
        Portfolio(double cash, double threshold, double longLimit, double shortLimit) : cash(cash), longPosition(0), shortPosition(0), longLimit(longLimit), shortLimit(shortLimit), threshold(threshold) {};

        std::vector<Trade> getTrades() { return trades; }
        double getCash() { return cash; }
        double getLongPosition() { return longPosition; }
        double getShortPosition() { return shortPosition; }

        void applySignal(const Candle& candle, const Signal& signal) {
            double amount = signal.amount;
            double price = candle.getClose();
            double tradeAmount = 0;

            if (signal.action == Action::BUY) {            
                // check allowed by buy limit
                double allowedByBuyLimit = longLimit - longPosition + shortPosition;
                if (allowedByBuyLimit <= 0) return;

                // check if I have enough cash
                double allowedByCash = cash / price;
                if (allowedByCash <= 0) return;

                // calculate the actual amount to be bought
                double actualAllowedLimit = std::min(std::min(amount, allowedByBuyLimit), allowedByCash);
                if (actualAllowedLimit <= 0) return;

                // check how much I can immediately buy to close short positions
                double amountFromShort = std::min(actualAllowedLimit, shortPosition);
                if (amountFromShort > 0) {
                    shortPosition -= amountFromShort;
                    cash -= amountFromShort * price;
                    actualAllowedLimit -= amountFromShort;
                    allowedByBuyLimit -= amountFromShort;
                    tradeAmount += amountFromShort;
                }

                // recalculate allowed by cash
                allowedByCash = cash / price;

                // check how much can be added to long positions
                if (amount > 0 && actualAllowedLimit > 0) {
                    double actualAmountForLong = std::min(std::min(actualAllowedLimit, allowedByBuyLimit), allowedByCash);
                    longPosition += actualAmountForLong;
                    cash -= actualAmountForLong * price;
                    actualAllowedLimit -= actualAmountForLong;
                    allowedByBuyLimit -= actualAmountForLong;
                    tradeAmount += actualAmountForLong;
                }

                // record Trade
                if (tradeAmount > 0) {
                    trades.push_back({signal.action, tradeAmount, price, candle.getTimestamp()});
                }
            } 
            else if (signal.action == Action::SELL) {
                // check allowed by sell limit
                double allowedBySellLimit = shortLimit - shortPosition + longPosition;
                if (allowedBySellLimit <= 0) return;

                double actualAllowedLimit = std::min(amount, allowedBySellLimit);

                // check how much can immediately be sold from long positions
                double amountFromLong = std::min(actualAllowedLimit, longPosition);
                if (amountFromLong > 0) {
                    longPosition -= amountFromLong;
                    cash += amountFromLong * price;
                    actualAllowedLimit -= amountFromLong;
                    allowedBySellLimit -= amountFromLong;
                    tradeAmount += amountFromLong;
                }

                // check how much can be added to short positions
                if (amount > 0 && allowedBySellLimit > 0) {
                    double actualAmountForShort = std::min(actualAllowedLimit, allowedBySellLimit);
                    shortPosition += actualAmountForShort;
                    cash += actualAmountForShort * price;  
                    actualAllowedLimit -= actualAmountForShort;
                    allowedBySellLimit -= actualAmountForShort;
                    tradeAmount += actualAmountForShort;
                }

                // record Trade
                if (tradeAmount > 0) {
                    trades.push_back({signal.action, tradeAmount, price, candle.getTimestamp()});
                }
            }
            else if (signal.action == Action::HOLD) {
                // do nothing
                return;
            }
            return;
        }
};