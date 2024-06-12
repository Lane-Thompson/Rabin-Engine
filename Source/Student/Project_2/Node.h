#pragma once

enum ListStatus { None, Open, Closed};

class Node
{
public:
	Node();
	Node(int x, int y, float hCost, Node* parent = nullptr);
	Node(GridPos pos, float hCost, Node* parent = nullptr);
	unsigned int DistanceToParent();
	bool operator==(Node& that);
	bool operator==(GridPos& that);

	Node* parent;
	float finalCost;
	float givenCost;
	byte validNeighbors;
	unsigned int xPos : 6;
	unsigned int yPos : 6;
	unsigned int listStatus : 2;
};

