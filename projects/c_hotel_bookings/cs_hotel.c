// cs_hotel
//
// This program was written by Zach Wan (z5417138)
// in 2022
//
// This program showcases information about a hotel and its rooms.

#include <stdio.h>

// Additional libraries here

// Provided constants
#define TRUE 1
#define FALSE 0

#define TOTAL_FLOORS 8
#define ROOMS_PER_FLOOR 8
#define INITIAL_PRICE 10
#define NO_BOOKING -1

// Your constants here
#define LOWEST_FLOOR 0
#define LOWEST_ROOM_NUMBER 0
#define HIGHEST_FLOOR 7
#define HIGHEST_ROOM_NUMBER 7


// Provided struct
struct room {
    int booking_id;
    int price;
    int royal_guest;
    int group_booking;
};

// Provided functions use for game setup
// You do not need to use these functions yourself.
void init_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

// You will need to use these functions for stage 1.
void print_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);
void print_hotel_with_prices(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]);

// Your function prototypes here:
void invalid_room_number(void);
int find_best_room_row(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                       int row, int column);
int find_best_room_column(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                          int row, int column);
int fill_cheapest_bookings(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                            int row, int column, int num_guests, 
                            int usable_floors[ROOMS_PER_FLOOR], int booking_id);
void change_floor_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                        int row, int column, int floor_number, int new_price);
void change_hotel_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                        int row, int column, int amount);
int sum_revenues(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], int row, 
                 int column, int revenue);
int add_group_booking(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                      int row, int column, int num_guests, 
                      int usable_floors[ROOMS_PER_FLOOR], int booking_id);
int add_group_booking_with_budget(struct room 
                                  hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                                  int row, int column, int budget, 
                                  int num_guests, 
                                  int usable_floors[ROOMS_PER_FLOOR], 
                                  int booking_id);
int insert_royal_guests(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                        int floor_number, int room_on_floor, int new_booking_id,
                        int booking_id);
int check_room_for_displaced(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                            int find_row, int find_column, int floor_number, 
                            int room_on_floor, int new_booking_id, 
                            int booking_id);
int remove_guests_for_royalty(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                              int row, int column, int current_group_id, 
                              int num_guests);
void group_booking_with_past_id(struct room 
                                hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], int row, 
                                int column, int num_guests, 
                                int usable_floors[TOTAL_FLOORS], 
                                int current_group_id, int booking_id);


