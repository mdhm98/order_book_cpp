#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Order.h"

class FileUtil {
public:
    static inline Order parseOrderFromCsv(const std::string& line) {
        std::stringstream ss(line);
        std::string id_str, side_str, type_str, price_str, qty_str, symbol, expiry;

        std::getline(ss, id_str, ',');
        std::getline(ss, side_str, ',');
        std::getline(ss, type_str, ',');
        std::getline(ss, price_str, ',');
        std::getline(ss, qty_str, ',');
        std::getline(ss, symbol, ',');
        std::getline(ss, expiry, ',');

        // Debugging line
        std::cout << "Parsed order: " << id_str << "," << side_str << "," << type_str << "," << price_str << "," << qty_str << std::endl;

        try {
            Order::OrderId id = { std::stoi(id_str) };
            Order::Side side = (side_str == "BUY") ? Order::Side::BUY : Order::Side::SELL;
            Order::PriceType type = (type_str == "LIMIT") ? Order::PriceType::LIMIT : Order::PriceType::MARKET;
            double price = std::stod(price_str);
            double qty = std::stod(qty_str);
            Order::Expiry exp = Order::Expiry::GOOD_FOR_DAY;  // Assume GOOD_FOR_DAY for simplicity

            return Order(id, side, type, price, qty, symbol, exp);
        }
        catch (const std::exception& e) {
            std::cerr << "Error parsing order: " << e.what() << std::endl;
        }
    }

};