#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

int *student_id;
int chair_in_use = -1;
int next_chair = 0;

//semaphores
sem_t sem_ta;
sem_t sem_student;
sem_t sem_chairs[3];

//threads, mutex
pthread_t *t_student;
pthread_t t_ta;
pthread_mutex_t chair_mutex;

int main(int ac, char *argv[]){
    int numStudent;
    printf("Enter number of students: ");
    scanf("%d", &numStudent);

    sem_init(&sem_ta, 0, 0);
    sem_init(&sem_student, 0, 0);
    for(int s = 0; s< 3; s++){
        sem_init(&sem_chairs[s], 0, 0);
    }
    
    pthread_attr_t attribute;
    pthread_attr_init(&attribute);

    pthread_create(&t_ta, NULL, ta, NULL); //threads  ta
    t_student = (pthread_t*) malloc(numStudent * sizeof(pthread_t));
    student_id = (int*) malloc(numStudent * sizeof(int));
    for(int i = 0; i < numStudent; i++){
        student_id[i] = i;
        pthread_create(&t_student[i], NULL, stu, &student_id[i]);//threads student
    }

    pthread_mutex_init(&chair_mutex, NULL);//mutex

    pthread_join(t_ta, NULL);
    for(int j = 0; j < numStudent; j++){
        pthread_join(t_student[j], NULL);
    }

    return 0;
}

void* stu(void* arg){
    int id = *(int*) arg;
    for(;;){
        printf("Student with id %d is programming.\n", id);
        sleep(3);
        pthread_mutex_lock(&chair_mutex);
        int temp = chair_in_use;
        pthread_mutex_unlock(&chair_mutex);
        

        if(chair_in_use < 3){
            pthread_mutex_lock(&chair_mutex);
            if(chair_in_use >= -1 && chair_in_use < 3){
                chair_in_use++;
            }else{
                chair_in_use = 3;
            }
            pthread_mutex_unlock(&chair_mutex);
            int current_chair = (chair_in_use + next_chair) % 3;

            if(temp > 0){
                sem_wait(&sem_chairs[current_chair]);
                sem_wait(&sem_student);
                printf("TA is helping the student.\n");
            }else{
                sem_post(&sem_ta);
                sem_wait(&sem_student);
                printf("TA has finished helping.\n");
                
            }

        }
    }

}

void *ta(){
    for(;;){
        sem_wait(&sem_ta);
        printf("TA is awake\n");

        for(;;){
            pthread_mutex_lock(&chair_mutex);
            if(chair_in_use != 0){
                sem_post(&sem_chairs[next_chair]);

                next_chair= (next_chair + 1) % 3;
                pthread_mutex_unlock(&chair_mutex);
                sleep(5);
                sem_post(&sem_student);

            }else if (chair_in_use == 0){
                printf("No students\n");
                sem_post(&sem_student);
                break;
            }
        }
    }
}