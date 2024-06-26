#pragma once
#include "OpenList.h"
class UnorderedFastArray :
    public OpenList
{
private:
	Node* list[400];
	int index = -1;
	//int mostNodes = 0;
	void Remove(int i);

public:
	Node* Pop();
	void Push(Node* node);
	bool Remove(Node* nodeToRemove);
	void Clear();
	int  Size() const;
	//void PrintMostNodes() const { printf("Most nodes in OpenList: %d\n", mostNodes); }
};

