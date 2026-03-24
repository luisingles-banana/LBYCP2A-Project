#include <stdio.h>
#include <string.h>

//#include "myhead.h"

// strcpy(str1, str2); this string function copies the content of string 2 to string 1.
// puts(string_var); a string output function. This function displays the string value
// strlen(str); this function returns the length of the string.
// strcat(str1, str2); it appends the string2 to the end of string 1.
// strlwr(str); this string function converts all uppercase letters to lowercase.
// strupr(str); this string function converts all lowercase letters to uppercase.
// strrev(str); reverses all the characters in the string.
// strcmp(str1, str2); compares two strings. If string1>string2, the function returns a positive value; if string 1<string2, the function returns a negative value. If string 1 == string2, the function returns a zero value.
// strcmpi(str1, str2); same with strcmp() except that lowercase and uppercase letters are treated equal.

int verify_choice(int choice, int uplimit, int dwlimit){
    if (choice < dwlimit || choice > uplimit) {
        return 1; //error
    } else {
        return 0;
    }
}

int login(char *stored_username[], char *stored_password[], int users, int retries){
    int inc = 0;
    char inp_username[20];
    char inp_password[20];
    int success = 0;

    do {
        printf("Username: ");
        scanf("%19s", inp_username); // use gets

        printf("Password: ");
        scanf("%19s", inp_password); // use gets

        for (int x = 0; x < users; x++) {
            if (strcmp(inp_username, stored_username[x]) == 0 && strcmp(inp_password, stored_password[x]) == 0) {
                success = 1;
                break;
            }
        }

        if (success) break;

        printf("Invalid login.\n\n");
        inc++;

    } while (inc < retries);

    return success;  // 1 = success, 0 = failed
}

void flight_info(int flights_avail, char *flight_ID[], 
                 char *flight_departure[], char *flight_arrival[], 
                 char *flight_destination[], int flight_seats[], 
                 float flight_fare[]){
    
    // Prints the flights as a table
    // Change the spacing to what ever you'd like
    printf("%-5s %-8s %-8s %-8s %-20s %-10s %-10s\n",
           "No.", "ID", "Depart", "Arrive", "Destination", "Seats", "Fare");
    printf("-------------------------------------------------------------------------------\n");

    for (int i = 0; i < flights_avail; i++) {
        printf("%-5d %-8s %-8s %-8s %-20s %-10d %-10.2f\n",
               i + 1,
               flight_ID[i],
               flight_departure[i],
               flight_arrival[i],
               flight_destination[i],
               flight_seats[i],
               flight_fare[i]);
    }
    printf("-------------------------------------------------------------------------------\n");
    return;
}

void flight_reservation(int flights_avail, char *flight_ID[], 
                        char *flight_departure[], char *flight_arrival[], 
                        char *flight_destination[], int flight_seats[], 
                        float flight_fare[]){

    int sel_flight = 0;

    int flights_selected[flights_avail];
    int flights_selected_index = 0;

    int tickets;
    int flight_tickets[flights_avail];
    float flight_price[flights_avail];

    while (1){
        printf("\n\nYour Reservations:\n");
        printf("%-5s %-8s %-8s %-8s %-20s %-10s %-10s\n",
            "No.", "ID", "Depart", "Arrive", "Destination", "Tickets", "Price");

        if (flights_selected_index > 0){
            printf("-------------------------------------------------------------------------------\n");
            for (int i = 0; i < flights_selected_index; i++) {
                int idx = flights_selected[i] - 1;
                printf("%-5d %-8s %-8s %-8s %-20s %-10d %-10.2f\n",
                    flights_selected[i],
                    flight_ID[idx],
                    flight_departure[idx],
                    flight_arrival[idx],
                    flight_destination[idx],
                    flight_tickets[i],
                    flight_price[i]);
            }
            printf("-------------------------------------------------------------------------------\n");
        } else {
            printf("No Reserved Flights.\n");
        }

        // If all flights are
        if (flights_selected_index == flights_avail){
            printf("Maximum flights selected!\n");
            int exit;
            printf("Enter '0' to exit, '2' to remove a flight: ");
            scanf("%d", &exit);
            if (exit == 0) break;
            if (exit == 2) goto remove;  // jump to remove ; tinuro ba toh ni sir?
            continue;
        }

        int exit;
        printf("Enter '0' to exit, '1' to add flight, '2' to remove: ");
        scanf("%d", &exit);
        if (exit == 0) break;

        // remove flight
        if (exit == 2){
            remove:;
            if (flights_selected_index == 0){
                printf("Nothing to remove!\n");
                continue;
            }

            int rem;
            printf("Enter reservation number to remove (1-%d): ", flights_selected_index);
            scanf("%d", &rem);

            if (verify_choice(rem, flights_selected_index, 1)){
                printf("Invalid Input!\n");
                continue;
            }

            rem = rem - 1;  // convert to index

            // restore seats
            flight_seats[flights_selected[rem] - 1] += flight_tickets[rem];

            // shift everything left to fill the gap
            for (int i = rem; i < flights_selected_index - 1; i++){
                flights_selected[i] = flights_selected[i + 1];
                flight_tickets[i]   = flight_tickets[i + 1];
                flight_price[i]     = flight_price[i + 1];
            }

            flights_selected_index--;
            printf("Reservation removed!\n");
            continue;
        }

        // Select flight
        while (1){        
            printf("\nSelect a Flight: ");
            scanf("%d", &sel_flight);

            if (verify_choice(sel_flight, flights_avail, 1)){
                printf("Invalid Input!\n");
                continue;
            } else if (flight_seats[sel_flight - 1] == 0){
                printf("No More Seats Available!\n");
                continue;
            }
            
            // check if flight already selected
            int duplicate = 0;
            for (int i = 0; i < flights_selected_index; i++){
                if (flights_selected[i] == sel_flight){
                    duplicate = 1;
                    break;
                }
            }
            if (duplicate){
                printf("Flight already reserved!\n");
                continue;
            }

            flights_selected[flights_selected_index] = sel_flight;
            break;
        }

        // Select tickets
        while (1){
            printf("\nHow many tickets: ");
            scanf("%d", &tickets);

            if (verify_choice(tickets, flight_seats[sel_flight - 1], 1)){
                printf("Invalid Input!\n");
                continue;
            } else {
                flight_tickets[flights_selected_index] = tickets;
                flight_price[flights_selected_index] = tickets * flight_fare[sel_flight - 1];
                flight_seats[sel_flight - 1] -= tickets;  // deduct seats
                break;
            }
        }

        flights_selected_index++;
    }

    return;
}

