#pragma once

class stack
{
public:
	void push(int a);
	int pop();
	void clear();
	void print();
	int isempty();
	stack();
	~stack();
private:
	int top;
	int *stos;
};


// kolejne