int main(void) {
    struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR];
    init_hotel(hotel);

    printf("Welcome to the CS Hotel!!\n\n");

    // Set-up of the initial bookings in the hotel.

    int i = 0;
    int row = 0;
    int column = 0;
    int num_guests;
    int floor_number;
    int room_on_floor;
    int booking_id = 0;
    int new_booking_id = 0;
    int find_row;
    int find_column;
    
    printf("Add your current guests, how many guests? ");
    scanf("%d", &num_guests);
    printf("Input in your guests room numbers\n");
    
    while (i < num_guests) {
        scanf("%d %d", &floor_number, &room_on_floor);
        
        if (floor_number < LOWEST_FLOOR || 
        floor_number > HIGHEST_FLOOR || 
        room_on_floor < LOWEST_ROOM_NUMBER || 
        room_on_floor > HIGHEST_ROOM_NUMBER) {
            invalid_room_number();
        
        } else if (hotel[floor_number][room_on_floor].booking_id > NO_BOOKING) {
            printf("Room already occupied!\n");
        
        } else {
            hotel[floor_number][room_on_floor].booking_id = booking_id;
            booking_id++;
        }
        i++;
    }

    print_hotel(hotel);
    printf("Enter a command: ");
    
    // The following code represents the commands that can be input.
    
    char input;
    int past_revenue = 0;
    while (scanf(" %c", &input) != EOF) {
    
        // When 'p' is input, print the hotel.
        if (input == 'p') {
            print_hotel(hotel);
        
            // When '?' is input, check if a room is occupied.
        } else if (input == '?') {
            scanf("%d %d", &floor_number, &room_on_floor);
            
            if (floor_number < LOWEST_FLOOR || 
                floor_number > HIGHEST_FLOOR || 
                room_on_floor < LOWEST_ROOM_NUMBER || 
                room_on_floor > HIGHEST_ROOM_NUMBER) {
                invalid_room_number();
            
            } else if (hotel[floor_number][room_on_floor].booking_id 
                       == NO_BOOKING) {
                printf("Floor %d, Room %d is empty.\n", floor_number, 
                       room_on_floor);
            
            } else {
                printf("Booking ID at Floor %d, Room %d is : %d\n", 
                       floor_number, room_on_floor, 
                       hotel[floor_number][room_on_floor].booking_id);
            }
        
            // When 'a' is input, add a booking to the hotel. 
        } else if (input == 'a') {
            scanf("%d %d", &floor_number, &room_on_floor);
        
            if (floor_number < LOWEST_FLOOR || 
                floor_number > HIGHEST_FLOOR || 
                room_on_floor < LOWEST_ROOM_NUMBER || 
                room_on_floor > HIGHEST_ROOM_NUMBER) {
                invalid_room_number();
            
            } else if (hotel[floor_number][room_on_floor].booking_id 
                       > NO_BOOKING) {
                printf("Room already occupied!\n");
            
            } else {
                hotel[floor_number][room_on_floor].booking_id = booking_id;
                printf("Guest's booking id is: %d\n", 
                       hotel[floor_number][room_on_floor].booking_id);
                booking_id++;
            }
        
            // When 'l' is input, remove a booking from the hotel.
        } else if (input == 'l') {
            scanf("%d %d", &floor_number, &room_on_floor);
            
            if (floor_number < LOWEST_FLOOR || 
                floor_number > HIGHEST_FLOOR || 
                room_on_floor < LOWEST_ROOM_NUMBER || 
                room_on_floor > HIGHEST_ROOM_NUMBER) {
                invalid_room_number();
            
            } else if (hotel[floor_number][room_on_floor].booking_id 
                       == NO_BOOKING) {
                printf("This room is already empty.\n");
            
            } else {
                // Reset all characteristics of the room.
                printf("Goodbye booking %d!\n", 
                       hotel[floor_number][room_on_floor].booking_id);
                hotel[floor_number][room_on_floor].booking_id = NO_BOOKING;
                hotel[floor_number][room_on_floor].royal_guest = FALSE;
                hotel[floor_number][room_on_floor].group_booking = FALSE;  
            }
        
            // When 'c' is input, print the hotel with prices. 
        } else if (input == 'c') {
            print_hotel_with_prices(hotel);
        
            // When '$' is input, change the price of rooms on a chosen floor.
        } else if (input == '$') {
            int new_price;
            scanf("%d %d", &floor_number, &new_price);
        
            if (floor_number < LOWEST_FLOOR || floor_number > HIGHEST_FLOOR) {
                printf("Invalid floor number!\n");
            
            } else if (new_price < 0) {
                printf("Invalid price!\n");
            
            } else {
                change_floor_price(hotel, row, column, floor_number, new_price);
            }
        
            // When '^' is input, change the price of all rooms in the hotel. 
        } else if (input == '^') {
            int amount;
            scanf("%d", &amount);
            
            change_hotel_price(hotel, row, column, amount);
                        
            // When 'r' is input, calculate the revenue earned and compare it to 
            // past revenue. 
        } else if (input == 'r') {
            int revenue = 0;
            int revenue_change;
            
            int current_revenue = sum_revenues(hotel, row, column, revenue);
            
            revenue_change = current_revenue - past_revenue;
            
            if (current_revenue > past_revenue) {
                printf("The total revenue for the night is $%d\n", 
                       current_revenue);
                printf("This is $%d more than last time :)\n", revenue_change);
                
            } else if (current_revenue < past_revenue) {
                printf("The total revenue for the night is $%d\n", 
                       current_revenue);
                printf("This is $%d less than last time (haha lol)\n", 
                       revenue_change / -1);
                
            } else {
                printf("The total revenue for the night is $%d\n", 
                       current_revenue);
                printf("This is the same as the last time you asked :\\ \n"); 
            }
            
            // Update the value of past_revenue which exists outside this body
            // of code so that future inputs of 'r' will remember the
            // previous day's revenue. 
            past_revenue = current_revenue;
        
            // When 'j' is input, find the best available room and add a guest. 
        } else if (input == 'j') {
            
            find_row = find_best_room_row(hotel, row, column);
            find_column = find_best_room_column(hotel, row, column);
            
            if (hotel[find_row][find_column].booking_id == NO_BOOKING) {
                hotel[find_row][find_column].booking_id = booking_id; 
                booking_id++;
                
            } else {
                printf("The hotel is full!\n");
            }
        
            // When 's' is input, swap all the characteristics of two bookings. 
        } else if (input == 's') {
            int floor_a;
            int room_on_floor_a;
            int floor_b;
            int room_on_floor_b;
            
            int details_a;
            int details_b;
            
            scanf("%d %d %d %d", &floor_a, &room_on_floor_a, &floor_b, 
                  &room_on_floor_b); 
            
            if (floor_a < LOWEST_FLOOR || 
                floor_a > HIGHEST_FLOOR || 
                floor_b < LOWEST_FLOOR || 
                floor_b > HIGHEST_FLOOR || 
                room_on_floor_a < LOWEST_ROOM_NUMBER || 
                room_on_floor_a > HIGHEST_ROOM_NUMBER || 
                room_on_floor_b < LOWEST_ROOM_NUMBER || 
                room_on_floor_b > HIGHEST_ROOM_NUMBER) {
                invalid_room_number();
                
            } else {
                // Switches the booking id's.
                details_a = hotel[floor_a][room_on_floor_a].booking_id;
                details_b = hotel[floor_b][room_on_floor_b].booking_id;
                
                hotel[floor_a][room_on_floor_a].booking_id = details_b;
                hotel[floor_b][room_on_floor_b].booking_id = details_a;
                
                // Switches the royal guest status (if any).
                details_a = hotel[floor_a][room_on_floor_a].royal_guest;
                details_b = hotel[floor_b][room_on_floor_b].royal_guest;
                
                hotel[floor_a][room_on_floor_a].royal_guest = details_b;
                hotel[floor_b][room_on_floor_b].royal_guest = details_a;
                
                // Switches the status of group booking (if any).
                details_a = hotel[floor_a][room_on_floor_a].group_booking;
                details_b = hotel[floor_b][room_on_floor_b].group_booking;
                
                hotel[floor_a][room_on_floor_a].group_booking = details_b;
                hotel[floor_b][room_on_floor_b].group_booking = details_a;   
            }
        
            // When 'g' is input, request an option.
        } else if (input == 'g') {
            int option;
            scanf("%d", &option);
            
            // When option '1' is scanned, add a group booking to the cheapest 
            // and lowest floor.
            if (option == 1) {
            
                scanf("%d", &num_guests);
                
                row = HIGHEST_FLOOR;
                int usable_floors[TOTAL_FLOORS];
                
                new_booking_id = add_group_booking(hotel, row, column, 
                                                   num_guests, usable_floors, 
                                                   booking_id);
                booking_id = new_booking_id;
            
                // When option '2' is scanned, add a group booking within a 
                // specified budget. 
            } else if (option == 2) {
            
                int budget;
                scanf("%d %d", &num_guests, &budget);
                
                row = LOWEST_FLOOR;
                int usable_floors[TOTAL_FLOORS];
                
                new_booking_id = add_group_booking_with_budget(hotel, row, 
                                                               column, budget, 
                                                               num_guests, 
                                                               usable_floors, 
                                                               booking_id);
                booking_id = new_booking_id;
            }
        
            // When '!' is input, add a royal guest to the hotel. 
        } else if (input == '!') {
        
            floor_number = LOWEST_FLOOR;
            room_on_floor = LOWEST_ROOM_NUMBER;
            num_guests = 0;
            scanf("%d %d", &floor_number, &room_on_floor);
            
            // Run insert_royal_guests when the room is empty.
            if (hotel[floor_number][room_on_floor].booking_id == NO_BOOKING) {
            
                new_booking_id = insert_royal_guests(hotel, floor_number, 
                                                     room_on_floor,
                                                     new_booking_id, 
                                                     booking_id);
                booking_id = new_booking_id;
            
                // If there is already a royal guest in the desired room, print 
                // an error message.    
            } else if (hotel[floor_number][room_on_floor].royal_guest == TRUE) {
            
                printf("Your Royal Highness, this room is not available\n");
            
                // If there is a non-group booking guest in the room, replace 
                // them with the royal guest and relocate the displaced guest
                // with 'j'.
            } else if (hotel[floor_number][room_on_floor].booking_id > 
                       NO_BOOKING && 
                       hotel[floor_number][room_on_floor].royal_guest == 
                       FALSE && 
                       hotel[floor_number][room_on_floor].group_booking == 
                       FALSE) {
                
                find_row = find_best_room_row(hotel, row, column);
                find_column = find_best_room_column(hotel, row, column);
                
                new_booking_id = check_room_for_displaced(hotel, find_row, 
                                                             find_column,  
                                                             floor_number, 
                                                             room_on_floor,
                                                             new_booking_id,
                                                             booking_id);
                booking_id = new_booking_id;
            
                // If there is a group booking in the room, relocate the entire
                // group with 'g1' while adding the royal guest to the room.
            } else if (hotel[floor_number][room_on_floor].booking_id > 
                       NO_BOOKING && 
                       hotel[floor_number][room_on_floor].royal_guest == 
                       FALSE && 
                       hotel[floor_number][room_on_floor].group_booking == 
                       TRUE) {
                
                int current_group_id = 
                hotel[floor_number][room_on_floor].booking_id;
                                
                int quantity_of_guests = remove_guests_for_royalty(hotel, row, 
                                                                   column, 
                                                               current_group_id, 
                                                                   num_guests);
                num_guests = quantity_of_guests;
                
                new_booking_id = insert_royal_guests(hotel, floor_number, 
                                                     room_on_floor,
                                                     new_booking_id, 
                                                     booking_id);
                booking_id = new_booking_id;
                
                row = HIGHEST_FLOOR;
                int usable_floors[TOTAL_FLOORS];
                
                group_booking_with_past_id(hotel, row, column, num_guests, 
                                           usable_floors, current_group_id, 
                                           booking_id);
        
            }
        }
        
        printf("Enter a command: ");
    }

    printf("Bye!\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Prints error message if an input doesn't pass conditions. 
void invalid_room_number(void) {
    printf("Invalid room number!\n");
}

// Returns the row value of the best available room according to 'j'.
int find_best_room_row(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                       int row, int column) {

    row = LOWEST_FLOOR;
    column = LOWEST_ROOM_NUMBER;
    
    while (hotel[row][column].booking_id > NO_BOOKING && row < HIGHEST_FLOOR) {
        
        if (column < HIGHEST_ROOM_NUMBER) {
            column++;
        
        } else if (column == HIGHEST_ROOM_NUMBER && row < HIGHEST_FLOOR) {
            column = LOWEST_ROOM_NUMBER;
            row++;    
        
        }
    }
    
    while (hotel[row][column].booking_id > NO_BOOKING && row == HIGHEST_FLOOR && 
           column != HIGHEST_ROOM_NUMBER) {
           
        column++;
    }
    return row; 
}

// Returns the column value of the best available room according to 'j'.
int find_best_room_column(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                          int row, int column) {

    row = LOWEST_FLOOR;
    column = LOWEST_ROOM_NUMBER;
    
    while (hotel[row][column].booking_id > NO_BOOKING && row < HIGHEST_FLOOR) {
        
        if (column < HIGHEST_ROOM_NUMBER) {
            column++;
        
        } else if (column == HIGHEST_ROOM_NUMBER && row < HIGHEST_FLOOR) {
            column = LOWEST_ROOM_NUMBER;
            row++;
                
        }
    }
    
    while (hotel[row][column].booking_id > NO_BOOKING && row == HIGHEST_FLOOR && 
           column != HIGHEST_ROOM_NUMBER) {
           
        column++;
    }
    return column;
}

// Analyses the entire hotel before selecting and returning the cheapest floor.
int fill_cheapest_bookings(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                           int row, int column, int num_guests, 
                           int usable_floors[TOTAL_FLOORS], 
                           int booking_id) {
                            
    // Finds the first usable floor from the bottom. 
    row = HIGHEST_FLOOR;
    
    while (usable_floors[row] != TRUE) {
        row--;
    }
    
    int cheapest_price = hotel[row][LOWEST_ROOM_NUMBER].price;
    int cheapest_floor;
    
    // This determines what is the cheapest price.
    while (row >= LOWEST_FLOOR) {
    
        if (hotel[row][LOWEST_ROOM_NUMBER].price < cheapest_price && 
            usable_floors[row] == TRUE) {
            
            cheapest_price = hotel[row][LOWEST_ROOM_NUMBER].price;
            
        }
        row--;
    }
    
    row = HIGHEST_FLOOR;
    
    // This selects the lowest floor with the cheapest price. 
    while (hotel[row][LOWEST_ROOM_NUMBER].price != cheapest_price || 
           usable_floors[row] != TRUE) {
           
        row--;
    }
    
    cheapest_floor = row;
    return cheapest_floor;
}

// Cycles through each room on a floor and changes the price.
void change_floor_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                        int row, int column, int floor_number, int new_price) {
                        
    column = LOWEST_ROOM_NUMBER;
    while (column < ROOMS_PER_FLOOR) {
    
        hotel[floor_number][column].price = new_price;
        column++;    
    }
}

