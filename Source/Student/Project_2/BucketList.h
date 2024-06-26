#pragma once
#include "OpenList.h"
#include "UnorderedFastArray.h"
class BucketList :
    public OpenList
{
    // max openlist size is 400
    // split by size of 2?

private:
    //float max_distance = 200;
    float bucket_difference = 1.4;
    //int num_buckets = max_distance / bucket difference = 100

    UnorderedFastArray list[150];
    int size = 0;
    int index = 1000;

    void UpdateIndex();

public:
    Node* Pop();
    void Push(Node* node);
    bool Remove(Node* nodeToRemove);
    void Clear();
    int  Size() const;
};

