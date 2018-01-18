#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

using namespace std;

stack::stack()
{
	this->top=0;
	this->stos=(int*)malloc(sizeof(int));
}

stack::~stack()
{
	free(stos);
}

void stack::clear()
{
	if(this->top==0)
	cout<< "stos pusty"<< endl;
	else
        this->top=0;
}

void stack::push(int a)
{
	stos=(int*)realloc(stos,sizeof(int)*(top+1));
	*(this->stos+this->top)=a;
	this->top++;
}


int stack::pop()
{
    int czy_pusty=isempty();
   	if(czy_pusty)
	{
		cout<< "stos pusty"<< endl;
		return 0;
	}
	else
   	{
		return this->stos[--this->top];
	}
}

void stack::print()
{
    int i,j;
    j=this->top;
    for(i=0;i<j;i++)
    {
		cout<< this->stos[i]<< endl;
    }
}

int stack::isempty()
{
	if (this->top==0)
		return 1;
	else
	return 0;
}

