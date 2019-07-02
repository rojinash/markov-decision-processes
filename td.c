#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "mdp.h"
#include "environment.h"

// Persistent  variables
mdp *         p_mdp;      /* MDP to operate on/in */
double        gamma;      /* Discount factor to use */
unsigned int* policy;     /* Policy: array of actions for each state */
double *      utilities;  /* Array of utilities */
double *      state_freq; /* Counts of state frequencies */
unsigned int  prevState;  /* Previous state encountered */
unsigned int  prevAction; /* Previous action taken */
double        prevReward; /* Previous reward received */
bool          prevValid = false;  /* A state variable indicating whether the
                                     previous state-action pair is valid
                                     (i.e., not restarting after terminal
                                     state */


/* Process command-line arguments, verifying usage */
void
process_args (int argc, char * argv[], double * gamma, unsigned int * trials );
  
/* td_initialize - Establish values of persistent variables for the environment
 *
 * Produces:
 *   [Nothing. Called for side effect.]
 *
 * Preconditions: 
 *   p_mdp_in points to a valid mdp structure 
 *   0 < gamma_in < 1
 *
 * Postconditions:
 *   The following assignments are made to the persistent variables:
 *     p_mdp = p_mdp_in
 *     gamma = gamma_in
 *     prevValid = 0
 *   The following persistent variables point to valid, allocated arrays
 *     policy[numStates] 
 *     utilities[numStates]
 *     state_freq[numStates], all entries initialized to zero
 *
 * Progenitor
 *   Jerod Weinman
 */
void
td_initialize ( mdp * p_mdp_in, double gamma_in )
{
  p_mdp = p_mdp_in;   // Assign MDP object
  gamma = gamma_in; // Set other constants

  // Allocate policy
  policy = malloc ( sizeof(unsigned int) * p_mdp->numStates );

  if (NULL == policy)
  {
    fprintf (stderr, "td_initialize: Unable to allocate policy (%s)",
             strerror (errno));
    exit (EXIT_FAILURE);
  }
  
  // Allocate utilities (zeroed by calloc)
  utilities = calloc ( p_mdp->numStates, sizeof(double) );
  
  if (NULL == utilities)
  {
    fprintf (stderr, "td_initialize: Unable to allocate utilities (%s)",
	    strerror (errno));
    exit (EXIT_FAILURE);
  }

  // Allocate visitation history (zeroed by calloc)
  state_freq = calloc ( p_mdp->numStates, sizeof(double) );

  if (NULL == state_freq)
  {
    fprintf (stderr, "td_initialize: Unable to allocate state_freq  (%s)",
             strerror (errno));
    exit (EXIT_FAILURE);
  }
  
  // Indicate no previous state
  prevValid = false;
} // td_initialize


/* td_cleanup - Release values of persistent variables for the environment
 *
 * Produces:
 *   [Nothing. Called for side effect.]
 *
 * Preconditions: 
 *   p_mdp points to a valid mdp structure 
 *
 * Postconditions:
 *   The following persistent (global) pointer variables are freed
 *     policy
 *     utilities
 *     state_freq
 *   p_mdp has been freed via procedure mdp_free
 *
 * Progenitor
 *   Jerod Weinman
 */
void
td_cleanup ( mdp * p_mdp )
{
  free(policy);
  free(utilities);
  free(state_freq);
  mdp_free(p_mdp);
} // td_cleanup

/* updateWeight - "Learning rate" multiplier
 *
 * Produces:
 *   alpha, a double
 *
 * Preconditions: 
 *   freq > 0
 *
 * Postconditions:
 *   alpha = O(1/freq)
 *
 *   The equation for alpha is taken from Russel & Norvig, Artificial
 *   Intelligence (2010), p. 837.
 *
 * Progenitor
 *   Jerod Weinman
 *
 */
double
updateWeight (double freq)
{
  return 60.0/(59.0 + freq);
} // updateWeight


/* rl_agent_action - 
 *   receive reward for a prior action; indicate action to take in given state
 * 
 * Produces
 *   action, an unsigned int
 *
 * Preconditions
 *   td_initialize has run successfully
 *   0 <= state < p_mdp->numStates
 *
 * Postconditions
 *   action = policy[state]
 *   utilities is updated according to the TD learning rule
 *
 * Progenitor
 *   Jerod Weinman
 */
unsigned int
rl_agent_action(unsigned int state, double reward)
{
  return policy[state]; // Return the policy action for the state
} // rl_agent_action

/*
 * Usage: td gamma mdpfile trials < policy
 *
 * Runs Passive-TD-Agent in an environment for the given number of trials
 * on a fixed policy read from standard input.
 *
 * Jerod Weinman
 */
int
main (int argc, char* argv[])
{
  // Read and process configurations
  double gamma;
  unsigned int trials;

  process_args (argc, argv, &gamma, &trials);

  // Initialize environment
  environment_setup(argv[2]);

  // Initialize agent
  td_initialize (environment_get_mdp(), gamma );

  // Read policy from stdin
  mdp_read_policy (stdin, p_mdp, policy);
  
  // Run Passive-TD-Agent!
  environment_run (trials);

  // Print utilities
  unsigned int state;
  for ( state=0 ; state < p_mdp->numStates ; state++)
    if (p_mdp->numAvailableActions[state] > 0 || p_mdp->terminal[state] )
      printf ("%1.3f\n", utilities[state]);
    else
      printf("X\n");

  td_cleanup (p_mdp);
} // main


/* Process command-line arguments, verifying usage */
void
process_args (int argc, char * argv[], double * gamma, unsigned int * trials )
{
  if (argc != 4)
  {
    fprintf (stderr,"Usage: %s gamma mdpfile trials\n",argv[0]);
    exit (EXIT_FAILURE);
  }
  
  char * endptr; // String End Location for number parsing

  // Read gamma, the discount factor, as a double
  *gamma = strtod (argv[1], &endptr);

  if ( (endptr - argv[1])/sizeof(char) < strlen (argv[1]) )
  {
    fprintf (stderr, "%s: Illegal non-numeric value in argument gamma=%s\n",
             argv[0], argv[1]);
    exit (EXIT_FAILURE);
  }

  // Read trials, number of times to run as an unsigned integer
  *trials = (unsigned int)strtol (argv[3], &endptr,10);

  if ( (endptr - argv[3])/sizeof(char) < strlen (argv[3]) )
  {
    fprintf (stderr, "%s: Illegal non-numeric value in argument trials=%s\n",
             argv[0], argv[3]);
    exit (EXIT_FAILURE);
  }

} // process_args
