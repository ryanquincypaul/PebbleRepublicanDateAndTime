#include <pebble.h> 
#include "FrenchCalendarConverter.h"

static const int vendemiaire = 0; //first french month
static const int vertu = 12; //last full french month
static const int revolution = 17; //french leap day
static const int daysFromBeginningToUnixEpoch = 64748; //Days between unix epoch and formation of National Convention

static const char * frenchMonthNames[] = { 
  "Vendémiaire", 
  "Brumaire", 
  "Frimaire", 
  "Nivôse", 
  "Pluviôse", 
  "Ventôse", 
  "Germinal", 
  "Floréal", 
  "Prairial", 
  "Messidor", 
  "Thermidor", 
  "Fructidor",
  "Vertu", 
  "Génie", 
  "Travail", 
  "Opinion", 
  "Récompenses", 
  "Révolution"
};

static const char * frenchDayNames[] = {
  "Primidi", 
  "Duodi",
  "Tridi", 
  "Quartidi", 
  "Quintidi", 
  "Sextidi", 
  "Septidi", 
  "Octidi", 
  "Nonidi", 
  "Decadi"
};

bool IsFrenchLeapYear(int year) {
  //obviously these years are before the unix epoch, but pasting them in here anyway for historicity
  if (year == 3 || year == 7 || year == 11 || year == 15) {
    return true;
  } else if (year < 20) {
    return false;
  } else {
    //using the Charles-Gilbert Romme rule, may he RIP in peace
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) ? true : false;
  }
}

int DaysInFrenchYear(int year) {
  return IsFrenchLeapYear(year) ? 366 : 365;
}

int DaysInFrenchMonth(int year, int month) {
  if (month < vertu) {
    return 30;
  } else if (month == revolution) {
    return (IsFrenchLeapYear(year)) ? 1 : 0;
  } else {
    return 1;
  }
}

char * GetFrenchMonthName(int month) {
  return (char * ) frenchMonthNames[month];
}

char * GetFrenchDayName(int dayOfWeek) {
  return (char * ) frenchDayNames[dayOfWeek];
}

int ToRoman(char *out, int n)
{
        int len = 0;
        if (n <= 0) return 0; /* error indication */
#       define RPUT(c) if (out) out[len] = c; len++
        while(n>= 1000) { n -= 1000;RPUT('M'); };
 
        if (n >= 900)   { n -= 900; RPUT('C'); RPUT('M'); };
        if (n >= 500)   { n -= 500; RPUT('D'); };
        if (n >= 400)   { n -= 400; RPUT('C'); RPUT('D'); };
        while (n >= 100){ n -= 100; RPUT('C'); };
 
        if (n >= 90)    { n -= 90;  RPUT('X'); RPUT('C'); };
        if (n >= 50)    { n -= 50;  RPUT('L'); };
        if (n >= 40)    { n -= 40;  RPUT('X'); RPUT('L'); };
        while (n >= 10) { n -= 10;  RPUT('X'); };
 
        if (n >= 9)     { n -= 9;   RPUT('I'); RPUT('X'); };
        if (n >= 5)     { n -= 5;   RPUT('V'); };
        if (n >= 4)     { n -= 4;   RPUT('I'); RPUT('V'); };
        while (n)       { n--; RPUT('I'); };
        RPUT('\0');
#       undef RPUT
 
        return len;
}

char * GetRomanizedYear(int year) {
  char * numeralYear =  (char*)malloc(16 * sizeof(char));
  ToRoman(numeralYear,year);
  return numeralYear;
}

void GetFrenchRepublicanDate(FrenchRepublicanDate *frd) {
  //total number of days since formation of national assembly
  int daysSinceUnixEpoch = (int) time(NULL)/(60*60*24);
  int totalTimeInDays = daysFromBeginningToUnixEpoch + daysSinceUnixEpoch;
  
  //What year? Remove full years and count them
  int year = 1;
	//TODO: I can probably just rework this to start at epoch date instead of
  //whiling through 2 centuries
	//Full Years
	while (totalTimeInDays >= DaysInFrenchYear(year)) {
		totalTimeInDays -= DaysInFrenchYear(year);
		year++;
	}
  
  //What Month? Remove full months and count them
  int month = vendemiaire; //start with vendémiaire
	while (totalTimeInDays >= DaysInFrenchMonth(year, month)) {
		totalTimeInDays -= DaysInFrenchMonth(year, month);
		month++;
	}
  
  frd->frd_day = totalTimeInDays + 1;
  frd->frd_dayName = (char*) GetFrenchDayName(totalTimeInDays % 10);
  frd->frd_month = month;
  frd->frd_monthName = (char*) GetFrenchMonthName(month);
  frd->frd_year = year;
  frd->frd_romanYear = (char*)GetRomanizedYear(year); 
}