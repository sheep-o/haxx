#include "Entity.h"

bool Entity::IsValid()
{
    if (this && this != 0 && this->Health > 0 && this->Health <= 100 && !this->IsDormant)
        return true;
    else
        return false;
}