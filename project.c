#include <stdio.h>
#include <string.h>

int flights_selected[10];      
int flight_tickets[10];        
float flight_price[10];        
int flights_selected_index = 0; // para macall din sa transaction summary, nagnnot declared in this scope e

int verify_choice(int choice, int uplimit, int dwlimit){ //LUISKEVIN
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
        scanf("%19s", inp_username); //use fgets or gets

        printf("Password: ");
        scanf("%19s", inp_password); //use fgets or gets

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
    int tickets;

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
		
        int exit;
        printf("Enter '0' to exit, '1' to add flight, '2' to remove: ");
        scanf("%d", &exit);
        if (exit == 0) break;


		// remove flight
        if (exit == 2){ // removed remove:;, removed din yung goto mo sa taas^^^
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

        flights_selected_index++; //LUISKEVIN
    }

    return;
}

void transaction_summary(char *flight_ID[], char *flight_destination[],
                         char *flight_departure[], char *flight_arrival[],
                         int flight_seats[], float flight_fare[]) {
    if (flights_selected_index == 0) {
        printf("\nNo reservations yet! Please reserve a flight first.\n");
        return;
    }

    char customer_name[50];
    printf("\nEnter your name: ");
    scanf("%49s", customer_name);

    float total_cost = 0;
    printf("\n================ TRANSACTION SUMMARY ================\n");
    for (int i = 0; i < flights_selected_index; i++) {
        int idx = flights_selected[i] - 1;
        printf("Flight: %s | Destination: %s | Tickets: %d | Price: %.2f\n",
               flight_ID[idx], flight_destination[idx],
               flight_tickets[i], flight_price[i]);
        total_cost += flight_price[i];
    }
    printf("-----------------------------------------------------\n");
    printf("Total Cost (No discount applied): %.2f\n", total_cost);

    int discount_type;
    float discount = 0;
    printf("\nDo you qualify for a discount?\n");
    printf("1: None\n2: Senior Citizen/PWD (20%%)\n3: Manager's Discount (30%%)\n");
    printf("\nEnter applicable discount: ");
    scanf("%d", &discount_type);

    if (discount_type == 2) discount = total_cost * 0.20;
    else if (discount_type == 3) discount = total_cost * 0.30;

    total_cost -= discount;

    float payment, change;
    printf("\nTotal cost after discount: %.2f\n", total_cost);
    printf("\nEnter payment amount: ");
    scanf("%f", &payment);

    if (payment < total_cost) {
        printf("Insufficient payment! Transaction cancelled.\n"); 
        return;
    }

    change = payment - total_cost;

    printf("\n================ RECEIPT ================\n");
    printf("Customer: %s\n", customer_name);
    for (int i = 0; i < flights_selected_index; i++) {
        int idx = flights_selected[i] - 1;
        printf("Flight: %s | Destination: %s | Tickets: %d | Price: %.2f\n",
               flight_ID[idx], flight_destination[idx],
               flight_tickets[i], flight_price[i]);
    }
    if (discount_type == 2) printf("Discount Applied: Senior/PWD (20%%)\n");
    else if (discount_type == 3) printf("Discount Applied: Manager (30%%)\n");
    else printf("Discount Applied: None\n");
    printf("Total Cost: %.2f\n", total_cost);
    printf("Payment: %.2f\n", payment);
    printf("Change: %.2f\n", change);
    printf("=========================================\n");
    printf("\nThank you for booking with Yon Yon Basta Airlines! Safe skies!\n"); //mag ingat kang animal ka

    int again;
	printf("\nWant to book another flight?\n"); //book ka ulet para ubos pera mo
	printf("Press 1 to book again, 0 to return to main menu: ");
	scanf("%d", &again);

	if (again == 1) {
    	flights_selected_index = 0;
    	printf("\nStarting a new booking...\n");
    	flight_reservation(5, flight_ID, flight_departure, flight_arrival,
                       flight_destination, flight_seats, flight_fare);
} else {
    printf("\nReturning to main menu...\n");
}

return; 
}
int main() { //LKMI

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
    char *flight_destination[] =        {"Japan",    "Peru",     "Thailand",        "Singapore", "Brazil" }; // DONT USE POINTERS
    int flight_seats[] =                {50,         60,         70,                80,          90       }; // DONT USE POINTERS
    float flight_fare[] =               {11000.00,   20000.00,   30000.00,          40000.00,    50000.00 }; // DONT USE POINTERS
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
                //printf("You chose option 1!\n\n");
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
                //printf("You chose option 2!\n\n");
                flight_reservation(flights_avail, flight_ID, 
                                   flight_departure, flight_arrival, 
                                   flight_destination, flight_seats, 
                                   flight_fare);
                break;
            case 3: // LKMI
    			//printf("You chose option 3!\n\n");
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