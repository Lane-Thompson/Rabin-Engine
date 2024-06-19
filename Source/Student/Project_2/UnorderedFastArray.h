#pragma once
#include "OpenList.h"
class UnorderedFastArray :
    public OpenList
{
private:
	Node* list[400];
	int index = -1;
	//int mostNodes = 0;

public:
	Node* Pop();
	void Push(Node* node);
	void Remove(Node* nodeToRemove);
	void Remove(int i);
	void Clear();
	int  Size() const;
	//void PrintMostNodes() const { printf("Most nodes in OpenList: %d\n", mostNodes); }
};

