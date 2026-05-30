#include "../include/OrderBook.h"
#include <iostream>

int main()
{
    OrderBook ob;

    ob.addOrder(Order(1, Side::BUY, 100, 10, 1));
    ob.addOrder(Order(2, Side::SELL, 95, 5, 2));

    ob.printTrades();

    const Order* o = ob.getOrder(1);

    if(o)
        std::cout << "FOUND\n";
    else
        std::cout << "NOT FOUND\n";

    ob.printTrades();

    return 0;
}