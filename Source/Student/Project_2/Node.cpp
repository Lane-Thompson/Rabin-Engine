#include "pch.h"
#include "Node.h"

#define sqrt2 1.41421356

Node::Node() 
	: parent(nullptr), finalCost(0), givenCost(0),validNeighbors(0),
		xPos(0), yPos(0), listStatus(ListStatus::None)
{
}

Node::Node(int x, int y, float hCost, Node* parent = nullptr)
	: parent(parent), finalCost(0), givenCost(0), validNeighbors(0),
	xPos(x), yPos(y), listStatus(ListStatus::None)
{
	if (parent != nullptr) givenCost = parent->givenCost + DistanceToParent();
	finalCost = givenCost + hCost;
}

Node::Node(GridPos pos, float hCost, Node* parent = nullptr)
{
	Node(pos.col, pos.row, hCost, parent);
}

unsigned int Node::DistanceToParent()
{
	if (xPos != parent->xPos && yPos != parent->yPos) { return sqrt2; }
	else return 1;
}

bool Node::operator==(Node& that)
{
	return (this->xPos == that.xPos && this->yPos == that.yPos);
}

bool Node::operator==(GridPos& that)
{
	return (this->xPos == that.col && this->yPos == that.row);
}

