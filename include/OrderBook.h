#pragma once

#include "Order.h"
#include "Trade.h"

#include <queue>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>   // std::greater

class OrderBook
{
private:

    // BUY side (highest price first)
    std::map<
        int,
        std::queue<int>,
        std::greater<int>
    > buyBook;

    // SELL side (lowest price first)
    std::map<
        int,
        std::queue<int>
    > sellBook;

    // orderId -> Order
    std::unordered_map<int, Order> orderMap;

    // executed trades
    std::vector<Trade> tradeHistory;

public:

    void addOrder(const Order& order);

    const Order* getOrder(int orderId);

    void cancelOrder(int orderId);

    void modifyOrder(
        int orderId,
        int newPrice,
        int newQuantity
    );

    void matchOrders();

    void printBook();

    void printTrades();
    void printOrder(int orderId);
};