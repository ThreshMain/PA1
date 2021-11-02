//
// Created by martin on 11/3/21.
//

#ifndef PT_HW03_ENERGY_CALCULATOR_H
#define PT_HW03_ENERGY_CALCULATOR_H
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
} date_time;

int isLeapYear(int year);
int daysInMonth(int year, int month);
int verifyYear(int year);

int verifyMonth(int month);
int verifyDay(int day, int year, int month);
int verifyHour(int hour);
int verifyMinute(int minute);
int verifyDate(date_time *date);
int dateTimeCompare(date_time *now, date_time *after);
int arraySum(const int array[], int arrayLength, int from, int to);
int normalizeDate(date_time *date, const int changeCost[], const int zeroCost[]);
int numberOfDays(date_time *from, date_time *to);
int energyConsumption(int y1, int m1, int d1, int h1, int i1,
                      int y2, int m2, int d2, int h2, int i2, long long int *consumption);
#endif //PT_HW03_ENERGY_CALCULATOR_H
