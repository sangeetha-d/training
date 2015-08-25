/*
********************************************************************************
**  File:   even.c                                                            **
**  Author: Sangeetha                                                         **
**  Date:   27-7-2015                                                         **
**                                                                            **
********************************************************************************
**  COPYRIGHT: (C) 2015 - All rights reserved - Network Marvels (I) Pvt. Ltd.,**
**  Thane (India)                                                             **
**                                                                            **
********************************************************************************
**  Program: To find 'n' number of even number from the given 'k'             **
**  Input:  Value for k and n                                                 **
**                                                                            **
********************************************************************************
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       **
**  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT         **
**  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     **
**  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      **
**  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     **
**  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT          **
**  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     **
**  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     **
**  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       **
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     **
**  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      **
**                                                                            **
********************************************************************************
*/

/*******************************************************************************
 *                  HEADERS USED                                              */
#include <stdio.h>         
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>    //for INT_MIN and INT_MAX
#include <string.h>
#include <time.h>      // to obtain timestamp
/******************************************************************************/

/*******************************************************************************
 *                 Definations and Macros                                     */
#define UP 2   
/******************************************************************************/

/*******************************************************************************
 *                  Prototypes of Local Functions                             */
void FindNEven(int , int);
void GetInput();
bool ValidateInput(char*, char*);
void MaintainLogFile(char*, char*);
/******************************************************************************/

/*******************************************************************************
 *                 Global variables                                           */
static int n;              /* holds total even number required */
static int k;              /* holds starting number          */
static char* my_string1;
static char* my_string2;
static char* log_type;
static char* log_entry;
/******************************************************************************/

main(int argc, char* argv[])
{
    fflush(stdout);
    log_type  = (char*) malloc(8);
    log_entry = (char*) malloc(70);
    strcpy(log_type, "DEBUG");
    strcpy(log_entry,"Allocating Memory ");
    MaintainLogFile(log_type, log_entry);
    
    my_string1 = (char*) malloc(15);
    my_string2 = (char*) malloc(15);
   
    FILE *fptr;          // use header file stdio.h
    char  *filename;
    filename = (char*) malloc(15);

    if(argc == 3)
    {
        my_string1 = argv[1];
        my_string2 = argv[2];
        strcpy(log_type, "DEBUG");
        strcpy(log_entry,"Got Input as Arguments ");
        MaintainLogFile(log_type, log_entry);
    }
    else if(argc == 2)
    {
        if(strcmp(argv[1], "-f") == 0)
        {
            printf("Enter Filename:");
            scanf("%s", filename);
                
            fptr= fopen(filename,"r");
        
            if(fptr == NULL)
            {
                printf("cannot open file \n");
                strcpy(log_type, "ERROR");
                strcpy(log_entry,"Cannot Open File ");
                MaintainLogFile(log_type, log_entry);
                exit(0);
            }
            else
            {   
                char *my_string;
                my_string = (char*) malloc(40);

                while(fgets(my_string, 40, fptr) != NULL)
                {
                    sscanf(my_string, "%s %s\n", my_string1, my_string2);

                    strcpy(log_type, "DEBUG");
                    strcpy(log_entry,"Got Input From File ");
                    MaintainLogFile(log_type, log_entry);

                    if(ValidateInput(my_string1, my_string2))
                        FindNEven(k, n);
                }

                free(my_string);
                goto LOG;
            }
        }

        else if(strcmp(argv[1], "--help") == 0)
        { 
            filename = "evenhelp";
            fptr= fopen(filename,"r");
        
            char * helpcontent;
            helpcontent = (char*) malloc(100);
            
            if(fptr == NULL)
            {
                printf("cannot open file \n");
                strcpy(log_type, "ERROR");
                strcpy(log_entry,"Can't open help file ");
                MaintainLogFile(log_type, log_entry);
            }
            else
            {
                while(fgets(helpcontent, 100, fptr) != NULL)
                {
                    printf("%s",helpcontent);
                }
                printf("\n");
                strcpy(log_type, "DEBUG");
                strcpy(log_entry,"Help Printed ");
                MaintainLogFile(log_type, log_entry);
            }
          
           free(helpcontent);
            goto LOG;           
        }
        else
        {   
            printf(" command not found!\n ");
            strcpy(log_type, "ERROR");
            strcpy(log_entry,"Invalid File Command ");
            MaintainLogFile(log_type, log_entry);
            exit(0);
        }
        free(filename);
    fclose(fptr);      
    }    
    else 
        GetInput();
    
    if(ValidateInput(my_string1, my_string2)) 
        FindNEven(k, n);
    else{
//       printf("exit");
        exit(0);
    }
        
    LOG:
        strcpy(log_type, "LOG ");
        strcpy(log_entry,"Transaction Complete");
        MaintainLogFile(log_type, log_entry);  
        fflush(stdout);
  //      printf("end\n");    
}   

/******************************************************************************/
/**                 Local functions                                          **/

/*******************************************************************************
 *    Function Name: GetInput                                                 
 *    Arguments:     0                                                           
 *    Returns:       void
 *    Description:   get values for given mumber k  and count n
 */

