#pragma once

enum class ItemStatus {

  UNKNOWN_STATUS,
  LOCAL,
  ON_LOAN,
  MISSING

};

// InventoryItem is the base class for all concrete objects which can exist in the user inventory 
class InventoryItem
{
private:
  unsigned int _itemID;
  ItemStatus _status;


public:
  InventoryItem();
  virtual ~InventoryItem();
};

