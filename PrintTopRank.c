/*
********************************************************************************
**  File:   PrintTopRank.c                                                    **
**  Author: Sangeetha                                                         **
**  Date:   04-08-2015                                                        **
**                                                                            **
********************************************************************************
**  COPYRIGHT: (C) 2015 - All rights reserved - Network Marvels (I) Pvt. Ltd.,**
**  Thane (India)                                                             **
**                                                                            **
********************************************************************************
**  Program:To Find the given number of toppers from the given student details**
**  Input: numbers of toppers record needed and student details               **
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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/******************************************************************************/

/*******************************************************************************
 *                 Definitions and Macros                                     */
#define MAX 20
#define MAX_MARK 600
#define HELPFILE "/home/sangeetha/cprg/rankhelp"
#define LOGFILE "/home/sangeetha/cprg/rank_log_file"
#define DEBUG 0
#define LOG 1
#define ERROR 2
/******************************************************************************/

/*******************************************************************************
 *                 Structures and Typedefs                                    */
struct strdata
{
    struct strdata *prev;
    char name[MAX];
    float marks;
    struct strdata *next;
};
typedef struct strdata* NODE_PTR;
/******************************************************************************/

/*******************************************************************************
 *                  Prototypes of Local Functions                             */
bool ReadStoreData(char*);
bool ValidateStData(char*);
int QuickSortMarks(struct strdata*, struct strdata*);
struct strdata *FindPivot(NODE_PTR*, NODE_PTR*);
void ExchangeData( struct strdata*, struct strdata*);
void PrintData( int);
void MaintainLogFile(int, char*);
/******************************************************************************/

/*******************************************************************************
 *                 Global variables                                           */
static struct strdata *curr_node, *first_node, *last_node;
static int top, size;
static char *log_entry;
/******************************************************************************/

int main(int argc, char* argv[])
{
    char *filename;
    filename = (char*) malloc(20);
    log_entry = (char*) malloc(100);

    MaintainLogFile(DEBUG, "Allocating memory");

    if(argc == 3)
    {    //when input is passed through command line
        MaintainLogFile(DEBUG, "Get Input through command Line");
        filename = argv[1];
        if(sscanf(argv[2], "%d", &top) != 1)
        {
            printf("no. of topper is invalid!!\n");
            MaintainLogFile(ERROR, "no. of topper is invalid!!");
            exit(0);
        }
    }
    else if(argc == 2)
    {   
        FILE* fptr;
        if(strcmp(argv[1], "--help") == 0)
        {
            MaintainLogFile(DEBUG, "Opening Help File");
            char* helpcontent;
            helpcontent = (char*) malloc(80);

            fptr= fopen(HELPFILE, "r");
        
            if(fptr == NULL)
            {
                printf("cannot open file \n");
                MaintainLogFile(ERROR, "Cannot Open Help File");
                exit(0);
            }
            else
            {
                while(fgets(helpcontent, 80, fptr) != NULL)
                {
                    printf("%s",helpcontent);
                }
                printf("\n");
            }
            free(helpcontent);
            close(fptr);
            MaintainLogFile(DEBUG,"Help File Printed!");
            goto LOG_SUCCESS;
        }
        else if(strcmp(argv[1], "-f") ==0)
        {   // when input is passed through file
            MaintainLogFile(DEBUG,"Read Input from file");
            char* filelistname;
            char *filelistcontent;
            filelistname = (char*) malloc(15);
            filelistcontent = (char*) malloc(50);

            printf("Enter Filename(having list of file):\n");
            scanf("%s", filelistname);
            fptr= fopen(filelistname, "r");
            if(fptr == NULL)
            {
                printf("cannot open file(filelist) \n");
                MaintainLogFile(ERROR,"Can't open file(filelist)");     
                exit(0);
            }
            else
            {
                int f = 0; //record count
               while(fgets( filelistcontent,50, fptr) != NULL
                       && strcmp( filelistcontent,"\n") != 0)
               {
                   f++;
                   MaintainLogFile(DEBUG,"Reading new Filename and top");
                   printf("%s",filelistcontent);
                   if( sscanf(filelistcontent, "%s %d\n", filename, &top)!= 2)
                   {
                       printf("Input data is invalid at record %d\n", f);
                       sprintf(log_entry,
                               "Input data is invalid at record %d", f);
                       MaintainLogFile(ERROR,log_entry);
                       exit(0);
                   }
                   MaintainLogFile(DEBUG, "Got Input From File ");
                
                   printf("filename: %s and  top= %d \n", filename, top);
                   if(ReadStoreData(filename) == true)//stores after validation
                   {
                       QuickSortMarks(first_node, last_node);//sort marks in asc
                       PrintData(top);  // prints data from last node
                   }
                   else
                   {
                        printf("Data Cannot be stored!!\n");
                        MaintainLogFile(ERROR,"Data Cannot be stored!!");
                        exit(0);
                   }
               }
            }
            free(filelistcontent);
            free(filelistname);
            close(fptr);
            goto LOG_SUCCESS;
        }
        else
        {
            filename = argv[1];
            MaintainLogFile(DEBUG, "Got Filename Through Command line");
            printf("Enter no of toppers required:\n");
            if(scanf("%d", &top) != 1)
            {
                printf("no. of topper is invalid!!\n");
                MaintainLogFile(ERROR, "no. of topper is invalid");
                exit(0);
            }
            MaintainLogFile(DEBUG, "Got Toppers value from Standard Input");
        
      /*         // when cmd is neither -f nor --help
            printf("Invalid command!!!");
            MaintainLogFile( ERROR, "Invalid Command !!!");
            exit(0);*/
        }
    }
    else
    {   // to get input as Standard Input
        printf("Enter filename:\n");   
        scanf("%s", filename) ;

        printf("Enter no of toppers required:\n");
        if(scanf("%d", &top) != 1)
        {
            printf("no. of topper is invalid!!\n");
            MaintainLogFile(ERROR, "no. of topper is invalid");
            exit(0);
        }
        MaintainLogFile(DEBUG, "Got Input from Standard Input");
    }
    
    bool flag;
    flag =  ReadStoreData(filename);
             /* reads student detail from file,
              *  then validates and stores it  */

    if(flag == true)
    {
        QuickSortMarks(first_node, last_node); // sort marks in asc
        PrintData(top);  // prints data from last node
    }
    else
    {
           printf("Data Cannot be stored!!\n");
           exit(0);

    }
   
LOG_SUCCESS:
    MaintainLogFile(LOG, "Transaction Complete...");
    // after user output is dispalyed

    return 0;
}

