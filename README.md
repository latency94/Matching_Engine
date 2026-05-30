# Order Matching Engine

## Overview

A simplified exchange-style Order Matching Engine built in C++20.

The engine supports:

* Limit Orders
* Market Orders
* Order Modification
* Order Cancellation
* Trade History
* Price-Time Priority Matching
* O(1) Order Lookup

## Tech Stack

* C++20
* STL
* std::map
* std::unordered_map
* std::queue
* g++

## Project Structure

order_book_engine/

├── include/

│ ├── Order.h

│ ├── OrderBook.h

│ └── Trade.h

├── src/

│ ├── OrderBook.cpp

│ └── main.cpp

├── tests/

├── README.md

└── .gitignore

## Architecture

Client Order

↓

OrderBook

├── Buy Book

├── Sell Book

└── Trade History

## Complexity

| Operation    | Complexity |
| ------------ | ---------- |
| Add Order    | O(log N)   |
| Lookup Order | O(1)       |
| Cancel Order | O(1)       |
| Modify Order | O(log N)   |

## Sample Output

TRADE 5 @ 105

===== TRADE HISTORY =====

BUY=2 SELL=1 PRICE=105 QTY=5
