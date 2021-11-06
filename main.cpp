
#include <algorithm>
#include <iostream>
#include <list>

#include "OrderBook.h"
#include "OrderCache.h"
#include <fstream>

using namespace std;
using std::cout;
void TestOrderBook(const std::string &filename)
{
    std::ifstream in(filename, std::ifstream::in);
    std::string command;
    Quote qt;
    std::string sid;

    OrderBook ob;
    while(in.good()){
        in >> command >> qt.orderId >> qt.securityId >> sid >> qt.quantity >> qt.user >> qt.firm;
        qt.side = sid == "Buy" ? Side::Buy : Side::Sell;
        //std::cout << qt;
        if ( command == "Add"){
            ob.addOrder(qt);
        }
    }

    std::cout << "Checking for match" << std::endl;
    ob.match();

    std::cout << ob;
    ob.delOrder("OrdId3");
    ob.match();
    std::cout << ob;
    std::cout << "Deleting orders for User1" << std::endl;
    ob.delUserOrder("User1");
    std::cout << ob;
    ob.delOrder(600);
    std::cout << ob;
}

void TestOrderCache(const std::string &file){
    std::ifstream in(file, std::ifstream::in);
    std::string command;
    Quote qt;
    std::string sid;

    OrderCache orderCache;
    while(in.good()) {
        in >> command >> qt.orderId >> qt.securityId >> sid >> qt.quantity >> qt.user >> qt.firm;
        qt.side = sid == "Buy" ? Side::Buy : Side::Sell;
        if ( command == "Add"){
            orderCache.addOrder(qt);
        }
    }
    orderCache.match();
}

int main()
{
    //std::cout << "Testing OrderBook" << std::endl;
    //TestOrderBook("/Users/rajeshthundil/CLionProjects/OrderCache/input.txt");
    std::cout << "Testing OrderCache, input 1" << std::endl;
    TestOrderCache("/Users/rajeshthundil/CLionProjects/OrderCache/TestCacheInput1.txt");

    std::cout << "Testing OrderCache, input 2" << std::endl;
    TestOrderCache("/Users/rajeshthundil/CLionProjects/OrderCache/TestCacheInput2.txt");

    std::cout << "Testing OrderCache, input 3" << std::endl;
    TestOrderCache("/Users/rajeshthundil/CLionProjects/OrderCache/TestCacheInput3.txt");

}