#include "quadraticProbingHashTable.h"
QuadraticProbingHashTable::QuadraticProbingHashTable(int m, int (*hash)(string, int)) : OpenAddressingHashTable(m, hash) {}
int power(int x, unsigned int y)
{
    if (y == 0)
        return 1;
    else if (y % 2 == 0)
        return power(x, y / 2) * power(x, y / 2);
    else
        return x * power(x, y / 2) * power(x, y / 2);
}

int QuadraticProbingHashTable::hi(string k, int i) const
{
    return(hash(k, m) +power(i, 2)) % m;
}
