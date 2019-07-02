#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include "mdp.h"
#include "environment.h"
#include "max.h"

// Persistent (external) variables

mdp *         p_mdp; /* MDP to operate on/in */
double        gamma; /* Discount factor to use */
double **     state_action_freq; /* Counts of state-action pair frequencies */
double **     state_action_value; /* Counts of state-action pair values */
unsigned int  prevState;  /* Previous state encountered */
unsigned int  prevAction; /* Previous action taken */
double        prevReward; /* Previous reward received */
bool          prevValid = false;  /* A state variable indicating whether the
                                     previous state-action pair is valid
                                     (i.e., not restarting after terminal
                                     state */
double        bestReward; /* "Optimistic estimate of best possible reward" */
double        minTries;   /* Minimum number of times agent must
			     attempt each state-action pair */

/* Process command-line arguments, verifying usage */
void
process_args (int argc, char * argv[], 
              double * gamma, double * reward,  double * attempts, 
              unsigned int * trials);

/* 
 * Procedure:
 *   qlearn_initialize
 *
 * Purpose:
 *   Initialize persistent variables for Q-learning using partial MDP information
 *
 * Parameters:
 *   p_mdp_in
 *   gamma_in
 *   reward
 *   attempts
 *
 * Preconditions:
 *   p_mdp_in points to a valid MDP struct that has the numStates field set
 *   0 < gamma_in < 1
 *
 * Postconditions:
 *   The following assignments are made to the persistent variables:
 *     p_mdp = p_mdp_in
 *     gamma = gamma_in
 *     bestReward = reward
 *     minTries = attempts
 *     prevValid = false
 *   The following persistent variables point to valid, allocated arrays
 *     state_action_freq[numStates][numActions], all entries initialized to zero
 *     state_action_value[numStates][numActions], all entries initialized to zero
 *
 * Progenitor
 *   Jerod Weinman
 */
void qlearn_initialize( mdp * p_mdp_in, double gamma_in, double reward, 
			double attempts)
{
  // Assign MDP object
  p_mdp = p_mdp_in;

  // Set other constants
  gamma = gamma_in;
  bestReward = reward;
  minTries = attempts;

  // Allocate N[s,a]
  state_action_freq = mdp_malloc_state_action( p_mdp->numStates, 
					       p_mdp->numActions );

  // Allocate Q[s,a]
  state_action_value = mdp_malloc_state_action( p_mdp->numStates, 
						p_mdp->numActions );

  // Indicate no previous state
  prevValid = false;
}


/*
 * Procedure
 *   updateWeight
 *
 * Purpose
 *   Give an adjustment factor based on state frequency
 *
 * Parameters
 *   freq
 *
 * Produces
 *   alpha, a double
 *
 * Postconditions
 *   alpha = O(1/freq)
 *
 *   The equation for alpha is taken from Russel & Norvig, Artificial
 *   Intelligence (2010), p. 837.
 *
 *  Author
 *    Jerod Weinman
 */

double updateWeight(double freq)
{
  return 60.0/(59.0 + freq);
}

/*
 * Procedure
 *   exploration_function
 *
 * Purpose
 *   Return an optimistic reward/utility value that encourages new actions
 *
 * Parameters
 *   u
 *   n
 *
 * Produces
 *   f, a double
 *
 * Postconditions
 *   If (n < minTries), f = bestReward.
 *   Otherwise, f = u  .
 *
 *   The equation for f is taken from Russel & Norvig, Artificial
 *   Intelligence (2010), p. 842.
 *
 *  Progenitor
 *    Jerod Weinman
 */

double exploration_function( double u, double n )
{
  if (n < minTries)
    return bestReward;
  else
    return u;
}
  
/* rl_agent_action - 
 *   receive reward for a prior action; indicate action to take in given state
 * 
 * Produces
 *   action, an unsigned int
 *
 * Preconditions
 *   qlearn_initialize has run successfully
 *   0 <= state < p_mdp->numStates
 *
 * Postconditions
 *   action is a member of p_mdp->actions[state]
 */
