/**
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses
 * 
 * The controlling expression of an #if or #elif preprocessing directive shall evaluate to 0 or 1
 * 
 * All identifiers used in the controlling expression of #if or #elif preprocessing 
 * directives shall be #define'd before evaluation
 * 
 * #define and #undef shall not be used on a reserved identifier or reserved macro name
 * Identifiers or macro names should not begin with an underscore;
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define M1(x, y) (x * y)     /* Non-compliant */
#define M2(x, y) ((x) * (y)) /* Compliant */

#define TRUE 1
#define FALSE 0

#if FALSE /* Compliant */
#endif

#if 10 /* Non-compliant */
#endif

#if !defined(X) /* Compliant */
#endif

#if A > B /* Compliant assuming A and B are numeric */
#endif

#if M == 0 /* Non-compliant - Does 'M' expand to zero or is it undefined? */
#endif

#if defined(M) /* Compliant - M is not evaluated */
#if M == 0     /* Compliant - M is known to be defined */
#endif
#endif

/* Compliant - B is only evaluated in ( B == 0 ) if it is defined */
#if defined(B) && (B == 0)
#endif

#undef __LINE__      /* Non-compliant - begins with _ */
#undef _BUILTIN_sqrt /* Non-compliant - the implementation may use _BUILTIN_sqrt for other purposes, e.g. generating a sqrt instruction */

#define defined        /* Non-compliant - reserved identifier */
#define errno my_errno /* Non-compliant - library identifier */

int main()
{
    return 0;
}