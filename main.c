#include "header.h"
#include "read.h"
#include "medicine.h"

void create_ShM1();
void create_ShM2();
void clean_ShM1();
void clean_ShM2();

int running_time_exceeded_threshold(double threshold_seconds);
int TooMany_bottled_medicine_outOfSpec();
int number_of_generated_liquid_exceeded_threshold();
int simulation_end_condition_met() ;


Config c;

#define MAX_CHILDREN 10
int child_pids[MAX_CHILDREN];
int num_children = 0;

void terminate_children() {
    printf("Terminating all child processes...\n");
    for (int i = 0; i < num_children; i++) {
    
        kill(child_pids[i], SIGTERM);  // Send SIGTERM to each child
        printf("Sent SIGTERM to child PID %d\n", child_pids[i]);
    }
        // Wait for all child processes to terminate
    for (int i = 0; i < num_children; i++) {
        waitpid(child_pids[i], NULL, 0);
    }
  
    
    printf("All child processes terminated.\n");
    
}

// Global variable to store the start time of the simulation
time_t start_time;

// Function to initialize the start time of the simulation
void initialize_start_time() {
    time(&start_time);
}

// Function to calculate the elapsed time since the start of the simulation in seconds
double elapsed_time() {
    time_t current_time;
    time(&current_time);
    return difftime(current_time, start_time);
}

int main() {

  c = read_config("userdefined.txt");
  
      // Initialize the start time of the simulation
    initialize_start_time();
  
printf("num_of_medicines = %d\n", c.num_of_medicines);
printf("num_of_Liquid_lines = %d\n", c.num_of_Liquid_lines);
printf("num_of_Pills_lines = %d\n", c.num_of_Pills_lines);
printf("max_failures_rang = %d\n", c.max_failures_rang);
printf("min_failures_rang = %d\n", c.min_failures_rang);
printf("max_failuresType = %d\n", c.max_failuresType);
printf("min_failuresType = %d\n", c.min_failuresType);
printf("threshold_liquid_produced = %d\n", c.threshold_liquid_produced);
printf("threshold_pills_produced = %d\n", c.threshold_pills_produced);
printf("threshold_bottled_out = %d\n", c.threshold_bottled_out);
printf("threshold_pill_based_out = %d\n", c.threshold_pill_based_out);
printf("threshold_pill_based_out = %d\n", c.threshold_pill_based_out);
printf("simulation_time = %d\n", c.simulation_time);
printf("min_Generate_period = %d\n", c.min_Generate_period);
printf("max_Generate_period = %d\n", c.max_Generate_period);
printf("min_package_period = %d\n", c.min_package_period);
printf("max_package_period = %d\n", c.max_package_period);
printf("THRESHOLD_RATIO = %lf\n", c.THRESHOLD_RATIO);
printf("num_pills_container = %d\n", c.num_pills_container);
printf("num_containers_perPackage = %d\n", c.num_containers_perPackage);

  create_ShM1();
   create_ShM2();

  
  

  // fork the liquid production lines 
  for (int i = 0; i < c.num_of_Liquid_lines; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      char productionLine_id_str[10];
      sprintf(productionLine_id_str, "%d", i + 1);
      execl("./liquid_line", "liquid_line", productionLine_id_str, (char *)NULL);
      perror("execl failed");
      exit(EXIT_FAILURE);
    } else if (pid < 0) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }else if (pid > 0) {  // Parent process
            child_pids[num_children++] = pid;
        } 
     sleep(2);
  }
  
   
  
  
    // fork the pills production lines 
  for (int i = 0; i < c.num_of_Pills_lines; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      char productionLine_id_str[10];
      sprintf(productionLine_id_str, "%d", i + 1);
      execl("./pills_line", "pills_line", productionLine_id_str, (char *)NULL);
      perror("execl failed");
      exit(EXIT_FAILURE);
    } else if (pid < 0) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }else if (pid > 0) {  // Parent process
            child_pids[num_children++] = pid;
        } 
  }
  
  //fork manager to watch the production rate
  
      pid_t pid = fork();
    if (pid == 0) {
      char manager_id_str[10];
      sprintf(manager_id_str, "%d", 1);
      execl("./manager", "manager", manager_id_str, (char *)NULL);
      perror("execl failed");
      exit(EXIT_FAILURE);
    } else if (pid < 0) {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }else if (pid > 0) {  // Parent process
            child_pids[num_children++] = pid;
        }
  
  
  pid_t animation;

    if((animation = fork()) == -1){
        perror("The animation fork error\n");
        exit(-1);
    }

    // To let the animation leave the main code and go to animation.c
    if(!animation){
        execlp("./animation", "animation", (char *) NULL);

        // If the program not have enough memory then will raise error
        perror("exec Failure\n");
        exit(-1);
    }
  
  while(1) {
  
  // End of simulation conditions
    if (simulation_end_condition_met()) {
        printf("Simulation ended.\n");
        break;
    } 
    
    sleep(3);
  }
  
  printf("clean all\n");
  clean_ShM1();
  clean_ShM2();
  return 0;

}


