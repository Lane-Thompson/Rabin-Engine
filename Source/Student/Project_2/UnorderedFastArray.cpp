#include "pch.h"
#include "UnorderedFastArray.h"


/// <summary>
/// Returns the cheapest node in the openlist. Returns nullptr if list empty
/// </summary>
Node UnorderedFastArray::Pop()
{
	if (Size() == 1)						// If there's only one item, it's the best
	{
		index--;
		return list[0];
	}
	int bestCost = INFINITY;
	unsigned int bestIndex = 0;
	for (int i = 0; i < index + 1; ++i)		// Brute force loop to find best node
	{
		if (list[i].finalCost < bestCost)
		{
			bestCost = list[i].finalCost;
			bestIndex = i;
		}
	}
	Node bestNode = list[bestIndex];
	if (bestIndex == index)			// if the best node is the last one, no swapping necessary
	{
		--index;
		return bestNode;
	}

	list[bestIndex] = list[index];			// Move the last node to empty space
	--index;
	return bestNode;
}
// Node* UnorderedFastArray::Pop()
//{
//	if (index == -1) return nullptr;
//	if (Size() == 1)						// If there's only one item, it's the best
//	{
//		index--;
//		return list[0];
//	}
//	int bestCost = INFINITY;
//	unsigned int bestIndex = 0;
//	for (int i = 0; i < index + 1; ++i)		// Brute force loop to find best node
//	{
//		if (list[i]->finalCost < bestCost)
//		{
//			bestCost = list[i]->finalCost;
//			bestIndex = i;
//		}
//	}
//	Node* bestNode = list[bestIndex];
//	if (bestIndex == index)			// if the best node is the last one, no swapping necessary
//	{
//		--index;
//		return bestNode;
//	}
//
//	list[bestIndex] = list[index];			// Move the last node to empty space
//	--index;
//	return bestNode;
//}

/// <summary>
/// Adds a node to the openlist.
/// </summary>
//void UnorderedFastArray::Push(Node* node)
//{
//	index++;
//	list[index] = node;
//}

/// <summary>
/// Adds a node to the openlist.
/// </summary>
void UnorderedFastArray::Push(Node node)
{
	index++;
	list[index] = node;
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
int UnorderedFastArray::Size()
{
	return index + 1;
}
