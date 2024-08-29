#include "header.h"
#include "medicine.h"
#include "read.h"

#define MAX_MEDICINES 20
#define ERROR_RATE 1.0 // 1% chance of error
int pills;

int line_id;
Config c;

void pills_line_behavior();
void generate_pills_medicine(Pills *medicine, double failure_ranges[],
                              int num_types, int id);
void initialize_employees();
void *employeeFunction(void *arg);
void detach_shared_memory(void *shmaddr);
void setup_signal_handlers();
void add_employee_handler(int sig);
void remove_employee_handler(int sig) ;
void sigterm_handler(int sig);


// Number of medicines to generate
int num_generated = 0;

// Shared buffer and synchronization variables
Pills *medicine_buffer;
int buffer_count = 0;
pthread_mutex_t buffer_mutex;
pthread_mutex_t accepted_mutex;
pthread_cond_t buffer_not_empty;
pthread_cond_t buffer_not_full;

pthread_t *employees; // Dynamic array of employee threads
int *employeeIDs;     // Array of employee IDs

// Declare a variable of type pills_types (defined in medicine.h)
Pills_types Ptypes;

// Accepted medicines and their types
AcceptedMedicine_pill accepted_medicines[MAX_MEDICINES];
int accepted_count = 0;

// Mutex for shared memory updates
pthread_mutex_t shared_memory_mutex;

// Termination flag
volatile int terminateFlag = 0;

Config c;

int main(int argc, char *argv[]) {

 c = read_config("userdefined.txt");

  // Seed the random number generator with a unique value
  srand(time(NULL) ^ getpid());
  
      // Setup signal handlers
    setup_signal_handlers();

  // Allocate memory for the medicines array
  medicine_buffer = (Pills *)malloc(MAX_MEDICINES * sizeof(Pills));
  if (medicine_buffer == NULL) {
    fprintf(stderr, "Failed to allocate memory for medicines\n");
    return EXIT_FAILURE;
  }

  c = read_config("userdefined.txt");

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <line_id>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  line_id = atoi(argv[1]);

  initialize_employees();

  pills_line_behavior();

  // Join employee threads
  for (int i = 0; i < c.num_employees_line; i++) {
    pthread_join(employees[i], NULL);
  }

  // Free the allocated memory
  free(medicine_buffer);
  free(employees);
  free(employeeIDs);

  // Destroy mutexes and condition variables
  pthread_mutex_destroy(&buffer_mutex);
  pthread_mutex_destroy(&accepted_mutex);
  pthread_cond_destroy(&buffer_not_empty);
  pthread_cond_destroy(&buffer_not_full);
  pthread_mutex_destroy(&shared_memory_mutex);

  return 0;
}

void initialize_employees() {
  employees = malloc(c.num_employees_line * sizeof(pthread_t)); // Dynamic allocation for employees
  employeeIDs = malloc(c.num_employees_line * sizeof(int)); // Allocate memory for employee IDs

  // Initialize mutex and condition variables
  pthread_mutex_init(&buffer_mutex, NULL);
  pthread_mutex_init(&accepted_mutex, NULL);
  pthread_cond_init(&buffer_not_empty, NULL);
  pthread_cond_init(&buffer_not_full, NULL);
  pthread_mutex_init(&shared_memory_mutex, NULL); // Initialize the shared memory mutex

  usleep(100000);

  // Create employee threads
  for (int i = 0; i < c.num_employees_line; i++) { // Changed to start from 0
    employeeIDs[i] = i + 1; // Assign a unique ID to each employee

    // Initialize employees for each line
    if (pthread_create(&employees[i], NULL, employeeFunction, &employeeIDs[i])) {
      perror("Failed to create employee thread");
      exit(EXIT_FAILURE);
    }
  }
}

