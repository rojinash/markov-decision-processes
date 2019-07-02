/* mdp.c
 *
 * A file containing implementation of utilities for an MDP (markov
 * decision process) struct.
 * 
 * Jerod Weinman
 * 8 August 2011
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "mdp.h"


/*  Procedure
 *    mdp_read_dimensions
 *
 *  Purpose
 *    Read the number of states and number of actions from an MDP file
 *
 *  Parameters
 *   stream
 *   p_numStates
 *   p_numActions
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    stream is a valid, open stream that may be read from
 *    The next line of data in stream is may be read as two unsigned integers
 *
 *  Postconditions
 *    *p_numStates contains the first unsigned integer, representing
 *    the number of MDP states. 
 *    *p_numActions contains the second unsigned integer, representing 
 *    the number of MDP actions. 
 *    stream has advanced only to just past these two values.
 *    Any failure causes program exit.
 */
void
mdp_read_dimensions (FILE * stream, unsigned int * p_numStates, 
                     unsigned int * p_numActions)
{

  int count; // Place holder for fscanf return values

  // Read number of states
  count = fscanf (stream, "%ud", p_numStates);

  // Check for errors 
  if ( EOF == count )
  {
    if ( ferror (stream) )
      fprintf (stderr,
               "mdp_read_dimensions failed: %s\n",
               strerror (errno));
    else
      fprintf (stderr, 
               "mdp_read_dimensions failed: %s\n",
               "Premature end of file");
    exit (EXIT_FAILURE);
  }
  else if (count != 1)
  {
    fprintf (stderr,
             "mdp_read_dimensions_failed: %s\n",
             "Unable to match unsigned int for numStates");
    exit (EXIT_FAILURE);
  }

  // Read number of states
  count = fscanf (stream, "%ud", p_numActions);
  
  // Check for errors 
  if ( EOF == count )
  {
    if ( ferror (stream) )
      fprintf (stderr,
               "mdp_read_dimensions failed: %s\n",
               strerror(errno));
    else
      fprintf (stderr, 
               "mdp_read_dimensions failed: %s\n",
               "Premature end of file");
    exit (EXIT_FAILURE);
  }
  else if (count != 1)
  {
    fprintf (stderr,
             "mdp_read_dimensions failed: %s\n",
             "Unable to match unsigned int for numActions");
    exit (EXIT_FAILURE);
  }
    
} // mdp_read_dimensions


/*  Procedure
 *    mdp_read_start
 *
 *  Purpose
 *    Read the initial state from an MDP file
 *
 *  Parameters
 *   stream
 *   p_mdp
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    stream is a valid, open stream that may be read from
 *    The next line of data in stream is may be read as an unsigned integer
 *
 *  Postconditions
 *    p_mdp->start is assigned as read from stream
 *    Any failure causes program exit.
 */
void
mdp_read_start (FILE * stream, mdp * p_mdp)
{

  int count; // Place holder for fscanf return values

  // Read start
  count = fscanf(stream, "%ud", &p_mdp->start);

  // Check for errors 
  if ( EOF == count )
  {
    if ( ferror (stream) )
      fprintf(stderr,
	      "mdp_read_start failed: %s\n",
	      strerror(errno));
    else
      fprintf(stderr, 
	      "mdp_read_start failed: %s\n",
	      "Premature end of file");
    exit(EXIT_FAILURE);
  }
  else if (count != 1)
  {
    fprintf(stderr,
	    "mdp_read_start_failed: %s\n",
	    "Unable to match unsigned int for start");
    exit(EXIT_FAILURE);
  }
} // mdp_read_start


