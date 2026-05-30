#include "../include/OrderBook.h"

int main()
{
    OrderBook ob;

    ob.addOrder(
        Order(1, Side::BUY, 100, 10, 1));

    ob.addOrder(
        Order(2, Side::BUY, 100, 20, 2));

    ob.cancelOrder(1);

    ob.printBook();

    return 0;
}