#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

//semaphores
sem_t sem_ta;
sem_t sem_student;
sem_t chairs[3];

//threads, mutex
pthread_t *t_student;
pthread_t t_ta;
pthread_mutex_t chair_mutex;

int main(int ac, char *argv[]){
    int numStudent;
    printf("Enter number of students: ");
    scanf("%d", &numStudent);

    students_thread = (pthread_t*)

    pthread_create(&t_ta, NULL, ta, NULL); //threads  ta
    
    for(int i = 0; i < numStudent; i++){
        pthread_create(&t_student, NULL, stu, NULL)//threads student
    }


    return 0;
}

void *stu(){

}

void *ta(){

}