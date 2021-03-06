#ifndef ExpressionBlock_H
#define ExpressionBlock_H

#include "LinkedList.h"
#include "ErrorObject.h"
typedef struct Expression_t Expression;
typedef struct Subscript_t Subscript;
#include "GetList.h"
#include "NodeChain.h"

#define CHANGE_ID_SUBSCRIPT(exprPtr, checkPtr, currentRank)                   \
                while(&(((Expression*)exprPtr->node)->id) != checkPtr->node){ \
                                                                              \
                  if(((Expression*)exprPtr->node)->id.name ==                 \
                      ((Subscript*)checkPtr->node)->name &&                   \
                    currentRank <= ((Subscript*)checkPtr->node)->index){      \
                    currentRank = ((Subscript*)checkPtr->node)->index + 1;    \
                  }                                                           \
                  checkPtr = checkPtr->next;                                  \
                }                                                             \
                ((Expression*)exprPtr->node)->id.index = currentRank;

                
#define CHANGE_OPERAND_A(exprPtr, checkPtr, oprARank)                           \
                while(&((Expression*)exprPtr->node)->oprdA != checkPtr->node){  \
                  if(((Expression*)exprPtr->node)->oprdA.name ==                \
                    ((Subscript*)checkPtr->node)->name &&                       \
                    oprARank < ((Subscript*)checkPtr->node)->index){            \
                    oprARank = ((Subscript*)checkPtr->node)->index;             \
                  }                                                             \
                  checkPtr = checkPtr->next;                                    \
                }                                                               \
                ((Expression*)exprPtr->node)->oprdA.index = oprARank;

                
#define CHANGE_OPERAND_B(exprPtr, checkPtr, oprBRank)                           \
                while(&((Expression*)exprPtr->node)->oprdB != checkPtr->node){  \
                  if(((Expression*)exprPtr->node)->oprdB.name ==                \
                    ((Subscript*)checkPtr->node)->name &&                       \
                    oprBRank < ((Subscript*)checkPtr->node)->index){            \
                    oprBRank = ((Subscript*)checkPtr->node)->index;             \
                  }                                                             \
                  checkPtr = checkPtr->next;                                    \
                }                                                               \
                ((Expression*)exprPtr->node)->oprdB.index = oprBRank;
                
                
                
typedef enum{
  PHI_FUNC,
  GREATER_THAN,
  SMALLER_THAN,
  EQUAL_TO,
  NOT_EQUAL_TO,
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  ASSIGN,
  COPY,
  IF_STATEMENT
}Operator;

struct Subscript_t{
  int name;
  int index;
};

struct Expression_t{
  Subscript id;
  Operator opr;
  Subscript oprdA;
  Subscript oprdB;
  Subscript condt;
};


Expression* createExpression(int thisID, Operator oprt,        \
                            int oprdA, int oprdB, int condt);

void arrangeSSA(Node* inputNode);
void assignAllNodeSSA(Node* inputNode, LinkedList* liveList, LinkedList* prevList);

#endif // ExpressionBlock_H
