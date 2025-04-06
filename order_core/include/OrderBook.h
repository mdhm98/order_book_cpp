#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/unordered/unordered_flat_map.hpp>
#include <vector>
#include "PriceLevel.h"
#include "Order.h"
#include <iostream>
#include <ostream>

class OrderBook {
private:
    std::string symbol_;

    boost::container::flat_map<double, PriceLevel> bids_; 
    boost::container::flat_map<double, PriceLevel> asks_; 

    boost::unordered_flat_map<Order::OrderId, Order, Order::OrderIdHash> orderMap_;

public:
    OrderBook(const std::string& symbol);

    bool addOrder(const Order& order);
    bool removeOrder(const Order::OrderId& orderId);
    bool updateOrder(const Order::OrderId& orderId, double newQuantity);

    double getBestBid() const { return bids_.empty() ? 0.0 : bids_.rbegin()->first; }
    double getBestAsk() const { return asks_.empty() ? 0.0 : asks_.begin()->first; }
    void matchOrder(const Order& incomingOrder);

    friend std::ostream& operator<<(std::ostream& os, const OrderBook& orderBook);
};


