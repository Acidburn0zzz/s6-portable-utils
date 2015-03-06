/* ISC license. */

#include <unistd.h>
#include <signal.h>
#include <skalibs/uint.h>
#include <skalibs/sgetopt.h>
#include <skalibs/strerr2.h>
#include <skalibs/sig.h>

#define USAGE "s6-pause [ -t ] [ -h ] [ -a ] [ -q ] [ -b ] [ -i ] [ -p signal,signal... ]"
#define dieusage() strerr_dieusage(100, USAGE)

#define MAX 64

int main (int argc, char const *const *argv)
{
  PROG = "s6-pause" ;
  unsigned int sigs[MAX] ;
  unsigned int nsig = 0 ;
  {
    subgetopt_t l = SUBGETOPT_ZERO ;
    for (;;)
    {
      register int opt = subgetopt_r(argc, argv, "thaqbip:", &l) ;
      if (opt == -1) break ;
      switch (opt)
      {
        case 't' : if (nsig >= MAX) dieusage() ; sigs[nsig++] = SIGTERM ;
        case 'h' : if (nsig >= MAX) dieusage() ; sigs[nsig++] = SIGHUP ;
        case 'a' : if (nsig >= MAX) dieusage() ; sigs[nsig++] = SIGALRM ;
        case 'q' : if (nsig >= MAX) dieusage() ; sigs[nsig++] = SIGQUIT ;
        case 'b' : if (nsig >= MAX) dieusage() ; sigs[nsig++] = SIGABRT ;
        case 'i' : if (nsig >= MAX) dieusage() ; sigs[nsig++] = SIGINT ;
        case 'p' :
        {
          unsigned int n ;
          if (!uint_scanlist(sigs + nsig, MAX - nsig, l.arg, &n)) dieusage() ;
          nsig += n ;
          break ;
        }
        default : dieusage() ;
      }
    }
    argc -= l.ind ; argv += l.ind ;
  }

  while (nsig--) sig_ignore(sigs[nsig]) ;
  pause() ;
  return 0 ;
}