////////////////////////////////////////////////////////////////////////////////
mdp *
mdp_malloc (const unsigned int numStates, const unsigned int numActions)
{

  //----------------------------------------
  // Root structure

  mdp * p_mdp = malloc(sizeof(mdp));

  if (NULL == p_mdp)
  {
    fprintf (stderr,"mdp_malloc failed: %s (%s)\n",
             "Could not allocate mdp",
             strerror (errno));
    exit (EXIT_FAILURE);
  }
  
  //----------------------------------------
  // Transition probability

  p_mdp->transitionProb = mdp_malloc_transitions (numStates, numActions);


  //----------------------------------------
  // Number of available actions

  p_mdp->numAvailableActions = malloc (sizeof(unsigned int) * numStates);

  if  ( NULL == p_mdp->numAvailableActions )
  {
    fprintf (stderr,"mdp_malloc failed: %s (%s)\n",
             "Could not allocate numAvailableActions",
             strerror (errno));
    exit (EXIT_FAILURE);
  }

  //----------------------------------------
  // Available actions
  // SECONDARY ARRAYS CANNOT BE ALLOCATED UNTIL numAvailableActions is known
  p_mdp->actions = malloc (sizeof(unsigned int*) * numStates);
  
  if  ( NULL == p_mdp->actions )
  {
    fprintf (stderr,"mdp_malloc failed: %s (%s)\n",
             "Could not allocate actions",
             strerror (errno));
    exit (EXIT_FAILURE);
  }

  //----------------------------------------
  // Rewards

  p_mdp->rewards = malloc (sizeof(double) * numStates);

  if  ( NULL == p_mdp->rewards )
  {
    fprintf (stderr,"mdp_malloc failed: %s (%s)\n",
             "Could not allocate rewards",
             strerror (errno));
    exit (EXIT_FAILURE);
  }

  //----------------------------------------
  // Terminal states
  p_mdp->terminal = malloc (sizeof(unsigned int) * numStates);

  if  ( NULL == p_mdp->terminal )
  {
    fprintf (stderr,"mdp_malloc failed: %s (%s)\n",
             "Could not allocate terminal",
             strerror (errno));
    exit (EXIT_FAILURE);
  }

  // Initialize to zero
  memset ( p_mdp->terminal, 0, sizeof(unsigned int) * numStates );

  
  return p_mdp;
} // mdp_read_start

////////////////////////////////////////////////////////////////////////////////
double ***
mdp_malloc_transitions (unsigned int numStates, unsigned int numActions)
{

  double *** transitionProb;

  transitionProb = malloc (sizeof(double**) * numStates); // P(s'|...

  if (NULL == transitionProb) 
  {
    fprintf (stderr,"mdp_malloc_transitions failed: %s (%s)\n",
             "Could not allocate transitionProb",
             strerror (errno));
    exit (EXIT_FAILURE);
  }

  unsigned int i,j,k;
  for ( i = 0 ; i<numStates ; i++ )
  {
    transitionProb[i] = malloc (sizeof(double*) * numStates); // P(s'|s,...
    
    if ( NULL == transitionProb[i] ) 
    {
      fprintf (stderr,"mdp_malloc_transitions failed: %s (%s)\n",
               "Could not allocate transitionProb[s']",
               strerror (errno));
      exit (EXIT_FAILURE);
    }
    
    for ( j = 0 ; j<numStates ; j++ )
    {
      // P(s'|s,a)
      transitionProb[i][j] = malloc (sizeof(double) * numActions); 
      
      if  ( NULL == transitionProb[i][j] )
      {
	fprintf (stderr,"mdp_malloc_transitions failed: %s (%s)\n",
                 "Could not allocate transitionProb[s'][s]",
                 strerror (errno));
	exit (EXIT_FAILURE);
      }

      for ( k = 0 ; k<numActions ; k++)
	transitionProb[i][j][k] = 0.0; // Initialize to zero as promised
    }
  }
  
  return transitionProb;
} // mdp_malloc_transitions


