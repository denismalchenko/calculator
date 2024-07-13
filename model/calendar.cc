#include "calendar.h"

#include <ctime>

namespace s21 {

Days operator""_d(unsigned long long day) { return Days(day); }
Months operator""_m(unsigned long long month) { return Months(month); }
Years operator""_y(unsigned long long year) { return Years(year); }

bool IsLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}
int GetDaysInYear(int year) { return IsLeapYear(year) ? 366 : 365; }

bool CheckDate(int year, int month, int day) {
  return month < 1 || month > 12 || day < 1 ||
         day > GetDaysInMonth(year, month);
}

int GetDaysInMonth(int year, int month) {
  if (month < 1 || month > 12) return 0;
  if (month == 2)
    return (IsLeapYear(year)) ? 29 : 28;
  else if ((month < 8 && month & 1) || (month > 7 && !(month & 1)))
    return 31;
  else
    return 30;
}

Calendar::Calendar(int year, unsigned short month, unsigned short day)
    : year_(year), month_(month), day_(day) {
  if (CheckDate(year, month, day)) {
    throw std::invalid_argument("Invalid date");
  }
}
Calendar::Calendar() {
  std::time_t now = std::time(nullptr);
  std::tm* local_time = std::localtime(&now);
  year_ = local_time->tm_year + 1900;
  month_ = local_time->tm_mon + 1;
  day_ = local_time->tm_mday;
}

Calendar& Calendar::operator=(const Calendar&& other) {
  if (this != &other) {
    year_ = other.year_;
    month_ = other.month_;
    day_ = other.day_;
  }
  return *this;
}
Calendar& Calendar::operator=(const Calendar& other) {
  if (this != &other) {
    year_ = other.year_;
    month_ = other.month_;
    day_ = other.day_;
  }
  return *this;
}

void Calendar::SetDate(int year, short month, short day) {
  if (!CheckDate(year, month, day)) {
    year_ = year;
    month_ = month;
    day_ = day;
  }
}

bool Calendar::operator<(const Calendar& other) const {
  if (year_ != other.year_) return year_ < other.year_;
  if (month_ != other.month_) return month_ < other.month_;
  return day_ < other.day_;
}

Calendar Calendar::operator++() {
  day_++;
  if (day_ > GetDaysInMonth()) {
    day_ = 1;
    AddMonth();
  }
  return *this;
}
Calendar Calendar::operator++(int) {
  Calendar temp = *this;
  ++(*this);
  return temp;
}
Calendar Calendar::operator--() {
  day_--;
  if (day_ == 0) {
    SubMonth();
    day_ = GetDaysInMonth();
  }
  return *this;
}
Calendar Calendar::operator--(int) {
  Calendar temp = *this;
  --(*this);
  return temp;
}
Calendar Calendar::operator+=(const Days& days) {
  if (days.day() < 0) return *this -= -days;
  day_ += days.day();
  while (CheckDate(year_, month_, day_)) {
    day_ -= GetDaysInMonth();
    AddMonth();
  }
  return *this;
}
Calendar Calendar::operator-=(const Days& days) {
  if (days.day() < 0) return *this += -days;
  day_ -= days.day();
  while (day_ <= 0) {
    SubMonth();
    day_ += GetDaysInMonth();
  }
  return *this;
}

Calendar Calendar::operator+=(const Months& months) {
  if (months.month() < 0) return *this -= -months;
  month_ += months.month() % 12;
  year_ += months.month() / 12;
  if (month_ > 12) {
    month_ -= 12;
    year_++;
  }
  if (CheckDate(year_, month_, day_)) day_ = GetDaysInMonth();
  return *this;
}

Calendar Calendar::operator-=(const Months& months) {
  if (months.month() < 0) return *this += -months;
  month_ -= months.month() % 12;
  year_ -= months.month() / 12;
  if (month_ <= 0) {
    month_ += 12;
    year_--;
  }
  if (CheckDate(year_, month_, day_)) day_ = GetDaysInMonth();
  return *this;
}

Calendar Calendar::operator+=(const Years& years) {
  year_ += years.year();
  if (CheckDate(year_, month_, day_)) day_ = 28;
  return *this;
}

Calendar Calendar::operator-=(const Years& years) {
  year_ -= years.year();
  if (CheckDate(year_, month_, day_)) day_ = 28;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Calendar& calendar) {
  os << calendar.year_ << "-" << std::setfill('0') << std::setw(2)
     << calendar.month_ << "-" << std::setfill('0') << std::setw(2)
     << calendar.day_;
  return os;
}

void Calendar::AddMonth() {
  month_++;
  if (month_ > 12) {
    month_ = 1;
    year_++;
  }
}
void Calendar::SubMonth() {
  month_--;
  if (month_ == 0) {
    month_ = 12;
    year_--;
  }
}

Calendar CalculateNextDate(const Calendar& startDate,
                           const std::variant<Days, Months, Years>& interval) {
  Calendar endDate = startDate;
  std::visit([&endDate](const auto& value) { endDate += value; }, interval);
  return endDate;
}

std::vector<Calendar> CalculateAllDates(const Calendar& startDate,
                                        const Calendar& endDate,
                                        const Periods& period) {
  std::vector<Calendar> result;
  if (startDate == endDate) {
    result.push_back(startDate);
    return result;
  }
  if (endDate < startDate) return result;
  Calendar date = startDate;
  for (int i = 1; date < endDate; ++i) {
    result.push_back(date);
    if (period == Periods::kDay)
      date = CalculateNextDate(startDate, Days(i));
    else if (period == Periods::kWeek)
      date = CalculateNextDate(startDate, Days(7 * i));
    else if (period == Periods::kMonth)
      date = CalculateNextDate(startDate, Months(i));
    else if (period == Periods::kQuarter)
      date = CalculateNextDate(startDate, Months(3 * i));
    else if (period == Periods::kHalfYear)
      date = CalculateNextDate(startDate, Months(6 * i));
    else if (period == Periods::kYear)
      date = CalculateNextDate(startDate, Years(i));
    else
      break;
  }
  result.push_back(endDate);
  return result;
}

}  // namespace s21
