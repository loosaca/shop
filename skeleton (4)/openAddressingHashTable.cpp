#include "openAddressingHashTable.h"
#include "hashTable.h"
void OpenAddressingHashTable::performRehashing()
{

    if (printSteps)
        cout << "Rehashing is needed!" << endl;
    m = m * 2;
    Cell *table1 = new Cell[m];
    for (int i = 0; i < m; i++)
        table1[i].magic = nullptr, table1[i].status = EMPTY;
    for (int i = 0; i < m/2; i++)
    {

        if (table[i].status == ACTIVE)
        {
            int hash_count = 0;

            string suffix = table[i].magic->suffix;

            string fullName = table[i].magic->prefix + suffix;
            int index = hash(suffix, m);

            if (printSteps)
                cout << "hash(\"" << suffix << "\") = " << index << endl;

            while (table1[index].status == ACTIVE)
            {
                comparisonCount++;
                if (printSteps)
                    cout << fullName << " collided at cell " << index << endl;
                index = hi(suffix, ++hash_count);
            }
            comparisonCount++;
            if (printSteps)
                cout << fullName << " added at cell " << index << std::endl;
            table1[index].magic = new Magic();
            table1[index].magic->prefix = table[i].magic->prefix;
            table1[index].magic->suffix = suffix;
            table1[index].magic->price = table[i].magic->price;
            table1[index].magic->quantity = table[i].magic->quantity;
            table1[index].status = ACTIVE;
        }
    }
    if (printSteps)
        cout << "Rehashing is done!" << endl;
    for(int i = 0; i < m/2; i++)
        if(table[i].status == ACTIVE)
            delete table[i].magic;
    delete[] table;
    table = table1;
}
OpenAddressingHashTable::OpenAddressingHashTable(int m, int (*hash)(string, int)) : HashTable(m, hash)
{
    table = new Cell[m];
    for (int i = 0; i < m; i++)
    {
        table[i].magic = nullptr;
        table[i].status = EMPTY;
    }
}
OpenAddressingHashTable::~OpenAddressingHashTable()
{
    for (int i = 0; i < m; i++)
    {
        if (table[i].status == ACTIVE)
            delete table[i].magic;
    }
    delete[] table;
}
struct Magic;
struct Cell;

// void add_val(Magic *&magic1, Magic *magic2)
// {
//     magic1->price = magic2->price;
//     magic1->quantity += magic2->quantity;
// }

void set_val(Magic *&magic1, Magic *magic2)
{

    magic1 = new Magic();
    magic1->prefix = magic2->prefix;
    magic1->suffix = magic2->suffix;
    magic1->price = magic2->price;
    magic1->quantity = magic2->quantity;
}

