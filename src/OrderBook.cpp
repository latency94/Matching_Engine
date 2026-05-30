#include "../include/OrderBook.h"

#include <iostream>
#include <algorithm>

// Naya order book me add karo
void OrderBook::addOrder(const Order& order)
{
    // Buy order
    if(order.side == Side::BUY)
{
    buyBook[order.price].push(order);
}
else
{
    sellBook[order.price].push(order);
}
    // Har order ke baad matching try karo
    matchOrders();
}

// Matching engine
void OrderBook::matchOrders()
{
    while(!buyBook.empty() &&
          !sellBook.empty())
    {
        auto buyIt = buyBook.begin();   // highest buy price
        auto sellIt = sellBook.begin(); // lowest sell price

        int buyPrice = buyIt->first;
        int sellPrice = sellIt->first;

        if(buyPrice < sellPrice)
        {
            break;
        }

        Order buy = buyIt->second.front();
        Order sell = sellIt->second.front();

        int tradedQty =
            std::min(
                buy.quantity,
                sell.quantity
            );

        std::cout
            << "BUY_ID=" << buy.orderId
            << " SELL_ID=" << sell.orderId
            << " QTY=" << tradedQty
            << " PRICE=" << sell.price
            << std::endl;

        buy.quantity -= tradedQty;
        sell.quantity -= tradedQty;

        buyIt->second.pop();
        sellIt->second.pop();

        if(buy.quantity > 0)
        {
            buyIt->second.push(buy);
        }

        if(sell.quantity > 0)
        {
            sellIt->second.push(sell);
        }

        if(buyIt->second.empty())
        {
            buyBook.erase(buyIt);
        }

        if(sellIt->second.empty())
        {
            sellBook.erase(sellIt);
        }
    }
}

void OrderBook::printBook()
{
    std::cout << "\n===== BUY BOOK =====\n";

    for(const auto& [price, orders] : buyBook)
    {
        std::cout
            << "PRICE="
            << price
            << " ORDERS="
            << orders.size()
            << '\n';
    }

    std::cout << "\n===== SELL BOOK =====\n";

    for(const auto& [price, orders] : sellBook)
    {
        std::cout
            << "PRICE="
            << price
            << " ORDERS="
            << orders.size()
            << '\n';
    }
}

void OrderBook::cancelOrder(int orderId)
{
    // BUY side search
    for(auto& [price, q] : buyBook)
    {
        std::queue<Order> temp;

        while(!q.empty())
        {
            Order order = q.front();
            q.pop();

            if(order.orderId != orderId)
            {
                temp.push(order);
            }
        }

        q = std::move(temp);
    }

    // SELL side search
    for(auto& [price, q] : sellBook)
    {
        std::queue<Order> temp;

        while(!q.empty())
        {
            Order order = q.front();
            q.pop();

            if(order.orderId != orderId)
            {
                temp.push(order);
            }
        }

        q = std::move(temp);
    }
}
