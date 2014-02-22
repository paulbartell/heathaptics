enum ModeType {STOP = 0, RUN = 1, DEBUG = 2, I2CSETUP = 3};
enum CommandType {STOPB = 0, SETMODE = 1, SETA = 'A', SETB = 'B', SETBOTH = 'C', LISTDEV = 'D', SETDEVID = 'E'};

struct SetOnePkt
{
  short setPt;
  char termination;
};

struct SetBothPkt
{
  short setPtA;
  short setPtB;
  char termination;
};
