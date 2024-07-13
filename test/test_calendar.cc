#include <gtest/gtest.h>

#include <cmath>

#include "../model/calendar.h"
namespace s21 {

TEST(DaysMonthsYears, Days) {
  Days d0, d1(7);
  EXPECT_EQ(d0.day(), 0);
  EXPECT_EQ(d1.day(), 7);
  d1.SetDay(-7);
  EXPECT_EQ(d1.day(), -7);
  d0 = -d1;
  EXPECT_EQ(d0.day(), 7);
  EXPECT_TRUE(7_d == d0);
}

TEST(DaysMonthsYears, Months) {
  Months m0, m1(7);
  EXPECT_EQ(m0.month(), 0);
  EXPECT_EQ(m1.month(), 7);
  m1.SetMonth(-7);
  EXPECT_EQ(m1.month(), -7);
  m0 = -m1;
  EXPECT_EQ(m0.month(), 7);
  EXPECT_TRUE(7_m == m0);
}

TEST(DaysMonthsYears, Years) {
  Years y0, y1(7);
  EXPECT_EQ(y0.year(), 0);
  EXPECT_EQ(y1.year(), 7);
  y1.SetYear(-7);
  EXPECT_EQ(y1.year(), -7);
  y0 = -y1;
  EXPECT_EQ(y0.year(), 7);
  EXPECT_TRUE(7_y == y0);
}

TEST(Calendar, Constructors) {
  // Insert today date for c0
  // Calendar c0;
  // EXPECT_EQ(c0.year(), 2023);
  // EXPECT_EQ(c0.month(), 9);
  // EXPECT_EQ(c0.day(), 18);
  Calendar c1(1945, 5, 8);
  EXPECT_EQ(c1.year(), 1945);
  EXPECT_EQ(c1.month(), 5);
  EXPECT_EQ(c1.day(), 8);
  const Calendar c2(c1);
  EXPECT_EQ(c2.year(), 1945);
  EXPECT_EQ(c2.month(), 5);
  EXPECT_EQ(c2.day(), 8);
  const Calendar c3(std::move(c1));
  EXPECT_EQ(c3.year(), 1945);
  EXPECT_EQ(c3.month(), 5);
  EXPECT_EQ(c3.day(), 8);
  EXPECT_THROW(Calendar(1945, 0, 8), std::invalid_argument);
  EXPECT_THROW(Calendar(1981, 13, 8), std::invalid_argument);
  EXPECT_THROW(Calendar(2003, -1, 8), std::invalid_argument);
  EXPECT_THROW(Calendar(1945, 2, 0), std::invalid_argument);
  EXPECT_THROW(Calendar(1978, 2, 29), std::invalid_argument);
  EXPECT_THROW(Calendar(2101, 4, 31), std::invalid_argument);
}

TEST(Calendar, LeapYear) {
  EXPECT_TRUE(IsLeapYear(2020));
  EXPECT_EQ(GetDaysInYear(2020), 366);
  EXPECT_FALSE(IsLeapYear(2021));
  EXPECT_EQ(GetDaysInYear(2021), 365);
  EXPECT_TRUE(IsLeapYear(2000));
  EXPECT_FALSE(IsLeapYear(2100));
  const Calendar date1(1961, 4, 12), date2(2000, 3, 26);
  EXPECT_FALSE(date1.IsLeapYear());
  EXPECT_EQ(date1.GetDaysInYear(), 365);
  EXPECT_TRUE(date2.IsLeapYear());
  EXPECT_EQ(date2.GetDaysInYear(), 366);
}

TEST(Calendar, GetDaysInMonth) {
  EXPECT_EQ(GetDaysInMonth(2023, 1), 31);
  EXPECT_EQ(GetDaysInMonth(2023, 2), 28);
  EXPECT_EQ(GetDaysInMonth(2024, 2), 29);
  EXPECT_EQ(GetDaysInMonth(2023, 3), 31);
  EXPECT_EQ(GetDaysInMonth(2023, 4), 30);
  EXPECT_EQ(GetDaysInMonth(2023, 5), 31);
  EXPECT_EQ(GetDaysInMonth(2023, 6), 30);
  EXPECT_EQ(GetDaysInMonth(2023, 7), 31);
  EXPECT_EQ(GetDaysInMonth(2023, 8), 31);
  EXPECT_EQ(GetDaysInMonth(2023, 9), 30);
  EXPECT_EQ(GetDaysInMonth(2023, 10), 31);
  EXPECT_EQ(GetDaysInMonth(2023, 11), 30);
  EXPECT_EQ(GetDaysInMonth(2023, 12), 31);
  EXPECT_EQ(GetDaysInMonth(2023, 0), 0);
  EXPECT_EQ(GetDaysInMonth(2023, 13), 0);
  EXPECT_EQ(GetDaysInMonth(2023, -1), 0);
  const Calendar date1(1961, 4, 12), date2(2000, 3, 26);
  EXPECT_EQ(date1.GetDaysInMonth(), 30);
  EXPECT_EQ(date2.GetDaysInMonth(), 31);
}

TEST(Calendar, CheckDate) {
  EXPECT_TRUE(CheckDate(2023, 13, 2));
  EXPECT_TRUE(CheckDate(2020, 0, 10));
  EXPECT_TRUE(CheckDate(1981, -1, 7));
  EXPECT_TRUE(CheckDate(2000, 2, 0));
  EXPECT_TRUE(CheckDate(1963, 7, 32));
  EXPECT_TRUE(CheckDate(2111, 5, -1));
  EXPECT_TRUE(CheckDate(2000, 2, 30));
  EXPECT_TRUE(CheckDate(2001, 2, 29));
  EXPECT_FALSE(CheckDate(2000, 2, 29));
  EXPECT_FALSE(CheckDate(2000, 2, 28));
  EXPECT_FALSE(CheckDate(2001, 2, 28));
  EXPECT_FALSE(CheckDate(2001, 1, 31));
  EXPECT_FALSE(CheckDate(2001, 3, 31));
  EXPECT_FALSE(CheckDate(2001, 4, 30));
  EXPECT_TRUE(CheckDate(2001, 4, 31));
  EXPECT_FALSE(CheckDate(2001, 5, 31));
  EXPECT_FALSE(CheckDate(2001, 6, 30));
  EXPECT_TRUE(CheckDate(2001, 6, 31));
  EXPECT_FALSE(CheckDate(2001, 7, 31));
  EXPECT_FALSE(CheckDate(2001, 8, 31));
  EXPECT_FALSE(CheckDate(2001, 9, 30));
  EXPECT_TRUE(CheckDate(2001, 9, 31));
  EXPECT_FALSE(CheckDate(2001, 10, 31));
  EXPECT_FALSE(CheckDate(2001, 11, 30));
  EXPECT_TRUE(CheckDate(2001, 11, 31));
  EXPECT_FALSE(CheckDate(2001, 12, 31));
}

TEST(Calendar, Assign) {
  Calendar date1(1961, 4, 12), date2;
  date2 = date1;
  EXPECT_EQ(date1.day(), date2.day());
  EXPECT_EQ(date1.month(), date2.month());
  EXPECT_EQ(date1.year(), date2.year());
  Calendar date3;
  date3 = std::move(date2);
  EXPECT_EQ(date1.day(), date3.day());
  EXPECT_EQ(date1.month(), date3.month());
  EXPECT_EQ(date1.year(), date3.year());
}

TEST(Calendar, Equal) {
  const Calendar date1(1961, 4, 12), date2(1961, 4, 12), date3(1962, 4, 12);
  EXPECT_TRUE(date1 == date2);
  EXPECT_FALSE(date1 != date2);
  EXPECT_FALSE(date1 == date3);
  EXPECT_TRUE(date1 != date3);
}

TEST(Calendar, Sets) {
  Calendar date(1960, 2, 29);
  date.SetYear(1980);
  EXPECT_TRUE(date == Calendar(1980, 2, 29));
  date.SetYear(2001);
  EXPECT_TRUE(date == Calendar(1980, 2, 29));
  date.SetDay(-1);
  EXPECT_TRUE(date == Calendar(1980, 2, 29));
  date.SetDay(33);
  EXPECT_TRUE(date == Calendar(1980, 2, 29));
  date.SetDay(30);
  EXPECT_TRUE(date == Calendar(1980, 2, 29));
  date.SetDay(12);
  EXPECT_TRUE(date == Calendar(1980, 2, 12));
  date.SetDate(2001, 3, 31);
  EXPECT_TRUE(date == Calendar(2001, 3, 31));
  date.SetMonth(0);
  EXPECT_TRUE(date == Calendar(2001, 3, 31));
  date.SetMonth(13);
  EXPECT_TRUE(date == Calendar(2001, 3, 31));
  date.SetMonth(4);
  EXPECT_TRUE(date == Calendar(2001, 3, 31));
  date.SetMonth(5);
  EXPECT_TRUE(date == Calendar(2001, 5, 31));
  date.SetDate(2001, 2, 29);
  EXPECT_TRUE(date == Calendar(2001, 5, 31));
  date.SetDate(1981, 6, 31);
  EXPECT_TRUE(date == Calendar(2001, 5, 31));
}

TEST(Calendar, Increment) {
  Calendar date(1961, 4, 12);
  EXPECT_TRUE(++date == Calendar(1961, 4, 13));
  Calendar date2 = date++;
  EXPECT_TRUE(date2 == Calendar(1961, 4, 13));
  EXPECT_TRUE(date == Calendar(1961, 4, 14));
  date = Calendar(2000, 1, 31);
  EXPECT_TRUE(++date == Calendar(2000, 2, 1));
  date = Calendar(2000, 2, 29);
  EXPECT_TRUE(++date == Calendar(2000, 3, 1));
  date = Calendar(2001, 2, 28);
  EXPECT_TRUE(++date == Calendar(2001, 3, 1));
  date = Calendar(2001, 3, 31);
  EXPECT_TRUE(++date == Calendar(2001, 4, 1));
  date = Calendar(2001, 4, 30);
  EXPECT_TRUE(++date == Calendar(2001, 5, 1));
  date = Calendar(2001, 5, 31);
  EXPECT_TRUE(++date == Calendar(2001, 6, 1));
  date = Calendar(2001, 6, 30);
  EXPECT_TRUE(++date == Calendar(2001, 7, 1));
  date = Calendar(2001, 7, 31);
  EXPECT_TRUE(++date == Calendar(2001, 8, 1));
  date = Calendar(2001, 8, 31);
  EXPECT_TRUE(++date == Calendar(2001, 9, 1));
  date = Calendar(2001, 9, 30);
  EXPECT_TRUE(++date == Calendar(2001, 10, 1));
  date = Calendar(2001, 10, 31);
  EXPECT_TRUE(++date == Calendar(2001, 11, 1));
  date = Calendar(2001, 11, 30);
  EXPECT_TRUE(++date == Calendar(2001, 12, 1));
  date = Calendar(2001, 12, 31);
  EXPECT_TRUE(++date == Calendar(2002, 1, 1));
  date = Calendar(1999, 12, 31);
  EXPECT_TRUE(++date == Calendar(2000, 1, 1));
}

TEST(Calendar, Decrement) {
  Calendar date(1961, 4, 12);
  EXPECT_TRUE(--date == Calendar(1961, 4, 11));
  Calendar date2 = date--;
  EXPECT_TRUE(date2 == Calendar(1961, 4, 11));
  EXPECT_TRUE(date == Calendar(1961, 4, 10));
  date = Calendar(2001, 1, 1);
  EXPECT_TRUE(--date == Calendar(2000, 12, 31));
  date = Calendar(2000, 12, 1);
  EXPECT_TRUE(--date == Calendar(2000, 11, 30));
  date = Calendar(2000, 11, 1);
  EXPECT_TRUE(--date == Calendar(2000, 10, 31));
  date = Calendar(2000, 10, 1);
  EXPECT_TRUE(--date == Calendar(2000, 9, 30));
  date = Calendar(2000, 9, 1);
  EXPECT_TRUE(--date == Calendar(2000, 8, 31));
  date = Calendar(2000, 8, 1);
  EXPECT_TRUE(--date == Calendar(2000, 7, 31));
  date = Calendar(2000, 7, 1);
  EXPECT_TRUE(--date == Calendar(2000, 6, 30));
  date = Calendar(2000, 6, 1);
  EXPECT_TRUE(--date == Calendar(2000, 5, 31));
  date = Calendar(2000, 5, 1);
  EXPECT_TRUE(--date == Calendar(2000, 4, 30));
  date = Calendar(2000, 4, 1);
  EXPECT_TRUE(--date == Calendar(2000, 3, 31));
  date = Calendar(2000, 3, 1);
  EXPECT_TRUE(--date == Calendar(2000, 2, 29));
  date = Calendar(2000, 2, 1);
  EXPECT_TRUE(--date == Calendar(2000, 1, 31));
  date = Calendar(2000, 1, 1);
  EXPECT_TRUE(--date == Calendar(1999, 12, 31));
  date = Calendar(1999, 3, 1);
  EXPECT_TRUE(--date == Calendar(1999, 2, 28));
}

TEST(Calendar, AddYear) {
  Calendar date(1961, 4, 12);
  date += 3_y;
  EXPECT_TRUE(date == Calendar(1964, 4, 12));
  date += -8_y;
  EXPECT_TRUE(date == Calendar(1956, 4, 12));
  date += 0_y;
  EXPECT_TRUE(date == Calendar(1956, 4, 12));
  date = Calendar(2004, 2, 29);
  date += 4_y;
  EXPECT_TRUE(date == Calendar(2008, 2, 29));
  date += 2_y;
  EXPECT_TRUE(date == Calendar(2010, 2, 28));
}

TEST(Calendar, SubYear) {
  Calendar date(1961, 4, 12);
  date -= 3_y;
  EXPECT_TRUE(date == Calendar(1958, 4, 12));
  date -= -8_y;
  EXPECT_TRUE(date == Calendar(1966, 4, 12));
  date -= 0_y;
  EXPECT_TRUE(date == Calendar(1966, 4, 12));
  date = Calendar(2004, 2, 29);
  date -= 4_y;
  EXPECT_TRUE(date == Calendar(2000, 2, 29));
  date -= 2_y;
  EXPECT_TRUE(date == Calendar(1998, 2, 28));
}

TEST(Calendar, AddMonth) {
  Calendar date(1961, 4, 12);
  date += 3_m;
  EXPECT_TRUE(date == Calendar(1961, 7, 12));
  date += -8_m;
  EXPECT_TRUE(date == Calendar(1960, 11, 12));
  date += 0_m;
  EXPECT_TRUE(date == Calendar(1960, 11, 12));
  date += 24_m;
  EXPECT_TRUE(date == Calendar(1962, 11, 12));
  date += 17_m;
  EXPECT_TRUE(date == Calendar(1964, 4, 12));
  date += 8_m;
  EXPECT_TRUE(date == Calendar(1964, 12, 12));
  date = Calendar(2021, 12, 30);
  date += 2_m;
  EXPECT_TRUE(date == Calendar(2022, 2, 28));
  date = Calendar(2022, 12, 30);
  date += 14_m;
  EXPECT_TRUE(date == Calendar(2024, 2, 29));
  date = Calendar(2022, 7, 31);
  date += 11_m;
  EXPECT_TRUE(date == Calendar(2023, 6, 30));
  date = Calendar(2022, 7, 31);
  date += 33_m;
  EXPECT_TRUE(date == Calendar(2025, 4, 30));
  date = Calendar(2022, 7, 31);
  date += 74_m;
  EXPECT_TRUE(date == Calendar(2028, 9, 30));
  date = Calendar(2022, 7, 31);
  date += 4_m;
  EXPECT_TRUE(date == Calendar(2022, 11, 30));
}

TEST(Calendar, SubMonth) {
  Calendar date(1961, 4, 12);
  date -= 3_m;
  EXPECT_TRUE(date == Calendar(1961, 1, 12));
  date -= -8_m;
  EXPECT_TRUE(date == Calendar(1961, 9, 12));
  date -= 0_m;
  EXPECT_TRUE(date == Calendar(1961, 9, 12));
  date -= 24_m;
  EXPECT_TRUE(date == Calendar(1959, 9, 12));
  date -= 17_m;
  EXPECT_TRUE(date == Calendar(1958, 4, 12));
  date -= 4_m;
  EXPECT_TRUE(date == Calendar(1957, 12, 12));
  date = Calendar(2021, 06, 30);
  date -= 4_m;
  EXPECT_TRUE(date == Calendar(2021, 2, 28));
  date = Calendar(2021, 06, 30);
  date -= 16_m;
  EXPECT_TRUE(date == Calendar(2020, 2, 29));
  date = Calendar(2022, 8, 31);
  date -= 2_m;
  EXPECT_TRUE(date == Calendar(2022, 6, 30));
  date = Calendar(2022, 8, 31);
  date -= 16_m;
  EXPECT_TRUE(date == Calendar(2021, 4, 30));
  date = Calendar(2022, 8, 31);
  date -= 71_m;
  EXPECT_TRUE(date == Calendar(2016, 9, 30));
  date = Calendar(2022, 8, 31);
  date -= 9_m;
  EXPECT_TRUE(date == Calendar(2021, 11, 30));
}

TEST(Calendar, AddDays) {
  Calendar date(1961, 4, 12);
  date += 3_d;
  EXPECT_TRUE(date == Calendar(1961, 4, 15));
  date += -8_d;
  EXPECT_TRUE(date == Calendar(1961, 4, 7));
  date += 0_d;
  EXPECT_TRUE(date == Calendar(1961, 4, 7));
  date += 30_d;
  EXPECT_TRUE(date == Calendar(1961, 5, 7));
  date += 47_d;
  EXPECT_TRUE(date == Calendar(1961, 6, 23));
  date += 1104_d;
  EXPECT_TRUE(date == Calendar(1964, 7, 1));
}

TEST(Calendar, SubDays) {
  Calendar date(1961, 4, 12);
  date -= 3_d;
  EXPECT_TRUE(date == Calendar(1961, 4, 9));
  date -= -8_d;
  EXPECT_TRUE(date == Calendar(1961, 4, 17));
  date -= 0_d;
  EXPECT_TRUE(date == Calendar(1961, 4, 17));
  date -= 30_d;
  EXPECT_TRUE(date == Calendar(1961, 3, 18));
  date -= 47_d;
  EXPECT_TRUE(date == Calendar(1961, 1, 30));
  date -= 1126_d;
  EXPECT_TRUE(date == Calendar(1957, 12, 31));
}

TEST(Calendar, StreamOperator) {
  Calendar date(1961, 4, 1);
  std::stringstream ss;
  ss << date;
  EXPECT_EQ(ss.str(), "1961-04-01");
  // std::cout << ss.str() << std::endl;
}

TEST(Calendar, CalculateNextDate) {
  Calendar date(2000, 3, 26);
  EXPECT_TRUE(CalculateNextDate(date, Days(1)) == Calendar(2000, 3, 27));
  EXPECT_TRUE(CalculateNextDate(date, Days(10)) == Calendar(2000, 4, 5));
  EXPECT_TRUE(CalculateNextDate(date, Months(1)) == Calendar(2000, 4, 26));
  EXPECT_TRUE(CalculateNextDate(date, Months(10)) == Calendar(2001, 1, 26));
  EXPECT_TRUE(CalculateNextDate(date, Years(1)) == Calendar(2001, 3, 26));
  EXPECT_TRUE(CalculateNextDate(date, Years(10)) == Calendar(2010, 3, 26));
}

TEST(Calendar, CalculateAllDates) {
  std::vector<Calendar> all =
      CalculateAllDates({2000, 2, 26}, {2000, 3, 4}, Periods::kDay);
  EXPECT_EQ(all.size(), 8);
  EXPECT_TRUE(all[0] == Calendar(2000, 2, 26));
  EXPECT_TRUE(all[1] == Calendar(2000, 2, 27));
  EXPECT_TRUE(all[2] == Calendar(2000, 2, 28));
  EXPECT_TRUE(all[3] == Calendar(2000, 2, 29));
  EXPECT_TRUE(all[4] == Calendar(2000, 3, 1));
  EXPECT_TRUE(all[5] == Calendar(2000, 3, 2));
  EXPECT_TRUE(all[6] == Calendar(2000, 3, 3));
  EXPECT_TRUE(all[7] == Calendar(2000, 3, 4));
  all = CalculateAllDates({2000, 2, 26}, {2000, 3, 26}, Periods::kWeek);
  EXPECT_EQ(all.size(), 6);
  EXPECT_TRUE(all[0] == Calendar(2000, 2, 26));
  EXPECT_TRUE(all[1] == Calendar(2000, 3, 4));
  EXPECT_TRUE(all[2] == Calendar(2000, 3, 11));
  EXPECT_TRUE(all[3] == Calendar(2000, 3, 18));
  EXPECT_TRUE(all[4] == Calendar(2000, 3, 25));
  EXPECT_TRUE(all[5] == Calendar(2000, 3, 26));
  all = CalculateAllDates({2000, 10, 31}, {2001, 5, 26}, Periods::kMonth);
  EXPECT_EQ(all.size(), 8);
  EXPECT_TRUE(all[0] == Calendar(2000, 10, 31));
  EXPECT_TRUE(all[1] == Calendar(2000, 11, 30));
  EXPECT_TRUE(all[2] == Calendar(2000, 12, 31));
  EXPECT_TRUE(all[3] == Calendar(2001, 1, 31));
  EXPECT_TRUE(all[4] == Calendar(2001, 2, 28));
  EXPECT_TRUE(all[5] == Calendar(2001, 3, 31));
  EXPECT_TRUE(all[6] == Calendar(2001, 4, 30));
  EXPECT_TRUE(all[7] == Calendar(2001, 5, 26));
  all = CalculateAllDates({1999, 11, 30}, {2001, 5, 26}, Periods::kQuarter);
  EXPECT_EQ(all.size(), 7);
  EXPECT_TRUE(all[0] == Calendar(1999, 11, 30));
  EXPECT_TRUE(all[1] == Calendar(2000, 2, 29));
  EXPECT_TRUE(all[2] == Calendar(2000, 5, 30));
  EXPECT_TRUE(all[3] == Calendar(2000, 8, 30));
  EXPECT_TRUE(all[4] == Calendar(2000, 11, 30));
  EXPECT_TRUE(all[5] == Calendar(2001, 2, 28));
  EXPECT_TRUE(all[6] == Calendar(2001, 5, 26));
  all = CalculateAllDates({1999, 10, 31}, {2001, 5, 26}, Periods::kHalfYear);
  EXPECT_EQ(all.size(), 5);
  EXPECT_TRUE(all[0] == Calendar(1999, 10, 31));
  EXPECT_TRUE(all[1] == Calendar(2000, 4, 30));
  EXPECT_TRUE(all[2] == Calendar(2000, 10, 31));
  EXPECT_TRUE(all[3] == Calendar(2001, 4, 30));
  EXPECT_TRUE(all[4] == Calendar(2001, 5, 26));
  all = CalculateAllDates({2000, 2, 29}, {2005, 5, 26}, Periods::kYear);
  EXPECT_EQ(all.size(), 7);
  EXPECT_TRUE(all[0] == Calendar(2000, 2, 29));
  EXPECT_TRUE(all[1] == Calendar(2001, 2, 28));
  EXPECT_TRUE(all[2] == Calendar(2002, 2, 28));
  EXPECT_TRUE(all[3] == Calendar(2003, 2, 28));
  EXPECT_TRUE(all[4] == Calendar(2004, 2, 29));
  EXPECT_TRUE(all[5] == Calendar(2005, 2, 28));
  EXPECT_TRUE(all[6] == Calendar(2005, 5, 26));
  all = CalculateAllDates({2000, 2, 29}, {2005, 5, 26}, Periods::kEnd);
  EXPECT_EQ(all.size(), 2);
  EXPECT_TRUE(all[0] == Calendar(2000, 2, 29));
  EXPECT_TRUE(all[1] == Calendar(2005, 5, 26));
  all = CalculateAllDates({2001, 4, 4}, {2001, 4, 4}, Periods::kMonth);
  EXPECT_EQ(all.size(), 1);
  EXPECT_TRUE(all[0] == Calendar(2001, 4, 4));
  all = CalculateAllDates({2001, 4, 4}, {2000, 4, 4}, Periods::kEnd);
  EXPECT_TRUE(all.empty());
}

}  // namespace s21