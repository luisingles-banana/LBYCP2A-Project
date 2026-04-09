#include <stdio.h>
#include <string.h>

int flights_selected[5] = {0};
int flight_tickets[5] = {0};
float flight_price[5] = {0};
float flight_discount[5] = {0};  
int flights_selected_index = 0;

int verify_choice(int choice, int uplimit, int dwlimit){
    if (choice < dwlimit || choice > uplimit) {
        return 1; //error
    } else {
        return 0;
    }
}

int login(char stored_username[][20], char stored_password[][20], int users, int retries){
    int inc = 0;
    char inp_username[20];
    char inp_password[20];
    int success = 0;

    do {
        printf("Username: ");
        scanf(" ");
        gets(inp_username);

        printf("Password: ");
        scanf(" ");
        gets(inp_password);

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

    return success;
}

void flight_info(int flights_avail, char flight_ID[][10],
                 char flight_departure[][10], char flight_arrival[][10],
                 char flight_destination[][30], int flight_seats[],
                 float flight_fare[]){

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

void flight_reservation(int flights_avail, char flight_ID[][10],
                        char flight_departure[][10], char flight_arrival[][10],
                        char flight_destination[][30], int flight_seats[],
                        float flight_fare[]){

    int sel_flight = 0;
    int tickets;

    while (1){
        // Prints the Table
        printf("\n\nYour Reservations:\n");
        printf("%-5s %-8s %-8s %-8s %-20s %-10s %-10s %-10s\n",
               "No.", "ID", "Depart", "Arrive", "Destination", "Tickets", "Discount", "Price");

        if (flights_selected_index > 0){
            printf("-------------------------------------------------------------------------------\n");
            for (int i = 0; i < flights_selected_index; i++) {
                int idx = flights_selected[i] - 1;
                printf("%-5d %-8s %-8s %-8s %-20s %-10d %-10.2f %-10.2f\n",
                    i + 1,
                    flight_ID[idx],
                    flight_departure[idx],
                    flight_arrival[idx],
                    flight_destination[idx],
                    flight_tickets[i],
                    flight_discount[i],
                    flight_price[i]);
            }
            printf("-------------------------------------------------------------------------------\n");
        } else {
            printf("No Reserved Flights.\n");
        }

        int exit;
        // Initial Options
        printf("Enter '0' to exit, '1' to add flight, '2' to remove: ");
        scanf("%d", &exit);

        // Exiting
        if (exit == 0) break;

        if (exit == 1 && flights_selected_index == flights_avail){
            printf("You Already Have All Flights!");
            continue;
        }

        // Remove Flight
        if (exit == 2){
            if (flights_selected_index == 0){
                printf("Nothing to remove!\n");
                continue;
            }

            while (1){
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

                (flights_selected_index)--;
                printf("Reservation removed!\n");
                break;
            }

            continue;
        }

        // Select Flight
        while (1){
            printf("\nSelect a Flight: ");
            scanf(" %d", &sel_flight);

            if (verify_choice(sel_flight, flights_avail, 1)){
                printf("Invalid Input!\n");
                continue;
            } else if (flight_seats[sel_flight - 1] == 0){
                printf("No More Seats Available!\n");
                continue;
            }

            // Checks if the flight is already selected
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

            // Appends selected flight to the array
            flights_selected[flights_selected_index] = sel_flight;
            break;
        }

        // Selecting Tickets
        while (1){
            printf("\nHow many tickets: ");
            scanf("%d", &tickets);

            if (verify_choice(tickets, flight_seats[sel_flight - 1], 1)){
                printf("Invalid Input!\n");
                continue;
            } else {
                flight_tickets[flights_selected_index] = tickets;
                flight_seats[sel_flight - 1] -= tickets;  // deduct seats
                // Dont Compute Price Yet
                break;
            }
        }

        char apply_discount;
        int discount_type;
        char sure;
        int seniorpwd_discount = 0;
        int manager_discount = 0;
        float discount_perc = 0.0;
        int discounted_tickets;
        int ticketsleft_discount = tickets;
 
        // Discount selection loop
        while (1){
            printf("\nDo you qualify for a discount? (y/n): ");
            scanf(" %c", &apply_discount);
 
            switch (apply_discount){
                case 'y':                   
                    while (1){
                        if (seniorpwd_discount && manager_discount){
                            printf("You already have all available discounts!\n");
                            break;
                        }
 
                        printf("\nSelect Type:\n");
                        printf("1 - Senior/PWD\n");
                        printf("2 - Manager\n");
                        printf("0 - Done\n");
                        printf("Input Option: ");
                        scanf(" %d", &discount_type);
 
                        if (discount_type == 0) break;
 
                        if (discount_type == 1 && seniorpwd_discount && ticketsleft_discount != 0){
                            printf("You already have this discount!\n");
                            continue;
                        }
                        if (discount_type == 2 && manager_discount && ticketsleft_discount != 0){
                            printf("You already have this discount!\n");
                            continue;
                        }
                        if (discount_type != 1 && discount_type != 2){
                            printf("Invalid Input!\n");
                            continue;
                        }
 
                        // Confirm Selection
                        int confirmed = 0;
                        while (!confirmed){
                            printf("Are you sure? (y/n): ");
                            scanf(" %c", &sure); 
 
                            switch (sure){
                                case 'y':           
                                    confirmed = 1;
                                    break;
                                case 'n':           
                                    confirmed = 1;       // exits the current loop and asks again the discount type
                                    discount_type = -1;  
                                    break;
                                default:
                                    printf("Invalid Input!\n");
                                    break;
                            }
                        }
 
                        if (discount_type == -1){ // user is not sure
                            continue;
                        }
                        
                        switch (discount_type){
                            case 1:
                                discount_perc += 0.20;
                                seniorpwd_discount = 1;
                                break;
                            case 2:
                                discount_perc += 0.30;
                                manager_discount = 1;
                                break;
                        }

                        // Ask how many of the tickets get this discount
                        while (1){
                            if (ticketsleft_discount == 0){
                                printf("Maximum Tickets Reached!\n");
                                break;
                            } 
                            printf("How many people? (1-%d)? ", ticketsleft_discount);
                            scanf(" %d", &discounted_tickets);

                            if (verify_choice(discounted_tickets, ticketsleft_discount, 1)){
                                printf("Invalid Input!\n");
                                continue;
                            }

                            float batch_price = flight_fare[sel_flight - 1] * discounted_tickets;
                            flight_discount[flights_selected_index] += batch_price * discount_perc;
                            flight_price[flights_selected_index]    += batch_price - (batch_price * discount_perc);

                            ticketsleft_discount -= discounted_tickets;
                            break;
                        }
                }
 
                    break;
 
                case 'n':                  
                    flight_discount[flights_selected_index] = 0.0;  // No Discount
                    flight_price[flights_selected_index] = flight_fare[sel_flight - 1] * tickets;  // Computes without discount
                    break;
 
                default:
                    printf("Invalid Input!\n");
                    continue;
            }
            break; 
        }
        flights_selected_index++;
    }
    return;
}

void transaction_summary(char flight_ID[][10], char flight_destination[][30],
                         char flight_departure[][10], char flight_arrival[][10],
                         int flight_seats[], float flight_fare[]) {

    if (flights_selected_index == 0) {
        printf("\nNo reservations yet! Please reserve a flight first.\n");
        return;
    }

    float total_cost = 0;
    printf("\n================ TRANSACTION SUMMARY ================\n");
    printf("%-8s %-20s %-10s %-10s\n",
           "ID", "Destination", "Tickets", "Price");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < flights_selected_index; i++) {
        int idx = flights_selected[i] - 1;
        printf("%-8s %-20s %-10d %-10.2f\n",
               flight_ID[idx],
               flight_destination[idx],
               flight_tickets[i],
               flight_price[i]);
        total_cost += flight_price[i];
    }

    printf("-----------------------------------------------------\n");
    printf("%-38s %-10.2f\n", "TOTAL", total_cost); 

    float payment, change;
    printf("\nTotal amount due: %.2f\n", total_cost);
    printf("Enter payment amount: ");
    scanf("%f", &payment);

    if (payment < total_cost) {
        printf("Insufficient payment! Transaction cancelled.\n");
        return;
    }
    change = payment - total_cost;

    char customer_name[50];
    printf("\nEnter your name: ");
    scanf(" ");
    gets(customer_name);


    printf("\n================ RECEIPT ================\n");
    printf("Customer: %s\n", customer_name);
    for (int i = 0; i < flights_selected_index; i++) {
        int idx = flights_selected[i] - 1;
        printf("Flight: %s | Destination: %s | Tickets: %d | Price: %.2f\n",
               flight_ID[idx], flight_destination[idx],
               flight_tickets[i], flight_price[i]);
    }
    printf("Total Cost: %.2f\n", total_cost);
    printf("Payment:    %.2f\n", payment);
    printf("Change:     %.2f\n", change);
    printf("=========================================\n");
    printf("\nThank you for booking with the Plane Airlines! Safe skies!\n");

    int again;
    printf("\nWant to book another flight?\n");
    printf("1 - Book again\n");
    printf("0 - Return to main menu\n");
    printf("Input Option: ");
    scanf("%d", &again);

    if (again == 1) {
        flights_selected_index = 0;                      
        for (int i = 0; i < 5; i++) {
            flights_selected[i] = 0;
            flight_tickets[i]   = 0;
            flight_price[i]     = 0.0;
            flight_discount[i]  = 0.0;
        }
        printf("\nStarting a new booking...\n");
        flight_reservation(5, flight_ID, flight_departure, 
                            flight_arrival, flight_destination, 
                            flight_seats, flight_fare);
    } else {
        printf("\nReturning to main menu...\n");
    }
    return;
}

int main() {

    /*------------------------------------------------------------------------------------------------------------------------

    Inputting Credentials

    ------------------------------------------------------------------------------------------------------------------------*/
    
    char stored_username[3][20] = {"STUDENT", "ADMIN", "TRAVELER"};
    char stored_password[3][20] = {"TAFT2401", "FLYHIGH", "SKYWAY"};
    int retries = 3;
    int users = 3;

    if (login(stored_username, stored_password, users, retries) == 0){
        printf("Too many attempts.\n");
        return 1;
    }
    printf("Login successful!\n\n\n\n");
    
    /*------------------------------------------------------------------------------------------------------------------------

    Actual Program

    ------------------------------------------------------------------------------------------------------------------------*/

    char flight_ID[5][10]          = {"FL01",    "FL02",    "FL03",      "FL04",      "FL05"   };
    char flight_departure[5][10]   = {"1:00",    "2:00",    "3:00",      "4:00",      "5:00"   };
    char flight_arrival[5][10]     = {"6:00",    "7:00",    "8:00",      "9:00",      "10:00"  };
    char flight_destination[5][30] = {"Japan",   "Peru",    "Thailand",  "Singapore", "Brazil" };
    int  flight_seats[5]           = {50,        60,        70,          80,          90       };
    float flight_fare[5]           = {11000.00,  20000.00,  30000.00,    40000.00,    50000.00 };
    int flights_avail = 5;

    int option;
    int dump;
    while (1){
        printf("\n\nWelcome to Plane Airlines!\n");
        printf("What would you like to do:\n");
        printf("1 - View Available Flights\n");
        printf("2 - Reserve a Flight\n");
        printf("3 - View your transactions\n");
        printf("Input Option: ");
        scanf(" %d", &option);

        switch (option){
            case 1:
                flight_info(flights_avail, flight_ID,
                            flight_departure, flight_arrival,
                            flight_destination, flight_seats,
                            flight_fare);

                while (1){
                    printf("Input '0' to continue.");
                    scanf("%d", &dump);
                    if (dump == 0){
                        break;
                    } else {
                        printf("Invalid Input!\n");
                        continue;
                    }
                }
                break;
            case 2:
                flight_reservation(5, flight_ID, flight_departure, 
                                   flight_arrival, flight_destination, 
                                   flight_seats, flight_fare);
                break;
            case 3:
                transaction_summary(flight_ID, flight_destination,
                                    flight_departure, flight_arrival,
                                    flight_seats, flight_fare);
                break;
            default:
                printf("Invalid Input!");
                break;
        }
    }

    return 0;
}