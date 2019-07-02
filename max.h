/*
 * File
 *   max.h
 *
 * Summary 
 *   A collection of procedures for finding the largest value in an
 *   array or the index of the largest value in an array over a
 *   restricted set of indices.
 *
 * Author
 *   Jerod Weinman
 */
#ifndef __MAX_H__
#define __MAX_H__

/*  Procedure
 *    max_value
 *
 *  Purpose
 *    Find the largest value in an array, using a restricted set of indices
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
		 const double * values);


/*  Procedure
 *    arg_max_value
 *
 *  Purpose
 *    Find the index of a largest value in an array, using a restricted index set
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
			   const double * values);

#endif // __MAX_H__