/******************************************************************************/
/**                 Local functions                                          **/

/*******************************************************************************
 *   Function Name: ReadStoreData                                                 
 *   Arguments:     1                  
 *   Argument     Type      I/O              Description
 *   ---------  --------  -------   -----------------------------------
 *   filename    *char     input     pointer to a file name string
 *
 *   Returns:       bool
 *   Description:   read student name & mark from file and stores data in
 *                  linked list after validating data with the help of
 *                  ValidateStData function; returns true if data has been
 *                  suuceesfuly stored else false 
 */
bool ReadStoreData(char* filename)
{
    size = 0;
    FILE *fptr;
    char *line_ptr;
    bool data_statusflag;

    line_ptr = (char*) malloc(60);
    data_statusflag = true;

    fptr = fopen(filename, "r");
    strcpy(log_entry, "Opening student data file");
    MaintainLogFile(DEBUG, log_entry);

    if(fptr == NULL)
    {
        printf("cannot open file\n");
        MaintainLogFile(ERROR, "student data file cannot be opened");
        exit(0);
    }
    else
    {
        strcpy(log_entry,"Creating current, first & last node");
        MaintainLogFile(DEBUG , log_entry);
        curr_node = (struct strdata *) malloc(sizeof(struct strdata));
        first_node = curr_node;
        last_node = curr_node;
        first_node->prev = NULL;

        while( fgets(line_ptr, 60, fptr) != NULL &&
                (strcmp(line_ptr, "\n") != 0))
        {
            if(size != 0)
            {
                MaintainLogFile(DEBUG, "Creating new node");
                curr_node->next
                    = (struct strdata *) malloc (sizeof(struct strdata));
                curr_node->next->prev = curr_node;
                curr_node = curr_node->next;
                last_node = curr_node;
            }

            if(ValidateStData(line_ptr) == true)
            {
                 if( sscanf(line_ptr, "%s %f",
                         curr_node->name, &curr_node->marks) == 2)
                {
                        size++;         //           Data updated
                }
                else
                {
                    data_statusflag = false;
                    printf("data cannot be stored\n");
                    MaintainLogFile(ERROR, "data cannot be stored in list");
                    exit(0);
                }
            } 
            else
            {
                 data_statusflag = false;
                 printf("data not stored");
                 exit(0);
            }

      //      printf("size: %d\n",size);
            fflush(stdout);
        }

        curr_node->next = NULL; // makes last node next pointer null
        MaintainLogFile(DEBUG, "Data succesfully stored in list");
    }

    /*   code for validating whether no of toppers to be printed is
     *   lesser than number of student present   
     */

    if(top > size)
    {
        printf(" number of toppers asked is greater "
                 " than number of student present\n");
        strcpy(log_entry,"Top value is greater than number of students");
        MaintainLogFile(ERROR, log_entry);
        exit(0);
    }
    free(line_ptr);
    close(fptr);

    return data_statusflag;
}
/******************************************************************************/

