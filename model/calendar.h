#ifndef CPP3_S21_SMARTCALCULATOR_MODEL_CALENDAR_H_
#define CPP3_S21_SMARTCALCULATOR_MODEL_CALENDAR_H_
#include <iomanip>
#include <iostream>
#include <utility>
#include <variant>
#include <vector>

namespace s21 {

enum class Periods { kDay, kWeek, kMonth, kQuarter, kHalfYear, kYear, kEnd };

class Days {
 public:
  explicit Days(int day) : day_(day) {}
  Days() : day_(0) {}
  int day() const { return day_; }
  void SetDay(int day) { day_ = day; }
  const Days operator-() const { return Days(-day_); }
  Days& operator=(const Days& other) {
    day_ = other.day_;
    return *this;
  }
  bool operator==(const Days& other) const { return day_ == other.day_; }

 private:
  int day_;
};

class Months {
 public:
  explicit Months(int month) : month_(month) {}
  Months() : month_(0) {}
  int month() const { return month_; }
  void SetMonth(int month) { month_ = month; }
  const Months operator-() const { return Months(-month_); }
  Months& operator=(const Months& other) {
    month_ = other.month_;
    return *this;
  }
  bool operator==(const Months& other) const { return month_ == other.month_; }

 private:
  int month_;
};

class Years {
 public:
  explicit Years(int year) : year_(year) {}
  Years() : year_(0) {}
  int year() const { return year_; }
  void SetYear(int year) { year_ = year; }
  const Years operator-() const { return Years(-year_); }
  Years& operator=(const Years& other) {
    year_ = other.year_;
    return *this;
  }
  bool operator==(const Years& other) const { return year_ == other.year_; }

 private:
  int year_;
};

Days operator""_d(unsigned long long day);
Months operator""_m(unsigned long long month);
Years operator""_y(unsigned long long year);

bool IsLeapYear(int year);
int GetDaysInYear(int year);
int GetDaysInMonth(int year, int month);

//  return true if date is not valid
bool CheckDate(int year, int month, int day);

class Calendar {
 public:
  Calendar(int year, unsigned short month, unsigned short day);
  Calendar();
  Calendar(const Calendar& other)
      : year_(other.year_), month_(other.month_), day_(other.day_) {}
  Calendar(Calendar&& other) { *this = std::move(other); }
  Calendar& operator=(const Calendar&& other);
  Calendar& operator=(const Calendar& other);
  int year() const { return year_; }
  int month() const { return month_; }
  int day() const { return day_; }
  void SetYear(int year) {
    if (!CheckDate(year, month_, day_)) year_ = year;
  }
  void SetMonth(short month) {
    if (!CheckDate(year_, month, day_)) month_ = month;
  }
  void SetDay(short day) {
    if (!CheckDate(year_, month_, day)) day_ = day;
  }
  void SetDate(int year, short month, short day);
  bool IsLeapYear() const {
    return (year_ % 4 == 0 && year_ % 100 != 0) || year_ % 400 == 0;
  }
  int GetDaysInYear() const { return IsLeapYear() ? 366 : 365; }
  int GetDaysInMonth() const {
    return (month_ == 2) ? (IsLeapYear() ? 29 : 28)
           : ((month_ < 8 && month_ & 1) || (month_ > 7 && !(month_ & 1))) ? 31
                                                                           : 30;
  }
  bool operator==(const Calendar& other) const {
    return (year_ == other.year_ && month_ == other.month_ &&
            day_ == other.day_);
  }
  bool operator!=(const Calendar& other) const { return !(*this == other); }

  bool operator<(const Calendar& other) const;

  Calendar operator++();
  Calendar operator++(int);
  Calendar operator--();
  Calendar operator--(int);
  Calendar operator+=(const Days& days);
  Calendar operator-=(const Days& days);
  Calendar operator+=(const Months& months);
  Calendar operator-=(const Months& months);
  Calendar operator+=(const Years& years);
  Calendar operator-=(const Years& years);
  friend std::ostream& operator<<(std::ostream& os, const Calendar& calendar);

 private:
  int year_;
  short month_;
  short day_;
  void AddMonth();
  void SubMonth();
};

Calendar CalculateNextDate(const Calendar& startDate,
                           const std::variant<Days, Months, Years>& interval);

std::vector<Calendar> CalculateAllDates(const Calendar& startDate,
                                        const Calendar& endDate,
                                        const Periods& period);

}  // namespace s21

#endif  // CPP3_S21_SMARTCALCULATOR_MODEL_CALENDAR_H_
