#include "../include/OrderBook.h"
#include <iostream>
#include <algorithm>

void OrderBook::addOrder(const Order& order)
{
    orderMap[order.orderId] = order;

    if(order.side == Side::BUY)
        buyBook[order.price].push(order.orderId);
    else
        sellBook[order.price].push(order.orderId);

    matchOrders();
}

const Order* OrderBook::getOrder(int orderId)
{
    auto it = orderMap.find(orderId);
    if(it == orderMap.end()) return nullptr;
    return &it->second;
}

void OrderBook::cancelOrder(int orderId)
{
    auto it = orderMap.find(orderId);
    if(it == orderMap.end()) return;

    it->second.active = false;
    it->second.status = OrderStatus::CANCELLED;
}

void OrderBook::matchOrders()
{
    // SIMPLE SAFE VERSION (so project builds first)

    while(!buyBook.empty() && !sellBook.empty())
{
    auto& buyLevel = buyBook.begin()->second;
    auto& sellLevel = sellBook.begin()->second;

    if(buyLevel.empty() || sellLevel.empty())
        break;

    int buyId = buyLevel.front();
    int sellId = sellLevel.front();

    if(orderMap.find(buyId) == orderMap.end())
    {
        buyLevel.pop();
        continue;
    }

    if(orderMap.find(sellId) == orderMap.end())
    {
        sellLevel.pop();
        continue;
    }

    Order& buy = orderMap[buyId];
    Order& sell = orderMap[sellId];

    if(!buy.active)
    {
        buyLevel.pop();
        continue;
    }

    if(!sell.active)
    {
        sellLevel.pop();
        continue;
    }

    if(buy.price < sell.price)
        break;

    int qty = std::min(buy.quantity, sell.quantity);

    std::cout << "TRADE " << qty << " @ " << sell.price << std::endl;

    Trade t;

    t.buyOrderId = buy.orderId;
    t.sellOrderId = sell.orderId;
    t.price = sell.price;
    t.quantity = qty;
    t.timestamp = std::max(
        buy.timestamp,
        sell.timestamp
    );

    tradeHistory.push_back(t);

    buy.quantity -= qty;
    sell.quantity -= qty;

    if(buy.quantity == 0)
        buy.status = OrderStatus::FILLED;
    else
        buy.status = OrderStatus::PARTIALLY_FILLED;

    if(sell.quantity == 0)
        sell.status = OrderStatus::FILLED;
    else
        sell.status = OrderStatus::PARTIALLY_FILLED;
        
    if(buy.quantity == 0)
        buyLevel.pop();

    if(sell.quantity == 0)
        sellLevel.pop();
}
}

void OrderBook::modifyOrder(int orderId, int newPrice, int newQuantity)
{
    auto it = orderMap.find(orderId);
    if(it == orderMap.end()) return;

    Order& order = it->second;

    // mark old inactive (important)
    order.active = false;

    // create new updated order
    Order updated(
        order.orderId,
        order.side,
        newPrice,
        newQuantity,
        order.timestamp
    );

    orderMap[orderId] = updated;

    if(updated.side == Side::BUY)
        buyBook[newPrice].push(orderId);
    else
        sellBook[newPrice].push(orderId);

    matchOrders();
}

void OrderBook::printTrades()
{
    std::cout << "\n===== TRADE HISTORY =====\n";

    for(const auto& t : tradeHistory)
    {
        std::cout
            << "BUY=" << t.buyOrderId
            << " SELL=" << t.sellOrderId
            << " PRICE=" << t.price
            << " QTY=" << t.quantity
            << '\n';
    }
}