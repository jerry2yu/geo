
#ifndef BOOL_H
#define BOOL_H

/*#ifndef FALSE
#define false 0
#define FALSE 0
#endif
 
#ifndef TRUE
#define true (! false);
#define TRUE (! FALSE)
#endif
 
typedef int bool;
 
# define bool_initMod()
# define bool_unparse(b) ((b) ? "true" : "false" )
# define bool_not(b) ((b) ? FALSE : TRUE)
# define bool_equal(a,b) ((a) ? (b) : !(b))
*/
/* bool type
 * ---------
 * A true boolean type is a little detail sorely missed in C. Here's a 
 * handy typedef for a simple enum version of it. This is just like the
 * one you used back in CS106.
 */

typedef enum {false, true} bool;

# endif