// Cycles through all rooms in the hotel and adjust their price. 
void change_hotel_price(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                        int row, int column, int amount) {
                        
    row = LOWEST_FLOOR;
    while (row < ROOMS_PER_FLOOR) {
    
        column = LOWEST_ROOM_NUMBER;
        
        while (column < TOTAL_FLOORS) {
            
            hotel[row][column].price = 
            hotel[row][column].price + amount;
            
            if (hotel[row][column].price < 0) {
            
                hotel[row][column].price = 0;
            }
            
            column++;
        }
        
        row++;
    }
}

// Calculates the revenue earned by adding prices of occupied rooms.
int sum_revenues(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], int row, 
                 int column, int revenue) {
                 
    row = LOWEST_FLOOR;
    while (row < ROOMS_PER_FLOOR) {
    
        column = LOWEST_ROOM_NUMBER;
        
        while (column < TOTAL_FLOORS) {
            
            // Only add to revenue if a room has a guest.
            if (hotel[row][column].booking_id != NO_BOOKING) {
            
                revenue = revenue + hotel[row][column].price;
            }        
            
            column++;
        }
        
        row++;
    }
    
    return revenue;
}

// Inserts group bookings into rooms after checking there's enough space and
// then returns the value of group booking for future inputs. 
int add_group_booking(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                      int row, int column, int num_guests, 
                      int usable_floors[TOTAL_FLOORS], int booking_id) {
                       
    // This determines which floors can fit guests.
    while (row >= LOWEST_FLOOR) {
        column = LOWEST_ROOM_NUMBER;
        int available_rooms = 0;
        
        while (column < ROOMS_PER_FLOOR) {
            if (hotel[row][column].booking_id == NO_BOOKING) {
                available_rooms = available_rooms + 1;
            }
            column++;
        }
        
        if (available_rooms >= num_guests) {
            usable_floors[row] = TRUE;
        } else {
            usable_floors[row] = FALSE;
        }
        
        row--;
    }
    
    // This prints an error message if there are no usable floors.
    if (usable_floors[0] == FALSE && usable_floors[1] == FALSE && 
    usable_floors[2] == FALSE && usable_floors[3] == FALSE && 
    usable_floors[4] == FALSE && usable_floors[5] == FALSE && 
    usable_floors[6] == FALSE && usable_floors[7] == FALSE) {
        printf("No available floor for your group booking!\n");
    } else {
    
        int lowest_cost_floor = fill_cheapest_bookings(hotel, row, 
        column, num_guests, usable_floors, booking_id);
        
        // This fills empty rooms with the booking. 
        column = 0;
        int guests_filled = 0;
        
        while (guests_filled < num_guests) {
            if (hotel[lowest_cost_floor][column].booking_id == NO_BOOKING) {
            
                hotel[lowest_cost_floor][column].booking_id = 
                booking_id;
                hotel[lowest_cost_floor][column].group_booking = TRUE;
                column++;
                guests_filled++;
            
            } else {
            
                column++;
            }
        }         
        booking_id++;
    }
    return booking_id;
}

