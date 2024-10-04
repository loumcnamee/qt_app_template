#pragma once

#include <mutex>
#include "InventoryItem.h"
#include <map>

using namespace std;

class Inventory
{
public:
  static Inventory& getInstance()
  {
    static Inventory instance; // Guaranteed to be destroyed.
                          // Instantiated on first use.
    return instance;
  }

  // C++ 11
  // =======
  // We can use the better technique of deleting the methods
  // we don't want.
  Inventory(Inventory const&) = delete;
  void operator=(Inventory const&) = delete;

  // Note: Scott Meyers mentions in his Effective Modern
  //       C++ book, that deleted functions should generally
  //       be public as it results in better error messages
  //       due to the compilers behavior to check accessibility
  //       before deleted status

  unsigned int size() const;
  
  void addItem(const InventoryItem& theItem);

private:
  Inventory() {} // Constructor? (the {} brackets) are needed here.

  std::map<unsigned int, const InventoryItem&> _inventory;

  unsigned int getNewID() const;


  
};