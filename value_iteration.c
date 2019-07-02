#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include "utilities.h"
#include "mdp.h"

/* Process command-line arguments, verifying usage */
void
process_args (int argc, char* argv[], double * gamma, double * epsilon,
              mdp ** p_mdp );


void
value_iteration ( const mdp * p_mdp, double epsilon, double gamma,
                  double* utilities)
{
 
  int util_length = p_mdp->numStates;
  size_t util_bytes = util_length * sizeof(double);

  // make updated utilities 
  double* util_update = (double*) calloc(util_length, sizeof(double));
  
   // utilities initially zero
  bzero(utilities, util_bytes);

  double delta;
  
  do
    {
      memcpy(utilities, util_update, util_bytes);
     
      delta = 0;
      for(unsigned int state = 0; state < p_mdp->numStates; state++)
        {
          if(p_mdp->terminal[state])
            {
              util_update[state] = p_mdp->rewards[state];
            } else
            {
              double meu = 0;
              unsigned int action;
              calc_meu(p_mdp, state, utilities, &meu, &action);
          
              util_update[state] = p_mdp->rewards[state] + gamma * meu;
            }
          
          if(fabs(util_update[state] - utilities[state]) > delta)
            delta = fabs(util_update[state] - utilities[state]);
            
        }
    } while (delta > (epsilon * (1 - gamma))/ gamma); // ????????????????????
  
  for(int i = 0; i < util_length; i++) {
    printf("utilities: %f\n", utilities[i]);
    printf("util_update: %f\n", util_update[i]);
  }
  
  // free utilities
  free(util_update);

} // value_iteration


/*
 * Main: value_iteration gamma epsilon mdpfile
 *
 * Runs value_iteration algorithm using gamma and with max
 * error of epsilon on utilities of states using MDP in mdpfile.
 *
 * Author: Jerod Weinman
 */
int
main (int argc, char* argv[])
{
  // Read and process configurations
  double gamma, epsilon;
  mdp *p_mdp;

  process_args (argc,argv,&gamma,&epsilon,&p_mdp);

  // Allocate utility array
  double * utilities = malloc ( sizeof(double) * p_mdp->numStates );

  if (NULL == utilities) // Verify we have memory for utility array
  {
    fprintf(stderr,
	    "%s: Unable to allocate utilities (%s)",
	    argv[0], strerror (errno));
    exit (EXIT_FAILURE);
  }

  // Run value iteration!
  value_iteration ( p_mdp, epsilon, gamma, utilities);

  // Print utilities
  unsigned int state;
  for ( state=0 ; state < p_mdp->numStates ; state++)
    if (p_mdp->numAvailableActions[state] > 0 || p_mdp->terminal[state])
      printf ("%1.3f\n", utilities[state]);
    else
      printf("X\n");

  
  // Clean up
  free (utilities);
  mdp_free (p_mdp);
} // main


/* Process command-line arguments, verifying usage */
void
process_args  (int argc, char * argv[], double * gamma, double * epsilon,
               mdp ** p_mdp )
{ 
  if (argc != 4)
  {
    fprintf (stderr,"Usage: %s gamma epsilon mdpfile\n",argv[0]);
    exit (EXIT_FAILURE);
  }

  char * endptr; // String End Location for number parsing
  
  *gamma = strtod(argv[1], &endptr); // Read gamma, the discount factor
  
  if ( (endptr - argv[1]) < strlen(argv[1]) ) 
  { // Error: The entire argument was not consumed by the conversion
    fprintf (stderr, "%s: Illegal non-numeric value in argument gamma=%s\n",
             argv[0], argv[1]);
    exit (EXIT_FAILURE);
  }
  
  // Read epsilon, maximum allowable state utility error
  *epsilon = strtod(argv[2], &endptr); 
  
  if ( (endptr - argv[2]) < strlen(argv[2]) )
  { // Error: The entire argument was not consumed by the conversion
    fprintf (stderr, "%s: Illegal non-numeric value in argument epsilon=%s\n",
             argv[0], argv[2]);
    exit (EXIT_FAILURE);
  }
  
  *p_mdp = mdp_read (argv[3]); // Read MDP file (exits with message if error)

  if (NULL == *p_mdp)
  { // mdp_read prints a message
      exit (EXIT_FAILURE);
  }
} // process_args


