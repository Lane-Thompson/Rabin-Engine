#include "pch.h"
#include "UnorderedFastArray.h"

constexpr auto sqrt2 = 1.41421356f;

/// <summary>
/// Returns the cheapest node in the openlist. Returns nullptr if list empty
/// </summary>
Node* UnorderedFastArray::Pop()
{
	if (index == -1) return nullptr;
	if (Size() == 1)						// If there's only one item, it's the best
	{
		index--;
		return list[0];
	}
	float bestCost = INFINITY;
	unsigned int bestIndex = 0;
	for (int i = 0; i < index + 1; ++i)		// Brute force loop to find best node
	{
		if (list[i]->finalCost < bestCost)
		{
			bestCost = list[i]->finalCost;
			bestIndex = i;
		}
	}
	Node* bestNode = list[bestIndex];
	if (bestIndex != index)			// if the best node is the last one, no swapping necessary
	{
		list[bestIndex] = list[index];			// Move the last node to empty space
	}
	--index;
	return bestNode;
}

/// <summary>
/// Adds a node to the openlist.
/// </summary>
void UnorderedFastArray::Push(Node* node)
{
	//if (Size() > mostNodes) mostNodes = Size();
	index++;
	list[index] = node;
}

void UnorderedFastArray::Remove(Node* nodeToRemove)
{
	if (Size() == 1)
	{
		Pop();
		return;
	}
	for (int i = 0; i < Size(); ++i)
	{
		if (list[i] == nodeToRemove) Remove(i);
	}
}

void UnorderedFastArray::Remove(int i)
{
	list[i] = list[index];			// Move the last node to empty space
	--index;
}

/// <summary>
/// Resets the openlist
/// </summary>
void UnorderedFastArray::Clear()
{
	index = -1;
}

/// <summary>
/// Returns the number of nodes in the openlist.
/// </summary>
int UnorderedFastArray::Size() const
{
	return index + 1;
}