unsigned int rl_agent_action(unsigned int state, double reward)
{
  double maxQ = 0;
  // if terminal state
  if (p_mdp->terminal[state]) {
    for (unsigned int action = 0; action < p_mdp->numActions; action++) {
      state_action_value[state][action] = reward;
    }
    maxQ = reward; 
  } else {
    maxQ = max_value(p_mdp->numAvailableActions[state], p_mdp->actions[state], state_action_value[state]);
  }

  if (prevValid) {
    state_action_freq[prevState][prevAction]++;
    state_action_value[prevState][prevAction] += updateWeight(state_action_freq[prevState][prevAction]) *
      (prevReward + gamma*maxQ - state_action_value[prevState][prevAction]);
  }

  if (p_mdp->terminal[state]) {
    prevValid = false;
  } else {
    prevState = state;
    for (unsigned int action = 0; action < p_mdp->numAvailableActions[state]; action++) {
      if (exploration_function(state_action_value[state][action], state_action_freq[state][action]) >
          exploration_function(state_action_value[state][prevAction], state_action_freq[state][prevAction])) {
        prevAction = p_mdp->actions[state][action];
      }
    }
    prevReward = reward;
    prevValid = true;
  }

  return prevAction;
}


/*
 Usage: qlearn gamma reward attempts mdpfile trials
 
 Runs Q-Learning-Agent in an environment for the given number of
 trials with an exploration function that uses reward as an optimistic
 estimate when the number of state-action experiences is less than
 attempts.

 Author: Jerod Weinman
*/
int
main (int argc, char* argv[])
{
  // Loop variables for later
  unsigned int state,action;

  // Read and process configurations
  double gamma, reward, attempts;
  unsigned int trials;

  process_args (argc,argv, &gamma, &reward, &attempts, &trials);

  // Initialize environment
  environment_setup (argv[4]);

  // Initialize agent
  qlearn_initialize (environment_get_mdp(), gamma, reward, attempts );

  // Run Q-Learning-Agent!
  environment_run (trials);

  // Print values
  printf("Q[s,a]\n");
  for ( state=0 ; state < p_mdp->numStates ; state++)
  {
    for ( action=0 ; action < p_mdp->numActions ; action++)
      printf ("%1.3f\t",state_action_value[state][action]);
    printf ("\n");
  }

  // Print utilities
  printf("\nU[s]\n");
  for ( state = 0; state < p_mdp->numStates ; state++) // For each state
    
    // if there are any actions in the state
    if (p_mdp->numAvailableActions[state] > 0)
      // print the maximum Q-value (which is the utility)
      printf ("%f\n", max_value (p_mdp->numAvailableActions[state],
                                 p_mdp->actions[state],
                                 state_action_value[state] ) );
  // Otherwise, if the state is terminal
    else if (p_mdp->terminal[state])
      // Print the value of the first action (which is the reward)
      printf ("%f\n", state_action_value[state][0] );
    else
      // Otherwise, just print X
      printf ("X\n");


  // Print policy
  printf ("\npolicy[s]\n");
  for ( state = 0; state < p_mdp->numStates ; state++)
    if (p_mdp->numAvailableActions[state] > 0)
      printf ("%u\n", arg_max_value ( p_mdp->numAvailableActions[state],
                                      p_mdp->actions[state],
                                      state_action_value[state] ) );
    else
      printf ("X\n");

  return 0;
} // main


/* Process user command line input in one handy, easy to read function */
void
process_args (int argc, char * argv[], 
              double * gamma, double * reward,  double * attempts, 
              unsigned int * trials)
{
  if (argc != 6)
  {
    fprintf (stderr,
             "Usage: %s gamma reward attempts mdpfile trials\n", argv[0]);
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

  // Read optimistic reward as a double
  *reward = strtod (argv[2], &endptr);

  if ( (endptr - argv[2])/sizeof(char) < strlen (argv[2]) )
  {
    fprintf (stderr, "%s: Illegal non-numeric value in argument reward=%s\n",
             argv[0], argv[2]);
    exit (EXIT_FAILURE);
  }

  // Read number of attempts to require as a double
  *attempts = strtod (argv[3], &endptr);

  if ( (endptr - argv[3])/sizeof(char) < strlen (argv[3]) )
  {
    fprintf (stderr, "%s: Illegal non-numeric value in argument attempts=%s\n",
             argv[0], argv[3]);
    exit (EXIT_FAILURE);
  }
  
  // Read trials, number of times to run as an unsigned integer
  *trials = (unsigned int)strtol(argv[5], &endptr, 10);
  
  if ( (endptr - argv[5])/sizeof(char) < strlen (argv[5]) )
  {
    fprintf (stderr, "%s: Illegal non-numeric value in argument trials=%s\n",
             argv[0], argv[5]);
    exit (EXIT_FAILURE);
  }
} // process_args