/*******************************************************************************
 *   Function Name: ValidateStData                                                
 *   Arguments:     1
 *   Argument     Type      I/O              Description
 *   ---------  --------  -------   -----------------------------------
 *   line_ptr    *char     input     pointer to a line from input file
 *
 *   Returns:       bool
 *   Description:   validate student name and marks;
 *                   returns true if validate is success else false
 */

bool ValidateStData(char *line_ptr)
{
    bool vs_flag = true;
    NODE_PTR temp;
    char *my_data1, *my_data2;
    char *err_msg;

    temp = (NODE_PTR) malloc(sizeof(struct strdata));

    my_data1 = (char*) malloc(15); 
    my_data2 = (char*) malloc(15);
    err_msg =  (char*) malloc(100);
    strcpy(err_msg, "NO ERROR");


    if( sscanf(line_ptr, "%s %s", my_data1, my_data2) != 2)
    {
        sprintf(err_msg, "Inadequate data at record%d", size+1);
        vs_flag = false;
    }
    else if( sscanf(my_data1, "%s", temp->name) != 1)
    {
         sprintf(err_msg, "Student name invalid at record%d", size+1);
         vs_flag = false;
    }
    else if( sscanf(my_data2, "%f", &temp->marks) != 1)
    {
         sprintf(err_msg, "Mark invalid at record%d", size +1);
         vs_flag = false;
    }
    else if( temp->marks > MAX_MARK )
    {
         sprintf(err_msg, "Mark assigned more than max. mark "
                 "at record %d", size+1);
         vs_flag = false;
    }
    else if( temp->marks < 0 )
    {
         sprintf(err_msg, "Negative mark assigned at record:%d ", size+1);
         vs_flag = false;
    }

    if(vs_flag == false)
    {
        strcpy(log_entry, err_msg);
        MaintainLogFile(ERROR, log_entry);
        printf("%s \n", err_msg);
        exit(0);
    }
    return vs_flag; 
}
/******************************************************************************/

/*******************************************************************************
 *   Function Name: QuickSortMarks                                                
 *   Arguments:     2                                                           
 *   Argument     Type             I/O     Description
 *  ---------- ----------------   -----   ----------------
 *    first    struct strdata*    input   pointer to first index for sorting
 *    last     struct strdata*    input   pointer to last index for sorting
 *    Returns:      0(int) as recursive  
 *    Description:  Sort marks in ascending using quick sort algorithm
 */
int QuickSortMarks(struct strdata *first,struct strdata *last)
{
    struct strdata *ptr;
    ptr =  FindPivot(&first, &last);
    if((first != ptr) && (first != (ptr->prev)))
        QuickSortMarks(first, ptr->prev);
    if((ptr != last)&& ((ptr->next) !=  last))
        QuickSortMarks(ptr->next, last);
    return 0;
}
/******************************************************************************/

/*******************************************************************************
 *  Function Name: FindPivot                                                
 *  Arguments:     2                                                           
 *   Argument     Type        I/O     Description
 *  ---------- -----------   -----   ----------------
 *    first    NODE_PTR*    input   pointer to first index for sorting
 *    last     NODE_PTR*    input   pointer to last index for sorting
 *  Returns:      pivot element
 *  Description:  Finds Pivot index
 */
struct strdata* FindPivot(NODE_PTR *first_p,NODE_PTR *last_p)
{
    struct strdata *first, *last;
    first = *first_p;
    last = *last_p;

    bool ch_flag = true;
    struct strdata *piv, *up, *down, *temp;
  
    piv = first;
    up = first;
    down = last;

    MaintainLogFile(DEBUG, "Finding pivot element");

    while(ch_flag == true  && up != down)
    {
        /* Finds first number greater than pivot in the list from starting
         */
        while((up->marks <= piv->marks)
                && (up->next != NULL)
                && (up != last))
        {
             up = up->next;
             if(up->prev == down)
                 ch_flag = false;
        }
        /* Finds first number lesser than pivot in the list from end
         */
        while((down->marks >  piv->marks)
                && (down->prev != NULL)      
                && (down != first))
        {
             down = down->prev;
             if(down->next == up)
                 ch_flag = false;
        }
    
        if(ch_flag == true && up != down)
        {
            //reassigning lastnode pointer before exchanging
            //first_node or first are not changed because pivot not exchanged
           if(down == last_node)
                    last_node = up;            
           if(down == last)
                 last = up;

            ExchangeData(up, down);
            temp = up;
            up = down;
            down = temp;
        }
    }
    
