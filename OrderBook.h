#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

class OrderBook {
public:
    // Enumerations for order types and sides
    enum class OrderType { Market, Limit, Stop, GoodTillCanceled, FillOrKill_Limit };
    enum class Side { Buy, Sell };

    // Order class representing individual orders
    class Order {
    public:
        // Constructor to initialize an order
        Order(int id, OrderType type, Side side, double price, int quantity)
            : id(id), type(type), side(side), price(price), quantity(quantity) {}

        // Getter methods for order attributes
        int getId() const { return id; }
        OrderType getType() const { return type; }
        Side getSide() const { return side; }
        double getPrice() const { return price; }
        int getQuantity() const { return quantity; }

        // Method to update order quantity
        void setQuantity(int new_quantity) { quantity = new_quantity; }

    private:
        int id;
        OrderType type;
        Side side;
        double price;
        int quantity;
    };

    // Method to add an order to the order book
    void addOrder(const Order& order) {
        orders.push_back(order);
    }

    // Method to cancel an order by its ID
    void cancelOrder(int orderId) {
        auto it = std::remove_if(orders.begin(), orders.end(), [orderId](const Order& order) {
            return order.getId() == orderId;
            });

        if (it != orders.end()) {
            std::cout << "Canceled Order ID: " << orderId << std::endl;
            orders.erase(it, orders.end());
        }
    }

    // Method to match orders in the order book
    void matchOrders() {
        // First, handle Market orders
        for (auto it = orders.begin(); it != orders.end();) {
            if (it->getType() == OrderType::Market) {
                auto matchIt = findMatch(it, it->getQuantity());
                if (matchIt != orders.end()) {
                    executeOrder(it, matchIt);
                    it = orders.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }

        // Handle GoodTillCanceled orders
        for (auto it = orders.begin(); it != orders.end();) {
            if (it->getType() == OrderType::GoodTillCanceled) {
                auto matchIt = findMatch(it, it->getQuantity());
                if (matchIt != orders.end()) {
                    executeOrder(it, matchIt);
                    it = orders.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }

        // Handle Fill-Or-Kill orders separately
        for (auto it = orders.begin(); it != orders.end();) {
            if (it->getType() == OrderType::FillOrKill_Limit) {
                auto matchIt = findMatch(it, it->getQuantity(), true); // Ensure full match
                if (matchIt != orders.end() && matchIt->getQuantity() >= it->getQuantity()) {
                    executeOrder(it, matchIt);
                    it = orders.erase(it);
                }
                else {
                    std::cout << "Canceled FOK Order ID: " << it->getId() << std::endl;
                    it = orders.erase(it);
                }
            }
            else {
                ++it;
            }
        }

        // Finally, handle Limit orders that were not matched by Market or GTC orders
        for (auto it = orders.begin(); it != orders.end();) {
            if (it->getType() == OrderType::Limit) {
                auto matchIt = findMatch(it, it->getQuantity());
                if (matchIt != orders.end()) {
                    executeOrder(it, matchIt);
                    it = orders.erase(it);
                }
                else {
                    ++it;
                }
            }
            else {
                ++it;
            }
        }
    }

    // Method to print all orders in the order book
    void printOrders() const {
        for (const auto& order : orders) {
            printOrder(order);
        }
    }

private:
    std::vector<Order> orders; // Vector to store orders in the order book

    // Helper method to find a match for a given order
    std::vector<Order>::iterator findMatch(std::vector<Order>::iterator orderIt, int quantity, bool fullMatch = false) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->getSide() != orderIt->getSide() &&
                ((orderIt->getSide() == Side::Buy && it->getPrice() <= orderIt->getPrice()) ||
                    (orderIt->getSide() == Side::Sell && it->getPrice() >= orderIt->getPrice())) &&
                (!fullMatch || it->getQuantity() >= quantity)) {
                return it;
            }
        }
        return orders.end();
    }

    // Helper method to execute an order
    void executeOrder(std::vector<Order>::iterator& orderIt, std::vector<Order>::iterator& matchIt) {
        double fillPrice = matchIt->getPrice();
        std::cout << "Matched Order ID: " << orderIt->getId() << " with Order ID: " << matchIt->getId() << " at Price: " << std::fixed << std::setprecision(2) << fillPrice << " quantity: " << orderIt->getQuantity() << std::endl;
        matchIt->setQuantity(matchIt->getQuantity() - orderIt->getQuantity());
        if (matchIt->getQuantity() == 0) {
            orders.erase(matchIt);
        }
    }

    // Helper method to print an individual order
    void printOrder(const Order& order) const {
        std::cout << "Order ID: " << order.getId()
            << ", Type: " << static_cast<int>(order.getType())
            << ", Side: " << (order.getSide() == Side::Buy ? "Buy" : "Sell")
            << ", Price: " << order.getPrice()
            << ", Quantity: " << order.getQuantity() << std::endl;
    }
};
