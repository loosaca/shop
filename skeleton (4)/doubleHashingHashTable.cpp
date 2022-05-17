#include "doubleHashingHashTable.h"

int DoubleHashingHashTable::hi(string k, int i) const
{
    int q = hash2(k, m) *i;
    int p =  (hash(k, m)+q) % m;
    return p;
}

DoubleHashingHashTable::DoubleHashingHashTable(int m, int (*hash)(string, int), int (*hash2)(string, int)) : OpenAddressingHashTable(m, hash)
{

    this->hash2 = hash2;
}
