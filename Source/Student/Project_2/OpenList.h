#pragma once
#include "Node.h"
class OpenList
{
public:
	virtual Node* Pop() = 0;
	virtual void  Push(Node* node) = 0;
	virtual bool  Remove(Node* nodeToRemove) = 0;
	virtual void  Update() {};
	virtual void  Clear () = 0;
	virtual int   Size () const = 0;
};