// Inserts group bookings into rooms after checking there's enough space and
// the rooms are within the groups' budget, then returns the value of group 
// booking for future inputs. 
int add_group_booking_with_budget(struct room 
                                  hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                                  int row, int column, int budget, 
                                  int num_guests, 
                                  int usable_floors[TOTAL_FLOORS], 
                                  int booking_id) {
                                  
    // This determines which floors can fit guests.
    while (row < TOTAL_FLOORS) {
        column = LOWEST_ROOM_NUMBER;
        int available_rooms = 0;
        
        while (column < ROOMS_PER_FLOOR) {
            if (hotel[row][column].booking_id == NO_BOOKING && 
                hotel[row][column].price <= budget) {
                available_rooms = available_rooms + 1;
            }
            column++;
        }
        
        if (available_rooms >= num_guests) {
            usable_floors[row] = TRUE;
        } else {
            usable_floors[row] = FALSE;
        }
        
        row++;
    }
    
    // This prints an error message if there are no usable floors.
    if (usable_floors[0] == FALSE && usable_floors[1] == FALSE && 
    usable_floors[2] == FALSE && usable_floors[3] == FALSE && 
    usable_floors[4] == FALSE && usable_floors[5] == FALSE && 
    usable_floors[6] == FALSE && usable_floors[7] == FALSE) {
        printf("No available floor for your group booking!\n");
    } else {
    
        // Finds the first usable floor from the top.
        row = LOWEST_FLOOR;
        while (usable_floors[row] != TRUE) {
            row++;
        }
        
        // This fills empty rooms with the booking. 
        column = 0;
        int guests_filled = 0;
        
        while (guests_filled < num_guests) {
            if (hotel[row][column].booking_id == NO_BOOKING) {
            
                hotel[row][column].booking_id = booking_id;
                hotel[row][column].group_booking = TRUE;
                column++;
                guests_filled++;
            
            } else {
            
                column++;
            }
        }
        booking_id++;
    }
    return booking_id;
}

