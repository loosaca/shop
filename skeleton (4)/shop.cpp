#include "shop.h"
#include <iostream>
#include <cctype> //hint: you may use the isupper function in it
using std::cout;
using std::endl;
Shop::Shop(HashTable *fireTable, HashTable *iceTable, HashTable *lightningTable) : fireTable{fireTable}, iceTable{iceTable}, lightningTable(lightningTable), profit(0) {}
Shop::~Shop() {
    if(fireTable) delete fireTable;
    if(iceTable) delete iceTable;
    if(lightningTable) delete lightningTable;
}
bool Shop::stockUp(string name, int quantity, int price) const
{

    if (name[0] == 'F')
    {
        Magic *m = new Magic();
        m->prefix = "Fire";
        m->suffix = name.substr(4, string::npos);
        m->price = price;
        m->quantity = quantity;
        return fireTable->add(m);
    }
    else if (name[0] == 'I')
    {
        Magic *m = new Magic();
        m->prefix = "Ice";
        m->suffix = name.substr(3, string::npos);
        m->price = price;
        m->quantity = quantity;
        return iceTable->add(m);
    }
    else
    {
        Magic *m = new Magic();
        m->prefix = "Lightning";
        m->suffix = name.substr(5, string::npos);
        m->price = price;
        m->quantity = quantity;
        return lightningTable->add(m);
    }
    return true;
}
bool Shop::sell(string name, int quantity)
{
    if (name[0] == 'F')
    {
        Magic *m = fireTable->get(name.substr(4, string::npos));

        if (!m || m->quantity < quantity)
            return false;
        else
        {
            profit += quantity * m->price;
            m->quantity -= quantity;
        }
    }
    else if (name[0] == 'I')
    {

        Magic *m = iceTable->get(name.substr(3, string::npos));
        if (!m ||m->quantity < quantity)
            return false;
        else
        {
            profit += quantity * m->price;
            m->quantity -= quantity;
        }
    }
    else
    {
        Magic *m = iceTable->get(name.substr(3, string::npos));
        if (!m ||m->quantity < quantity)
            return false;
        else
        {
            profit += quantity * m->price;
            m->quantity -= quantity;
        }
    }
    return true;
}