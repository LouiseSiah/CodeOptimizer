#include "ExpressionBlock.h"
#include <stdlib.h>
#include <stdio.h>


Expression* createExpression(int thisID, Operator oprt, int oprdA,\
                            int oprdB, int condt){
  Expression* newExp = malloc(sizeof(Expression));
  
  newExp->id.name    = thisID;
  newExp->id.subs    = 0;
  newExp->opr        = oprt;
  newExp->oprdA.name = oprdA;
  newExp->oprdA.subs = 0;
  newExp->oprdB.name = oprdB;
  newExp->oprdB.subs = 0;
  newExp->condition  = condt;

  return newExp;            
}


/*
 *  getSubsList is a simple function that extract all the
 *  variable in all the expressions in the Node into a LinkedList.
 *
 *  The function take the input LinkedList* expression that contain
 *  all the expression in the Node.
 */
LinkedList* getSubsList(LinkedList* expression){
  LinkedList* subsList = createLinkedList();
  ListElement* exprPtr = expression->head;
  
  while(exprPtr != NULL){
    if(((Expression*)exprPtr->node)->opr != IF_STATEMENT){
      if(((Expression*)exprPtr->node)->opr != ASSIGN){
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdA);
        addListLast(subsList, &((Expression*)exprPtr->node)->oprdB); 
      }
      addListLast(subsList, &((Expression*)exprPtr->node)->id);
    }
    exprPtr = exprPtr->next;
  }
  
  return subsList;
}

/*
 *  getLargestIndex
 *  
 *  find the largest number of the input argument subscript
 *  in the exprList
 *
 */
int getLargestIndex(LinkedList* exprList, Subscript* subsName){
  ListElement *checkPtr, *resultPtr;
  
  checkPtr  = exprList->head;
  while(checkPtr != NULL){
    if(((Subscript*)checkPtr->node)->name == subsName->name)
        resultPtr = checkPtr;
    checkPtr = checkPtr->next;
  }
  
  if(resultPtr == NULL)
    return 0;
  
  return ((Expression*)resultPtr->node)->id.subs;
}

/*********************************************************************
 *  arrangeSSA take in the inputNode and arrange all the expression
 *  in the Node to the correct subscript.
 *  Eg.
 *    x0 = x0 + x0
 *  
 *  arrangeSSA should arrange the equation above to
 *  x1 = x0 + x0
 *
 ********************************************************************/
void arrangeSSA(Node* inputNode){
  LinkedList* exprList  = inputNode->block;
  LinkedList* checkList = getSubsList(inputNode->block);
  ListElement* exprPtr  = exprList->head;
  ListElement* checkPtr;
  
  int currentRank, oprARank, oprBRank;
  
  while(exprPtr != NULL){
    currentRank = 0;
    oprARank = 0;
    oprBRank = 0;
    
    if(((Expression*)exprPtr->node)->opr != IF_STATEMENT){
      checkPtr = checkList->head;
      CHANGE_ID_SUBSCRIPT(exprPtr, checkPtr, currentRank);  
                
      if(((Expression*)exprPtr->node)->opr != ASSIGN){
        checkPtr = checkList->head;
        CHANGE_OPERAND_A(exprPtr, checkPtr, oprARank);

        checkPtr = checkList->head;
        CHANGE_OPERAND_B(exprPtr, checkPtr, oprBRank);
      }
    }
    exprPtr = exprPtr->next;
  }
}


LinkedList* getLiveList(Node* inputNode, LinkedList* prevLiveList){
  LinkedList* newLiveList = createLinkedList();
  LinkedList* checkList   = createLinkedList();
  LinkedList* expList     = inputNode->block;
  
  ListElement *newPtr, *exprPtr, *checkPtr;
  int validFlag;
  exprPtr   = expList->head;
  
  /***********************************************************
   * Form newLiveList with the living variable in this block
   ***********************************************************/
  while(exprPtr != NULL){
    checkPtr = newLiveList->head;
    while(checkPtr != NULL && ((Subscript*)checkPtr->node)->name \
          != ((Expression*)exprPtr->node)->id.name){
      checkPtr = checkPtr->next;
    }
    if(checkPtr != NULL)
      exprPtr = exprPtr->next;
    else
      addListLast(newLiveList, &((Expression*)exprPtr->node)->id);
  }
  /******************************************************
   *  Find the correct subscript index and assign to it
   ******************************************************/
  newPtr = newLiveList->head;
  while(newPtr != NULL){
    ((Subscript*)newPtr->node)->subs = getLargestIndex(expList, (Subscript*)newPtr->node);
    newPtr = newPtr->next;
  }
  
  //CASCADE PREV & NEW
  prevLiveList = newLiveList;
  return prevLiveList;
}

void assignAllNodeSSA(Node* inputNode, LinkedList* liveList){
  inputNode->visitFlag = 1;
  ListElement* livePtr = liveList->head;
  int subsName, i;
  Expression* newExpr;
  
  while(livePtr != NULL){
    subsName  = ((Subscript*)livePtr->node)->name;
    newExpr   = createExpression(subsName, ASSIGN, subsName, 0, 0);
    
    if(inputNode->imdDom == inputNode->parent)
      newExpr->id.subs = newExpr->oprdA.subs + 1;
    else
      newExpr->id.subs = newExpr->oprdA.subs + 2;

    addListFirst(inputNode->block, newExpr);
    livePtr   = livePtr->next;
  }
  
  arrangeSSA(inputNode);
  LinkedList* myLiveList = getLiveList(inputNode, liveList);
  
  for(i=0; i < inputNode->numOfChild; i++){
    if(inputNode->children[i]->visitFlag != 1)
      assignAllNodeSSA(inputNode->children[i], myLiveList);
  }
}






