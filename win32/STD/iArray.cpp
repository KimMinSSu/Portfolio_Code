#include "iArray.h"

#include "iStd.h"

iArray::iArray(ARRAY_METHOD m)
{
	node = NULL;
	count = 0;
	method = m;
}

iArray::~iArray()
{
	removeAllObject();
}

void iArray::addObject(void* data)
{
	iNode* n = new iNode;
	n->data = data;
	n->prev = node;

	node = n;
	count++;
}

void iArray::addObject(int index, void* data)
{
	if (index < 0)
		index = 0;
	else
	{
		addObject(data);
		return;
	}

	iNode* n = node;
	for (int i = 0; i < count; i++)
	{
		if (count - 1 - i == index)
		{
			iNode* a = new iNode;
			a->data = data;
			a->prev = n->prev;

			n->prev = a;
			count++;
			return;
		}
		n = n->prev;
	}
}

void iArray::removeObject(int index)
{
	iNode* prevN = NULL;
	iNode* n = node;
	for (int i = 0; i < count; i++)
	{
		if (count - 1 - i == index)
		{
			//저장된 순서 기준
			if (prevN == NULL)
			{
				//제일 마지막 데이터
				node = n->prev;
			}
			else if (n->prev == NULL)
			{
				//제일 처음 데어터
				prevN->prev = NULL;
			}
			else
			{
				//사이 데이터
				prevN->prev = n->prev;
			}

			if (method)
				method(n->data);
			delete n;
			count--;
			return;
		}
		prevN = n;
		n = n->prev;
	}
#if 0
	iNode* currN = NULL;
	iNode* prevN = NULL;
	iNode* nextN = NULL;

	if (index == count - 1)
	{
		//제일 마지막 데이터
		currN = node;
		node = currN->prev;
	}
	else if (index == 0)
	{
		//제일 처음 데어터
		nextN = nodeAtIndex(1);
		currN = nextN->prev;
		nextN->prev = NULL;
	}
	else
	{
		//사이 데이터
		nextN = nodeAtIndex(index + 1);
		currN = nextN->prev;
		prevN = currN->prev;
		nextN->prev = prevN;
	}

	if (method)
		method(currN->data);

	delete currN;
	count--;
#endif
}

void iArray::removeData(void* data)
{
	iNode* prevN = NULL;
	iNode* n = node;
	for (int i = 0; i < count; i++)
	{
		if (n->data == data)//if (count - 1 - i == index)
		{
			prevN->prev = n->prev;
			if (method)
				method(n->data);
			delete n;
			count--;
		}
		prevN = n;
		n = n->prev;
	}
}

void iArray::removeAllObject()
{
	iNode* n = node;
	for (; n;)
	{
		iNode* p = n->prev;
		if (method)
			method(n->data);
		delete n;

		n = p;
	}
}

void* iArray::objectAtIndex(int index)
{
	iNode* n = node;
	int i = count - 1;
	for (; n;)
	{
		if (i == index)
			return n->data;
		n = n->prev;
		i--;
	}

	return NULL;
}

void iArray::replaceAtIndex(int index, void* data, bool del)
{
	iNode* n = node;
	int i = count - 1;
	for (; n;)
	{
		if (i == index)
		{
			if (del)
			{
				if (method)
					method(n->data);
			}
			n->data = data;
			return;
		}
		n = n->prev;
		i--;
	}
}


iNode* iArray::nodeAtIndex(int index)
{
	iNode* n = node;
	int i = count - 1;
	for (; n;)
	{
		if (i == index)
			return n;
		n = n->prev;
		i--;
	}

	return NULL;
}

