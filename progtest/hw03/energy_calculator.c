typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
} date_time;

int isLeapYear(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) && year % 4000 != 0;
}

int daysInMonth(int year, int month) {
    switch (month) {
        case 1:
            return 31;
        case 2:
            if (isLeapYear(year))
                return 29;
            return 28;
        case 3:
            return 31;
        case 4:
            return 30;
        case 5:
            return 31;
        case 6:
            return 30;
        case 7:
        case 8:
            return 31;
        case 9:
            return 30;
        case 10:
            return 31;
        case 11:
            return 30;
        case 12:
            return 31;
        default:
            return -1;
    }
}

int verifyYear(int year) {
    return year >= 1600;
}

int verifyMonth(int month) {
    return month >= 1 && month <= 12;
}

int verifyDay(int day, int year, int month) {
    return day > 0 && day <= daysInMonth(year, month);
}

int verifyHour(int hour) {
    return hour >= 0 && hour <= 23;
}

int verifyMinute(int minute) {
    return minute >= 0 && minute <= 59;
}

int verifyDate(date_time *date) {
    return verifyYear(date->year) &&
           verifyMonth(date->month) &&
           verifyDay(date->day, date->year, date->month) &&
           verifyHour(date->hour) &&
           verifyMinute(date->minute);
}

int dateTimeCompare(date_time *now, date_time *after) {
    if (after->year > now->year)
        return 1;
    if (after->year < now->year) {
        return -1;
    }

    if (after->month > now->month) {
        return 1;
    }
    if (after->month < now->month) {
        return -1;
    }

    if (after->day > now->day) {
        return 1;
    }
    if (after->day < now->day) {
        return -1;
    }

    if (after->hour > now->hour) {
        return 1;
    }
    if (after->hour < now->hour) {
        return -1;
    }

    if (after->minute > now->minute) {
        return 1;
    }
    if (after->minute < now->minute) {
        return -1;
    }
    return 0;
}

int arraySum(const int array[], int arrayLength, int from, int to) {
    int sum = 0;
    if (from >= arrayLength) {
        from %= arrayLength;
    }
    while (from != to) {
        sum += array[from];
        from++;
        if (from >= arrayLength) {
            from %= arrayLength;
        }
    }
    return sum;
}

long leapYearsBeforeYear(int year) {
    return year / 4 - year / 100 + year / 400 - year / 4000;
}

long long numberOfDays(date_time *from, date_time *to) {
    long long count = 0;
    int year = from->year;
    int month = from->month;
    int day = from->day;
    if (month > to->month || year > to->year) {
        count += daysInMonth(year, month) - day + 1;
        day = 0;
        month++;
        for (; month <= 12; ++month) {
            count += daysInMonth(year, month);
        }
        month = 0;
        year++;
    }
    count += 365 * (to->year - year);
    count += leapYearsBeforeYear(to->year-1) - leapYearsBeforeYear(year-1);
    year = to->year;
    while (month < to->month) {
        count += daysInMonth(year, month);
        month++;
    }
    count += to->day - day;
    return count;
}

int normalizeDate(date_time *date, const int changeCost[], const int zeroCost[]) {
    int consumption = 0;
    if (date->minute >= 60) {
        consumption += zeroCost[5];
        date->minute -= 60;
        if (date->hour < 23) {
            consumption += changeCost[date->hour % 10];
        }
        date->hour += 1;
    }
    if (date->hour == 24) {
        consumption += zeroCost[2] + zeroCost[3];
        date->hour -= 24;
        date->day += 1;
    }
    while (date->month > 12) {
        date->month -= 12;
        date->year += 1;
    }
    while (date->day > daysInMonth(date->year, date->month)) {
        date->day -= daysInMonth(date->year, date->month);
        date->month += 1;
        while (date->month > 12) {
            date->month -= 12;
            date->year += 1;
        }
    }
    return consumption;
}

int energyConsumption(int y1, int m1, int d1, int h1, int i1,
                      int y2, int m2, int d2, int h2, int i2, long long int *consumption) {
    date_time from = {y1, m1, d1, h1, i1};
    date_time to = {y2, m2, d2, h2, i2};
    // energy cost for number to change from number at index i to the next
    // example 0 -> 1 = 4 changes so value on index 0 is 4
    int changeCost[] = {4, 5, 2, 3, 3, 1, 5, 4, 1, 2};
    int changeCostLength = sizeof(changeCost) / sizeof(changeCost[0]);

    // energy cost for number to change back to 0
    int zeroCost[] = {0, 4, 3, 3, 4, 3, 2, 3, 1, 2};

    if (verifyDate(&from) && verifyDate(&to) && (dateTimeCompare(&from, &to) >= 0)) {
        *consumption = 0;
        int minuteMultiplier = 200;
        int hourMultiplier = 12200;
        int difference;

        if (from.minute > to.minute) {
            difference = 60 - from.minute;
            *consumption += (difference) * minuteMultiplier + difference / 10 * 30 +
                            arraySum(changeCost, changeCostLength, from.minute % 10, 0) +
                            arraySum(changeCost, changeCostLength, from.minute / 10, 5);
            from.minute = 60;
            *consumption += normalizeDate(&from, changeCost, zeroCost);
        }
        difference = to.minute - from.minute;
        *consumption += (difference) * minuteMultiplier + (difference / 10) * 30 +
                        arraySum(changeCost, changeCostLength, from.minute % 10, to.minute % 10) +
                        arraySum(changeCost, changeCostLength, from.minute / 10, to.minute / 10);
        from.minute = to.minute;

        if (dateTimeCompare(&from, &to) == 0) {
            return 1;
        }

        if (from.hour > to.hour) {
            difference = 24 - from.hour;
            *consumption += difference * hourMultiplier + (difference / 10) * 30;
            if (from.hour % 10 >= 4) {
                *consumption -= 30;
                *consumption += arraySum(changeCost, changeCostLength, from.hour % 10, 0);
                from.hour -= from.hour % 10;
            }
            *consumption += arraySum(changeCost, changeCostLength, from.hour % 10, 4) -
                            (from.hour % 10 == 4 ? 0 : 3) +
                            arraySum(changeCost, changeCostLength, from.hour / 10, 2);
            from.hour = 24;
            *consumption += normalizeDate(&from, changeCost, zeroCost);
        }
        difference = to.hour - from.hour;
        *consumption += difference * hourMultiplier + (difference / 10) * 30 +
                        arraySum(changeCost, changeCostLength, from.hour % 10, to.hour % 10) +
                        arraySum(changeCost, changeCostLength, from.hour / 10, to.hour / 10);
        from.hour = to.hour;
        if (dateTimeCompare(&from, &to) == 0) {
            return 1;
        }
        *consumption += (hourMultiplier * 24 + 86) * numberOfDays(&from, &to);
        return 1;
    }
    return 0;
}