////////////////////////////////////////////////////////////////////////////////
double **
mdp_malloc_state_action (unsigned int numStates, unsigned int numActions)
{

  double ** count;

  count = malloc (sizeof(double*) * numStates); // N[s,...

  if (NULL == count) 
  {
    fprintf (stderr,"mdp_malloc failed: %s (%s)\n",
             "Could not allocate count",
             strerror (errno));
    exit (EXIT_FAILURE);
  }

  unsigned int i;
  for ( i = 0 ; i<numStates ; i++ )
  {
    count[i] = calloc (numActions, sizeof(double) ); // N[s,a]
    
    if ( NULL == count[i] ) 
    {
      fprintf (stderr,"mdp_malloc_state_action failed: %s (%s)\n",
               "Could not allocate count[s]",
               strerror (errno));
      exit (EXIT_FAILURE);
    }
  }
  
  return count;
} // mdp_malloc_state_action
                         

////////////////////////////////////////////////////////////////////////////////
void
mdp_free_transitions ( unsigned int numStates, double *** transitions )
{
  unsigned int i,j;

  for ( i = 0 ; i< numStates ; i++ )
  {
    for ( j = 0 ; j < numStates ; j++ )
      free (transitions[i][j]);
    free (transitions[i]);
  }
  
  free (transitions);
} // mdp_free_transitions


////////////////////////////////////////////////////////////////////////////////
void
mdp_free_state_action ( unsigned int numStates, double ** count )
{
  unsigned int i;
  
  for ( i = 0 ; i < numStates ; i++)
    free (count[i]);

  free (count);
} // mdp_free_state_action


/*  Procedure
 *    mdp_malloc_actions
 *
 *  Purpose
 *    Allocate arrays for the available number of actions
 *
 *  Parameters
 *    p_mdp
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    p_mdp points to a valid mdp struct
 *    p_mdp->numAvailableActions is a valid pointer to an array of
 *    length p_mdp->numStates with non-negative entries strictly less
 *    than p_mdp->numActions.
 *    p_mdp->actions is a valid pointer to an array of length p_mdp->numStates
 *
 *  Postconditions
 *    For 0 <= i < p_mdp->numStates, p_mdp->actions[i] is a valid
 *    pointer to an unsigned int array of length
 *    p_mdp->numAvailableActions[i],
 *    Any failure causes program exit.
 */
void
mdp_malloc_actions (mdp * p_mdp)
{
  unsigned int i;

  for ( i=0 ; i < p_mdp->numStates ; i++)
  {
    p_mdp->actions[i] = malloc ( sizeof(unsigned int) * 
                                 p_mdp->numAvailableActions[i] );

    if ( NULL == p_mdp->actions[i] )
    {
      fprintf (stderr,"mdp_malloc_actions failed: %s (%s)\n",
               "Could not allocate actions[i]",
               strerror (errno));
      exit (EXIT_FAILURE);
    }
  }
} // mdp_malloc_actions


/*  Procedure
 *    mdp_read_transitions
 *
 *  Purpose
 *    Read the transition matrix from a file and assign values to mdp struct
 *
 *  Parameters
 *   stream
 *   p_mdp
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    stream is a valid, open stream that may be read from
 *    The next lines of data in stream is may be read as doubles
 *
 *  Postconditions
 *    All values in p_mdp->transitionProb are assigned as read from stream
 *    Any failure causes program exit.
 */
void
mdp_read_transitions ( FILE * stream, mdp * p_mdp)
{
  unsigned int i,j,k;
  
  int count;

  for (i=0 ; i < p_mdp->numStates ; i++)
    for (j=0 ; j < p_mdp->numStates ; j++)
      for (k=0 ; k< p_mdp->numActions ; k++)
      {
	// Read/assign entry
	count = fscanf (stream, "%lf", &(p_mdp->transitionProb[i][j][k]) );

	// Check for errors
	if ( EOF == count )
	{
	  if ( ferror (stream) )
	    fprintf (stderr,
                     "mdp_read_transitions failed: %s\n",
                     strerror (errno));
	  else
	    fprintf (stderr, 
                     "mdp_read_transitions failed: %s\n",
                     "Premature end of file");
	  exit (EXIT_FAILURE);
	}
	else if (count != 1)
	{
	  fprintf (stderr,
                   "mdp_read_transition failed: %s\n",
                   "Unable to match double for transition probability");
	  exit (EXIT_FAILURE);
	}

	// Minimal error checking
	if (p_mdp->transitionProb[i][j][k] < 0)
	  fprintf (stderr,
                   "mdp_read_transition warning: %s\n",
                   "Negative transition probability");
	if (p_mdp->transitionProb[i][j][k] > 1)
	  fprintf (stderr,
                   "mdp_read_transition warning: %s\n",
                   "Transition probability exceeds 1");
      }

} // mdp_read_transitions


