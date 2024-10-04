#include <gtest/gtest.h>

#include "BookItem.h"
#include "Date.h"
#include "Inventory.h"

// Demonstrate some basic assertions.
TEST(DateTest, ConstructorTest) {
  
  ItemDate date1 = ItemDate(12,23,2021);

  ASSERT_EQ(date1.getYear(), 2021);
  ASSERT_EQ(date1.getMonth(), 12);
  ASSERT_EQ(date1.getDay(), 23);
}

// Demonstrate some basic assertions.
TEST(BookItemTest, ConstructorTest) {
  
  ItemDate date1 = ItemDate(12,23,2021);

  BookItem book1 = BookItem("auth1","title1",date1);

  ASSERT_EQ(book1.getAuthor(), "auth1");
  ASSERT_EQ(book1.getTitle(), "title1");

  const ItemDate date1a = book1.getPubDate();

  ASSERT_EQ(date1a.getYear(), 2021);
  ASSERT_EQ(date1a.getMonth(), 12);
  ASSERT_EQ(date1a.getDay(), 23);


}

// Demonstrate some basic assertions.
TEST(BookItemTest, AccessorTest) {
  
  ItemDate date1 = ItemDate(12,23,2021);

  BookItem book1 = BookItem();
  book1.setAuthor("auth1");
  book1.setTitle("title1");
  book1.setPublishDate(date1);

  ASSERT_EQ(book1.getAuthor(), "auth1");
  ASSERT_EQ(book1.getTitle(), "title1");

  const ItemDate date1a = book1.getPubDate();

  ASSERT_EQ(date1a.getYear(), 2021);
  ASSERT_EQ(date1a.getMonth(), 12);
  ASSERT_EQ(date1a.getDay(), 23);

}

// verify the Inventpry is a Singleton
TEST(InventoryTest, SingletonTest) {
  
  
  Inventory& aInventory = Inventory::getInstance();

  Inventory& bInventory = Inventory::getInstance();

  ASSERT_EQ(&aInventory,&bInventory);



}

// Add 1 book to the inventory
TEST(InventoryTest, AddItemTest) {
  
  Inventory& myInventory = Inventory::getInstance();

  ASSERT_EQ(myInventory.size(),0);

  ItemDate date1 = ItemDate(12,23,2021);

  BookItem book1 = BookItem();
  book1.setAuthor("auth1");
  book1.setTitle("title1");
  book1.setPublishDate(date1);

  myInventory.addItem(book1);

  ASSERT_EQ(myInventory.size(),1);

  ItemDate date2 = ItemDate(11,10,2021);

  BookItem book2 = BookItem();
  book1.setAuthor("auth2");
  book1.setTitle("title2");
  book1.setPublishDate(date2);

  myInventory.addItem(book2);

  ASSERT_EQ(myInventory.size(),2);

}
