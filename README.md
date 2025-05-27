# C++ Backtesting Framework

A lightweight, high-performance backtesting framework for algorithmic trading strategies written in C++20.

## Overview

This framework allows you to:
- Implement custom trading strategies
- Backtest strategies against historical price data
- Track portfolio performance including long/short positions
- Manage position limits and cash constraints
- Record and analyze trade history

## Features

- **Strategy Interface**: Implement your own trading strategies by inheriting from the `Strategy` class
- **Portfolio Management**: 
  - Support for both long and short positions
  - Position limits for risk management
  - Cash management with transaction costs
- **Data Handling**: 
  - CSV-based historical price data input
  - OHLCV (Open, High, Low, Close, Volume) candle data
- **Performance Tracking**: 
  - Trade history recording
  - Portfolio value tracking
  - Position tracking

## Building

```bash
# Clone the repository
git clone <repository-url>
cd cpp-backtester

# Build the project
g++ -std=c++20 -O2 main.cpp -o backtest
```

## Usage

1. Create your strategy by inheriting from the `Strategy` class:
```cpp
class MyStrategy : public Strategy {
public:
    Signal onCandle(const Candle& candle) override {
        // Implement your strategy logic here
        // Return a Signal with Action (BUY/SELL/HOLD) and amount
        return Signal{Action::HOLD, 0};
    }
};
```

2. Prepare your data file in CSV format:
```
open,close,high,low,volume,timestamp
100.0,101.0,102.0,99.0,1000,1234567890
...
```

3. Run the backtest:
```bash
./backtest <data-file.csv>
```

## Project Structure

- `Strategy.h`: Base strategy class and signal definitions
- `Candle.h`: OHLCV data structure
- `Portfolio.h`: Portfolio management and trade execution
- `Backtester.h`: Main backtesting engine
- `main.cpp`: Entry point and example usage

## Example

```cpp
#include "Backtester.h"
#include "Strategy.h"

class SimpleMAStrategy : public Strategy {
    Signal onCandle(const Candle& candle) override {
        // Example: Buy if price is above 100, sell if below
        if (candle.getClose() > 100) {
            return Signal{Action::BUY, 1.0};
        } else if (candle.getClose() < 100) {
            return Signal{Action::SELL, 1.0};
        }
        return Signal{Action::HOLD, 0};
    }
};

int main() {
    SimpleMAStrategy strategy;
    Backtester backtester(&strategy, "data.csv", 10000.0, 0.0);
    backtester.run();
    return 0;
}
```

## License

[Your chosen license]

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. 