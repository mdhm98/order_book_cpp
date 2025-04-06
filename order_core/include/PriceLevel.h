#ifndef PRICE_LEVEL
#define PRICE_LEVEL

#include "Order.h"
#include <vector>
#include <unordered_map>
#include <stdexcept>

class PriceLevel {
private:
    double price_;
    std::vector<Order> orders_;
    std::unordered_map<Order::OrderId, std::size_t, Order::OrderIdHash> orderIndexMap_; // Maps OrderId to index in orders_

public:
    PriceLevel(double price, std::vector<Order> orders = {});
    ~PriceLevel() = default;

    void addOrder(const Order& order);
    void removeOrder(const Order::OrderId& id);
    void updateOrder(const Order::OrderId& id, double quantity);

    double getPrice() const { return price_; }
    double getTotalQuantity() const;
    std::vector<Order>& getOrders() { return orders_; }
};

#endif