/* ISC license. */

#ifndef S6LOCK_H
#define S6LOCK_H

#include <errno.h>
#include "uint16.h"
#include "tai.h"
#include "genalloc.h"
#include "gensetdyn.h"
#include "skaclient.h"
#include "s6-config.h"


 /* Constants */

#define S6LOCKD_PROG S6_BINPREFIX "s6lockd"
#define S6LOCKD_HELPER_PROG S6_BINPREFIX "s6lockd-helper"

#define S6LOCK_BANNER1 "s6lock v1.0 (b)\n"
#define S6LOCK_BANNER1_LEN (sizeof S6LOCK_BANNER1 - 1)
#define S6LOCK_BANNER2 "s6lock v1.0 (a)\n"
#define S6LOCK_BANNER2_LEN (sizeof S6LOCK_BANNER2 - 1)


 /* The s6lock_t itself */

typedef struct s6lock_s s6lock_t, *s6lock_t_ref ;
struct s6lock_s
{
  skaclient2_t connection ;
  genalloc list ; /* array of uint16 */
  gensetdyn data ; /* set of char */
} ;
#define S6LOCK_ZERO { SKACLIENT2_ZERO, GENALLOC_ZERO, GENSETDYN_INIT(int, 2, 0, 1) }
extern s6lock_t const s6lock_zero ;


 /* Starting and ending a session */

#define s6lock_start(a, ipcpath, deadline, stamp) skaclient2_start(&(a)->connection, ipcpath, S6LOCK_BANNER1, S6LOCK_BANNER1_LEN, S6LOCK_BANNER2, S6LOCK_BANNER2_LEN, deadline, stamp)
#define s6lock_start_g(a, ipcpath, deadline) s6lock_start(a, ipcpath, (deadline), &STAMP)
extern int s6lock_startf (s6lock_t_ref, char const *, struct taia const *, struct taia *) ;
#define s6lock_startf_g(a, lockdir, deadline) s6lock_startf(a, lockdir, (deadline), &STAMP)
extern void s6lock_end (s6lock_t_ref) ;
                    

 /* Asynchronous primitives */

#define s6lock_fd(a) skaclient2_fd(&(a)->connection)
extern int s6lock_update (s6lock_t_ref) ;
extern int s6lock_check (s6lock_t_ref, uint16) ;


 /* Synchronous functions */

#define S6LOCK_OPTIONS_SH 0x0000U
#define S6LOCK_OPTIONS_EX 0x0001U

extern int s6lock_acquire (s6lock_t_ref, uint16 *, char const *, uint32, struct taia const *, struct taia const *, struct taia *) ;
#define s6lock_acquire_g(a, id, path, options, limit, deadline) s6lock_acquire(a, id, path, options, limit, (deadline), &STAMP)
#define s6lock_acquire_sh(a, id, path, limit, deadline, stamp) s6lock_aquire(a, id, path, S6LOCK_OPTIONS_SH, limit, deadline, stamp)
#define s6lock_acquire_ex(a, id, path, limit, deadline, stamp) s6lock_aquire(a, id, path, S6LOCK_OPTIONS_EX, limit, deadline, stamp)
#define s6lock_acquire_sh_g(a, id, path, limit, deadline) s6lock_acquire_sh(a, id, path, limit, (deadline), &STAMP)
#define s6lock_acquire_ex_g(a, id, path, limit, deadline) s6lock_acquire_ex(a, id, path, limit, (deadline), &STAMP)
extern int s6lock_release (s6lock_t_ref, uint16, struct taia const *, struct taia *) ;
#define s6lock_release_g(a, id, deadline) s6lock_release(a, id, (deadline), &STAMP)

extern int s6lock_wait_and (s6lock_t_ref, uint16 const *, unsigned int, struct taia const *, struct taia *) ;
#define s6lock_wait_and_g(a, list, len, deadline) s6lock_wait_and(a, list, len, (deadline), &STAMP)
extern int s6lock_wait_or  (s6lock_t_ref, uint16 const *, unsigned int, struct taia const *, struct taia *) ;
#define s6lock_wait_or_g(a, list, len, deadline) s6lock_wait_or(a, list, len, (deadline), &STAMP)

#endif
