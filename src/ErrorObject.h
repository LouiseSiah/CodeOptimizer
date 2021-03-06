#ifndef ErrorObject_H
#define ErrorObject_H

typedef enum{
  ERR_NULL_ELEMENT,
  ERR_NULL_LIST,
  ERR_EMPTY_LIST,
  ERR_NULL_NODE,
  ERR_NULL_SUBSCRIPT,
  ERR_UNDECLARE_VARIABLE,
  ERR_INVALID_BRANCH,
  ERR_UNHANDLE_ERROR
} ErrorCode;


typedef struct{
  char *errorMsg;									//errorMsg give a message about the error.
  ErrorCode errorCode;						//ErrorCode is to show the type of error that throw in the function.
} ErrorObject;

void freeError(ErrorObject *errObj);
void ThrowError(ErrorCode errCode, char *message, ...);
#endif // ErrorObject_H

