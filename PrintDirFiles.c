/*
********************************************************************************
**  File:   PrintDirFiles.c                                                   **
**  Author: Sangeetha                                                         **
**  Date:   12-08-2015       `                                                **
**                                                                            **
********************************************************************************
**  COPYRIGHT: (C) 2015 - All rights reserved - Network Marvels (I) Pvt. Ltd.,**
**  Thane (India)                                                             **
**                                                                            **
********************************************************************************
**  Program:   To print all names of all files and directories, and its       **
**  sub-directories and files name of given dirctory                          **
**  Input: Full path name of the directory                                    **
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
#include <string.h>
#include <stdlib.h>
#include <dirent.h> //DIR, struct dirent
#include <sys/stat.h> //struct stat
//#include <sys/types.h>
#include <unistd.h> // for chdir(), getcwd()
#include <errno.h>  // to print error no
#include <time.h>
/******************************************************************************/

/*******************************************************************************
 *                 Definations and Macros                                     */
#define HELPFILE "/home/sangeetha/cprg/dirhelp"
#define LOGFILE "/home/sangeetha/cprg/dir_log_file"
#define DEBUG 0
#define LOG 1
#define ERROR 2
/******************************************************************************/

/*******************************************************************************
 *                 Structures and Typedefs                                    */
struct stack
{
    struct dirent *sub_dir;
    struct stack *next_ptr;
};

typedef struct stack* NODE_PTR;
/******************************************************************************/

/*******************************************************************************
 *                  Prototypes of Local Functions                             */
int FindDirFiles(char*);
void PushDirName(NODE_PTR*, struct dirent*);
struct dirent* PopDirName(NODE_PTR*);
void MaintainLogFile(int ,char*);
/******************************************************************************/

/*******************************************************************************
 *                 Global variables                                           */
static char* log_entry;
/******************************************************************************/

int main(int argc, char* argv[])
{
    char *root_path;
    root_path = (char*) malloc(80);
    log_entry = (char*) malloc(100);
    MaintainLogFile(DEBUG, "Initialising and Allocating memory");

    if(argc == 2)
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
                printf("cannot open help file \n");
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
            fclose(fptr);
            MaintainLogFile(DEBUG,"Help File Printed!");
            goto LOG_SUCCESS;
        }

        else if(strcmp(argv[1], "-f") == 0)
        {
            char* filename;
            char* file_line;
            filename = (char*) malloc(15);
            file_line = (char*) malloc(15);

            printf("Enter Filename:\n");
            scanf("%s", filename);
            fptr = fopen(filename, "r");

            if(fptr == NULL)
            {
                printf("Cannot open file\n");
                MaintainLogFile(ERROR,"Cannot open File!!");
                exit(0);
            }
            else
            {
                while(fgets(file_line,80,fptr) != NULL 
                        && strcmp(file_line, "\n") != 0)
                {
                    if(sscanf(file_line, "%s\n", root_path) == 1)
                    {
                        printf("Given Directory Name: %s\n", root_path);
                        MaintainLogFile(DEBUG, "Got Input Through file");                        
                    }
                    else
                    {
                        printf("Incorrect root path\n");
                        MaintainLogFile(ERROR, "Incorrect root path");
                        exit(0);
                    }

                    if( chdir(root_path) == 0)
                        MaintainLogFile(DEBUG, "Current path changed");
                   //      printf("Dir changed\n");

//                  printf("Directory and files Inside the dir. %s are \n ", root_path);
                    FindDirFiles(root_path);
                    MaintainLogFile(DEBUG, "Files and Directories Printed");
                    printf("\n");
                }
            }
            fclose(fptr);
            goto LOG_SUCCESS;
        }
        else
        {
            root_path = argv[1];
            MaintainLogFile(DEBUG, "Got Input as Arguments");
        }
    }
    else
    {
        printf("Enter root directory:\n");
        scanf("%s", root_path);
        MaintainLogFile(DEBUG, "Got Input through standard input");
    }

    if( chdir(root_path) == 0)
    {
        MaintainLogFile(DEBUG, "Current path changed");
        FindDirFiles(root_path);
        MaintainLogFile(DEBUG, "Files and Directories Printd");
    }
    else
    {
        printf("Invalid Directory name\n");
        MaintainLogFile(ERROR, "Invalid Directory name");
        exit(0);
    }

    LOG_SUCCESS:
        MaintainLogFile(LOG, "Transaction Complete");

    return 0;
}
/******************************************************************************/
/**                 Local functions                                          **/