int main() {

    /*------------------------------------------------------------------------------------------------------------------------
    
    Inputting Credentials

    ------------------------------------------------------------------------------------------------------------------------*/

    char *stored_username[] = {"STUDENT", "ADMIN", "TRAVELER"};  // DONT USE POINTERS
    char *stored_password[] = {"TAFT2401", "FLYHIGH", "SKYWAY"}; // DONT USE POINTERS
    int retries = 3;
    int users = sizeof(stored_username)/sizeof(stored_username[0]); // Gets the lenght of the 'stored_username' Array to 
                                                                    // determine how many users are available.

    if (login(stored_username, stored_password, users, retries) == 0){
        printf("Too many attempts.\n");
        return 1; //error
    }
    printf("Login successful!\n\n\n\n");

    /*------------------------------------------------------------------------------------------------------------------------
    
    Actual Program

    ------------------------------------------------------------------------------------------------------------------------*/

    char *flight_ID[] =                 {"FL01",     "FL02",     "FL03",            "FL04",      "FL05"   }; // DONT USE POINTERS
    char *flight_departure[] =          {"1:00",     "2:00",     "3:00",            "4:00",      "5:00"   }; // DONT USE POINTERS
    char *flight_arrival[] =            {"6:00",     "7:00",     "8:00",            "9:00",      "10:00"  }; // DONT USE POINTERS
    char *flight_destination[] =        {"Japan",    "Ewan",     "San pa ba pwede", "bahay",     "doon"   }; // DONT USE POINTERS
    int flight_seats[] =                {50,         60,         70,                80,          90       }; // DONT USE POINTERS
    float flight_fare[] =               {10000.00,   20000.00,   30000.00,          40000.00,    50000.00 }; // DONT USE POINTERS
    int flights_avail = sizeof(flight_ID)/sizeof(flight_ID[0]); // Gets the lenght of the Flight_ID Array to determine how 
                                                                // many flights are available.

    int option;
    int dump;
    while (1){
        printf("Welcome to yon yon basta Airlines!\n");
        printf("What would you like to do:\n");
        printf("1: View Available Flights\n");
        printf("2: Reserve a Flight\n");
        printf("3: View your transactions\n");
        printf("Input Option: ");
        scanf(" %d", &option);

        switch (option){
            case 1:
                printf("You chose option 1!\n\n");
                flight_info(flights_avail, flight_ID, 
                            flight_departure, flight_arrival, 
                            flight_destination, flight_seats, 
                            flight_fare);

                printf("Press any key to continue.");
                getchar(); //tinuro ba ni sir toh? Doesnt work
                break;
            case 2:
                printf("You chose option 2!\n\n");
                flight_reservation(flights_avail, flight_ID, 
                                   flight_departure, flight_arrival, 
                                   flight_destination, flight_seats, 
                                   flight_fare);
                break;
            case 3:
                printf("You chose option 3!");
                break;
            default:
                printf("Invalid Input!");
                break; 
        }
    }


    return 0;
}