void pills_line_behavior() {


  // Initialize the shared memory types and counts
  char *types[] = {"Cough Syrup", "Antibiotic Suspension", "Pain Relief Pills",
                   "Vitamin Syrup", "Antacid Pills"};
  int num_types = sizeof(types) / sizeof(types[0]);
  for (int i = 0; i < num_types; i++) {
    strncpy(Ptypes.types[i], types[i], sizeof(Ptypes.types[i]));
    Ptypes.counts[i] = 0;
  }

  // Define failure ranges
  double failure_ranges[] = {
      0.05, // Probability that color is not correct
      0.02, // Probability that size is not correct 
      0.03, //  Probability that expiry date is not printed correctly
  };

  int id_counter = 1; // Unique ID counter
  
  int period = rand() % (c.max_Generate_period - c.min_Generate_period + 1) + c.min_Generate_period;
  printf("period for generating line %d is %d\n",line_id, period);

  while (!terminateFlag) {
    //int period = rand() % (c.max_Generate_period - c.min_Generate_period + 1) + c.min_Generate_period;

    pthread_mutex_lock(&buffer_mutex);

    // Wait for buffer to have space
    while (buffer_count == MAX_MEDICINES && !terminateFlag) {
      pthread_cond_wait(&buffer_not_full, &buffer_mutex);
    }

    if (terminateFlag) {
      pthread_mutex_unlock(&buffer_mutex);
      break;
    }

    // Generate and add the medicine to the buffer
    generate_pills_medicine(&medicine_buffer[buffer_count++], failure_ranges,
                             num_types, id_counter);

    // Increment the id_counter for the next medicine
    id_counter++;
    num_generated++;
    
    // Simulate generation period with periodic checks for termination
        for (int i = 0; i < period && !terminateFlag; i++) {
            sleep(1);
        }

 

    // Open shared memory to store the value of generated medicines
    pthread_mutex_lock(&shared_memory_mutex);
    int shmid2 = shmget(pills_key, sizeof(Numbers_Pill_lines), 0666);
    if (shmid2 == -1) {
      perror("shmget");
      pthread_mutex_unlock(&shared_memory_mutex);
      exit(EXIT_FAILURE);
    }

   Numbers_Pill_lines *shared_data = (Numbers_Pill_lines *)shmat(shmid2, NULL, 0);
    if (shared_data == (void *)-1) {
      perror("shmat");
      pthread_mutex_unlock(&shared_memory_mutex);
      exit(EXIT_FAILURE);
    }
    
      
    
    shared_data[line_id].pid = getpid();
    shared_data[line_id].num_generated += 1;
    
       // Signal that the buffer is not empty
    pthread_cond_signal(&buffer_not_empty);
    pthread_mutex_unlock(&buffer_mutex);

    //printf("num_generated from pills line %d is %d\n", line_id, shared_data[line_id].num_generated);

    detach_shared_memory(shared_data);
    pthread_mutex_unlock(&shared_memory_mutex);

    //sleep(period);
    
      // Simulate generation period with periodic checks for termination
      /*  for (int i = 0; i < period && !terminateFlag; i++) {
            sleep(1);
        }*/
    

   
  }
  
  

  // Signal employee threads to exit after it finishes generating medicines
  pthread_mutex_lock(&buffer_mutex);
  terminateFlag = 1;
  pthread_cond_broadcast(&buffer_not_empty);
  pthread_cond_broadcast(&buffer_not_full);
  pthread_mutex_unlock(&buffer_mutex);
}

void generate_pills_medicine(Pills *medicine, double failure_ranges[],
                              int num_types, int id) {
  // Assign the unique ID
  medicine->id = id;

  // Assign a type for this pills medicine
  int random_index = rand() % num_types;
  medicine->type = Ptypes.types[random_index];
  Ptypes.counts[random_index]++;

  // Assign random failures to medicine's characteristics
  medicine->isCorrect_size = (rand() / (double)RAND_MAX) >= failure_ranges[0];
  medicine->isCorrect_color = (rand() / (double)RAND_MAX) >= failure_ranges[1];
  medicine->isExpirydate_printed = (rand() / (double)RAND_MAX) >= failure_ranges[2];
  
  int pills;
  
      // Decide randomly if there should be an error
    if ((rand() % 100) < ERROR_RATE) {
        // Introduce a small error: +/- 1
        pills = (rand() % 2) ? c.num_pills_container + 1 : c.num_pills_container - 1;
    } else {
        pills = c.num_pills_container; // No error
    }

    // Output the result
   printf("Number of pills in :%d\n", pills);
   
   medicine->isNo_Pill_Missing = (pills == c.num_pills_container) ? 1: 0; 
 

  printf("pills line %d generated medicine with ID %d\n", line_id, id);
  printf("  Type: %s\n", medicine->type);
  printf("  is Pill missing: %s\n", medicine->isNo_Pill_Missing ? "Yes" : "No");
  printf("  correct size: %s\n", medicine->isCorrect_size ? "Yes" : "No");
  printf("  Color correct: %s\n", medicine->isCorrect_color? "Yes" : "No");
  printf("  Expiry date printed: %s\n", medicine->isExpirydate_printed ? "Yes" : "No");
}