/*******************************************************************************
 *   Function Name: FindDirFiles                                                 
 *   Arguments:     1                  
 *   Argument     Type      I/O              Description
 *   ---------  --------  -------   -----------------------------------
 *   dir_name   *char     input     pointer to name of directory
 *
 *   Returns:       int
 *   Description:   Prints names of all files and directories of the given 
 *                  dir_name input using reccursion and stack
 */

int FindDirFiles(char* dir_name)
{
    DIR *d;
    struct dirent *element;
    static char curr_path[80];
    struct stat statbuf;
    struct stack *top_element;
    char* file;
    
    top_element = NULL;
    file = (char*) malloc(80);

    d = opendir(dir_name);
    if(d)
    {
        MaintainLogFile(DEBUG, "Directory Stream Opened");
        while((element = readdir(d) ) != NULL)
        {
            if( strcmp(element->d_name, ".") == 0 ||
                strcmp(element->d_name, "..") == 0 )
                     continue;

            if( element->d_type == DT_DIR)  
            {
                PushDirName(&top_element,element);
            }
            else
            {
                getcwd(curr_path, 80);
                strcpy(file, curr_path);
                strcat(file, "/");
                strcat(file, element->d_name);
                printf("   %s\n", file);
            }
        }
      
        MaintainLogFile(DEBUG, "Reading files and directories");
        while(top_element != NULL)
        {
            element = PopDirName(&top_element);
            
            if(chdir(element->d_name) !=0)
            {
                printf("%s/%s: %s\n", curr_path, element->d_name,
                        strerror(errno));
                MaintainLogFile(DEBUG, strerror(errno));
                continue;
            }
            getcwd(curr_path, 80);
            dir_name = curr_path;
            printf("Directory: %s  ->->\n", dir_name);
            FindDirFiles(dir_name);
            chdir("..");
            getcwd(curr_path, 80);
        }
    }
    else if( ENOENT  == errno )
    {
//      printf("No such file or directory\n");
        printf("%s\n", strerror(errno));
        MaintainLogFile(ERROR, strerror(errno));
        exit(0);
    }
    else
    {
        printf("%s\n", strerror(errno));
        MaintainLogFile(ERROR, strerror(errno));
        exit(0);
    }
    free(top_element);
    return 0;
}

/*******************************************************************************
 *   Function Name: PushDirName 
 *   Arguments:     2                  
 *   Argument         Type         I/O              Description
 *   -----------  --------------  -------   -----------------------------------
 *   top_element   NODE_PTR*      input     pointer to top element in the stack 
 *   element      struct dirent*  input     pointer to dir_name to be pushed
 *   Returns:       void
 *   Description:   store dir. name in stack using PUSH operation 
 */

void PushDirName(NODE_PTR *top_element, struct dirent *element)
{
    struct stack *new_node;
    new_node = (struct stack*) (malloc) (sizeof(struct stack));
    
    new_node->sub_dir = element;
    new_node->next_ptr = *top_element;
    
    *top_element = new_node;
//  MaintainLogFile(DEBUG, "Directory Pushed");
}
/******************************************************************************/

/*******************************************************************************
 *   Function Name: PopDirName 
 *   Arguments:     1                  
 *   Argument        Type      I/O              Description
 *   ------------  ---------  -------   -----------------------------------
 *   top_element   NODE_PTR*   input     pointer to top element in the stack 
 *
 *   Returns:       struct dirent* (pointer to dir_name to be poped)
 *   Description:   pop dir. name in stack 
 */

struct dirent* PopDirName(NODE_PTR *top_element)
{
   struct dirent* temp;
   temp = (*top_element)->sub_dir;
   *top_element = (*top_element)->next_ptr;

// MaintainLogFile(DEBUG, "Directory Poped");
   return temp;
}
/******************************************************************************/

/********************************************************************************
 *    Function Name: MaintainLogFile                                                   
 *    Arguments:     2                                                           
 *    Argument      Type      I/O       Description
 *  -----------   --------   ------    ----------------
 *    log_type      int      input       type of log 
 *    log_entry    *char     input       type of log entry
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
        case 1: strcpy(log_type,"LOG");
            break;
        case 2: strcpy(log_type,"ERROR");
            break;
        default: strcpy(log_type,"ERROR");
            break;        
    }            

    FILE *fptr1;
    fptr1 = fopen(LOGFILE, "a+");

    if(fptr1 == NULL)
        printf("log file does not exists");
    else
    {

        log_id++;
        fprintf(fptr1, "  %02d \t %s  %s \t  %6s\t %s\n", 
                log_id,__TIME__,__DATE__,
                log_type,
                log_entry);

        if((strcmp(log_type, "LOG") == 0)||(strcmp(log_type, "ERROR") == 0))
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