/*  Procedure
 *    mdp_read_available_actions
 *
 *  Purpose
 *    Read the number of available actions from a file and assign
 *    values to mdp struct
 *
 *  Parameters
 *   stream
 *   p_mdp
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    stream is a valid, open stream that may be read from
 *    The next line of data in stream is may be read as unsigned ints
 *
 *  Postconditions
 *    All values in p_mdp->numAvailableActions are assigned as read from stream
 *    Any failure causes program exit.
 */
void
mdp_read_available_actions (FILE * stream, mdp * p_mdp)
{
  unsigned int i;

  int count;

  for (i=0 ; i < p_mdp->numStates ; i++)
  {
    // Read/assign entry
    count = fscanf (stream, "%ud", &(p_mdp->numAvailableActions[i]) );

    // Check for errors
    if ( EOF == count )
    {
      if ( ferror (stream) )
	fprintf (stderr,
                 "mdp_read_available_actions failed: %s\n",
                 strerror (errno));
      else
	fprintf (stderr, 
                 "mdp_read_available_actions failed: %s\n",
                 "Premature end of file");
      exit (EXIT_FAILURE);
    }
    else if (count != 1)
    {
      fprintf (stderr,
               "mdp_read_available_actions failed: %s\n",
               "Unable to match unsigned int for available actions");
      exit (EXIT_FAILURE);
    }

    // Validate entry
    if (p_mdp->numAvailableActions[i] > p_mdp->numActions)
    {
      fprintf (stderr,
               "mdp_read_available_actions failed: %s\n",
               "Action index exceeds bound");
      exit (EXIT_FAILURE);
    }
  }
} // mdp_read_available_actions


////////////////////////////////////////////////////////////////////////////////
mdp *
mdp_duplicate ( mdp * p_mdp )
{
  unsigned int s,t; // Loop variables: states s and t

  // Allocate a new struct
  mdp * p_mdp_out = mdp_malloc ( p_mdp->numStates, p_mdp->numActions);

  // Copy simple data to output struct
  p_mdp_out->numStates = p_mdp->numStates;
  p_mdp_out->numActions = p_mdp->numActions;
  p_mdp_out->start = p_mdp->start;
  
  // Copy number of available actions to output struct
  memcpy ( p_mdp_out->numAvailableActions, 
           p_mdp->numAvailableActions, 
           sizeof(unsigned int) * p_mdp->numStates );

  // Copy transitions to output struct
  for ( s=0 ; s < p_mdp->numStates ; s++)
    for ( t=0 ; t < p_mdp->numStates ; t++)
      memcpy ( p_mdp_out->transitionProb[s][t],
               p_mdp->transitionProb[s][t],
               sizeof(unsigned int) *  p_mdp->numActions);

  // Allocate actions
  mdp_malloc_actions ( p_mdp_out );

  // Copy available actions to output struct
  for ( s=0 ; s < p_mdp->numStates ; s++)
    memcpy ( p_mdp_out->actions[s], 
             p_mdp->actions[s], 
             sizeof(unsigned int) * p_mdp->numAvailableActions[s] );
  
  // Copy rewards to output
  memcpy ( p_mdp_out->rewards,
           p_mdp->rewards,
           sizeof(double) * p_mdp->numStates );
  
  // Copy terminals to output
  memcpy ( p_mdp_out->terminal,
           p_mdp->terminal,
           sizeof(unsigned int) * p_mdp->numStates );
  
  return p_mdp_out;
} // mdp_duplicate


