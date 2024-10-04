#include "Date.h"

ItemDate::ItemDate()
{
  //Initialize variables.
  month = 0, day = 0, year = 0;
}
ItemDate::ItemDate(unsigned int Month, unsigned int Day, unsigned int Year)
{
  month = Month;
  day = Day;
  year = Year;
}
void ItemDate::setDay(unsigned int d)
{
  if (d < 1 && d > 31)
    cout << "The day is invalid" << endl;
  else
    day = d;

}
void ItemDate::setMonth(unsigned int m)
{
  if (m < 1 && m > 12)
    cout << "The month is invalid" << endl;
  else
    month = m;

}
void ItemDate::setYear(unsigned int y)
{
  if (y < 1950 && y > 2020)
    cout << "The year is invalid" << endl;
  else
    year = y;
}


unsigned int ItemDate::getYear() const
 {
    return year;
 }

unsigned int ItemDate::getMonth() const
{
    return month;
}
unsigned int ItemDate::getDay() const
{
    return day;
}

void ItemDate::showDate1()
{
  cout << month << " /" << day << " /" << year << endl;
}
void ItemDate::showDate2()
{
  string monthName[] = { "January", "February", "March",
    "April", "May", "June", "July",
    "August", "September", "October",
    "November", "December" };
  cout << monthName[month - 1] << "  " << day << "  " << year << endl;
}
void ItemDate::showDate3()
{
  string monthName[] = { "January", "February", "March",
    "April", "May", "June", "July",
    "August", "September", "October",
    "November", "December" };
  cout << day << "  " << monthName[month - 1] << "  " << year << endl;
}