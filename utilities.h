/* utilities.h
 *
 * A file containing declarations for two procedures, one for
 * calculating the expected utility taking an action in a state of an
 * MDP, and another that returns the action giving maximal expected
 * utility in a state of an MDP.
 * 
 * Jerod Weinman
 * 8 August 2011
 *
 */

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include  "mdp.h"

/*  Procedure
 *    calc_eu
 *
 *  Purpose
 *    Calculate the expected utility of a state and action in an MDP
 *
 *  Parameters
 *   p_mdp
 *   state
 *   utilities
 *   action
 *
 *  Produces
 *   eu
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struc
 *    0 <= state < p_mdp->numStates
 *    utilities points to a valid array of length p_mdp->numStates
 *
 *  Practica
 *    The following are not preconditions, per se, but lend themselves
 *    to meaningful results:
 *      p_mdp->terminal[state] = 0 (No meaningful actions in a terminal state)
 *      action belongs to the array p_mdp->actions[state]
 *
 *  Postconditions
 *    eu is the average utility of subsequent states that arise from taking the
 *    specified action in the given state:
 *       eu = sum_{s'=0..p_mdp->numStates} P(s'|state,action) * utilities(s')
 *    where P(s'|s,a) represents the transition probability in the MDP
 */
double
calc_eu ( const mdp *  p_mdp, unsigned int state, const double * utilities,
          const unsigned int action);


/*  Procedure
 *    calc_meu
 *
 *  Purpose
 *    Calculate the action of maximum expected utility of a state in an MDP
 *
 *  Parameters
 *   p_mdp
 *   state
 *   utilities
 *   meu
 *   action
 *
 *  Produces
 *   [Nothing.]
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struc
 *    0 <= state < p_mdp->numStates
 *    utilities points to a valid array of length p_mdp->numStates
 *    meu != NULL
 *    action != NULL
 *
 *  Postconditions
 *    *meu is the maximum expected utility of state in p_mdp:
 *        max_{a} EU(state,a) 
 *      = max_{a} sum_{s'=0..p_mdp->numStates} P(s'|state,a) * 
 *                                                  utilities(s')
 *    where P(s'|s,a) represents the transition probability in the MDP
 *    
 *    *action is a value of a that yields *meu: argmax_{a} EU(state,a) 
 */
void
calc_meu ( const mdp *  p_mdp, unsigned int state, const double * utilities,
           double * meu, unsigned int * action );

#endif // __UTILITIES_H__
