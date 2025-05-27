#pragma once

class Candle {
    private:
        double open;
        double close;
        double high;
        double low;
        double volume;
        double timestamp;
    public:
        Candle(double open, double close, double high, double low, double volume, double timestamp) : open(open), close(close), high(high), low(low), volume(volume), timestamp(timestamp) {};
        double getOpen() const { return open; }
        double getClose() const { return close; }
        double getHigh() const { return high; }
        double getLow() const { return low; }
        double getVolume() const { return volume; }
        double getTimestamp() const { return timestamp; }
};