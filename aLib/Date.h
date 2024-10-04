
#pragma once

#include <iostream>

#include <string>

using namespace std;

class ItemDate {
private:
  unsigned int month;
  unsigned int day;
  unsigned int year;
public:
  //These are constructors
  ItemDate();
  ItemDate(unsigned int, unsigned int, unsigned int);
  //Destructor 
  ~ItemDate() {}

  unsigned int getYear() const;
  unsigned int getMonth() const;
  unsigned int getDay() const;

  //void do not return values
  void setDay(unsigned int);
  void setMonth(unsigned int);
  void setYear(unsigned int);
  void showDate1();
  void showDate2();
  void showDate3();
};