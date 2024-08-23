
#include "OrderBook.h"
#include "httpRequest.hpp"


int main() {
    // Create an instance of OrderBook
   // std::string uri = "http://www.google.com";
    //std::string method = "GET";
    //std::string arguments;
    //std::string output;
    //auto protocol = http::InternetProtocol::v4;

    //http::Request request{ uri, protocol };
    //const auto response = request.send(method, arguments, {
      //      {"Content-Type", "application/x-www-form-urlencoded"},
        //    {"User-Agent", "runscope/0.1"},
          //  {"Accept", "*/*"}
       // }, std::chrono::seconds(2));

    //std::cout << response.status.reason << '\n';

    /*if (response.status.code == http::Status::Ok)
    {
        if (!output.empty())
        {
            
            std::cout << (response.body.data());
                
        }
        else
            std::cout << std::string{ response.body.begin(), response.body.end() } << '\n';
    }*/

    OrderBook orderBook;

    // Create sample orders of different types
    OrderBook::Order order1(1, OrderBook::OrderType::Market, OrderBook::Side::Buy, 0, 10);
    OrderBook::Order order2(2, OrderBook::OrderType::Limit, OrderBook::Side::Sell, 101.0, 20);
    OrderBook::Order order3(3, OrderBook::OrderType::Limit, OrderBook::Side::Sell, 99.0, 5);
    OrderBook::Order order4(4, OrderBook::OrderType::Market, OrderBook::Side::Sell, 0, 15);
    OrderBook::Order order5(5, OrderBook::OrderType::GoodTillCanceled, OrderBook::Side::Buy, 102.0, 10);
    OrderBook::Order order6(6, OrderBook::OrderType::FillOrKill_Limit, OrderBook::Side::Sell, 100.0, 8);
    OrderBook::Order order7(7, OrderBook::OrderType::FillOrKill_Limit, OrderBook::Side::Buy, 99.0, 12);
    OrderBook::Order order8(8, OrderBook::OrderType::FillOrKill_Limit, OrderBook::Side::Buy, 101.0, 8);

    // Add orders to the order book
    orderBook.addOrder(order1);
    orderBook.addOrder(order2);
    orderBook.addOrder(order3);
    orderBook.addOrder(order4);
    orderBook.addOrder(order5);
    orderBook.addOrder(order6);
    orderBook.addOrder(order7);
    orderBook.addOrder(order8);

    // Print the order book before matching orders
    std::cout << "Order Book before matching:" << std::endl;
    orderBook.printOrders();

    // Match orders in the order book
    orderBook.matchOrders();

    // Print the order book after matching
    std::cout << "Order Book after matching:" << std::endl;
    orderBook.printOrders();
    

   

    return 0;
}
