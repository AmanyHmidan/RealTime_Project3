#ifndef MEDICINE_H
#define MEDICINE_H

#include "header.h"

#define liquid_key 0x1236
#define pills_key 0x1238

typedef struct{

  int id;
 char *type;
 char semName[30];     // Name of the semaphore for each medicine
 pthread_mutex_t lock; // Mutex for synchronizing access to the medicine

 int isNo_Pill_Missing ;
 int isCorrect_size;
 int isCorrect_color;
 int isExpirydate_printed;


}Pills;

typedef struct{
 int id;
 char *type;
 char semName[30];     // Name of the semaphore for each medicine
 pthread_mutex_t lock; // Mutex for synchronizing access to the medicine
 int isLevel_normal;
 int isColor_correct;
 int isSealed;
 int isLabel_placed;
 int isExpirydate_printed;


}Liquid;

typedef struct {
    char types[5][30];  // Array of types (adjust size as needed)
    int counts[5];      // Array of counts corresponding to each type
   
}Liquid_types;

typedef struct {
    char types[5][30];  // Array of types (adjust size as needed)
    int counts[5];      // Array of counts corresponding to each type
   
}Pills_types;

typedef struct {
   
  Liquid medicine;
  int isBottled;
  int isPrescription_added;
} AcceptedMedicine;

typedef struct {
   
 Pills medicine;
  int isPackeged;
  int isPrescription_added;
  int num_of_containers ; 
} AcceptedMedicine_pill;

typedef struct{
  int pid;
  int num_generated;
  int num_rejected;
  int num_packaged; 
  int num_employees; 

} Numbers_liquid_lines;

typedef struct{
  int pid;
  int num_generated;
  int num_rejected;
  int num_packaged; 
  int num_employees; 

} Numbers_Pill_lines;



#endif
