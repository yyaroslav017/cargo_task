#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_VEHICLES = 100000; // num of vehicles, limit

typedef struct {
    int from;
    int to;
    int capacity;
    int rent;
} Vehicle;

Vehicle vehicles[MAX_VEHICLES]; // vehicles array
int vehicle_count = 0;   // vehicle count

// 
void trim_input(char *input) {
    while (*input == ' ' || *input == '\n') {
        input++;
    }

    // trimming
    char *end = input + strlen(input) - 1;
    while (end > input && (*end == ' ' || *end == '\n')) {
        end--;
    }

    // string termintation
    *(end + 1) = '\0';
}

void parse_vehicle_list(const char *input) {
    char input_copy[100000];
    strcpy(input_copy, input);

    
    trim_input(input_copy);

    // Check if the input starts and ends with curly brackets
    if (input_copy[0] != '{' || input_copy[strlen(input_copy) - 1] != '}') {
        printf("Invalid input.\n");
        exit(1);
    }

    // Remove the curly brackets for easier processing
    input_copy[strlen(input_copy) - 1] = '\0'; // Remove the closing '}'
    char *ptr = input_copy + 1;  // Skip the opening '{'

    // Parsing vehicles inside the curly brackets
    while (*ptr && vehicle_count < MAX_VEHICLES) {
        if (*ptr == '[') {
            int from, to, capacity, rent;
            // Try to match the vehicle format
            if (sscanf(ptr, "[ %d - %d , %d , %d ]", &from, &to, &capacity, &rent) != 4) {
                printf("Invalid input.\n");
                exit(1);
            }
            if (from < 0 || to < 0 || from > to || capacity <= 0 || rent <= 0) {
                printf("Invalid input.\n");
                exit(1);
            }

            // Add the vehicle to the list
            Vehicle new_vehicle = {from, to, capacity, rent};
            vehicles[vehicle_count++] = new_vehicle;

            // Move pointer past this vehicle's data
            while (*ptr != ']' && *ptr != '\0') {
                ptr++;
            }
        }
        ptr++;
    }
}

void calculate_delivery(int start_day, int cargo) {
    int total_cost = 0;
    int finish_day = start_day;
    int remaining_cargo = cargo;

    int max_day = 0; // no inf loop occurrence

    // max day locator
    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].to > max_day) {
            max_day = vehicles[i].to;
        }
    }

    // Process the days
    for (int day = start_day; remaining_cargo > 0 && day <= max_day; day++) {
        int daily_capacity = 0;

        // All vehicles iteration
        for (int i = 0; i < vehicle_count; i++) {
            if (vehicles[i].from <= day && vehicles[i].to >= day) {
                daily_capacity += vehicles[i].capacity;
                total_cost += vehicles[i].rent;
            }
        }

        // Reducing capacity
        remaining_cargo -= daily_capacity;

        // Updating finish_day if the cargo is fully delivered on this day
        if (remaining_cargo <= 0) {
            finish_day = day;
        }
    }

  
    if (remaining_cargo <= 0) {
        printf("Finish: %d, cost: %d\n", finish_day, total_cost);
    } else {
        printf("Cargo too big, not enough capacity.\n");
    }
}

int main() {
    char input[100000];

    // Input
    printf("Cargo delivery:\n");
    if (!fgets(input, sizeof(input), stdin)) {
        printf("Invalid input.\n");
        return 1;
    }
    parse_vehicle_list(input);

    // Calculations from functions
    int start_day, cargo;
    printf("Cargo:\n");
    while (scanf("%d %d", &start_day, &cargo) == 2) {
        if (start_day < 0 || cargo <= 0) {
            printf("Invalid input.\n");
            return 1;
        }
        calculate_delivery(start_day, cargo);
    }

    return 0;
}

