#include "pch.h"
#include "BucketList.h"

void BucketList::UpdateIndex()
{
    for (int bucket = 0; bucket < 100; ++bucket)
    {
        if (list[bucket].Size() > 0)
        {
            index = bucket;
            return;
        }
    }
    index = 1000;
}

Node* BucketList::Pop()
{
    if (size == 0) return nullptr;
    size--;
    Node* bestNode = list[index].Pop();
    if (list[index].Size() == 0) UpdateIndex();
    return bestNode;
}

void BucketList::Push(Node* node)
{
    size++;

    float cost = node->finalCost;
    int bucket = static_cast<int>(floor(cost / bucket_difference));
    list[bucket].Push(node);
    if (index > bucket) index = bucket;
}

bool BucketList::Remove(Node* nodeToRemove)
{
    for (auto& bucket : list)
    {
        if (bucket.Remove(nodeToRemove))
        {
            size--;
            UpdateIndex();
            return true;
        }
    }
    return false;
}

void BucketList::Clear()
{
    for (auto& bucket : list)
    {
        bucket.Clear();
    }
    index = 1000;
    size = 0;
}

int BucketList::Size() const
{
    return size;
}
