//
// Created by Rajesh Thundil on 11/5/21.
//
#include <iostream>
#include "OrderCache.h"

void OrderCache::addOrder(const Quote &qt) {
    auto itr = cache.find(qt.securityId);
    if ( itr == cache.end()){
        //New Security ID
        auto &ref = cache[qt.securityId];
        ref.setSecurityId(qt.securityId);
        ref.addOrder(qt);
        orderToBookMap.insert(std::make_pair(qt.orderId, &ref));
    }else{
        cache[qt.securityId].addOrder(qt);
    }
}

void OrderCache::cancelOrder(const std::string &oid) {
    auto itr = orderToBookMap.find(oid);
    if ( itr != orderToBookMap.end()){
        std::cout << "Deleting Order: " << oid << std::endl;
        itr->second->delOrder(oid);
    }
}

void OrderCache::cancelUserOrder(const std::string &user) {
    for ( auto item : cache){
        item.second.delUserOrder(user);
    }
}

void OrderCache::cancelSecurityOrders(const std::string &securityid, unsigned int qty) {
    if ( auto itr = cache.find(securityid); itr != cache.end()){
        itr->second.delOrder(qty);
    }
}

void OrderCache::match() {
    for ( auto &item : cache){
        item.second.match();
//        if (item.first == "SecId3")
//        {
//            std::cout << item.second;
//        }
    }
}