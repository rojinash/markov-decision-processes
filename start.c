/* start.c
 *
 * A small test program to load and print info on an MDP file
 *
 * Jerod Weinman
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <inttypes.h>

#include "mdp.h"



////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    fprintf(stderr,"Usage: %s mdpfile\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Pointer for the MDP struct
  mdp *p_mdp;

  // Read the MDP file
  p_mdp = mdp_read(argv[1]);

  if (NULL == p_mdp)
    // mdp_read prints a message upon failure
    exit(EXIT_FAILURE);
    
	// Print information about the markov decision process (MDP)
  printf("MDP Info for %s\n",argv[1]);
  printf("Number of states: %u\n", p_mdp->numStates);
  printf("Number of actions: %u\n", p_mdp->numActions);

  printf("Available actions\n");
  
  unsigned int state; // Loop variable over states

  for (state=0 ; state < p_mdp->numStates ; state++)
  {
    printf("State %u: ", state);
    
  }

  // Clean up
  mdp_free(p_mdp);

}
