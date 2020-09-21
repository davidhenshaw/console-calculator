#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cctype>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

double solve(char str[101] , char *strEnd);   //Accepts a string that contains an arithmetic
                            //expression separated by only symbols and no spaces
                            //then recursively simplifies it to one number

int main()
{
    char expression[101];
//    double before;
//    double after;
    cout << "**********Enter an arithmetic expression**********\n \t(Must be less than 100 characters)" << endl;

    while(true)
    {
        cin >> expression;

        long double answer = solve(expression, expression + strlen(expression) -1);

        cout << " = " << answer << "\n" << endl;
    }



    return 0;
}

//Checks if the parentheses at leftPtr and rightPtr are matching
bool isOuterParentheses(char *leftPtr, char *rightPtr)
{

    if(leftPtr > rightPtr)
        return false;

    while( leftPtr < rightPtr && rightPtr > leftPtr )
    {
        leftPtr++;

        if(*leftPtr == '\)' && leftPtr != rightPtr)
            return false;

        if(*leftPtr == '\(' /*&& &*rightPtr != &*leftPtr*/)
            return true;
    }

    return true;
}

//Returns true if there is any matching set of parentheses around chr
//Counts the number of open and closed parentheses on each side of chr
//  if they do not match on each side, return true

bool isEmbedded(char* chr, char* leftBound, char* rightBound)
{
    int opL = 0;
    int clL = 0;
    int opR = 0;
    int clR = 0;
    char* leftCheck = chr;
    char* rightCheck = chr;

    while(leftCheck > leftBound-1 || rightCheck < rightBound+1)
    {
        //Stop moving left pointer if it reaches the leftBound
        if(*leftCheck == '\(' && leftCheck > leftBound -1)
        {
            opL++;
        }else
        if(*leftCheck == '\)' && leftCheck > leftBound-1)
        {
            clL++;
        }
        //Stop moving right pointer if it reaches the rightBound
        if(*rightCheck == '\(' && rightCheck < rightBound+1)
        {
            opR++;
        }else
        if(*rightCheck == '\)' && rightCheck < rightBound+1)
        {
            clR++;
        }

        if(leftCheck >= leftBound)
            leftCheck--;

        if(rightCheck <= rightBound)
            rightCheck++;

        //If there is ANY matching set of parentheses around
        //the character, return true
//        if(*leftCheck == '\(' && *rightCheck == '\)')
//        {
//            if(isOuterParentheses(leftCheck, rightCheck))
//                return true;
//        }

    }

    if(opL == clL && opR == clR)
    {
        return false;
    }

    return true;
}

char *findOperation(char op1, char op2, char *leftPtr, char *rightPtr)
{
    char *ret = rightPtr;

    while(ret > leftPtr)
    {
        if(*ret == op1 || *ret == op2)
        {
            if(!isEmbedded(ret, leftPtr, rightPtr))
            {
                return ret;
            }
        }

        ret--;
    }

    return ret;
}

char *findOperation(char op1, char *leftPtr, char *rightPtr)
{
    char *ret = rightPtr;

    while(ret > leftPtr)
    {
        if(*ret == op1)
        {
            if(!isEmbedded(ret, leftPtr, rightPtr))
            {
                return ret;
            }
        }

        ret--;
    }

    return ret;
}