// Adds a booking with royal_guest status to a particular room and returns
// booking id for future inputs.
int insert_royal_guests(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], 
                        int floor_number, int room_on_floor, int new_booking_id,
                        int booking_id) {
                        
    hotel[floor_number][room_on_floor].booking_id = booking_id;
    hotel[floor_number][room_on_floor].royal_guest = TRUE;
    booking_id++;
    
    return booking_id;
}

// Checks whether the hotel has addiitonal room for guests displaced by royalty, 
// then returns booking id for future inputs. 
int check_room_for_displaced(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                        int find_row, int find_column, int floor_number, 
                        int room_on_floor, int new_booking_id, int booking_id) {
                        
    if (hotel[find_row][find_column].booking_id == NO_BOOKING) {
        
        hotel[find_row][find_column].booking_id 
        = hotel[floor_number][room_on_floor].booking_id;
        
        new_booking_id = insert_royal_guests(hotel, floor_number, room_on_floor, 
                                             new_booking_id, booking_id);
        booking_id = new_booking_id;
    
    } else {
    
        printf("The hotel is full!\n");
        
        new_booking_id = insert_royal_guests(hotel, floor_number, room_on_floor,
                                             new_booking_id, booking_id);
        booking_id = new_booking_id;
    }    
    return booking_id;
}

