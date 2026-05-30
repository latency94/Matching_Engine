#include "../include/OrderBook.h"
#include <iostream>

int main()
{
    OrderBook ob;

    ob.addOrder(Order(1, Side::BUY, 100, 10, 1));
    ob.addOrder(Order(2, Side::SELL, 105, 5, 2));

    ob.modifyOrder(1, 110, 10);

    const Order* o = ob.getOrder(1);

    if(o)
        std::cout << "FOUND\n";
    else
        std::cout << "NOT FOUND\n";

    ob.printTrades();

    return 0;
}