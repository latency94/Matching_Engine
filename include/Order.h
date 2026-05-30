#pragma once

enum class Side
{
    BUY,
    SELL
};

enum class OrderStatus
{
    ACTIVE,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED
};

struct Order
{
    int orderId{};
    Side side{};
    int price{};
    int quantity{};
    long long timestamp{};

    bool active{true};
    OrderStatus status{OrderStatus::ACTIVE};

    Order() = default;

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
};