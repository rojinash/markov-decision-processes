/*
 * File
 *   max.c
 *
 * Summary 
 *   A collection of functions for finding the largest value in an
 *   array or the index of the largest value in an array over a
 *   restricted set of indices.
 *
 * Author
 *   Jerod Weinman
 */

#include "max.h"
#include <assert.h>

/*  Procedure
 *    max_value
 *
 *  Purpose
 *    Find a largest value in an array, using a restricted set of indices
 *
 *  Parameters
 *   len
 *   indices
 *   values
 *
 *  Produces,
 *   largest
 *
 *  Preconditions
 *    len > 0
 *    indices refers to a valid unsigned int array of length len
 *    values refers to a valid double array
 *    all entries of indices are valid indices for values, that is
 *      values[indices[i]] is a valid reference for 0<=i<len
 *
 *  Postconditions
 *    There exists a j such that 0<=i<len and largest = values[indices[j]]
 *    There does not exist a k such that 0<=k<len and k!=j such that 
 *      values[indices[k]] > largest
 */
double max_value(unsigned int len, const unsigned int* indices, 
		 const double * values)
{
  // Return the value of the argmax
  return values[arg_max_value(len, indices, values)];
}

////////////////////////////////////////////////////////////////////////////////

/*  Procedure
 *    arg_max_value
 *
 *  Purpose
 *    Find index of a largest value in an array, using a restricted index set
 *
 *  Parameters
 *   len
 *   indices
 *   values
 *
 *  Produces,
 *   index
 *
 *  Preconditions
 *    len > 0
 *    indices refers to a valid unsigned int array of length len
 *    values refers to a valid double array
 *    all entries of indices are valid indices for values, that is
 *      values[indices[i]] is a valid reference for 0<=i<len
 *
 *  Postconditions
 *    There exists a j such that 0<=i<len and index = indices[j]
 *    There does not exist a k such that 0<=k<len and k!=j such that 
 *      values[indices[k]] > values[index]
 */
unsigned int arg_max_value(unsigned int len, const unsigned int* indices, 
			   const double * values)
{
  double max;
  unsigned int arg;

  assert (len>0);

  max = values[indices[0]]; // Start with first value as max
  arg = indices[0];

  unsigned int i; // Loop variable
  for ( i=1 ; i<len ; i++)  // Check the rest
    if  (max < values[indices[i]])   // if value less than current max
    {
      max = values[indices[i]];      // re-assign max to value
      arg = indices[i];
    }
  return arg;
}
