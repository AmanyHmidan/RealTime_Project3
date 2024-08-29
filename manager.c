#include "header.h"
#include "read.h"
#include "medicine.h"

int id;

void manager_behaviour();

Config c;

int main(int argc, char *argv[]) {

   c = read_config("userdefined.txt");
  
    if (argc != 2) {
    fprintf(stderr, "Usage: %s <line_id>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  id = atoi(argv[1]);
  
  manager_behaviour();
  

}

void manager_behaviour(){

  //Attach to shared memory for liquid 
          int shmid2 = shmget(liquid_key, sizeof(Numbers_liquid_lines), 0666);
        if (shmid2 == -1) {
          perror("shmget");
          exit(EXIT_FAILURE);
        }

        Numbers_liquid_lines *shared_data = (Numbers_liquid_lines *)shmat(shmid2, NULL, 0);
        if (shared_data == (void *)-1) {
          perror("shmat");
          exit(EXIT_FAILURE);
        }
        
     //Attach to shared memory for pills
        int shmid1 = shmget(pills_key, sizeof(Numbers_Pill_lines), 0666);
        if (shmid1 == -1) {
          perror("shmget");
          exit(EXIT_FAILURE);
        }

        Numbers_Pill_lines *shared_data2 = (Numbers_Pill_lines *)shmat(shmid2, NULL, 0);
        if (shared_data2 == (void *)-1) {
          perror("shmat");
          exit(EXIT_FAILURE);
        }
        
            // Manager loop
    while (1) {
        // Read values from shared memory for each production line
        for (int i = 1; i <= c.num_of_Liquid_lines; i++) {
            double ratio = (double)(shared_data[i].num_packaged + shared_data[i].num_rejected) / shared_data[i].num_generated;
            printf("liquid Line %d: Generated: %d, Packaged: %d, Ratio: %.2f\n",
                   i, shared_data[i].num_generated, shared_data[i].num_packaged, ratio);

            // Check if the ratio exceeds the threshold
            if (ratio < c.THRESHOLD_RATIO && ratio > 0) {
                
                printf("liquid Line %d needs additional packaging employees!\n", i);
                
                // Try to find a line with surplus employees
                for (int j = 1; j <= c.num_of_Liquid_lines; j++) {
                    if (j != i) {
                        double other_ratio = (double)(shared_data[i].num_packaged + shared_data[i].num_rejected)/ shared_data[j].num_generated;
                        if (other_ratio >= c.THRESHOLD_RATIO && shared_data[i].num_employees > 2 ) {
                            // Send signal to remove an employee from line j
                            kill(shared_data[j].pid, SIGUSR2);
                            // Send signal to add an employee to line i
                            kill(shared_data[i].pid, SIGUSR1);
                            printf("Moved an employee from liquid line %d to line %d\n", j, i);
                            break;
                        }
                        
                        printf("other liquid line is also slow\n");
                    }
            }
        }

        
    }
    
    
    //now check for pills
            for (int i = 1; i <= c.num_of_Pills_lines; i++) {
            double ratio = (double)(shared_data2[i].num_packaged + shared_data2[i].num_rejected) / shared_data2[i].num_generated;
            printf("pills Line %d: Generated: %d, Packaged: %d, Ratio: %.2f\n",
                   i, shared_data2[i].num_generated, shared_data2[i].num_packaged, ratio);

            // Check if the ratio exceeds the threshold
            if (ratio < c.THRESHOLD_RATIO && ratio > 0) {
                
                printf("pills Line %d needs additional packaging employees!\n", i);
                
                // Try to find a line with surplus employees
                for (int j = 1; j <= c.num_of_Pills_lines; j++) {
                    if (j != i) {
                        double other_ratio = (double)(shared_data2[i].num_packaged + shared_data2[i].num_rejected) / shared_data2[j].num_generated;
                        if (other_ratio >= c.THRESHOLD_RATIO && shared_data2[i].num_employees > 2) {
                            // Send signal to remove an employee from line j
                            kill(shared_data2[j].pid, SIGUSR2);
                            // Send signal to add an employee to line i
                            kill(shared_data2[i].pid, SIGUSR1);
                            printf("Moved an employee from pills line %d to line %d\n", j, i);
                            break;
                        }
                        
                        printf("other pills line is also slow\n");
                    }
            }
        }

        
    }
    
    // Sleep for a while before checking again
        sleep(5);


}


}
