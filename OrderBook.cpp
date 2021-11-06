//
// Created by Rajesh Thundil on 10/31/21.
//

#include "OrderBook.h"
#include <iostream>
#include <algorithm>
#include <set>

void OrderBook::addOrder(const Quote &qt) {
    std::list<OrderEntry> & list = qt.side == Side::Buy ? buy:sell;
    auto &ref = list.emplace_back(qt);
    auto val = std::make_pair(qt.side, --list.end());
    orderIdMap.insert({qt.orderId, val});

    if (qt.side == Side::Buy )
        userOrderMap[qt.user].buy.emplace_back(qt.orderId);
    else
        userOrderMap[qt.user].sell.emplace_back(qt.orderId);

}

void OrderBook::delOrder(const std::string &oid) {
    auto it = orderIdMap.find(oid);
    if ( it != orderIdMap.end()){
        delUserMapEntry(it->second.second->user, it->second.first, it->second.second->orderId);
    }
    delOrderEntry(oid);
    std::cout << "Order " << oid << " deleted" << std::endl;
}

void OrderBook::delOrderEntry(const std::string &orderid) {
    auto it = orderIdMap.find(orderid);

    if ( it != orderIdMap.end()){
        if ( it->second.first == Side::Buy){
            buy.erase(it->second.second);
        }else{
            sell.erase(it->second.second);
        }
        orderIdMap.erase(it);
    }else {
        std::cout << "OrderId: " <<  orderid << " not found" << std::endl;
    }
}

void OrderBook::delUserOrder(const std::string &user) {
    auto u = userOrderMap.find(user);
    if ( u != userOrderMap.end()){
        //Cancel Buy Orders
        for(auto &oid: u->second.buy){
            delOrderEntry(oid);
        }
        for(auto &oid: u->second.sell){
            delOrderEntry(oid);
        }
        userOrderMap.erase(u);
    }
}

void OrderBook::match() {
    unsigned int total_match = 0;
    std::list<OrderEntry> tmpSell;
    std::list<OrderEntry> tmpBuy(buy);

    /*
     * First find the orders from the same company
     */
    std::set<std::string> buyFirms;
    for( auto &o: buy) {
        buyFirms.insert(o.firm);
    }

    std::set<std::string> commonFirms; //Firms with position on both side
    std::for_each(sell.begin(), sell.end(), [&tmpSell, &buyFirms, &commonFirms](OrderEntry &oe){
        if ( auto itr = buyFirms.find(oe.firm); itr != buyFirms.end()){
            //Firm has orders on both sides
            commonFirms.insert(oe.firm);
        }
       tmpSell.push_back(oe);
    });

    //Allocate common firms first
    for (auto &bo : tmpBuy){
        if ( commonFirms.find(bo.firm) == commonFirms.end()) {
            continue;
        }
        unsigned int remaining = bo.quantity;
        for (auto i = tmpSell.begin(); i != tmpSell.end() && remaining != 0; ){
            if ( bo.firm != i->firm){
                unsigned int matched_qty = std::min(bo.quantity, i->quantity);
                if ( i->quantity - matched_qty == 0){
                    i = tmpSell.erase(i);
                }else{
                    i->quantity -= matched_qty;
                    ++i;
                }
                bo.quantity -= matched_qty;
                total_match += matched_qty;

                remaining -= matched_qty;
            }else{
                ++i;
            }
        }
    }
    //std::cout << "Common match: " << securityId << " " << total_match <<  std::endl;

    //Allocate for the rest
    for (auto &bo : tmpBuy){
        if ( commonFirms.find(bo.firm) != commonFirms.end()) continue;
        unsigned int remaining = bo.quantity;
        for (auto i = tmpSell.begin(); i != tmpSell.end() && remaining != 0; ){

            if ( bo.firm != i->firm){
                //std::cout << bo.orderId << "<->" << i->orderId << std::endl;
                unsigned int matched_qty = std::min(bo.quantity, i->quantity);
                if ( i->quantity - matched_qty == 0){
                    i = tmpSell.erase(i);
                }else{
                    i->quantity -= matched_qty;
                    ++i;
                }
                bo.quantity -= matched_qty;
                total_match += matched_qty;

                remaining -= matched_qty;
            }else{
                ++i;
            }
        }
    }
    std::cout << securityId << " " << total_match << std::endl;
}

void OrderBook::delUserMapEntry(const std::string &user, Side side, const std::string &oid){
    auto userItr = userOrderMap.find(user);

    if ( userItr != userOrderMap.end()){
        std::list<std::string> &list = side == Side::Buy ? userItr->second.buy : userItr->second.sell;

        for (auto itr = list.begin(); itr != list.end(); ++itr){
            if ( *itr == oid){
                list.erase(itr);
                break;
            }
        }
    }else{
        std::cout << "Error: Order info not in FirmOrderMap" << std::endl;
    }

}

void OrderBook::delOrder(unsigned int  qty){
    auto bItr = buy.begin();
    while (bItr != buy.end()){
        if ( bItr->quantity >= qty){
            bItr = buy.erase(bItr);
            continue;
        }
        ++bItr;
    }
    auto sItr = sell.begin();
    while (sItr != sell.end()){
        if ( sItr->quantity >= qty){
            sItr = sell.erase(sItr);
            continue;
        }
        ++sItr;
    }

}

std::ostream& operator<<(std::ostream &os, const Quote &qt){
    os << "++++++++++++++++++++++" << std::endl;
    os << "OrderID: " << qt.orderId << std::endl;
    os << "Security ID: " << qt.securityId << std::endl;
    os << "Side: " << (qt.side == Buy ? "Buy" : "Sell") << std::endl;
    os << "Quantity: " << qt.quantity << std::endl;
    os << "User: " << qt.user << std::endl;
    os << "Firm: " << qt.firm << std::endl;
    os << "++++++++++++++++++++++" << std::endl;
    return os;
}
std::ostream& operator<<(std::ostream &os, const OrderEntry &oe){
    os << "OrderId:" << oe.orderId;
    os << ",Quantity" << oe.quantity;
    os << ",User:" << oe.user;
    os<< ",Firm" << oe.firm;
    os << std::endl;
    return os;
}
std::ostream& operator<<(std::ostream &os, const OrderBook &ob){
    os << "===========OrderBook=============" << std::endl;
    os << "Buy:" << std::endl;
    for(auto &oe: ob.buy){
        os << oe;
    }
    os << "Sell: " << std::endl;
    for (auto &oe: ob.sell){
        os << oe;
    }
    os << "===========OrderBook End=============" << std::endl;
    return os;
}