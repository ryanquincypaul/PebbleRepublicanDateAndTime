#pragma once

//extern const int vertu; //last full french month
//extern const int revolution; //french leap day

//extern const char* monthNames[];

//extern const char* dayNames[];

typedef struct FrenchRepublicanDate
{
  int frd_day;
  char* frd_dayName;
  int frd_month;
  char* frd_monthName;
  int frd_year;
  char* frd_romanYear;
} FrenchRepublicanDate;

bool IsFrenchLeapYear(int year);

int DaysInFrenchYear(int year);

int DaysInFrenchMonth(int year, int month);

char* GetFrenchMonthName(int month);

char* GetFrenchDayName(int dayOfWeek);

int ToRoman(char *out, int n);

char * GetRomanizedYear(int year);

void GetFrenchRepublicanDate(FrenchRepublicanDate *frd);