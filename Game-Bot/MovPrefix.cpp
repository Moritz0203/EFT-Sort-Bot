#include "MovPrefix.h"

AssignPrefix::AssignPrefix()
{
}

AssignPrefix::AssignPrefix(string NameOfItem, uint16_t MinHp, uint8_t IdMov)
{
	this->NameOfItem = NameOfItem;
	this->MinHp = MinHp;
	this->IdMov = IdMov;
}

std::vector<AssignPrefix> AssignPrefix::assignPrefix;


MovPrefix::MovPrefix()
{
}

MovPrefix::MovPrefix(string NameOfItem, uint8_t IdMov)
{
	this->NameOfItem = NameOfItem;
	this->IdMov = IdMov;
}

std::vector<MovPrefix> MovPrefix::movPrefix;