    //reassigning firstnode and lastnode pointer before exchanging
    if(piv == first_node)      
       first_node = down;   
    if(first == piv)
       first = down;
    if(down == last)
       last = piv;
    if(down == last_node)
        last_node = piv;
    ExchangeData(piv, down);
    *first_p = first;
    *last_p = last;
    return piv;
}
/******************************************************************************/

/*******************************************************************************
 *   Function Name: ExchangeData                                                
 *   Arguments:     2                                                           
 *   Argument     Type        I/O     Description
 *  ---------- -----------   -----   ----------------
 *      a       NODE_PTR*    input   pointer to first index to be exchanged
 *      b       NODE_PTR*    input   pointer to next index to be exchanged
 *    Returns:      void
 *    Description:  exchange a and b node in the list
 */
void ExchangeData(struct strdata *a, struct strdata *b)
{
    struct strdata *temp_a, *temp_b, *temp;
    temp_a = (struct strdata *) malloc(sizeof(struct strdata));
    temp_b = (struct strdata *) malloc(sizeof(struct strdata));

    if(a != b)
    {
        MaintainLogFile(0, "Swaping Nodes");

        temp_a->prev = a->prev;
        temp_a->next = a->next;    
        temp_b->prev = b->prev;
        temp_b->next = b->next;    
   //     printf("a != b\n");

        if(a->prev != NULL )
        {  // when a is not first node
            a->prev->next = b;
        }
        if(a->next != b)
        {  //when b is not next to a
            a->next->prev = b;
            b->prev->next = a;
        }
        if(b->next != NULL) // when b is not last node
            b->next->prev = a;
        if(a->next != b)
        { // when b is not next to a
            a->prev = temp_b->prev;
            b->next = temp_a->next; 
        }
        else
        { // when b is next to a
            a->prev = b;
            b->next = a;
        }
        if(temp_a->prev != NULL)
            b->prev = temp_a->prev;
        if(temp_b->next != NULL)
            a->next = temp_b->next;
    }
    free(temp_a);
    free(temp_b);
}
/******************************************************************************/

/*******************************************************************************
 *   Function Name: FindPivot                                                
 *   Arguments:     1                                                           
 *   Argument   Type     I/O     Description
 *  ---------  -------  -----   ----------------
 *    top       int     input   number of toppers detail to be printed
 *    Returns:      void
 *    Description:  Print list from end(descending order)
 *                  till number of top count specified by user
 */
void PrintData(int top)
{
    int i = 1;
    struct strdata *c;
    c = last_node; // printing data from last node
                   // as list is sorted in asc.
    printf("Rank  Name  Marks\n");
    while( c != NULL && i<= top)
    {
        printf("%d\t %s\t %f\n", i, c->name, c->marks);
        c = c->prev;
        i++;
    }
}
/********************************************************************************
 *    Function Name: MaintainLogFile                                                   
 *    Arguments:     2                                                           
 *    Argument      Type      I/O       Description
 *  -----------   --------   ------    ----------------
 *     log_type     int      input       type of log 
 *     log_entry   *char     input       type of log entry
 *    Returns:       void
 *    Description:   Maintains log file
 */
void MaintainLogFile(int log,char* log_entry)
{
    static int log_id = 0;
    int j;
    char* log_type;
    log_type = (char*) malloc(8);
    time_t raw_time;
    struct tm *ptr_ts;
    time ( &raw_time );
    ptr_ts = localtime( &raw_time );
    
    switch(log)
    {
        case 0: strcpy(log_type,"DEBUG");
            break;
        case 1: strcpy(log_type,"LOG ");
            break;
        case 2: strcpy(log_type,"ERROR");
            break;
        default:strcpy(log_type,"ERROR");
                    break;        
    }            


    FILE *fptr1;
    fptr1 = fopen(LOGFILE, "a+");

    if(fptr1 == NULL)
        printf("log file does not exists");
    else
    {

        log_id++;
        fprintf(fptr1, "  %02d  \t"
                "%02d:%02d:%02d "
                "%02d/%02d/%d\t\t"
                "%6s\t"
                " %s\n", 
                log_id,
               (5 + ptr_ts->tm_hour)%24,(30+ ptr_ts->tm_min)%60,ptr_ts->tm_sec,
                ptr_ts->tm_mday,(1+ptr_ts->tm_mon ), (1900 + ptr_ts->tm_year),
                log_type,
                log_entry);
            
        if((strcmp(log_type, "LOG ") == 0)||(strcmp(log_type, "ERROR") == 0))
        {
            for(j=0;j<80;j++)  
                fprintf(fptr1, "~");
            fprintf(fptr1, "\n");
        }
    }
   fclose(fptr1);
}

/*******************************************************************************/
/*******************************************************************************/
