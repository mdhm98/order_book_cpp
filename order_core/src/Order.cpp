// Order.cpp
#include "Order.h"
#include <chrono>
#include <functional>
#include <sstream>

Order::Order(Order::OrderId id, Side side, PriceType priceType,
    double price, double quantity, std::string symbol,
    Expiry expiry)
    : id_(id), side_(side), priceType_(priceType),
    timestamp_(std::chrono::system_clock::now().time_since_epoch().count()),
    price_(price), quantity_(quantity), symbol_(std::move(symbol)), expiry_(calculateExpiryTimestamp(expiry)) {
}

Order::Order(const Order& order)
    : id_(order.id_), side_(order.side_), priceType_(order.priceType_),
    timestamp_(order.timestamp_), expiry_(order.expiry_), price_(order.price_), quantity_(order.quantity_),
    symbol_(order.symbol_) {
}

Order::Order(Order&& other) noexcept
    : id_(std::move(other.id_)), side_(other.side_), priceType_(other.priceType_),
    timestamp_(other.timestamp_), expiry_(other.expiry_), price_(other.price_), quantity_(other.quantity_),
    symbol_(std::move(other.symbol_)) {
}

Order& Order::operator=(const Order& other) {
    if (this != &other) {
        id_ = other.id_;
        side_ = other.side_;
        priceType_ = other.priceType_;
        timestamp_ = other.timestamp_;
        expiry_ = other.expiry_;
        price_ = other.price_;
        quantity_ = other.quantity_;
        symbol_ = other.symbol_;
    }
    return *this;
}

Order& Order::operator=(Order&& other) noexcept {
    if (this != &other) {
        id_ = std::move(other.id_);
        side_ = other.side_;
        priceType_ = other.priceType_;
        timestamp_ = other.timestamp_;
        expiry_ = other.expiry_;
        price_ = other.price_;
        quantity_ = other.quantity_;
        symbol_ = std::move(other.symbol_);
    }
    return *this;
}

Order::OrderId Order::getId() const { return id_; }
Order::Side Order::getSide() const { return side_; }
Order::PriceType Order::getPriceType() const { return priceType_; }
uint64_t Order::getTimestamp() const { return timestamp_; }
uint64_t Order::getExpiry() const { return expiry_; }
double Order::getPrice() const { return price_; }
double Order::getQuantity() const { return quantity_; }
std::string Order::getSymbol() const { return symbol_; }
void Order::setQuantity(const double newQuantity) { this->quantity_ = newQuantity; }

uint64_t Order::calculateExpiryTimestamp(Expiry expiry) const {
    using namespace std::chrono;
    auto now = system_clock::now();
    if (expiry == Expiry::GOOD_FOR_DAY) {
        auto today = floor<days>(now);
        auto tomorrow = today + days{ 1 };
        return duration_cast<milliseconds>(tomorrow.time_since_epoch()).count();
    }
    return UINT64_MAX;
}