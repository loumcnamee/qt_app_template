// LibraryApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

#include "Inventory.h"
#include "date.h"

int main()
{
    std::cout << "Starting Inventory Management Application..." << std::endl; 

    Inventory& myInventory = Inventory::getInstance();

    std::cout << "Library has " << myInventory.size() << "items " << std::endl;

    ItemDate date1 = ItemDate(12,21,2021);

    std::cout << "Hello, World\n";

   std::vector<int> V{1, 2, 3, 4, 5};
   int a = 8;

   for(auto &e : V) {
      std::cout << e << ' ';
   }
   std::cout << '\n';

   std::cout << "a = " << a << '\n';

    std::cout << "Closing Inventory Management Application" << std::endl;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
