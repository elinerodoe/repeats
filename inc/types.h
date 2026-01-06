#ifndef TYPES_H
#define TYPES_H

#define MAX_CHAR 256

struct Repeat {
    int index;
    int period;
    int count;
    int shift;
};

struct Interval {
    int start;
    int end;
    int length;
    int group;
};

#endif