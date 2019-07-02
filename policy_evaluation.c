#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"

/*  Procedure
 *    policy_evaluation
 *
 *  Purpose
 *    Iteratively estimate state utilities under a fixed policy
 *
 *  Parameters
 *   policy
 *   p_mdp
 *   epsilon
 *   gamma
 *   utilities
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    policy points to a valid array of length p_mdp->numStates
 *    Each policy entry respects 0 <= policy[s] < p_mdp->numActions
 *       and policy[s] is an entry in p_mdp->actions[s]
 *    p_mdp is a pointer to a valid, complete mdp
 *    epsilon > 0
 *    0 < gamma < 1
 *    utilities points to a valid array of length p_mdp->numStates
 *
 *  Postconditions
 *    utilities[s] has been updated according to the simplified Bellman update
 *    so that no update is larger than epsilon
 */
void policy_evaluation( const unsigned int* policy, const mdp* p_mdp,
			double epsilon, double gamma,
			double* utilities)
{
  double delta; // max change in U of any state at any iteration
  int util_length = p_mdp->numStates;
  size_t util_bytes = util_length * sizeof(double);
  double* util_update = (double*) calloc(util_length, sizeof(double));

  do
    {
      delta = 0;
      for(unsigned int state = 0; state < p_mdp->numStates; state++)
        {
          if(p_mdp->terminal[state])
            {
              util_update[state] = p_mdp->rewards[state];
            } else
            {
              unsigned int action = 0;
              util_update[state] = p_mdp->rewards[state] +
                gamma * calc_eu(p_mdp, state, utilities, action);
            }
          if(fabs(util_update[state] - utilities[state]) > delta)
              delta = fabs(util_update[state] - utilities[state]);
            
          
        }
       memcpy(utilities, util_update, util_bytes);
       
    } while(delta > epsilon);

  // free util_update
  free(util_update);
}
