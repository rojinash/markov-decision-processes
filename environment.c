#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "mdp.h"
#include "environment.h"

// Persistent (external) variables

 mdp *         p_mdp_env; /* MDP to operate on/in */

////////////////////////////////////////////////////////////////////////////////
void environment_setup( char * mdpfile)
{
  p_mdp_env = mdp_read (mdpfile);

  if (NULL == p_mdp_env)
  {
    fprintf(stderr,"environment_setup: Failed to read MDP file %s\n",mdpfile);
    exit(EXIT_FAILURE);
  }
}

////////////////////////////////////////////////////////////////////////////////
mdp* environment_get_mdp()
{
  mdp * p_mdp_out =mdp_duplicate(p_mdp_env); // Create a copy

  unsigned int s,t,a; // Loop variables

  // Zero-out transition probabilities
  for ( s=0 ; s < p_mdp_env->numStates ; s++)
    for ( t=0 ; t < p_mdp_env->numStates ; t++)
      for ( a=0 ; a < p_mdp_env->numActions ; a++)
	p_mdp_out->transitionProb[s][t][a] = 0.0;

  // Zero-out rewards
  for ( s=0 ; s < p_mdp_env->numStates ; s++)
    p_mdp_out->rewards[s] = 0;

  return p_mdp_out;
}

////////////////////////////////////////////////////////////////////////////////
unsigned int environment_get_num_states() { return p_mdp_env->numStates; }
unsigned int environment_get_num_actions() { return p_mdp_env->numActions; }

////////////////////////////////////////////////////////////////////////////////
void environment_run(const unsigned int trials)
{
  unsigned int iter;  

  for (iter=0 ; iter<trials ; iter++)
    environment_run_trial();

}


////////////////////////////////////////////////////////////////////////////////
void environment_run_trial()
{
  unsigned int iter = 0;

  unsigned int state; // Current state (initial or arising from agent action)
  unsigned int action; // Last action taken by the agent
  double reward; // Reward for the current state

  double randNum; // Random number in [0,1]
  double cumProb; // Cumulative of P(t|s,a) for t=0..

  unsigned int nextState; // Subsequent state for transition due to action

  state = p_mdp_env->start; // Initialize the start state

  do
  {
    reward = p_mdp_env->rewards[state]; // Determine reward of the current state

    action = rl_agent_action(state,reward); // Get an action from the agent

    if (p_mdp_env->terminal[state]) // Finish if state was terminal
      break;

    // Next, we have to choose the subsequent state randomly by
    // sampling from the MDP's conditional transition probability P(t|s,a)
    
    // Get a random number in [0,1]
    randNum = ((double)random()) / RAND_MAX;
    
    // Find the nexState for which the cumulative meets the random value
    cumProb = 0.0;
    nextState = 0; 

    do
    { // Add to the cumulative
      cumProb += p_mdp_env->transitionProb[nextState][state][action];

      if ( cumProb > randNum) // If CDF has passed our random point,
	break;                // then we're at the right state, so exit
      nextState++;            // otherwise go to the last state,
    }  // but if we're now at the last state, exit the loop 
    while (nextState < p_mdp_env->numStates-1);
    
    state = nextState; // Update state <-- nextState for next iteration

    iter++;
  } 
  while(1); // terminal state test is within do loop
}
