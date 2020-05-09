#ifndef USER_PROCESS_OL70 /*header guard*/
#define USER_PROCESS_OL70
/*******************************************************************************
* The functions in this file handle a watch dog flow
*
* Written by: OL-70
* Last update: 24.09.2019
*******************************************************************************/

#define INTERVAL (6)
#define ATTEMPTS (2)
/*******************************************************************************
*
*******************************************************************************/
void *MMI(size_t intervals, size_t attempts, int argc, char *argv[]);

void DNR(void *param);

#endif  /*USER_PROCESS_OL70*/