/*  Procedure
 *    mdp_read_actions
 *
 *  Purpose
 *    Read the available actions list from a file and 
 *    assign values to mdp struct
 *
 *  Parameters
 *   stream
 *   p_mdp
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    stream is a valid, open stream that may be read from
 *    The next lines of data in stream is may be read as unsigned integers
 *
 *  Postconditions
 *    All values in p_mdp->actions are assigned as read from stream
 *    Any failure causes program exit.
 */
void
mdp_read_actions ( FILE * stream, mdp * p_mdp)
{
  unsigned int i,j;
  
  int count;

  for (i=0 ; i < p_mdp->numStates ; i++)
    for (j=0 ; j < p_mdp->numAvailableActions[i] ; j++)
    {
      // Read/assign entry
      count = fscanf (stream, "%ud", &(p_mdp->actions[i][j]) );

      // Check for errors
      if ( EOF == count )
      {
	if ( ferror (stream) )
	  fprintf (stderr,
                   "mdp_read_actions failed: %s\n",
                   strerror (errno));
	else
	  fprintf (stderr, 
                   "mdp_read_actions failed: %s\n",
                   "Premature end of file");
	exit (EXIT_FAILURE);
      }
      else if (count != 1)
      {
	fprintf (stderr,
                 "mdp_read_actions failed: %s\n",
                 "Unable to match unsigned int for available action");
	exit (EXIT_FAILURE);
      }

      // Validate entry
      if (p_mdp->actions[i][j] >= p_mdp->numActions)
      {
	fprintf (stderr,
                 "mdp_read_actions failed: %s\n",
                 "Action index exceeds bound");
	exit (EXIT_FAILURE);
      }      
    }
} // mdp_read_actions


/*  Procedure
 *    mdp_read_rewards
 *
 *  Purpose
 *    Read the rewards from a file and assign values to mdp struct
 *
 *  Parameters
 *   stream
 *   p_mdp
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    stream is a valid, open stream that may be read from
 *    The next line of data in stream is may be read as doubles
 *
 *  Postconditions
 *    All values in p_mdp->rewards are assigned as read from stream
 *    Any failure causes program exit.
 */
void
mdp_read_rewards (FILE * stream, mdp * p_mdp)
{
  unsigned int i;
  int count;

  for (i=0 ; i < p_mdp->numStates ; i++)
  {
    // Read/assign entry
    count = fscanf (stream, "%lf", &(p_mdp->rewards[i]) );

    // Check for errors
    if ( EOF == count )
    {
      if ( ferror (stream) )
	fprintf (stderr,
                 "mdp_read_rewards failed: %s\n",
                 strerror (errno));
      else
	fprintf (stderr, 
                 "mdp_read_rewards failed: %s\n",
                 "Premature end of file");
	  exit (EXIT_FAILURE);
    }
    else if (count != 1)
    {
      fprintf (stderr,
               "mdp_read_rewards failed: %s\n",
               "Unable to match double for reward");
      exit (EXIT_FAILURE);
    }
  }
} // mdp_read_rewards


/*  Procedure
 *    mdp_read_terminal
 *
 *  Purpose
 *    Read the terminal states from a file and assign values to mdp struct
 *
 *  Parameters
 *   stream
 *   p_mdp
 *
 *  Produces,
 *   [Nothing.]
 *
 *  Preconditions
 *    stream is a valid, open stream that may be read from
 *    The next line of data in stream may be read as unsigned ints
 *
 *  Postconditions
 *    Values in p_mdp->terminal are assigned as read from stream
 *    Any failure causes program exit.
 */
