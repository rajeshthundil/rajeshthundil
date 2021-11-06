//
// Created by Rajesh Thundil on 11/5/21.
//

#ifndef ORDERCACHE_ORDERCACHE_H
#define ORDERCACHE_ORDERCACHE_H

#include "OrderBook.h"

class OrderCache{
public:
    void addOrder(const Quote &qt);
    void cancelOrder(const std::string &oid);
    void cancelUserOrder(const std::string &user);
    void cancelSecurityOrders(const std::string &securityid, unsigned int qty);
    void match();
private:
    std::map<std::string, OrderBook> cache;
    std::map<std::string, OrderBook*> orderToBookMap;
};
#endif //ORDERCACHE_ORDERCACHE_H
