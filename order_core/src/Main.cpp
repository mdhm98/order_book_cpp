#include "Order.h"
#include "OrderBook.h"
#include "FileUtil.h"
#include <fstream>

int main() {
	auto& symbol = "AAPL";
	auto orderBook = OrderBook(symbol);

    std::ifstream file("C:\\Users\\muhdh\\Source\\Repos\\order_book_cpp\\order_core\\resources\\orders.csv");
    if (!file.is_open()) {
		std::cerr << "Could not open orders.csv file!" << std::endl;
		return 1;
	}
    std::string line;
    // Skip the header line
    std::getline(file, line);

    // Read each order from the file and add it to the order book
    while (std::getline(file, line)) {
        std::cout << "Reading line:: " << line << std::endl;
        Order order = FileUtil::parseOrderFromCsv(line);
        std::cout << "Adding order: " << order.getId().value << std::endl;
        orderBook.matchOrder(order);
        // Optionally, print the state of the order book after each order
        std::cout << "Current Order Book:" << std::endl;
        std::cout << orderBook << std::endl;
    }

    file.close();
    return 0;
}