bool OpenAddressingHashTable::add(Magic *magic)
{

    int count = 0;
    int hash_count = 0;
    string fullName = magic->prefix + magic->suffix;

    int index = hash(magic->suffix, m);
    if (printSteps)
        cout << "hash(\"" << magic->suffix << "\") = " << index << endl;
    bool printstate_temp = printSteps;
    int get_comp_temp = comparisonCount;
    printSteps = false;
    if (Magic *m_get = get(magic->suffix))
    {

        printSteps = printstate_temp;

        while (table[index].magic != m_get)
        {
            if (printSteps)
                cout << fullName << " collided at cell " << index << endl;
            index = hi(magic->suffix, ++hash_count);
        }
    }
    else
    {
        comparisonCount = get_comp_temp;
        // comparisonCount = get_comp_temp;
        printSteps = printstate_temp;
        // int temp = comparisonCount;
        if (activeCellCount > m / 2)
        {
            performRehashing();
            index = hash(magic->suffix, m);
        }
        // count += comparisonCount - temp;
        // comparisonCount = temp;

        while (table[index].status == ACTIVE)
        {
            if (printSteps)
                cout << fullName << " collided at cell " << index << endl;
            if (++count >= m)
            {
                if (printSteps)
                    cout << fullName << " cannot be added" << endl;
                comparisonCount += m;
                return false;
            }

            index = hi(magic->suffix, ++hash_count);
        }
    }

    if (count++ >= m)
    {

        if (printSteps)
            cout << fullName << " cannot be added" << endl;
        comparisonCount += m;
        return false;
    }
    if (printSteps)
        cout << fullName << " added at cell " << index << std::endl;
    // table.magic = new Magic();
    // table.magic->prefix = magic->prefix;
    // table.magic->suffix = magic->suffix;
    // table.magic->price = magic->price;
    // table.magic->quantity = magic->quantity;
    // table.status = ACTIVE;
    activeCellCount++;
    if (table[index].status == ACTIVE)
    {
        table[index].magic->price = magic->price;
        table[index].magic->quantity += magic->quantity;
    }
    else
    {
        set_val(table[index].magic, magic);
    }
    delete magic;
    table[index].status = ACTIVE;
    comparisonCount += count;
    return true;
}
bool OpenAddressingHashTable::remove(string key)
{
    int index = hash(key, m);
    int hash_count = 0;
    if (printSteps)
        cout << "hash(\"" << key << "\") = " << index << endl;

    int count = 0;
    for (; index < m; index = hi(key, ++hash_count))
    {

        if (count >= m)
        {
            if (printSteps)
                cout << "visited cell " << index << " but could not find it" << endl;
            if (printSteps)
                cout << key << " cannot be removed" << endl;
            comparisonCount += m;
            return false;
        }

        if (table[index].status == EMPTY)
        {
            if (printSteps)
                cout << "visited cell " << index << " but could not find it" << endl;
            if (printSteps)
                cout << key << " cannot be removed" << endl;
            comparisonCount += count + 1;
            return false;
        }
        else if (table[index].status == ACTIVE)
        {
            if (table[index].magic->suffix == key)
            {
                table[index].status = DELETED;
                activeCellCount--;
                if (printSteps)
                    cout << table[index].magic->prefix + key << " removed at cell " << index << endl;

                delete table[index].magic;
                comparisonCount += count + 1;
                return true;
            }
        }
        if (printSteps)
            cout << "visited cell " << index << " but could not find it" << endl;

        count++;
    }
}
Magic *OpenAddressingHashTable::get(string key)
{
    int index = hash(key, m);
    int hash_count = 0;
    if (printSteps)
        cout << "hash(\"" << key << "\") = " << index << endl;
    int count = 0;
    while (true)
    {
        count++;

        if (count >= m)
        {
            if (printSteps)
            {
                cout << "visited cell " << index << " but could not find it" << endl;

                cout << key << " cannot be found" << endl;
            }
            comparisonCount += m;
            return nullptr;
        }

        if (table[index].status == EMPTY)
        {
            if (printSteps)
            {
                cout << "visited cell " << index << " but could not find it" << endl;

                cout << key << " cannot be found" << endl;
            }
            comparisonCount += count;
            return nullptr;
        }
        else if (table[index].status == ACTIVE)
        {
            if (table[index].magic->suffix == key)
            {
                if (printSteps)
                    cout << table[index].magic->prefix + table[index].magic->suffix << " found at cell " << index << endl;

                comparisonCount += count;
                return table[index].magic;
            }
            if (count >= m)
            {
                if (printSteps)
                {
                    cout << "visited cell " << index << " but could not find it" << endl;

                    cout << key << " cannot be found" << endl;
                }
                comparisonCount += m;
                return nullptr;
            }
        }

        if (printSteps)
            cout << "visited cell " << index << " but could not find it" << endl;
        index = hi(key, ++hash_count);
    }
}
int OpenAddressingHashTable::getClusterCount() const
{
    int count = 0;
    int i1 = 0, i2 = m - 1;

    if (table[i1].status == ACTIVE && table[i2].status == ACTIVE)
    {
        count++;
        i1 = 1, i2 = m - 2;

        while (table[i1].status == ACTIVE)
            i1++;
        while (table[i2].status == ACTIVE)
            i2--;
    }
    i2++;
    for (; i1 < i2; i1++)
    {
        if (table[i1].status == ACTIVE)
        {
            count++;
            while (i1 < i2 && table[i1].status == ACTIVE)
                i1++;
        }
    }
    return count;
}
int OpenAddressingHashTable::getLargestClusterSize() const
{
    int count = 0;
    int res = 0;
    int i1 = 0, i2 = m - 1;

    if (table[i1].status == ACTIVE && table[i2].status == ACTIVE)
    {
        res += 2;
        i1 = 1, i2 = m - 2;

        while (table[i1].status == ACTIVE)
            i1++, res++;
        while (table[i2].status == ACTIVE)
            i2--, res++;
    }
    i2++;
    for (; i1 < i2; i1++, count = 0)
    {

        if (table[i1].status == ACTIVE)
        {
            while (i1 < i2 && table[i1].status == ACTIVE)
                count++, i1++;
        }
        res = count > res ? count : res;
    }
    return res;
}

void insertSort(int arr[], int target, int size)
{
    for (int i = 0; i < size; i++)
    {

        if (arr[i] < target)
        {
            int temp = arr[i];
            arr[i] = target;

            for (int j = i + 1; j < size - 1; j++)
            {
                int temp2 = arr[j];
                arr[j] = temp;
                temp = temp2;
            }
        }
    }
}
string OpenAddressingHashTable::getClusterSizeSortedList() const
{
    string res = "";
    int num = 0;
    int count = 0;
    bool state = true;
    int i1 = 0, i2 = m - 1;
    int *arr = new int[m]();
    if (table[i1].status == ACTIVE && table[i2].status == ACTIVE)
    {
        state = false;
        count += 2, num++;
        i1 = 1, i2 = m - 2;
        while (table[i1].status == ACTIVE)
            i1++, count++;
        while (table[i2].status == ACTIVE)
            i2--, count++;
        arr[0] = count;
    }
    i2++;
    if (state)
    {
        for (; i1 < i2; i1++)
        {
            if (table[i1].status == ACTIVE)
            {
                num++;
                while (i1 < i2 && table[i1].status == ACTIVE)
                    count++, i1++;
                arr[0] = count;
                break;
            }
        }
    }

    i1++;
    count = 0;
    for (; i1 < i2; i1++, count = 0)
    {
        if (table[i1].status == ACTIVE)
        {
            num++;
            while (i1 < i2 && table[i1].status == ACTIVE)
                count++, i1++;
            insertSort(arr, count, num);
        }
    }
    for (int i = 0; i < num - 1; i++)
        res.append(std::to_string(arr[i])), res.append(",");
    if (num > 0)
        res.append(std::to_string(arr[num - 1]));
    delete[] arr;
    return res.empty() ? "(empty)" : res;
}