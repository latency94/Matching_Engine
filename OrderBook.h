#pragma once

#include "Order.h"
#include "Trade.h"

#include <queue>
#include <vector>

#include <map>


class OrderBook
{
private:

    std::map<
    int,
    std::queue<Order>,
    std::greater<int>
> buyBook;

std::map<
    int,
    std::queue<Order>
> sellBook;

public:

    void addOrder(const Order& order);

    void matchOrders();

    void printBook();
    void cancelOrder(int orderId);
};


