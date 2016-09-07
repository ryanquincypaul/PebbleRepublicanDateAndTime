#pragma once
/*
 * Decimal Time routines
 * Copyright (c) 2013 Ed McLaughlin <epmclaughlin@gmail.com>
 *
 * I would like to maintain the above info about the original author
 * but I did make many stylistic changes and had to deal with the removal of PblTim in SDK 2.x.
 *
 * Original file found below...
 * https://github.com/vuzzbox/pebble/blob/master/modern_decimal/include/decimal_time.h
 */

//maybe in the future just make one republican object that contains time and a FrenchRepublicanDate?
//Can you put structs in structs or is that too OOP?
typedef struct PblDeciTm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
} PblDeciTm;

void get_decimal_time(PblDeciTm *t);