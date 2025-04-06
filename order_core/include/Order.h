// Order.h
#ifndef ORDER
#define ORDER

#include <map>
#include <string>
#include <cstdint>
#include <functional>

class Order {
public:
    enum class Side { BUY, SELL };
    enum class Expiry { GOOD_FOR_DAY, GOOD_TILL_EXPIRY };
    enum class PriceType { MARKET, LIMIT, MARKET_OPEN, MARKET_CLOSE };

    struct OrderId {
        uint64_t value;

        bool operator==(const OrderId& other) const {
            return value == other.value;
        }
    };

    struct OrderIdHash {
        size_t operator()(const OrderId& id) const {
            return std::hash<uint64_t>{}(id.value);
        }
    };

    Order(OrderId id, Side side, PriceType priceType,
        double price, double quantity, std::string symbol, 
        Expiry expiry);

    Order(const Order& other);
    Order(Order&& other) noexcept;
    Order& operator=(const Order& other);
    Order& operator=(Order&& other) noexcept;

    OrderId getId() const;
    Side getSide() const;
    PriceType getPriceType() const;
    uint64_t getTimestamp() const;
    uint64_t getExpiry() const;
    double getPrice() const;
    double getQuantity() const;
    std::string getSymbol() const;
    void setQuantity(const double newQuantity);

private:
    uint64_t calculateExpiryTimestamp(Expiry expiry) const;

    OrderId id_;
    Side side_;
    PriceType priceType_;
    uint64_t timestamp_;
    uint64_t expiry_;
    double price_;
    double quantity_;
    std::string symbol_;
};

#endif // ORDER
