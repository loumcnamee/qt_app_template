#pragma once

#include "InventoryItem.h"
#include <string>
#include "Date.h"

class BookItem :   public InventoryItem 
{
private:
  std::string _author;
  std::string _title;
  ItemDate _publishDate;
  
  

public:
  BookItem();
  BookItem(const std::string& author, const std::string& title, const ItemDate& pubDate) {
    _author = author;
    _title = title;
    _publishDate = pubDate;
  };
  void setAuthor(const std::string& theAuthor) {_author = theAuthor; };
  void setTitle(const std::string& theTitle) {_title = theTitle; };
  void setPublishDate(const ItemDate& pubDate) {_publishDate = pubDate; };
  const std::string getAuthor() {return _author;};
  const std::string getTitle() {return _title;};
  const ItemDate getPubDate() {return _publishDate; };
  ~BookItem();
};

