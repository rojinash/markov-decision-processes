/*  Procedure
 *    setup
 *
 *  Purpose
 *    Perform prepatory setup for an RL environment
 *
 *  Parameters
 *   mdpfile
 *
 *  Produces
 *   [Nothing.]
 *
 *  Preconditions
 *    mdpfile is a null-terminated string (character array) that refers to a 
 *    readable file containing a valid MDP description
 *
 *  Postconditions
 *    Other methods, assuming their other preconditions have been
 *    meet, should succeed.
 */
void environment_setup(char * mdpfile);

/*  Procedure
 *    get_mdp
 *
 *  Purpose
 *    Retrieve an incomplete MDP struct (sans rewards and transitions data)
 *
 *  Parameters
 *   [None.]
 *
 *  Produces
 *   p_mdp
 *
 *  Preconditions
 *    setup has been run successfully.
 *
 *  Postconditions
 *    p_mdp is a pointer to a valid mdp struct with rewards and
 *    transitions arrays present, but zeroed out.
 *    A failure may cause program exit.
 *    No pointers to any part of p_mdp are shared
 */
mdp* environment_get_mdp();


/*  Procedure
 *    get_num_states
 *
 *  Purpose
 *    Retrieve the number of states for the MDP environment
 *
 *  Parameters
 *   [None.]
 *
 *  Produces
 *   numStates
 *
 *  Preconditions
 *    setup has been run successfully.
 *
 *  Postconditions
 *    numStates is the number of states for the current MDP environment
 */
unsigned int environment_get_num_states();


/*  Procedure
 *    get_num_actions
 *
 *  Purpose
 *    Retrieve the number of actions for the MDP environment
 *
 *  Parameters
 *   [None.]
 *
 *  Produces
 *   numActions
 *
 *  Preconditions
 *    setup has been run successfully.
 *
 *  Postconditions
 *    numActions is the number of actions for the current MDP environment
 */
unsigned int environment_get_num_actions();

/*  Procedure
 *    rl_agent_action
 *
 *  Purpose
 *    Update the agent and produce an action for the given state
 *
 *  Parameters
 *    state
 *    reward
 *
 *  Produces
 *    action
 *
 *  Preconditions
 *    setup has been run successfully
 *    state is a valid state index
 *
 *  Postconditions
 *    action is a valid reward index for the  given state
 */
unsigned int rl_agent_action(unsigned int state, double reward);


/*  Procedure
 *    environment_run
 *
 *  Purpose
 *    Run the agent in the environment for a specified number of trials
 *
 *  Parameters
 *    trials
 *
 *  Produces
 *    [Nothing.]
 *
 *  Preconditions
 *    setup has been run successfully
 *    Program has been linked with an object file that defines rl_agent_action
 *
 *  Postconditions
 *    environment_run_trial() has been called trials times in a
 *    simulation of the current environment
 */
void environment_run(const unsigned int trials);



/*  Procedure
 *    environment_run_trial
 *
 *  Purpose
 *    Run the agent in the environment until a terminal state is reached
 *
 *  Parameters
 *    [None.]
 *
 *  Produces
 *    [Nothing.]
 *
 *  Preconditions
 *    setup has been run successfully
 *    Program has been linked with an object file that defines rl_agent_action
 *
 *  Postconditions
 *    rl_agent_action(state,reward) is called until given an argument that is a
 *    terminal state
 *    
 */
void environment_run_trial();

