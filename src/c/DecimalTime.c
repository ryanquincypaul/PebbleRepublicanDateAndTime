/**
 * Convert archaic time to decimal time.
 * Learn more here: http://en.wikipedia.org/wiki/Decimal_time)
 *  
 * Copyright (c) 2013 Ed McLaughlin <epmclaughlin@gmail.com>
 *
 * I would like to maintain the above info about the original author
 * but I did make many stylistic changes and had to deal with the removal of PblTim in SDK 2.x.
 *
 * Original file found below...
 * https://github.com/vuzzbox/pebble/blob/master/modern_decimal/src/decimal_time.c
 */

#include <math.h>
#include <pebble.h>
#include "DecimalTime.h"

void get_decimal_time(PblDeciTm *t) {
  time_t temp = time(NULL);
	struct tm *archaic_time = localtime(&temp);
  unsigned int decimal_time;
  int decimal_hour;
  int decimal_minute;
  int decimal_seconds;
  
  decimal_time = floor(((archaic_time->tm_hour * 3600) + (archaic_time->tm_min * 60) + archaic_time->tm_sec) / .864);

  decimal_hour = floor(decimal_time / 10000);
  decimal_time = decimal_time - (decimal_hour * 10000);

  decimal_minute = floor(decimal_time / 100);
  decimal_time = decimal_time - (decimal_minute * 100);
  
  decimal_seconds = decimal_time;

  t->tm_sec = decimal_seconds;
  t->tm_min = decimal_minute;
  t->tm_hour = decimal_hour;
}