double solve(char exp[101], char *expEnd)
{
    /*This function does order of operations "backward" so that
    recursion will flip the order*/
    double ret;
    int expLength = strlen(exp)- (strlen(expEnd)-1);
    bool isPar;                 //is there a parenthesis present?
    bool isAdd;                 //is there addition or subtraction present?
    bool isMult;                //is there multiplication or division present?
    bool isPwr;                 //is there an exponent (as not to be confused with expression [exp])?
    char *ptrL;                 //Points to outermost left parenthesis
    char *ptrR;                 //Points to outermost right parenthesis
    char *ptrAdd;                //Points to rightmost addition or subtraction symbol NOT IN PARENTHESES
    char *ptrMult;               //Points to rightmost multiplication or division symbol NOT IN PARENTHESES
    char *ptrPwr;                //Points to rightmost exponent symbol (^)
    char *ptrFac;               //Points to rightmost factorial symbol (!)
    ptrL = exp;                 //Put the first pointer at the beginning
                                // of the string
    ptrR = expEnd;
    //ptrR += expLength - 1;    //Put the second pointer at the last letter

    //Check to see if expression is only one number

    /*Base case 1: the input string has only one number and it is an integer, return it*/
    while(isdigit(*ptrL) || *ptrL == '.'|| *ptrL == ' ')   //Check that each char is a digit; stop when you find a non-digit
    {
        ptrL +=1;
    }

    if( ptrL >= expEnd )     //if you stopped the end of the expression
    {
        return atof(exp);//return the expression
    }
    else                    //If you stopped somewhere in the middle
    {
        ptrL = exp;         //Go to beginning and prepare to count parentheses
    }

    /*parse functions*/


    /*
    Start addition/subtraction
    */


    ptrAdd = findOperation('+', '-', exp, expEnd); //Find the rightmost addition/subtraction sign

    if(*ptrAdd == '+')
    {
        return solve(exp, ptrAdd -1 ) + solve( ptrAdd+1, expEnd);
    }else
    if(*ptrAdd == '-')
    {
       return solve(exp, ptrAdd -1 ) - solve( ptrAdd+1, expEnd);
    }else{
        //cout << "did not find addition symbol" << endl;
    }

    /*Begin parsing multiplication and division signs*/

    ptrMult = findOperation('*', '/', exp, expEnd);

    if(*ptrMult == '*')
    {
        return solve(exp, ptrMult - 1)*solve(ptrMult+1, expEnd);
    }
    else
    if(*ptrMult == '/')
    {
        return solve(exp, ptrMult - 1)/(solve(ptrMult+1, expEnd));
    }

    /*Begin parsing exponent signs*/

    ptrPwr = findOperation('^', exp, expEnd);

    if(*ptrPwr == '^')
    {
        return pow(solve(exp, ptrPwr-1), solve(ptrPwr+1, expEnd));
    }


    /*Begin parsing parentheses*/
            //Find the last open parenthesis from the right (i.e. the first one from the left)

        ptrL = expEnd;
        ptrR = exp;
        char *oppPtr = exp;   //attempts to point to first open parenthesis
        char *clpPtr = expEnd;   //attempts to point to the last closed parenthesis

    for(int i = 0; i < expLength; i++) //Until ptrL and ptrR cross the boundary of the expression
    {
        if(*ptrR == '\)')
        {
            //cout << "Open parenthesis at " << expLength - i -1 <<endl;
            clpPtr = ptrR;
        }

        if(*ptrL == '\(')
        {
            //cout <<"Closed parenthesis at " << i << endl;
            oppPtr = ptrL;
        }

            ptrL-=1;
            ptrR+=1;

            //Only executes on the last loop
            //Corrects the switch in naming convention (swaps left and right)
            if(i == expLength -1)
            {
                ptrL = oppPtr;
                ptrR = clpPtr;
            }
    }

//    if(/*ptrL is to the right of ptrR*/0)
//    {
//        /*INVALID EXPRESSION*/
//    }

    for(int timeout =0; ptrR > ptrL && timeout < expLength; timeout++) /*move right pointer to left until leftmost closed parenthesis is found*/
                                        /*Or until you've looped enough times to time out*/
    {
        if(*ptrL == '\(')
        {
            if(isOuterParentheses(ptrL, ptrR))
            {
                break;
            }
        }
        ptrL++;
    }

    //If after all the checks, ptrR and ptrL are still pointing to parentheses
    //and still at the ends of the expression, recur on the inside of the parentheses
    if( *ptrL == '\(' && *ptrR == '\)' && ptrR == expEnd && ptrL == exp)
    {
        isPar = true;
        return solve(ptrL+1,ptrR-1);
    }

    return 0;
}
