#include "mdp.h"
#include "utilities.h"

#include <stdio.h> // for error message only

double
calc_eu ( const mdp *  p_mdp, unsigned int state, const double * utilities,
          const unsigned int action)
{
  double eu = 0;   // Expected utility

  // Calculate expected utility: sum_{s'} P(s'|s,a)*U(s')
  fprintf (stderr,
           "WARNING! calc_eu not implemented. Results will be invalid!\n");
  
  return eu;
}

void
calc_meu ( const mdp * p_mdp, unsigned int state, const double * utilities,
           double * meu, unsigned int * action )
{
  // Calculated maximum expected utility (use calc_eu):
  fprintf (stderr,
           "WARNING! calc_meu not implemented. Results will be invalid!\n");
}
