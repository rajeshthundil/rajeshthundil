//
// Created by Rajesh Thundil on 10/31/21.
//

#ifndef ORDERCACHE_ORDERBOOK_H
#define ORDERCACHE_ORDERBOOK_H

#include <list>
#include <map>
#include <string>
#include <ostream>

enum Side{
    Buy,
    Sell
};
struct Quote{
    std::string orderId;
    std::string securityId;
    Side side;
    unsigned int quantity;
    std::string user;
    std::string firm;
    friend std::ostream& operator<<(std::ostream &os, const Quote &);
};



struct OrderEntry{
    std::string orderId;
    unsigned int quantity=0;
    std::string user;
    std::string firm;
    OrderEntry()=default;
    explicit OrderEntry(const Quote &qt):orderId(qt.orderId),
    quantity(qt.quantity),
    user(qt.user),
    firm(qt.firm){

    }
    bool operator<(const OrderEntry &oe)const{
        return orderId < oe.orderId;
    }
    friend std::ostream& operator<<(std::ostream &os, const OrderEntry &oe);
};


struct OrderList{
    std::list<std::string> buy;
    std::list<std::string> sell;
};

class OrderBook{
public:
    OrderBook()=default;
    void addOrder(const Quote &qt);
    void delOrder(const std::string &oid);
    void delOrder(unsigned int  qty);
    void delUserOrder(const std::string &user);
    void match();
    void setSecurityId(const std::string &secid){
        securityId = secid;
    }

private:
    std::string securityId;
    std::list<OrderEntry> buy;
    std::list<OrderEntry> sell;
    std::map<std::string, std::pair<Side, std::list<OrderEntry>::iterator>> orderIdMap;
    std::map<std::string, OrderList> userOrderMap;

    void delUserMapEntry(const std::string &user, Side side, const std::string &oid);
    void delOrderEntry(const std::string &orderid);
    friend std::ostream& operator<<(std::ostream &os, const OrderBook &ob);
};
#endif //ORDERCACHE_ORDERBOOK_H
