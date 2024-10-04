#include "Inventory.h"


unsigned int Inventory::size() const {
  return _inventory.size();
}

void Inventory::addItem(const InventoryItem& theItem)
{
  _inventory.insert( std::pair<unsigned int, const InventoryItem&>(getNewID(), theItem) );
}

unsigned int Inventory::getNewID() const
{

  unsigned int newID = _inventory.size() + 1;

  return newID;
}