// Function to check if any simulation end condition is met
int simulation_end_condition_met() {
    // Check each end condition
   if (running_time_exceeded_threshold(c.simulation_time)) {
         terminate_children();
        return 1;
    }


    if (number_of_generated_liquid_exceeded_threshold()) {
         terminate_children();
        return 1;
    }
    
    if(TooMany_bottled_medicine_outOfSpec()){
        
        terminate_children();
        return 1;
    
    }

 

    return 0;  // No simulation end condition met
}

int number_of_generated_liquid_exceeded_threshold(){

  int shmid = shmget(liquid_key, sizeof(Numbers_liquid_lines), 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  Numbers_liquid_lines *shared_data = (Numbers_liquid_lines *)shmat(shmid, NULL, 0);
  if (shared_data == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }
  
  //check if number of generated liquid (bottled) medicines exceeds a user defined threshold
  
  int total = 0;
  
  for(int i = 1; i <= c.num_of_Liquid_lines; i++){
     
     total += shared_data[i].num_generated;
  
  }
  
  if((total) > c.threshold_liquid_produced ) {
     printf("Ending simulation : number of generated liquid (bottled) medicines exceeds a user defined threshold\n");
      return 1;
     
  }
  
  return 0; 

}

int TooMany_bottled_medicine_outOfSpec(){
     int shmid = shmget(liquid_key, sizeof(Numbers_liquid_lines), 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  Numbers_liquid_lines *shared_data = (Numbers_liquid_lines *)shmat(shmid, NULL, 0);
  if (shared_data == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }
  
  //check if number of generated liquid (bottled) medicines exceeds a user defined threshold
  
  int total = 0;
  
  for(int i = 1; i <= c.num_of_Liquid_lines; i++){
     
     total += shared_data[i].num_rejected;
  
  }
  
  if((total) > c.threshold_liquid_produced ) {
     printf("Ending simulation : Too many bottled medicine out of spec\n");
      return 1;
     
  }
  
  return 0;

}

// Function to check if the running time has exceeded a user-defined threshold
int running_time_exceeded_threshold(double threshold_seconds) {
    double elapsed_seconds = elapsed_time();
   // printf("Elapsed time: %.2f seconds\n", elapsed_seconds);
    if (elapsed_seconds > threshold_seconds) {
        printf("Ending simulation: Time limit reached\n");
        return 1; // Exceeded threshold
    }
    return 0; // Not exceeded threshold
}

void create_ShM1(){
     int shmid =
      shmget(liquid_key, sizeof(Numbers_liquid_lines), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  Numbers_liquid_lines *shared_data = (Numbers_liquid_lines *)shmat(shmid, NULL, 0);
  if (shared_data == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }
  
  for(int i = 1; i<= c.num_of_Liquid_lines; i++){
  shared_data[i].num_generated = 0;
  shared_data[i].num_rejected = 0;
  shared_data[i].num_packaged = 0; 
  shared_data[i].num_employees = c.num_employees_line;
  }

  
  if (shmdt(shared_data) == -1) {
    perror("shmdt");
    exit(EXIT_FAILURE);
  }


}

void create_ShM2(){
       int shmid =
      shmget(pills_key, sizeof(Numbers_Pill_lines), IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  Numbers_Pill_lines *shared_data = (Numbers_Pill_lines *)shmat(shmid, NULL, 0);
  if (shared_data == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }
  
  for(int i = 1; i<= c.num_of_Pills_lines; i++){
  shared_data[i].num_generated = 0;
  shared_data[i].num_rejected = 0;
  shared_data[i].num_packaged = 0; 
  shared_data[i].num_employees = c.num_employees_line;
  }

  
  if (shmdt(shared_data) == -1) {
    perror("shmdt");
    exit(EXIT_FAILURE);
  }


}

void clean_ShM1() {
  int shmid = shmget(liquid_key, sizeof(Numbers_liquid_lines), 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("shmctl");
    exit(EXIT_FAILURE);
  }
}

void clean_ShM2() {
  int shmid = shmget(pills_key, sizeof(Numbers_Pill_lines), 0666);
  if (shmid == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }

  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("shmctl");
    exit(EXIT_FAILURE);
  }
}
