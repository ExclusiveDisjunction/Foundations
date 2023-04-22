#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <cstring>

class HashableItem
{
    public:
    virtual int GetHash() const = 0;
    virtual bool EquatesTo(const HashableItem& Other) const
    {
        return GetHash() == Other.GetHash();
    }
};

template<typename T> requires (std::is_base_of<HashableItem, T>().value == true)
class HashTableEntry
{
private:
    T* Value;
    int ValueSize = 0;

public:
    void Destroy()
    {
        if (ValueSize == 1)
            delete Value;
        else if (ValueSize != 0)
            free(Value);

        Value = nullptr;
    }

    void AppendValue(const T& NewValue)
    {
        if (ContainsItem(NewValue)) //Pre Check to see if the object already exists.
            return;


        if (Value == nullptr || ValueSize == 0) //If the node is empty, then adding the value is as simple as setting the value to the Value Variable.
        {
            Value = (T*)malloc(sizeof(T));
            *Value = NewValue;
            ValueSize++;
            return;
        }
        else //If Value is not nullptr, or ValueSize is not zero, then a list must be constructed. 
        {
            ++ValueSize; //Expands the bounds of the array. 
            Value = (T*)realloc(Value, sizeof(T) * ValueSize); //Resizes the array so that a new item can be added in, since Size = ValueSize + 1. 
            Value[ValueSize - 1] = NewValue; //Assigns the new value. 
            return;
        }
    }
    bool RemoveValue(const T& OldValue)
    {
        /*
            If the ValueSize is 1, then the node is "simple". If the value of the node is equal to the OldValue, then remove it. If not, do nothing. 
            If the ValueSize is 0, then the node is "intrinsic", and therefore the OldValue cannot exist in the node.
            If the ValueSize > 0, then the node is "complex", and the value list must be searched for OldValue to remove it. 
        */

        if (ValueSize == 0)
            return false;
        else if (ValueSize == 1)
        {
            if (OldValue->EquatesTo(Value))
            {
                delete Value;
                Value = nullptr;
                ValueSize = 0;
                return true;
            }
            return false;
        }
        else
        {
            int Target = -1;
            for (int i = 0; i < ValueSize; i++)
            {
                if (Value[i]->EquatesTo(Value))
                    Target = i;
            }

            if (Target < 0) //The item was never found.
                return false;

            /*
                Now that we know the index, the list must be shifted so that all of the items after the target element are moved one item back. However, if the index is at the begining, the system must move the whole list. If the item is at the end, realloc can just simply re-allocate the list to be one item less.
            */

            if (Target == ValueSize - 1) //End
                Value = (T*)realloc(Value, sizeof(T) * (--ValueSize));
            else //Somewhere in the middle or at begining
            {
                memcpy(Value, Value[Target + 1], sizeof(T) * (ValueSize - 1));
                Value = (T*)realloc(Value, sizeof(T) * (--ValueSize));
            }
            return true;
        }
    }
    
    bool ContainsItem(const T& Obj)
    {
        if (ValueSize == 0)
            return false;
        else if (ValueSize == 1)
            return Obj.EquatesTo(*Value);
        else
        {
            for (int i = 0; i < ValueSize; i++)
                if (Value[i].EquatesTo(Obj))
                    return true;
            return false;
        }
    }
};

template<typename T> requires (std::is_base_of<HashableItem, T>().value == true)
class HashTable
{
private:
    HashTableEntry<T>* Values;
    int Size = 0;

    HashTableEntry<T>* HashAndSort(T* Input, int Size)
    {
        //First, compute the hash & mod for each item in the list, kept in a pair. 
        std::vector<std::pair<int, int>> Locations(Size);
        for (int i = 0; i < Size; i++)
        {
            std::pair<int, int> ThisPair(i, 0);
            T ThisInstance = Input[i];
            int Hash = ThisInstance.GetHash();
            int Mod = Hash % Size;

            ThisPair.second = Mod;
            Locations.push_back(ThisPair);
        }

        HashTableEntry<T>* Return = (HashTableEntry<T>*)malloc(sizeof(HashTableEntry<T>) * Size);
        memset(Return, 0, sizeof(HashTableEntry<T>) * Size);

        for (std::pair<int, int>& Location : Locations)
            Return[Location.second].AppendValue(Input[Location.first]);

        return Return;
    }
public:
    HashTable(std::vector<T> Items)
    {

    }
    HashTable(T* const& First, int Size)
    {

    }
    HashTable() noexcept
    {

    }

    void Add(std::vector<T> Items)
    {
        if (Items.size() == 0)
            return;

        T* LargeItems = (T*)malloc(sizeof(T) * (Items.size() + Size));
        int i = 0;
        for (T& Item : Items)
            LargeItems[i++] = Item;

        ++i;
        memcpy(&LargeItems[i], Values, sizeof(T) * Size);
        
        free(Values);
        Values = HashAndSort(LargeItems, (this->Size += Items.size()));
        free(LargeItems);
    }
    void Add(T* const& Items, int ItemsSize) //Adds a singular or list of items. 
    {
        if (ItemsSize == 0)
            return;

        T* LargeItems = (T*)malloc(sizeof(T) * (ItemsSize + Size));
        memcpy(LargeItems, Items, sizeof(T) * ItemsSize);
        memcpy(&LargeItems[ItemsSize], Values, sizeof(T) * Size);
        Size += ItemsSize;

        free(Values);
        Values = HashAndSort(LargeItems, Size);
        free(LargeItems);
    }
};

class Test : public HashableItem
{
public:
    int Thing = 0;
    Test() noexcept : Thing(0) {}
    Test(int Value) noexcept : Thing(Value) {}

    int GetHash() const override
    {
       std::hash<int> Hash = std::hash<int>();
       return Hash(Thing);
    }
};

int main()
{
    std::vector<Test> TestObjects;
    TestObjects.push_back(Test(50));
    TestObjects.push_back(Test(70));

    HashTable<Test> ListTest;
    ListTest.Add(TestObjects);
}