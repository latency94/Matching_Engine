#pragma once

enum class Side
{
    BUY,
    SELL
};

struct Order
{
    int orderId;
    Side side;
    int price;
    int quantity;
    long long timestamp;

    Order(
        int id,
        Side s,
        int p,
        int q,
        long long ts
    )
        : orderId(id),
          side(s),
          price(p),
          quantity(q),
          timestamp(ts)
    {
    }
    bool operator==(const Order& other) const
{
    return orderId == other.orderId;
}
};