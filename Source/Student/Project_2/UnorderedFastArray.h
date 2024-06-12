#pragma once
#include "OpenList.h"
class UnorderedFastArray :
    public OpenList
{
private:
	Node list[800];
	int index = -1;

public:
	Node Pop();
	void  Push(Node node);
	void  Clear();
	int   Size();
};

