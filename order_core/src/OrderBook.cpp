#include "OrderBook.h"
#include <algorithm>

OrderBook::OrderBook(const std::string& symbol) {
	this->symbol_ = symbol;
}


bool OrderBook::addOrder(const Order& order) {
    // Determine which side (bids or asks) the order belongs to
    auto& sideMap = (order.getSide() == Order::Side::BUY) ? bids_ : asks_;

    // Check if the price level already exists
    auto it = sideMap.find(order.getPrice());
    if (it != sideMap.end()) {
        it->second.addOrder(order);
    }
    else {
        PriceLevel priceLevel(order.getPrice());
        priceLevel.addOrder(order);
        sideMap.insert({ order.getPrice(), priceLevel });
    }

    // Add the order to the orderMap_ for tracking
    orderMap_.insert({ order.getId(), order });

    // Return true to indicate success
    return true;
}

bool OrderBook::removeOrder(const Order::OrderId& orderId) {
    auto it = orderMap_.find(orderId);
    if (it != orderMap_.end()) {
        auto& order = it->second;
        auto& sideMap = (order.getSide() == Order::Side::BUY ? bids_ : asks_);
        auto mapIt = sideMap.find(order.getPrice());
        sideMap.erase(mapIt);
        return true;
    }
    return false;
}

bool OrderBook::updateOrder(const Order::OrderId& orderId, double newQuantity) {
    if (auto it = orderMap_.find(orderId); it != orderMap_.end()) {
        auto& order = it->second;
        order.setQuantity(newQuantity);
        return true;
    }
    return false;
}

void OrderBook::matchOrder(const Order& incomingOrder) {
    double quantityToMatch = incomingOrder.getQuantity();
    const auto& side = incomingOrder.getSide();
    bool isBuy = side == Order::Side::BUY ? true : false;
    auto& bookSide = isBuy ? asks_ : bids_;
    auto& ownSide = isBuy ? bids_ : asks_;

    if (isBuy) {         
        // Match BUY order with lowest ask prices (ascending order)
        auto it = bookSide.begin();
        while (quantityToMatch > 0 && it != bookSide.end()) {
            auto& priceLevel = it->second;

            if (incomingOrder.getPrice() < priceLevel.getPrice()) {
                break;
            }

            auto& ordersAtLevel = priceLevel.getOrders();
            for (size_t i = 0; i < ordersAtLevel.size() && quantityToMatch > 0;) {
                auto& restingOrder = ordersAtLevel[i];
                double matchQty = std::min(restingOrder.getQuantity(), quantityToMatch);
                std::cout << "[MATCHED] "
                    << "Buy "
                    << matchQty << " @ " << priceLevel.getPrice()
                    << " (with " << restingOrder.getId().value << ")\n";
                double remainderQty = restingOrder.getQuantity() - matchQty;
                restingOrder.setQuantity(remainderQty);
                quantityToMatch -= matchQty;

                if (restingOrder.getQuantity() <= 0) {
                    priceLevel.removeOrder(restingOrder.getId());
                }
                else {
                    ++i;
                }
                if (quantityToMatch <= 0) {
                    break;
                }
            }

            if (priceLevel.getOrders().empty()) {
                it = bookSide.erase(it);
            }
            else {
                ++it;
            }

        }
    }
    else {         
        // Match SELL order with highest bid prices (descending order)
        auto it = bookSide.end();
        while (quantityToMatch > 0 && it != bookSide.begin()) {
            --it;
            auto& priceLevel = it->second;

            if (incomingOrder.getPrice() > priceLevel.getPrice()) {
                break;
            }

            auto& ordersAtLevel = priceLevel.getOrders();
            for (size_t i = 0; i < ordersAtLevel.size() && quantityToMatch > 0;) {
                auto& restingOrder = ordersAtLevel[i];
                double matchQty = std::min(restingOrder.getQuantity(), quantityToMatch);
                std::cout << "[MATCHED] "
                    << "Sell "
                    << matchQty << " @ " << priceLevel.getPrice()
                    << " (with " << restingOrder.getId().value << ")\n";
                double remainderQty = restingOrder.getQuantity() - matchQty;
                restingOrder.setQuantity(remainderQty);
                quantityToMatch -= matchQty;

                if (restingOrder.getQuantity() <= 0) {
                    priceLevel.removeOrder(restingOrder.getId());
                }
                else {
                    ++i;
                }

                if (quantityToMatch <= 0) {
                    break;
                }
            }

            if (priceLevel.getOrders().empty()) {
                it = bookSide.erase(it);
            }

        }
    }
    // If any quantity remains unmatched, re-add the order to the book
    if (quantityToMatch > 0) {
        Order updatedOrder = incomingOrder;
        updatedOrder.setQuantity(quantityToMatch);
        if (auto it = ownSide.find(incomingOrder.getPrice()); it != ownSide.end()) {
            it->second.addOrder(updatedOrder);
        }
        else {
            PriceLevel level(incomingOrder.getPrice());
            level.addOrder(updatedOrder);
            ownSide.insert({ incomingOrder.getPrice(), level });
        }
    }
}

std::ostream& operator<<(std::ostream& os, const OrderBook& orderBook) {
    os << "Order Book for Symbol: " << orderBook.symbol_ << "\n";
    os << "--------------------\n";
    os << "    ASK QTY @ PRICE\n";

    // Top 5 asks (lowest price first – already sorted)
    int askCount = 0;
    for (auto it = orderBook.asks_.begin(); it != orderBook.asks_.end() && askCount < 5; ++it, ++askCount) {
        const PriceLevel& level = it->second;
        os << "    " << level.getTotalQuantity() << " @ " << level.getPrice() << "\n";
    }

    os << "--------------------\n";
    os << "    BID QTY @ PRICE\n";

    // Top 5 bids (need to reverse iterate since flat_map is ascending)
    int bidCount = 0;
    for (auto it = orderBook.bids_.rbegin(); it != orderBook.bids_.rend() && bidCount < 5; ++it, ++bidCount) {
        const PriceLevel& level = it->second;
        os << "    " << level.getTotalQuantity() << " @ " << level.getPrice() << "\n";
    }

    return os;
}

