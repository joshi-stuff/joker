#ifndef _COMMON_H_
#define _COMMON_H_

#include <libc.h>

/* Define ALIASNAME as a strong alias for NAME.  */
# define strong_alias(name, aliasname) _strong_alias(name, aliasname)
# define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));
/* Same, but does not check for type match. Use sparingly.
   Example: strong_alias(stat,stat64) may fail, this one works: */
# define strong_alias_untyped(name, aliasname) \
  _strong_alias_untyped(name, aliasname)
# define _strong_alias_untyped(name, aliasname) \
  extern __typeof (aliasname) aliasname __attribute__ ((alias (#name)));

#define attribute_hidden
#define libc_hidden_def(x)
#define libm_hidden_def(x)
#define libc_hidden_weak(x)

typedef void Wvoid;
typedef char Wchar;
typedef unsigned char Wuchar;

#endif /* _COMMON_H_ */
