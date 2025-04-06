#include "Order.h"
#include "OrderBook.h"

int main() {
	auto& symbol = "AAPL";
	auto order = Order(Order::OrderId{ 1 }, Order::Side::BUY,
		Order::PriceType::MARKET, 240.0, 10,
		symbol, Order::Expiry::GOOD_FOR_DAY);

	auto orderBook = OrderBook(symbol);
	orderBook.addOrder(order);

	std::cout << "Order Book: " << std::endl;
	std::cout << orderBook << std::endl;

	return 1;
}