void
mdp_read_terminal (FILE * stream, mdp * p_mdp)
{
  unsigned int i;

  int count;
  unsigned int state;

  for (i=0 ; i < p_mdp->numStates ; i++)
  {
    // Read/assign entry
    count = fscanf (stream, "%ud", &state );

    // Check for errors
    if ( EOF == count &&  ferror (stream) )
      fprintf (stderr,
               "mdp_read_terminal failed: %s\n",
               strerror (errno));
    else if (count != 1 && !feof (stream))
    {
      fprintf (stderr,
               "mdp_read_terminal failed: %s\n",
               "Unable to match unsigned int for terminal");
      exit (EXIT_FAILURE);
    }
    else if ( feof (stream) )
      // Reached end of file, so we're done!
      return;
    
    // Validate state value
    if (state >= p_mdp->numStates)
    {
      fprintf (stderr,
               "mdp_read_terminal failed: %s\n",
               "Terminal state index exceeds bound");
      exit (EXIT_FAILURE);
    }
    
    // Assign state value appropriately
    p_mdp->terminal[state] = 1;
  }
} // mdp_read_terminal


////////////////////////////////////////////////////////////////////////////////
mdp *
mdp_read (const char * fileName)
{

  mdp * p_mdp;
  int ret;
  unsigned int numStates, numActions; 

  FILE* stream = fopen (fileName, "r");   // Open the file for reading

  if ( NULL == stream )
  {
    fprintf (stderr, 
             "mdp_read(\"%s\") failed: %s\n",
             fileName,
	    strerror (errno));
    return NULL;
  }
  
  // Get initial data about MDP
  mdp_read_dimensions (stream, &numStates, &numActions);

  p_mdp = mdp_malloc (numStates, numActions);   // Allocate space for MDP
  p_mdp->numStates = numStates; // Assign dimension variables to struct
  p_mdp->numActions = numActions;

  mdp_read_start (stream, p_mdp);        // Read initial/starting state
  mdp_read_transitions (stream,  p_mdp); // Read transition probability matrix

  // Read number of available actions array
  mdp_read_available_actions (stream, p_mdp);
  mdp_malloc_actions (p_mdp);        // Allocate secondary actions array
  mdp_read_actions (stream, p_mdp);  // Read actions
  mdp_read_rewards (stream, p_mdp);  // Read rewards
  mdp_read_terminal (stream, p_mdp); // Read terminal states

  ret = fclose(stream);

  if ( 0 != ret )
    fprintf (stderr,
             "mdp_read(\"%s\") Error closing file: %s\n",
             fileName,
             strerror (errno));
  
  // All finished!
  return p_mdp;
} // mdp_read


////////////////////////////////////////////////////////////////////////////////
void
mdp_read_policy (FILE* stream, const mdp * p_mdp, unsigned int * policy)
{
  unsigned int s;
  int count;

  for (s=0 ; s < p_mdp->numStates ; s++)
  {
    // Read/assign entry
    count = fscanf (stream, "%ud", &(policy[s]) );
    
    // Check for errors
    if ( EOF == count )
    {
      if ( ferror (stream) )
	fprintf (stderr,
                 "mdp_read_policy failed: %s\n",
                 strerror (errno));
      else
	fprintf (stderr, 
                 "mdp_read_policy failed: %s\n",
                 "Premature end of file");
      exit (EXIT_FAILURE);
    }
    else if (count != 1)
    {
      fprintf (stderr,
               "mdp_read_policy failed: %s\n",
               "Unable to match unsigned int for policy");
      exit (EXIT_FAILURE);
    }
  }  
} // mdp_read_policy

////////////////////////////////////////////////////////////////////////////////
void
mdp_free (mdp * p_mdp)
{
  unsigned int i;

  //----------------------------------------
  // Transition probability
  mdp_free_transitions (p_mdp->numStates, p_mdp->transitionProb);

  //----------------------------------------
  // Number of available actions
  free (p_mdp->numAvailableActions);
  
  //----------------------------------------
  // Available actions
  for ( i=0 ; i < p_mdp->numStates ; i++)
    free (p_mdp->actions[i]);
  free (p_mdp->actions);

  //----------------------------------------
  // Rewards
  free (p_mdp->rewards);

  //----------------------------------------
  // Terminal states
  free(p_mdp->terminal);
  
  //----------------------------------------
  // Root structure
  free(p_mdp);
  
} // mdp_free
