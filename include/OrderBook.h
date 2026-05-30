#pragma once

#include "Order.h"
#include "Trade.h"

#include <queue>
#include <vector>
#include <unordered_map>
#include <map>

class OrderBook
{
private:

    // price -> orderIds
    std::map<int, std::queue<int>, std::greater<int>> buyBook;
    std::map<int, std::queue<int>> sellBook;

    // orderId -> Order
    std::unordered_map<int, Order> orderMap;
    std::vector<Trade> tradeHistory;

public:

    void addOrder(const Order& order);
    const Order* getOrder(int orderId);
    void cancelOrder(int orderId);
    void matchOrders();
    void printBook();
    void modifyOrder(int orderId, int newPrice, int newQuantity);
    void printTrades();
};