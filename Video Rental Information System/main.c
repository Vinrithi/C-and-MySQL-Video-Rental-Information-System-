#include <stdio.h>
#include <stdlib.h>
#include "mysql-connector/include/mysql.h"
#include "db_manipulation.h"

/*
    The main.c file contains the function invocations for creating databases and tables. It also has menus for assisting users in
    navigation.
*/

MYSQL *db1_con,*db2_con;  //global variables for connections to databases videorental and videorentalcustomers respecively.

int main(int argc, char **argv)
{
      createDatabase();

      db1_con = mysql_init(NULL);
      db2_con = mysql_init(NULL);

      createConnectionStream(db1_con,db2_con); //making connection to the database
      createTableCustomers(db1_con); //creates a table which holds the customer records
      createTableVideos(db1_con); //creates a table which holds the video records
      createTableRented(db1_con); //creates a table which holds the rented videos' records

      printf("***************************************************************\n");
      printf("*                 VIDEO RENTAL INFORMATION SYSTEM             *\n");
      printf("***************************************************************\n");
      printf("                              Welcome                          \n");
      printf("---------------------------------------------------------------\n\n");
      mainUsersMenu();
      closeConnections(db1_con,db2_con); //closes connection to the databases

      exit(0);
}

/*
function which provides a menu for the three main users of the system i.e. branch manager, registration desk and rental desk.
Each user group has their own functions that they perform hence the need to be separated.
*/
void mainUsersMenu()
{
    int choice;
    printf("Please choose which user section you would like to access\n");
    printf("1. Branch Manager\n");
    printf("2. Registration desk\n");
    printf("3. Rental desk\n");
    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
            branchManager(); //displays the menu for the operations that branch manager can perform
        break;
        case 2:
            registrationDesk(); //displays the menu for the operations that registration desk can perform
        break;
        case 3:
            rentalDesk(); //displays the menu for the operations that rental desk can perform
        break;
        default:
              printf("\nPlease make a selection from the given options\n");
              mainUsersMenu(); //recurse until user makes an acceptable pick
        break;
    }

}

/*This function provides a menu for all operations that the branch manager performs.
The BM can add a new video, alter/change the details of existing video records or delete a video record.
*/
void branchManager()
{
      system("cls");
      printf("-----------------------------------------------\n");
      printf("-            Branch manager                   -\n");
      printf("-----------------------------------------------\n");
      printf("Please choose an operation to perform\n");
      printf("1. Add new video title\n");
      printf("2. Update video record\n");
      printf("3. Delete video record\n");

      int choice;
      scanf("%d",&choice);
      switch(choice)
      {
            case 1:
                newVideo(db1_con); //adding a new video record
            break;

            case 2:
                updateVideoDetails(db1_con); //updating an existing video record
            break;

            case 3:
                deleteVideo(db1_con); //deleting a video record
            break;

            default:
                printf("\nPlease make a selection from the given options\n");
                branchManager(); //recurses until user makes an acceptable pick
            break;
      }
}


/*This function provides customer record handling services done by the registration desk. They include
adding a new customer, updating details of an existing customer and deleting details of a customer.
*/
void registrationDesk()
{
      system("cls");
      printf("-----------------------------------------------\n");
      printf("-            Registration desk                -\n");
      printf("-----------------------------------------------\n");
      printf("Please choose an operation to perform\n");
      printf("1. Add new customer\n");
      printf("2. Update customer record\n");
      printf("3. Delete customer record\n");

      int choice;
      scanf("%d",&choice);
      switch(choice)
      {
            case 1:
                newCustomer(db1_con,db2_con); //adding a new customer
            break;

            case 2:
                updateCustomerDetails(db1_con); //updating an existing customer record
            break;

            case 3:
                deleteCustomer(db1_con); //deleting a customer record
            break;

            default:
                printf("\nPlease make a selection from the given options\n");
                registrationDesk(); //recurse until user makes acceptable pick
            break;
      }
}

/*This function provides for video rental services done by the rental desk. They include renting out video, returning of a video by
customer, checking a customer's rental history, showing all available video records and showing all rented video records.
The system uses a subsystem of search which is invoked during most of these operations
*/
void rentalDesk()
{
      system("cls");
      printf("-----------------------------------------------\n");
      printf("-                Rental desk                  -\n");
      printf("-----------------------------------------------\n");
      printf("Please choose an operation to perform\n");
      printf("1. Rent out video\n");
      printf("2. Video return\n");
      printf("3. Check rental history\n");
      printf("4. Show all video records\n");
      printf("5. Show all rented videos\n");

      int choice;
      scanf("%d",&choice);
      switch(choice)
      {
            case 1:
                rentVideo(db1_con,db2_con); //renting out video to customer
            break;

            case 2:
                returnVideo(db1_con,db2_con); //registering return of a video rented by customer
            break;

            case 3:
                //showRentalHistory(db2_con); //showing a customer's rental history
            break;

            case 4:
                showAllVideos(db1_con); //displaying all available videos to be rented
            break;

            case 5:
                showAllRentedVideos(db1_con); //displaying all videos that have been rented
            break;

            default:
                printf("\nPlease make a selection from the given options\n");
                rentalDesk(); //recurse until user makes acceptable pick
            break;
      }
}



















