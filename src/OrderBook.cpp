#include "../include/OrderBook.h"
#include "../include/Trade.h"

#include <iostream>
#include <algorithm>

void OrderBook::addOrder(const Order& order)
{
    if(orderMap.find(order.orderId) != orderMap.end())
    {
        std::cout
            << "ERROR: Order ID "
            << order.orderId
            << " already exists\n";

        return;
    }

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

    if(it == orderMap.end())
        return nullptr;

    return &it->second;
}

void OrderBook::cancelOrder(int orderId)
{
    auto it = orderMap.find(orderId);

    if(it == orderMap.end())
        return;

    it->second.active = false;
    it->second.status = OrderStatus::CANCELLED;
}

void OrderBook::matchOrders()
{
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

        bool canMatch = false;

    if(buy.type == OrderType::MARKET ||
    sell.type == OrderType::MARKET)
    {
        canMatch = true;
    }
    else if(buy.price >= sell.price)
    {
        canMatch = true;
    }

    if(!canMatch)
    {
        break;
    }

    int qty = std::min(
        buy.quantity,
        sell.quantity
    );
        std::cout
            << "TRADE "
            << qty
            << " @ "
            << sell.price
            << std::endl;

        Trade t;

        t.buyOrderId = buy.orderId;
        t.sellOrderId = sell.orderId;
        if(buy.type == OrderType::MARKET)
        t.price = sell.price;
        else if(sell.type == OrderType::MARKET)
        t.price = buy.price;
        else
    t.price = sell.price;
        t.quantity = qty;
        t.timestamp =
            std::max(buy.timestamp,
                     sell.timestamp);

        tradeHistory.push_back(t);

        buy.quantity -= qty;
        sell.quantity -= qty;

        buy.status =
            (buy.quantity == 0)
            ? OrderStatus::FILLED
            : OrderStatus::PARTIALLY_FILLED;

        sell.status =
            (sell.quantity == 0)
            ? OrderStatus::FILLED
            : OrderStatus::PARTIALLY_FILLED;

        if(buy.quantity == 0)
            buyLevel.pop();

        if(sell.quantity == 0)
            sellLevel.pop();
    }
}

void OrderBook::modifyOrder(
    int orderId,
    int newPrice,
    int newQuantity)
{
    auto it = orderMap.find(orderId);

    if(it == orderMap.end())
        return;

    Order oldOrder = it->second;

    oldOrder.active = false;

    Order updated(
        orderId,
        oldOrder.side,
        newPrice,
        newQuantity,
        oldOrder.timestamp
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
    std::cout
        << "\n===== TRADE HISTORY =====\n";

    for(const auto& t : tradeHistory)
    {
        std::cout
    << "TRADE "
    << t.quantity
    << " @ "
    << t.price
    << std::endl;
    }
}

void OrderBook::printBook()
{
    std::cout << "\n========== ORDER BOOK ==========\n";

    std::cout << "\nSELL SIDE\n";
    for(const auto& [price, orders] : sellBook)
    {
        std::cout
            << "Price: "
            << price
            << " | Orders: "
            << orders.size()
            << '\n';
    }

    std::cout << "\nBUY SIDE\n";
    for(const auto& [price, orders] : buyBook)
    {
        std::cout
            << "Price: "
            << price
            << " | Orders: "
            << orders.size()
            << '\n';
    }

    std::cout << "===============================\n";
}


void OrderBook::printOrder(int orderId)
{
    auto it = orderMap.find(orderId);

    if(it == orderMap.end())
    {
        std::cout << "ORDER NOT FOUND\n";
        return;
    }

    const Order& o = it->second;

    std::cout
        << "\nORDER DETAILS\n"
        << "ID: " << o.orderId << '\n'
        << "PRICE: " << o.price << '\n'
        << "QTY: " << o.quantity << '\n';

    std::cout << "SIDE: ";

    if(o.side == Side::BUY)
        std::cout << "BUY\n";
    else
        std::cout << "SELL\n";

    std::cout << "STATUS: ";

    switch(o.status)
    {
        case OrderStatus::ACTIVE:
            std::cout << "ACTIVE";
            break;

        case OrderStatus::PARTIALLY_FILLED:
            std::cout << "PARTIALLY_FILLED";
            break;

        case OrderStatus::FILLED:
            std::cout << "FILLED";
            break;

        case OrderStatus::CANCELLED:
            std::cout << "CANCELLED";
            break;
    }

    std::cout << "\n";
}