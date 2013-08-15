#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define MINUTES 60
#define CHANCE 3 // 30% chance of new student showing up every minute
#define TOTALSEATS 3

//shared
int *numinline;

void *student(int *numinline);

int main(int argc, char *argv[])
{
   srand(time(NULL));
   
   int workdone = 0;
   int studentshelped = 0;
   int currentstudentstime = ( (rand() % 5) + 1 ); //initial
   numinline = 0;
   int busy = 0; //not busy to start

   pthread_t tid;
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   int i;
   for (i = 0; i < MINUTES; i++)
   {
      printf("%d: ", i+1);
      pthread_create(&tid, &attr, student, &numinline);
      pthread_join(tid, NULL);

      if (busy == 1)
      {
         currentstudentstime--;
         if (currentstudentstime == 0)
         {
            printf("Finished helping a student.\n");
            studentshelped++;
            busy = 0;
            if (numinline > 0)
            {
               printf("The next student walks in.\n");
               int temp = (int)numinline;
               temp--;
               numinline = temp;
               currentstudentstime = ( (rand() % 5) + 1 );
               busy = 1;
            }
         }

         else
         {
            printf("Helping same student for %d more minutes.\n", currentstudentstime);
         }
      }

      else //not busy
      {
         if (numinline == 0)
         {
            printf("Working on her language.\n");
            workdone++;
         }
         
         else //someone in line
         {
            //set rand currentstudentstime
            currentstudentstime = ( (rand() % 5) + 1 );
            printf("Helping a new student for %d minutes.\n", currentstudentstime);
            busy = 1;

            //numinline-- was giving weird issues
            int temp = (int)numinline;
            temp--;
            numinline = temp;
         }
      }

      if (numinline > 0)
         printf("Currently %d in line.\n", numinline);
      printf("\n");

      sleep(1);
   }

   printf("Office hours over. Students in line all leave.\n");
   if (busy == 1)
   {
      i++;
      while (currentstudentstime > 0)
      {

         printf("%d: ", i);
         currentstudentstime--;
         if (currentstudentstime == 0)
         {
            printf("Finished helping a student in overtime.\n");
            studentshelped++;
         }
         else
         {
            printf("Helping a student in overtime for %d more minutes.\n", currentstudentstime);
            sleep(1);
         }
         printf("\n");
         i++;
      }
   }

   printf("%d students helped.\n", studentshelped);
   printf("%d minutes of work done.\n", workdone);
   exit(0);
}

void *student(int *numinline)
{
   int arrive = (rand() % 10); //random arrivals

   if (arrive < CHANCE) //someone arrives some % of the time
   {
      if (*numinline < TOTALSEATS)
      {
            (*numinline)++;
            printf("A new student arrives.\n");
      }
      else //no seats left
      {
         printf("A new student arrived, but left because there were no seats.\n");
      }
   }

   pthread_exit(0);
}