void *employeeFunction(void *arg) {
  int employeeID = *((int *)arg);
  printf("Employee with ID %d of pills line %d is running\n", employeeID, line_id);
 
 int period = rand() % (c.max_package_period - c.min_package_period + 1) + c.min_package_period;

  while (!terminateFlag) {
    pthread_mutex_lock(&buffer_mutex);

    // Wait for the buffer to have at least one medicine or for termination
    while (buffer_count == 0 && !terminateFlag) {
      pthread_cond_wait(&buffer_not_empty, &buffer_mutex);
    }

    // Check the termination flag to exit the loop
    if (terminateFlag && buffer_count == 0) {
      pthread_mutex_unlock(&buffer_mutex);
      break;
    }

    // Process the medicine if available
    if (buffer_count > 0) {
    
      Pills medicine = medicine_buffer[--buffer_count];
    
   
  
    

      // Signal that the buffer has space for new medicines
      pthread_cond_signal(&buffer_not_full);
      pthread_mutex_unlock(&buffer_mutex);

      // Inspection
      // Check if the medicine is accepted
      if (medicine.isNo_Pill_Missing && medicine.isCorrect_size &&
          medicine.isCorrect_color && medicine.isExpirydate_printed ) {

        pthread_mutex_lock(&accepted_mutex); // Lock before accessing accepted_medicines

        accepted_medicines[accepted_count].medicine = medicine;
        accepted_medicines[accepted_count].isPackeged = 0;
        accepted_medicines[accepted_count].isPrescription_added = 0;

        printf("Employee %d from line %d handled medicine with ID %d: %s (Accepted)\n",
               line_id,employeeID, medicine.id, medicine.type);

        // Packaging
        // Random speed for packaging
        //int period = rand() % (c.max_package_period - c.min_package_period + 1) + c.min_package_period;
        
        printf("period for packaging line %d is %d\n",line_id, period);

        sleep(period);

        accepted_medicines[accepted_count].isPrescription_added = 1;
          if(buffer_count % 2 == 0){
          Pills medicine2 =  medicine_buffer[--buffer_count];
          if(strcmp(medicine.type, medicine2.type)){
             accepted_medicines[accepted_count].num_of_containers = c.num_containers_perPackage;
          
          }
      }
       
        accepted_medicines[accepted_count].isPackeged= 1;
      

        accepted_count++;

        // Open shared memory to store the value of packaged medicines
        pthread_mutex_lock(&shared_memory_mutex);
        int shmid2 = shmget(pills_key, sizeof(Numbers_Pill_lines), 0666);
        if (shmid2 == -1) {
          perror("shmget");
          pthread_mutex_unlock(&shared_memory_mutex);
          exit(EXIT_FAILURE);
        }

        Numbers_Pill_lines *shared_data = (Numbers_Pill_lines *)shmat(shmid2, NULL, 0);
        if (shared_data == (void *)-1) {
          perror("shmat");
          pthread_mutex_unlock(&shared_memory_mutex);
          exit(EXIT_FAILURE);
        }

        shared_data[line_id].num_packaged += 1;

        printf("num_packaged from pills line %d is %d\n", line_id, shared_data[line_id].num_packaged);

        detach_shared_memory(shared_data);
        pthread_mutex_unlock(&shared_memory_mutex);

        pthread_mutex_unlock(&accepted_mutex); // Unlock after accessing accepted_medicines

        printf("Employee %d packaged and added Prescription to medicine with ID %d: %s\n",
               employeeID, medicine.id, medicine.type);
      } else {
        printf("Employee %d handled medicine with ID %d: %s (Rejected)\n",
               employeeID, medicine.id, medicine.type);

        // Open shared memory to store the value of rejected medicines
        pthread_mutex_lock(&shared_memory_mutex);
        int shmid2 = shmget(pills_key, sizeof(Numbers_Pill_lines), 0666);
        if (shmid2 == -1) {
          perror("shmget");
          pthread_mutex_unlock(&shared_memory_mutex);
          exit(EXIT_FAILURE);
        }

      Numbers_Pill_lines *shared_data = (Numbers_Pill_lines *)shmat(shmid2, NULL, 0);
        if (shared_data == (void *)-1) {
          perror("shmat");
          pthread_mutex_unlock(&shared_memory_mutex);
          exit(EXIT_FAILURE);
        }

        shared_data[line_id].num_rejected += 1;
        printf("num_rejected from pills line %d is %d\n", line_id, shared_data[line_id].num_rejected);

        detach_shared_memory(shared_data);
        pthread_mutex_unlock(&shared_memory_mutex);
      }
    } else {
      pthread_mutex_unlock(&buffer_mutex);
    }

    // Simulate handling time
    sleep(1);
  }
    printf("Employee %d is exiting.\n", employeeID);
    pthread_exit(NULL);
}

