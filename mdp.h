/* mdp.h
 *
 * A file containing information on an MDP (markov decision process) struct and
 * related utilities.
 * 
 * Jerod Weinman
 * 8 August 2011
 *
 */

#ifndef __MDP_H__
#define __MDP_H__

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  unsigned int numStates;  /* Total number of possible states */
  unsigned int numActions; /* Total number of possible actions */
  unsigned int start;      /* Starting state for this MDP */
  double ***transitionProb;/* A numStates x numStates x numActions array of
                              transition probabilities for the model world:
                              transitionProb[t][s][a] := P(t|s,a) */
  unsigned int *numAvailableActions; /* A numStates length array, where each
                                        entry indicates the number of
                                        actions available in the given state */
  unsigned int **actions;  /* A numStates x numAvailableActions[state] length
                              array of the actions available in a given state */
  double *rewards;         /* A numStates length array of the reward
                              for a given state */
  bool *terminal;          /* A numStates length array, each entry indicating
                              whether a given state is terminal */
} mdp; 


/*  Procedure
 *    mdp_read
 *
 *  Purpose
 *    Read an MDP from a file
 *
 *  Parameters
 *   fileName, a string
 *
 *  Produces,
 *   p_mdp, an mdp*
 *
 *  Preconditions
 *    fileName is a null-terminated string (character array) that refers to a 
 *    readable file containing a valid MDP description
 *
 *  Postconditions
 *    Memory is allocated for all fields in pmdp. p_mdp is populated
 *    with data read from fileName
 */
mdp *
mdp_read (const char * fileName);


/*  Procedure
 *    mdp_free
 *
 *  Purpose
 *    Free memory for all fields of an MDP
 *
 *  Parameters
 *   p_mdp, an mdp*
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struct with all fields having valid references
 *
 *  Postconditions
 *    Memory is freed for all fields in p_mdp
 */
void
mdp_free (mdp * p_mdp);


/*  Procedure
 *    mdp_malloc_transition
 *
 *  Purpose
 *    Allocate arrays for transition probabilities or counts
 *
 *  Parameters
 *    numStates
 *    numActions
 *
 *  Produces,
 *    double transition[numStates][numStates][numActions]
 *
 *  Preconditions
 *    numStates > 0
 *    numActions > 0
 *    Sufficient memory is available 
 *    (e.g., numStates^2 * numActions * sizeof(double)
 *
 *  Postconditions
 *    transition is a pointer to a valid three-dimensional array of size 
 *      numState x numStates x numActions
 *    All entries in the array are initialized to zero.
 *    Any failure causes program exit.
 */
double ***
mdp_malloc_transitions (unsigned int numStates, unsigned int numActions);


/*  Procedure
 *    mdp_free_transitions
 *
 *  Purpose
 *    Free arrays for transition probabilities or counts
 *
 *  Parameters
 *    numStates
 *    transition[][][]
 *
 *  Produces,
 *    [Nothing.]
 *
 *  Preconditions
 *    numStates > 0
 *    transition is a pointer to a valid three-dimensional array of size 
 *      numStates x numStates x numActions
 *
 *  Postconditions
 *    All arrays pointed to by transition are freed
 */
void
mdp_free_transitions ( unsigned int numStates, double *** transitions );


/*  Procedure
 *    mdp_malloc_state_action
 *
 *  Purpose
 *    Allocate arrays for state-action counts
 *
 *  Parameters
 *    numStates
 *    numActions
 *
 *  Produces,
 *    double count[numStates][numActions]
 *
 *  Preconditions
 *    numStates > 0
 *    numActions > 0
 *    Sufficient memory is available 
 *    (e.g., numStates * numActions * sizeof(double)
 *
 *  Postconditions
 *    count is a pointer to a valid two-dimensional array of size 
 *      numStates x numActions
 *    All entries in the array are initialized to zero.
 *    Any failure causes program exit.
 */
double **
mdp_malloc_state_action (unsigned int numStates, unsigned int numActions);


/*  Procedure
 *    mdp_free_state_action
 *
 *  Purpose
 *    Free arrays for state-action counts
 *
 *  Parameters
 *    numStates
 *    count[][]
 *
 *  Produces,
 *    [Nothing.]
 *
 *  Preconditions
 *    numStates > 0
 *    count is a pointer to a valid two-dimensional array of size 
 *      numStates x numActions
 *
 *  Postconditions
 *    All arrays pointed to by count are freed
 */
void
mdp_free_state_action ( unsigned int numStates, double ** count );


/*  Procedure
 *    mdp_duplicate_transition
 *
 *  Purpose
 *    Construct a deep clone of an MDP
 *
 *  Parameters
 *    p_mdp
 *
 *  Produces,
 *    p_mdp_out
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struct
 *
 *  Postconditions
 *    p_mdp_out points to a valid mdp struct that is an identical copy
 *    of p_mdp with respect to content
 *    mdp_free(p_mdp_out) may be called with no ill-effects upon p_mdp
 *    Any failure causes program exit.
 */
mdp *
mdp_duplicate ( mdp *  p_mdp);

/*  Procedure
 *    mdp_read_policy
 *
 *  Purpose
 *    Read a policy for an MDP from a file stream
 *
 *  Parameters
 *    stream
 *    p_mdp
 *    policy
 *
 *  Produces,
 *    [Nothing.]
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struct
 *    policy is a p_mdp->numStates length array of unsigned integers
 *    The next line of data in stream is may be read as unsigned integers
 *
 *  Postconditions
 *    p_mdp->start is assigned as read from stream
 *    Any failure causes program exit.
 *
 *  Postconditions
 */
void
mdp_read_policy (FILE* stream, const mdp * p_mdp, unsigned int * policy);

  
#endif /* __MDP_H__ */
