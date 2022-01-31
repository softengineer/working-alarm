#ifndef TIMESETTING_H
#define TIMESETTING_H


class TimeSetting
{
public:
    TimeSetting();

    inline int getRestMinute() { return restMinutes;}
    inline int getWorkingMinute() { return workingMinutes;}
    inline int getStandMinute() { return standMinutes;}
private:
    const int restMinutes = 10;
    const int workingMinutes = 25;
    const int standMinutes = 15;
};

#endif // TIMESETTING_H
