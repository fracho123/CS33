/* 
 * CS:APP Data Lab 
 * 
 * <Franklin Choi	405-401-024>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 *   Examples: bitParity(5) = 0, bitParity(7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int bitParity(int x) {
    /*compare halves. no matter what order the digits in the binary number is, you'll always have the same number of zeroes and ones.
    We can therefore compare the 1s and 0s from each half*/
    int half;
    half = x >> 16;
    x = x ^ half;
    half = x >> 8;
    x = x ^ half;
    half = x >> 4;
    x = x ^ half;
    half = x >> 2;
    x = x ^ half;
    half = x >> 1;
    return (x ^ half) & 1;
}
/* 
 * rotateRight - Rotate x to the right by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateRight(0x87654321,4) = 0x18765432
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateRight(int x, int n) {
    /*create a mask(clearend) to make the back n digits 0 after the the arithmetic shift right.
    extract the beginning n digits from the front of x and place them onto the 0 with or.*/
    int clearend, begin, neg1,negn;
    neg1 = ~0;
    negn = ~n+1;
    clearend = ~(neg1 << (32 + negn)); //but when n = 0, this becomes 0
    begin = x << (32 + negn);
    return ((x >> n) & clearend) | begin;
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    /*extract the bytes and then reshift them into the swapped places. Clear the bytes on x with byte
    gaps of 0s and then or the nth and mth bytes into place*/
    int nBits, mBits, nth, mth, nInverse, mInverse, withBlanks;
    nBits = n << 3;
    mBits = m << 3;
    nth = (x >> nBits) & 255;
    mth = (x >> mBits) & 255;
    nth = nth << mBits;
    mth = mth << nBits;
    nInverse = ~(255 << nBits);
    mInverse = ~(255 << mBits);
    withBlanks = (x & nInverse) & mInverse;
    return (withBlanks | nth) | mth;
}
/* 
 * fitsShort - return 1 if x can be represented as a 
 *   16-bit, two's complement integer.
 *   Examples: fitsShort(33000) = 0, fitsShort(-32768) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int fitsShort(int x) {
    /*short covers -32768 to 32767. Logically the same as fits bits. Get the signs and convert negative to positive.
    if it can do the positive form, it can do the negative one too*/
    int sign, pos, neg; 
    sign= x >> 31;
    pos = ~(sign) & x; //will result in x if positive and 0 if negative
    neg = sign & ~x; //will result in -(x+1) if negative and 0 if postive
    return !((pos + neg) >> 15);
}
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
    /*use or on the complements of x and y to isolate the places where they coincide with 0s.*/
    int both;
    both = ~x | ~y;
    return ~both;
}
/* 
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
    /*can only overflow when they are different signs and when they overflow, the product will have the same sign as y*/
    int negX, negY, negDiff, differentSigns, sameAsY;
    //all 1 when negative, all 0 when >=0
    negDiff = (x + (~y + 1)) >> 31;
    negX = x >> 31;
    negY = y >> 31;
    differentSigns = negX ^ negY; //all 1s if different, all 0s if same
    sameAsY = ~(negDiff ^ negY); //all 1s if same, all 0s if different
    return !(differentSigns & sameAsY);
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
    /*we look for special cases when false: when y is clearly greater than x (positive and negative respectively)
    or when they have the same sign but y is still greater. If neither of these are the case, we can say x is greater.*/
    int negX, negY, XnegYpos, positiveDiff, differentSigns, sameSignPosDiff;
    negX = x >> 31; //all 1s if negative
    negY = y >> 31; //all 0s if positive
    XnegYpos = negX & !negY; //look for when y > x
    positiveDiff = (x + ~y) >> 31; //all 0 if x>y, all 1 if x<=y
    differentSigns = !(negX ^ negY); //all 0 if different signs
    sameSignPosDiff = differentSigns & positiveDiff; // all 1s if x<=y and they have same sign.
    return !(XnegYpos | sameSignPosDiff); 
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    /*make the integer positive so that a right shift of n-1 will result in all 0s, since the highest significance bit could be 1 if negative*/
    int pos, neg, negOne;
    pos = ~(x >> 31) & x; //will result in x if positive and 0 if negative
    neg = (x >> 31) & ~x; //will result in -(x+1) if negative and 0 if postive
    negOne = ~0;
    return !((pos + neg) >> (n+negOne));
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    /*exploit property where complements' absolute values are 1 off from each other*/
    return ~x + 1;
}
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    /*exploit overflow to calculate 0 from the tmax and return it with the logical operator !
    Also, this calculation will also lead -1 to 0, so we need to check if it's -1*/
    int notx;
    notx = x + 1;
    x = x + notx + 1;
    return (!x & !!(notx));
}