// Count and return the number of guests in the group while removing them from 
// their rooms to make way for royalty.
int remove_guests_for_royalty(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR],
                              int row, int column, int current_group_id, 
                              int num_guests) {

    row = LOWEST_FLOOR;
    while (row < TOTAL_FLOORS) {
        column = LOWEST_ROOM_NUMBER;
        
        while (column < ROOMS_PER_FLOOR) {
            if (hotel[row][column].booking_id == current_group_id) {
            
                num_guests = num_guests + 1;
                hotel[row][column].booking_id = NO_BOOKING;
                hotel[row][column].royal_guest = FALSE;
                hotel[row][column].group_booking = FALSE;
            }
            
            column++;
        }
        
        row++;
    }
    return num_guests;
}

// Relocates group bookings into different rooms, retaining their same 
// booking id, after checking there's enough space. 
void group_booking_with_past_id(struct room 
                                hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR], int row, 
                                int column, int num_guests, 
                                int usable_floors[TOTAL_FLOORS], 
                                int current_group_id, int booking_id) {
                                          
    // This determines which floors can fit guests.
    while (row >= LOWEST_FLOOR) {
        column = LOWEST_ROOM_NUMBER;
        int available_rooms = 0;
        
        while (column < ROOMS_PER_FLOOR) {
            if (hotel[row][column].booking_id == NO_BOOKING) {
                available_rooms = available_rooms + 1;
            }
            
            column++;
        }
        
        if (available_rooms >= num_guests) {
            usable_floors[row] = TRUE;
        } else {
            usable_floors[row] = FALSE;
        }
        
        row--;
    }
                    
    // This prints an error message if there are no usable floors.
    if (usable_floors[0] == FALSE && usable_floors[1] == FALSE && 
    usable_floors[2] == FALSE && usable_floors[3] == FALSE && 
    usable_floors[4] == FALSE && usable_floors[5] == FALSE && 
    usable_floors[6] == FALSE && usable_floors[7] == FALSE) {
        printf("No available floor for your group booking!\n");
    } else {
    
        int lowest_cost_floor = fill_cheapest_bookings(hotel, row, 
        column, num_guests, usable_floors, booking_id);
        
        // This fills empty rooms with the booking. 
        column = LOWEST_ROOM_NUMBER;
        int guests_filled = 0;
        
        while (guests_filled < num_guests) {
            if (hotel[lowest_cost_floor][column].booking_id == NO_BOOKING) {
                
                hotel[lowest_cost_floor][column].booking_id = current_group_id;
                hotel[lowest_cost_floor][column].group_booking = TRUE;
                column++;
                guests_filled++;
                
            } else {
            
                column++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
/////////////////////////// (DO NOT EDIT BELOW HERE) ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided Function (you shouldn't need to edit this in stage 1 or 2)
// Initalises the booking id and price of each room in the hotel to be
// the default values.
void init_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        while (col < ROOMS_PER_FLOOR) {
            hotel[row][col].booking_id = NO_BOOKING;
            hotel[row][col].price = INITIAL_PRICE;
            hotel[row][col].royal_guest = FALSE;
            hotel[row][col].group_booking = FALSE;
            col++;
        }
        row++;
    }
}

// Provided Function - DO NOT EDIT
// Prints out the hotel with the booking number of each room, or empty
// if there is no booking.
void print_hotel(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    printf(
        "\n"
        "         /--^--\\\n"
        "   _     |  O  |     _\n"
        " _/ \\___/_^___^_\\___/ \\_\n"
        "/                       \\\n");

    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        printf("|--+--+--+--+--+--+--+--|\n");
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
            if (hotel[row][col].booking_id == NO_BOOKING) {
                printf("  ");
            } else {
                printf("%02d", hotel[row][col].booking_id);
            }
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(
        "|--+--+--+--+--+--+--+--|\n"
        "|         /^^^\\         |\n"
        "|_________|_\"_|_________|\n\n");
}

// Provided Function - DO NOT EDIT
// Prints out the hotel with the the price and booking number included.
void print_hotel_with_prices(struct room hotel[TOTAL_FLOORS][ROOMS_PER_FLOOR]) {
    printf(
        "\n"
        "             /--^--\\\n"
        "   _         |  O  |         _\n"
        " _/ \\_______/_^___^_\\_______/ \\_\n"
        "/                               \\\n");

    int row = 0;
    while (row < TOTAL_FLOORS) {
        int col = 0;
        printf("|---+---+---+---+---+---+---+---|\n");
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
            if (hotel[row][col].booking_id == NO_BOOKING) {
                printf("   ");
            } else {
#ifndef NO_COLORS
                printf("\033[1;35m");
#endif
                printf("%03d", hotel[row][col].booking_id);
#ifndef NO_COLORS
                printf("\033[0m");
#endif
            }
            col++;
        }
        printf("|\n");
        col = 0;
        while (col < ROOMS_PER_FLOOR) {
            printf("|");
#ifndef NO_COLORS
            printf("\033[1;32m");
#endif
            printf("%03d", hotel[row][col].price);
#ifndef NO_COLORS
            printf("\033[0m");
#endif
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(
        "|---+---+---+---+---+---+---+---|\n"
        "|             /^^^\\             |\n"
        "|_____________|_\"_|_____________|\n\n");
}

