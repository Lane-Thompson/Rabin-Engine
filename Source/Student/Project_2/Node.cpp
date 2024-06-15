#include "pch.h"
#include "Node.h"

constexpr auto sqrt2 = 1.41421356f;

Node::Node() 
	: parent(nullptr), finalCost(0), givenCost(0),validNeighbors(0),
		xPos(0), yPos(0), listStatus(ListStatus::None)
{
}

Node::Node(int x, int y, float hCost, Node* parent)
	: parent(parent), finalCost(0), givenCost(0), validNeighbors(0),
	xPos(x), yPos(y), listStatus(ListStatus::None)
{
	if (parent != nullptr) givenCost = parent->givenCost + DistanceToParent();
	finalCost = givenCost + hCost;
}

Node::Node(GridPos pos, float hCost, Node* parent)
{
	*this = Node(pos.col, pos.row, hCost, parent);
}

void Node::CalculateValues(float hCost, Node* parent)
{
	listStatus = ListStatus::None;
	parent = parent;
	givenCost = parent == nullptr ? 0 : parent->givenCost + DistanceToParent();
	finalCost = givenCost + hCost;
}

GridPos Node::GetGridPos() const
{
	return GridPos{ static_cast<int>(yPos), static_cast<int>(xPos) };
}

float Node::DistanceToParent() const
{
	if (xPos != parent->xPos && yPos != parent->yPos) { return sqrt2; }
	else return 1;
}

bool Node::operator==(Node& that) const
{
	return (this->xPos == that.xPos && this->yPos == that.yPos);
}

bool Node::operator==(GridPos& that) const
{
	return (this->xPos == that.col && this->yPos == that.row);
}

