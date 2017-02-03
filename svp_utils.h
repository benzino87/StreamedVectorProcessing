/*
 *
 * Author: Jason Bensel
 * Version: CIS452 - Project 2 - 2/3/2017
 * Description: Method declarations for handling bits at each phase of the pipe
 *
 *
 */

#ifndef SVP_UTILS_H_
#define SVP_UTILS_H_

/*
 *
 * Complimenter reads from a file containing the vector B, one binary number at
 * a time.  It should then perform the bitwise complement on each binary number
 * b(subi)that it reads, and send each number through the pipe to process P1
 * (again, one number at a time).
 * Operation:  read | complement | send.  Repeat.
 *
 * param: filename - name of file containing vector B
 *
 * returns: compliment of incoming bit
 *
 */
 int complimenter(int *bit);

/*
 *
 * Incrementer reads values in from its input pipe (one number at a time).  It
 * should then add 1 to each individual binary number to form its
 * Two's-complement, and then send each number through the pipe to process P2.
 * Operation:  read | increment | send.  Repeat.
 *
 * param: bit - current complimented bit in vector
 *
 * returns: Two's compliment of bit
 *
 */
 int incrementer(int *bit);

/*
 *
 * Adder reads values in from its pipe, and it reads from a file
 * containing the vector A (both reads are for one number at a time)
 * It should then add corresponding values from the two streams, effectively
 * performing a(subi)-b(subi), etc.  It should then write the results to a file
 * (one number at a time).
 * Operation:  read/read | add | write
 *
 * param: bit - current Two's compliment bit (Vector B)
 *
 * param: filename - File containing vector A
 *
 * returns: A(subi) - B(subi)
 *
 */
 int adder(int *bit, char *filename);

#endif
