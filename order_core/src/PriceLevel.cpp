#include "PriceLevel.h"

PriceLevel::PriceLevel(double price, std::vector<Order> orders)
    : price_(price), orders_(std::move(orders)) {
    // Populate the orderIndexMap_
    for (std::size_t i = 0; i < orders_.size(); ++i) {
        orderIndexMap_[orders_[i].getId()] = i;
    }
}

void PriceLevel::addOrder(const Order& order) {
    orders_.emplace_back(order);
    orderIndexMap_[order.getId()] = orders_.size() - 1;
}

void PriceLevel::removeOrder(const Order::OrderId& id) {
    auto it = orderIndexMap_.find(id);
    if (it == orderIndexMap_.end()) {
        throw std::runtime_error("Order not found");
    }

    // Move the last order to the position of the removed order
    std::size_t index = it->second;
    if (index != orders_.size() - 1) {
        orders_[index] = std::move(orders_.back());
        orderIndexMap_[orders_[index].getId()] = index;
    }

    // Remove the last order
    orders_.pop_back();
    orderIndexMap_.erase(it);
}

void PriceLevel::updateOrder(const Order::OrderId& id, double quantity) {
    auto it = orderIndexMap_.find(id);
    if (it == orderIndexMap_.end()) {
        throw std::runtime_error("Order not found");
    }

    orders_[it->second].setQuantity(quantity);
}

double PriceLevel::getTotalQuantity() const {
    double totalQuantity = 0.0;
    for (const auto& order : orders_) {
        totalQuantity += order.getQuantity();
    }
    return totalQuantity;
}