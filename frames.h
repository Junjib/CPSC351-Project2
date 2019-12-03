#ifndef FRAMES_H
#define FRAMES_H

struct Frames
{
  int mem;
  int page;
  int proc = 0;
  int highEnd;
  int lowEnd;
  int validBit = 0;
};

#endif
