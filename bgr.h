#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/ktrace.h>

#define Z static
#define R return
#define O printf
#define E exit
#define SZ sizeof
#define N const
#define X extern

#define DO(x,y) for(int i=0;i<x;i++)y

#define P(x,y) if(x){y;}
#define U(x) P(!(x), R 0)
#define UB(x) P(!(x), break)

#define ERR(...) syslog(LOG_ERR, __VA_ARGS__)
#define WARN(...) syslog(LOG_WARNING, __VA_ARGS__)

#define DOEXIT(x,y) {x; E(y); }
#define ES(x) DOEXIT(x, EXIT_SUCCESS)
#define EF(x) DOEXIT(x, EXIT_FAILURE)

#define EQF(v, e) { v = e; P(!v, EF(O("fault\n"))); }

#define SW switch
#define CS(x, y) case x: y
#define CSW(x, y) case x: y; break
#define CSD(x) default: x

#define VA(x, i, z, t) {t p;va_list ap;va_start(ap, x);i;while((p = va_arg(ap, t)) != NULL)z;va_end(ap);}

typedef struct IMG { unsigned char name[MAXNAMLEN + 1]; } IMG;
typedef char C;
typedef void V;
typedef char* S;
typedef const char* cS;
typedef int I;
