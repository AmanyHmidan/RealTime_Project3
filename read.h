#include "header.h"

typedef struct {
    
    int num_of_medicines;
    int num_of_Liquid_lines;
    int num_of_Pills_lines;
    int num_employees_line;
    int max_failures_rang;
    int min_failures_rang;
    int max_failuresType;
    int min_failuresType;
    int threshold_liquid_produced;
    int threshold_pills_produced;
    int threshold_bottled_out;
    int threshold_pill_based_out;
    int simulation_time;
    int min_Generate_period;
    int max_Generate_period ;
    int min_package_period ;
    int max_package_period;
    double THRESHOLD_RATIO;
    int num_pills_container;
    int num_containers_perPackage;
    
    int terminateFlag;
    
} Config;

// Function to read config from file
Config read_config(const char *filename) {
    Config config = {0};
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        
        fscanf(file, "num_of_medicines = %d\n", &config.num_of_medicines);
        fscanf(file, "num_of_Liquid_lines = %d\n", &config.num_of_Liquid_lines);
        fscanf(file, "num_of_Pills_lines = %d\n", &config.num_of_Pills_lines);
        fscanf(file, "num_employees_line = %d\n", &config.num_employees_line);
        fscanf(file, "max_failures_rang = %d\n", &config.max_failures_rang);
        fscanf(file, "min_failures_rang = %d\n", &config.min_failures_rang);
        fscanf(file, "max_failuresType = %d\n", &config.max_failuresType);
        fscanf(file, "min_failuresType = %d\n", &config.min_failuresType);
        fscanf(file, "threshold_liquid_produced = %d\n", &config.threshold_liquid_produced);
        fscanf(file, "threshold_pills_produced = %d\n", &config.threshold_pills_produced);
        fscanf(file, "threshold_bottled_out = %d\n", &config.threshold_bottled_out);
        fscanf(file, "threshold_pill_based_out = %d\n", &config.threshold_pill_based_out);
        fscanf(file, "simulation_time = %d\n", &config.simulation_time);
        fscanf(file, "min_Generate_period = %d\n", &config.min_Generate_period);
        fscanf(file, "max_Generate_period = %d\n", &config.max_Generate_period);
        fscanf(file, "min_package_period = %d\n", &config.min_package_period);
        fscanf(file, "max_package_period = %d\n", &config.max_package_period);
        fscanf(file, "THRESHOLD_RATIO = %lf\n", &config.THRESHOLD_RATIO);
        fscanf(file, "num_pills_container = %d\n", &config.num_pills_container);
        fscanf(file, "num_containers_perPackage = %d\n", &config.num_containers_perPackage);
        fclose(file);
    } else {
        fprintf(stderr, "Error: Unable to open the configuration file.\n");
        exit(EXIT_FAILURE);
    }
    
    config.terminateFlag = 0;

    return config;
}