void setup_signal_handlers() {
    struct sigaction sa_add, sa_remove;

    sa_add.sa_handler = add_employee_handler;
    sa_add.sa_flags = 0;
    sigemptyset(&sa_add.sa_mask);
    if (sigaction(SIGUSR1, &sa_add, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa_remove.sa_handler = remove_employee_handler;
    sa_remove.sa_flags = 0;
    sigemptyset(&sa_remove.sa_mask);
    if (sigaction(SIGUSR2, &sa_remove, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    
        struct sigaction sa;
    sa.sa_handler = sigterm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    sigaction(SIGTERM, &sa, NULL);
    
    
}

void sigterm_handler(int sig) {
    terminateFlag = 1;
}


    
void remove_employee_handler(int sig) {
    printf("Received SIGUSR2 for removing an employee\n");

    // Attach to shared memory to access number of employees in this line
    int shmid2 = shmget(pills_key, sizeof(Numbers_Pill_lines), 0666);
    if (shmid2 == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    Numbers_Pill_lines *shared_data = (Numbers_Pill_lines *)shmat(shmid2, NULL, 0);
    if (shared_data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    int num_emp = shared_data[line_id].num_employees;


    if (num_emp > 0) {
        pthread_cancel(employees[num_emp - 1]);  // Corrected index to avoid off-by-one error
        shared_data[line_id].num_employees--;
        printf("Removed employee %d from line %d, total employees: %d\n",
               num_emp, line_id, shared_data[line_id].num_employees);
    } else {
        printf("num_emp <= 0, skipping removal\n");
    }

    
   detach_shared_memory(shared_data); 

}

   
    
    
    

void add_employee_handler(int sig) {

   printf("Received SIGUSR1 for adding an employee\n");

    //attach to shared memory to access number of employees in this line
     int shmid2 = shmget(pills_key, sizeof(Numbers_Pill_lines), 0666);
        if (shmid2 == -1) {
          perror("shmget");
          pthread_mutex_unlock(&shared_memory_mutex);
          exit(EXIT_FAILURE);
        }

        Numbers_Pill_lines *shared_data = (Numbers_Pill_lines *)shmat(shmid2, NULL, 0);
        if (shared_data == (void *)-1) {
          perror("shmat");
          pthread_mutex_unlock(&shared_memory_mutex);
          exit(EXIT_FAILURE);
        }

        int num_emp = shared_data[line_id].num_employees;
    
    
    pthread_t new_employee;
    int *employee_id = malloc(sizeof(int));
    *employee_id = num_emp + 1; // Assign a new ID

    if (pthread_create(&new_employee, NULL, employeeFunction, employee_id) != 0) {
        perror("Failed to create new employee thread");
        exit(EXIT_FAILURE);
    }

    employees = realloc(employees, (num_emp + 1) * sizeof(pthread_t));
    employees[num_emp] = new_employee;
    employeeIDs = realloc(employeeIDs, (num_emp + 1) * sizeof(int));
    employeeIDs[num_emp] = *employee_id;
    shared_data[line_id].num_employees++;

    printf("Added a new employee to line %d, total employees: %d\n", line_id, shared_data[line_id].num_employees);
    
    detach_shared_memory(shared_data);
}

void detach_shared_memory(void *shmaddr) {
  if (shmdt(shmaddr) < 0) {
    perror("shmdt failed");
    exit(EXIT_FAILURE);
  }
}