void GetInput()
{
    printf("Enter starting number:\n");
    scanf("%s", my_string1);
    printf("Enter number of even number required:\n");
    scanf("%s", my_string2);
    strcpy(log_type, "DEBUG");
    strcpy(log_entry,"Got Input from STDIN ");
    MaintainLogFile(log_type, log_entry);
}  
/*******************************************************************************/

/********************************************************************************
 *    Function Name: ValidateInput                                                   
 *    Arguments:     2                                                           
 *    Argument      Type      I/O       Description
 *  -----------   --------   ------    ----------------
 *   my_string1    *char     input      Starting number
 *   my_string2    *char     input        Total  count
 *
 *    Returns:       true if Input are Valid otherwise false
 *    Description:   Find the even number from the given mumber k of count n
 */

bool ValidateInput(char* my_string1,char* my_string2)
{
    bool Validatesuccess_flag = true;
    char *error_msg = (char*) malloc(70);
    strcpy(error_msg, "no error");

    if(sscanf(my_string1, "%d", &k) == 0)
        strcpy(error_msg, "starting number is invalid!\n");
    else if(k <= INT_MIN || INT_MAX <= k)
        strcpy(error_msg, "starting number is out of range\n");  
    
    else if(sscanf(my_string2, "%d", &n) == 0)
        strcpy(error_msg, "Entered count is invalid!\n");
    else if(n <= INT_MIN || INT_MAX <= n )
        strcpy(error_msg, "count is out of range\n");
    else if( n <= 0 ) 
        strcpy(error_msg, "count should be greater than zero\n");

    else if( (k+(n*2)) < k)  
           strcpy(error_msg, "For given Starting Number and count, output becomes out of range\n");
    
    if(strcmp(error_msg, "no error") == 0 )
    {
        strcpy(log_type, "DEBUG");
        strcpy(log_entry, "Validation successful");
    }
    else
    {
        strcpy(log_type, "ERROR");
        strcpy(log_entry,error_msg);
        printf("%s\n", error_msg);
        Validatesuccess_flag = false;
    }
    MaintainLogFile(log_type, log_entry);
    free(error_msg);
    
    return Validatesuccess_flag;
}

/********************************************************************************
 *    Function Name: FindNEven                                                   
 *    Arguments:     2                                                           
 *    Argument      Type      I/O       Description
 *  -----------   --------   ------    ----------------
 *      k         integer    input      Starting number
 *      n         integer    input        Total  count
 *
 *    Returns:       void
 *    Description:   Find the even number from the given mumber k of count n
 */

void FindNEven(int k,int n)
{
    /* checks whether number is even or odd
     * if even then increment by 2
     * if odd then increment by 1
     */
    int i=1;
    printf("First %d even numbers from %d:\n", n, k);
    if ((k%2) != 0)    
    {
        k++;             
    } 
    while (i<=n)
    {
          printf("%d\t", k);
          k=k+UP;
          i++;
    }
    printf("\n"); 
    strcpy(log_type, "DEBUG");
    strcpy(log_entry,"OUTPUT PRINTED ");
    MaintainLogFile(log_type, log_entry);
}

/*******************************************************************************/

/********************************************************************************
 *    Function Name: MaintainLogFile                                                   
 *    Arguments:     2                                                           
 *    Argument      Type      I/O       Description
 *  -----------   --------   ------    ----------------
 *     log_type    *char     input       type of log 
 *     log_entry   *char     input       type of log entry
 *    Returns:       void
 *    Description:   Maintains log file
 */

void MaintainLogFile(char* log_type, char* log_entry)
{
    static int log_id = 0;
    int j;

    time_t raw_time;
    struct tm *ptr_ts;
    time ( &raw_time );
    ptr_ts = localtime( &raw_time );

    FILE *fptr1;
    fptr1 = fopen("even_log_file", "a+");

    if(fptr1 == NULL)
        printf("log file does not exists");
    else
    {
        log_id++;
        fprintf(fptr1, "  %02d  \t"
                "%02d:%02d:%02d %02d/%02d/%d\t\t"
                "%6s\t\t"
                " %s\n", 
                log_id,
                (5 + ptr_ts->tm_hour)%24, (30+ ptr_ts->tm_min)%60,
                    ptr_ts->tm_sec, ptr_ts->tm_mday,
                    (1+ptr_ts->tm_mon ), (1900 + ptr_ts->tm_year),
                log_type,
                log_entry);
        if( (strcmp(log_type, "LOG ") == 0)
             || (strcmp(log_type, "ERROR") == 0) )
            for(j=0;j<80;j++)
                fprintf(fptr1, "~");
       fprintf(fptr1, "\n");
    }
/*    printf ("Time: %02d:%02d:%02d %02d/%02d/%02d", 
             (5 + ptr_ts->tm_hour)%24,
             (30+ ptr_ts->tm_min)%60,
             ptr_ts->tm_sec, 
             ptr_ts->tm_mday,
             1+ptr_ts->tm_mon,
             1900 + ptr_ts->tm_year
            );
*/
  //  printf("%s",  asctime( localtime(&timestamp)) );
   fclose(fptr1);
}

/*******************************************************************************/
/*******************************************************************************/
