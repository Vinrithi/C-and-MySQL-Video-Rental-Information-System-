#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mysql-connector/include/mysql.h"
#include "db_manipulation.h"

//a structure to hold the customer details when adding a new customer record
typedef struct Customer
{
    char surname[30];
    char otherNames[70];
    long telephone;
    char *customerID;
    long address;
};

//a structure to hold the video details when adding a new video record
typedef struct Video
{
    char title[50];
    char type[15];
    char director[50];
    char actor[50];
    char genre[25];
    int copies;
    int charges;
    char *videoID;
};

//a structure to hold the rented video details when adding a new rented video record
typedef struct RentedVideo
{
    char videoID[15];
    char customerID[15];
    char charges[7];
    char dateDue[20];
};

//a structure to hold the video details for the field(s) being updated

typedef struct UpdateVideo
{
    char title[30];
    char charges[7];
    int copies;
    char videoID[15];
};

//a structure to hold the customer details for the field(s) being updated
typedef struct UpdateCustomer
{
    char surname[30];
    char otherNames[40];
    char telephone[15];
    char address[15];
    int copies;
    char customerID[15];
};

//a structure to hold details when making a return recording of a rented video
typedef struct VideoReturn
{
    char penalty[10];
    char customerID[15];
    char videoID[15];
    char dateRented[15];
};


/*function for adding a new customer record. If successfully added, a new table for the customer is created for storing their
rental history records
parameters - connection to videorental db, connection to videorentalcustomers db
*/

void newCustomer(MYSQL *db1_con,MYSQL *db2_con)
{
    printf("\n      New Customer Record            ");
    printf("\n---------------------------------------\n");
    struct Customer *custPtr,cust1;
    custPtr = &cust1;  //initialize pointer to structure which will hold the details for the new customer during the registration process
    printf("Please enter the following fields. (All fields must be filled)\n");

    printf("Surname: ");
    scanf ("%s",custPtr->surname);

    printf("\nOtherNames: ");
    scanf ("%s",custPtr->otherNames);

    printf("\nTelephone: ");
    scanf ("%ld",&custPtr->telephone);

    printf("\nAddress: ");
    scanf ("%ld",&custPtr->address);

    custPtr->customerID = generateCustomerID();
    printf("\nYour Customer ID = %s\n",custPtr->customerID);


    //this section provides for concatenating the details and parts of sql query to form a full sql query that can be executed.

    char s1[500] = "INSERT INTO Customers (CustomerID,Surname,OtherNames,Telephone,Address) VALUES('";
    strcat(s1,custPtr->customerID);
    strcat(s1,"', '");
    strcat(s1,custPtr->surname);
    strcat(s1,"', '");
    strcat(s1,custPtr->otherNames);
    strcat(s1,"',");

    char t_tel[15];
    itoa(custPtr->telephone, t_tel, 10);
    strcat(s1,t_tel);
    strcat(s1,", ");

    char t_address[15];
    itoa(custPtr->address, t_address, 10);
    strcat(s1,t_address);

    char *query = strcat(s1,")");

    if (mysql_query(db1_con, query)) {
        display_error(db1_con);
    }
    else
    {
        printf("\nSucessfully added!!\n");
    }
    createPerCustomerTable(db2_con,custPtr->customerID); //creates a new table with the customer id as its name
}


/*function for adding a new video record.
parameters - connection to videorental db
*/

void newVideo(MYSQL *db1_con)
{
    struct Video *vidPtr,vid1;
    vidPtr = &vid1;     //initialize pointer to structure which will hold the details for the new customer during the registration process

    printf("\n          New Video Record        ");
    printf("\n-------------------------------------\n");
    printf("Please enter the following fields. (All fields must be filled)\n");

    printf("Title: ");
    scanf ("%s",vidPtr->title);

    printf("Type: ");
    scanf ("%s",vidPtr->type);

    printf("Director: ");
    scanf ("%s",vidPtr->director);

    printf("Actor: ");
    scanf ("%s",vidPtr->actor);


    printf("Genre: ");
    scanf ("%s",vidPtr->genre);

    printf("Copies: ");
    scanf ("%d",&vidPtr->copies);

    printf("Charges: ");
    scanf ("%d",&vidPtr->charges);

    vidPtr->videoID = generateVideoID();
    printf("\nVideo ID = %s\n",vidPtr->videoID);

    //this section provides for concatenating the details and parts of sql query to form a full sql query that can be executed.

    char s1[500] = "INSERT INTO Videos (VideoID,Title,Type,Copies,Director,Genre,Actor,Charges) VALUES('";
    strcat(s1,vidPtr->videoID);
    strcat(s1,"', '");
    strcat(s1,vidPtr->title);
    strcat(s1,"', '");
    strcat(s1,vidPtr->type);
    strcat(s1,"',");

    char t_copies[15];
    itoa(vidPtr->copies, t_copies, 10);
    strcat(s1,t_copies);
    strcat(s1,", '");

    strcat(s1,vidPtr->director);
    strcat(s1,"', '");
    strcat(s1,vidPtr->genre);
    strcat(s1,"', '");
    strcat(s1,vidPtr->actor);
    strcat(s1,"',");

    char t_charges[15];
    itoa(vidPtr->charges, t_charges, 10);
    strcat(s1,t_charges);

    char *query = strcat(s1,")");

    if (mysql_query(db1_con, query)) {
        display_error(db1_con);
    }
    else
    {
        printf("\nSucessfully added!!\n");
    }
}

/*function for updating existing video record details.
parameters - connection to videorental db
*/

void updateVideoDetails(MYSQL *con)
{
    struct UpdateVideo *updPtr,update;
    updPtr = &update;
    printf("\n          Update Video Record        ");
    printf("\n-------------------------------------\n");
    searchForVideo(con);
    printf("\nPlease re-enter the video ID\n");
    scanf("%s",updPtr->videoID);
    printf("\nPlease choose the field to update\n");
    printf("1. Title\n");
    printf("2. Copies\n");
    printf("3. Charges\n");
    int choice;
    scanf("%d",&choice);

    char s1[250] = "UPDATE Videos SET ";

    switch(choice)
    {
        case 1:
            printf("Please enter new title\n");
            scanf("%s",updPtr->title);
            strcat(s1,"Title = '");
            strcat(s1,updPtr->title);
            strcat(s1,"'");
        break;
        case 2:
            printf("Please enter new charges\n");
            scanf("%s",updPtr->charges);
            strcat(s1,"Charges = ");
            strcat(s1,updPtr->charges);
        break;

        case 3:
            printf("Please update number of copies\n");
            scanf("%d",&updPtr->copies);
            strcat(s1,"Copies = ");

            char t_copies[15];
            itoa(updPtr->copies, t_copies, 10);
            strcat(s1,t_copies);

        break;

        default:
            printf("Please choose a selection from the options given\n");
            updateVideoDetails(con);
        break;
    }

    strcat(s1," WHERE VideoID = '");
    strcat(s1,updPtr->videoID);
    char *query = strcat(s1,"'");

    if (mysql_query(con, query))
    {
         display_error(con);
    }
    else
    {
        printf("Video record successfully updated\n");
    }
}

/*function for updating existing customer record details.
parameters - connection to videorental db
*/
void updateCustomerDetails(MYSQL *con)
{
    printf("\n      Update Customer Record            ");
    printf("\n---------------------------------------\n");
    struct UpdateCustomer *updPtr,update;
    updPtr = &update;
    printf("Please enter the customerID\n");
    scanf("%s",updPtr->customerID);
    printf("Please choose the field to update\n");
    printf("1. Surname\n");
    printf("2. Other Names\n");
    printf("3. Telephone\n");
    printf("4. Address\n");
    int choice;
    scanf("%d",&choice);

    char s1[200] = "UPDATE Customers SET ";

    switch(choice)
    {
        case 1:
            printf("Please enter new surname\n");
            scanf("%s",updPtr->surname);
            strcat(s1,"Surname = '");
            strcat(s1,updPtr->surname);
            strcat(s1,"'");
        break;
        case 2:
            printf("Please change other name\n");
            scanf("%s",updPtr->otherNames);
            strcat(s1,"OtherNames = '");
            strcat(s1,updPtr->surname);
            strcat(s1,"'");
        break;

        case 3:
            printf("New Telephone\n");
            scanf("%s",updPtr->telephone);
            strcat(s1,"Telephone = ");
            strcat(s1,updPtr->telephone);

        break;

        case 4:
            printf("New Address\n");
            scanf("%s",updPtr->address);
            strcat(s1,"Address = ");
            strcat(s1,updPtr->address);

        break;

        default:
            printf("Please choose a selection from the options given\n");
            updateCustomerDetails(con);
        break;
    }

    strcat(s1," WHERE CustomerID = '");
    strcat(s1,updPtr->customerID);
    char *query = strcat(s1,"'");

    if (mysql_query(con, query))
    {
         display_error(con);
    }
    else
    {
        printf("Customer record successfully updated\n");
    }
}


void searchForVideo(MYSQL *con)
{
    char movie_val[100];
    printf("Please search using either Title, Director or Actor\n");
    scanf("%s",movie_val);

    char s1[500] = "SELECT * FROM Videos WHERE Title LIKE UPPER ('%";
    strcat(s1,movie_val);
    strcat(s1,"%') OR Director LIKE UPPER ('%");
    strcat(s1,movie_val);
    strcat(s1,"%') OR Actor LIKE UPPER ('%");
    strcat(s1,movie_val);
    char *query = strcat(s1,"%') ORDER BY No ASC");

    if (mysql_query(con, query))
    {
         display_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        display_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    printf("\n----------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n","No","Video ID","Title","Copies","Type","Director","Actor","Genre","Charges");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------\n");
	while ((row = mysql_fetch_row(result)))
    {
        int i=0;

        for(i = 0; i < num_fields; i++)
        {
              printf("%-15s", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------\n");

    mysql_free_result(result);
}


void searchForRentedVideo(MYSQL *con,char *customerID)
{
    char movie_val[100];
    printf("Please search using either Title, Director or Actor\n");
    scanf("%s",movie_val);

    char s1[500] = "SELECT VideoID,Title FROM Videos WHERE Title LIKE UPPER ('%";
    strcat(s1,movie_val);
    strcat(s1,"%') OR Director LIKE UPPER ('%");
    strcat(s1,movie_val);
    strcat(s1,"%') OR Actor LIKE UPPER ('%");
    strcat(s1,movie_val);
    char *query = strcat(s1,"%') ORDER BY No ASC");

    if (mysql_query(con, query))
    {
         display_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        display_error(con);
    }

    MYSQL_ROW row = mysql_fetch_row(result);

	char *title = row[1];

    char s2[200] = "SELECT VideoID, DateRented,Penalty FROM Rented WHERE CustomerID = '";
	strcat(s2,customerID);
	strcat(s2,"' AND VideoID = '");
	strcat(s2,row[0]);
	char *query2 = strcat(s2,"'");

	if (mysql_query(con, query2))
    {
         display_error(con);
    }

    MYSQL_RES *result2 = mysql_store_result(con);

    if (result2 == NULL)
    {
        display_error(con);
    }

    int num_fields2 = mysql_num_fields(result2);

    MYSQL_ROW row2 = mysql_fetch_row(result2);

	printf("\n-------------------------------------------------------------------\n");
	printf("%-15s%-15s%-15s%-15s","Video ID","Title","Date Rented","Penalty");
	printf("\n-------------------------------------------------------------------\n");
	printf("%-15s%-15s%-15s%-15s",row2[0],title,row2[1],row2[2]);
	printf("\n-------------------------------------------------------------------\n");

	mysql_free_result(result);
    mysql_free_result(result2);
}


void deleteVideo(MYSQL *con)
{
	printf("\n          Delete Video Record        ");
    printf("\n-------------------------------------\n");
    char vid_id[15];
    printf("Please use the video ID to do the deletion\n");
    searchForVideo(con);
    printf("Please enter the video ID for video to be deleted\n");
    scanf("%s",vid_id);

    char s1[150] = "DELETE FROM Videos WHERE VideoID = '";
    strcat(s1,vid_id);
    char *query = strcat(s1,"'");

    if (mysql_query(con, query))
    {
         display_error(con);
    }
    else
    {
        printf("Video successfully deleted\n");
    }
}


void deleteCustomer(MYSQL *con)
{
    printf("\n      Delete Customer Record            ");
    printf("\n---------------------------------------\n");
    char cust_id[15];
    printf("Please enter the customer ID\n");
    scanf("%s",cust_id);

    char s1[150] = "DELETE FROM Customers WHERE CustomerID = '";
    strcat(s1,cust_id);
    char *query = strcat(s1,"'");

    if (mysql_query(con, query))
    {
         display_error(con);
    }
    else
    {
        printf("Customer record successfully deleted\n");
    }
}



void rentVideo(MYSQL *con,MYSQL *con2)
{
    printf("\n          Rent Video                 ");
    printf("\n-------------------------------------\n");
    struct RentedVideo *rentedPtr,rented1;
    rentedPtr = &rented1;

    printf("Please enter customer id \n");
    scanf("%s",rentedPtr->customerID);


    char s1[100] = "SELECT SUM(Penalty) AS total_penalty FROM Rented WHERE CustomerID = '";
    strcat(s1,rentedPtr->customerID);
    char *query = strcat(s1,"'");

    if (mysql_query(con, query))
    {
         display_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        display_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row = mysql_fetch_row(result);

    int j = atoi(row[0]);
    if(j>0)
    {
        printf("You have a pending penalty of: %d\n",j);
        printf("Do you still want to proceed with the rental?[y/n]\n");
        char cho;
        scanf("%s",&cho);
        switch(cho)
        {
            case 'y':
            case 'Y':
            break;
            case 'n':
            case 'N':
                printf("Thank you.\n");
                exit(0);
            break;
            default:
                system("cls");
                mainUsersMenu();
            break;

        }

    }

    mysql_free_result(result);


    searchForVideo(con);
    printf("Please enter the following details:\n");
    printf("Video ID\n");
    scanf("%s",rentedPtr->videoID);

    printf("Fee: ");
    scanf ("%s",rentedPtr->charges);

    printf("Deadline (yyyy-mm-dd)\n");
    scanf("%s",rentedPtr->dateDue);

    char s2[500] = "INSERT INTO Rented (VideoID,CustomerID,DateRented,Fee,Deadline) VALUES('";
    strcat(s2,rentedPtr->videoID);
    strcat(s2,"','");
    strcat(s2,rentedPtr->customerID);
    strcat(s2,"',NOW(),");
    strcat(s2,rentedPtr->charges);
    strcat(s2,",'");
    strcat(s2,rentedPtr->dateDue);
    char *query2 = strcat(s2,"')");

    //printf("%s",query2);
    if (mysql_query(con, query2))
    {
         display_error(con);
    }

    char s3[100] = "UPDATE Videos SET Copies = Copies - 1 WHERE VideoID = '";
    strcat(s3,rentedPtr->videoID);
    char *query3 = strcat(s3,"'");

    if (mysql_query(con, query3))
    {
         display_error(con);
    }

    char s4[200] = "INSERT INTO ";
    strcat(s4,rentedPtr->customerID);
    strcat(s4," (VideoID,DateRented) VALUES('");
    strcat(s4,rentedPtr->videoID);
    char *query4 = strcat(s4,"',NOW())");
    if (mysql_query(con2, query4))
    {
         display_error(con2);
    }
	else
	{
		printf("\nSuccesfully rented out video\n");
	}
}


void returnVideo(MYSQL *con,MYSQL *con2)
{
    printf("\n          Return Rented Video        ");
    printf("\n-------------------------------------\n");
    int amountPayed;
    struct VideoReturn *penlPtr,penalty1;
    penlPtr = &penalty1;

    printf("Please enter the customer ID\n");
    scanf("%s",penlPtr->customerID);
    searchForRentedVideo(con, penlPtr->customerID);

    printf("\nPlease enter the video ID and the date rented (yyyy-mm-dd)\n");
    scanf("%s%s",penlPtr->videoID,penlPtr->dateRented);



    char s1[100] = "SELECT Penalty FROM Rented WHERE CustomerID = '";
    strcat(s1,penlPtr->customerID);
    char *query = strcat(s1,"'");

    if (mysql_query(con, query))
    {
         display_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        display_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row = mysql_fetch_row(result);

    int j = atoi(row[0]);

    if(j>0)
    {
        printf("There is a penalty of %d\n",j);
        doPenalization(j);
    }

    printf("You have been cleared\n");

    char s2[300] = "UPDATE ";
    strcat(s2,penlPtr->customerID);
    strcat(s2," SET DateReturned = NOW(), Penalty = ");
    strcat(s2,row[0]);
    strcat(s2," WHERE VideoID = '");
    strcat(s2,penlPtr->videoID);
    strcat(s2,"' AND DateRented = '");
    strcat(s2,penlPtr->dateRented);
    char *query2 = strcat(s2,"'");

    if (mysql_query(con2, query2))
    {
         display_error(con2);
    }

    char s3[200] = "UPDATE Videos SET Copies = Copies + 1 WHERE VideoID = '";
    strcat(s3,penlPtr->videoID);
    char *query3 = strcat(s3,"'");
    if (mysql_query(con, query3))
    {
         display_error(con);
    }

    char s4[200] = "DELETE FROM Rented WHERE CustomerID = '";
    strcat(s4,penlPtr->customerID);
    strcat(s4,"' AND VideoID = '");
    strcat(s4,penlPtr->videoID);
    char *query4 = strcat(s4,"'");
    if (mysql_query(con, query4))
    {
         display_error(con);
    }
    else
    {
        printf("\nCLEARANCE SUCCSSFUL\n");
    }
}

void doPenalization(int penalty)
{
    int amountPayed;

    printf("Please enter payment: \n");
    scanf("%d",&amountPayed);

    int balance = amountPayed - penalty;
    if(balance>0)
    {
        printf("Your balance is %d\n",balance);
    }
    else if(balance<0)
    {
        int left = penalty-amountPayed;
        printf("Please add %d more.\n",left);
        doPenalization(left);
    }
}


void showAllVideos(MYSQL *con)
{
    printf("\n      Display all Video Records        ");
    printf("\n---------------------------------------\n");
	if (mysql_query(con, "SELECT * FROM Videos ORDER BY No ASC"))
    {
         display_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        display_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

	printf("\n----------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n","No","Video ID","Title","Copies","Type","Director","Actor","Genre","Charges");
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------\n");
	while ((row = mysql_fetch_row(result)))
    {
        int i=0;

        for(i = 0; i < num_fields; i++)
        {
              printf("%-15s", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }
	printf("\n----------------------------------------------------------------------------------------------------------------------------------------\n");
	mysql_free_result(result);
}

//function for displaying all the videos that have been rented
void showAllRentedVideos(MYSQL *con)
{
    printf("\n      Display Rented Video Records        ");
    printf("\n-----------------------------------------\n");
	if (mysql_query(con, "SELECT v.Title,c.Surname, r.DateRented,r.Fee,r.Deadline,r.Penalty FROM Rented r LEFT JOIN Videos v ON r.VideoId = v.VideoId LEFT JOIN Customers c ON r.CustomerID = c.CustomerID"))
    {
         display_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con); //gets result from the query and stores them as a queryset

    if (result == NULL)
    {
        display_error(con);
    }

    int num_fields = mysql_num_fields(result); //counts the number of columns for the queryset

    MYSQL_ROW row;

	printf("\n-----------------------------------------------------------------------------------------------------------\n");
	printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n","No","Title","Surname","Date Rented","Charges","Deadline","Penalty");
	printf("\n-----------------------------------------------------------------------------------------------------------\n");
	int j=1;

	//loops to get data per row and display it on the console in the specified format
	while ((row = mysql_fetch_row(result)))
    {
        int i=0;
        printf("%-15d",j);
        for(i = 0; i < num_fields; i++)
        {
              printf("%-15s", row[i] ? row[i] : "NULL");
        }
        printf("\n");
        j++;
    }
	printf("\n-----------------------------------------------------------------------------------------------------------\n");
	mysql_free_result(result); //frees the result set to avoid memory leaks
}

/*void showRentalHistory(MYSQL *con)
{
    char customerID[15];
    printf("Please enter customer ID\n");
    scanf("%s",customerID);
    char s1[100] = "SELECT v.Title,c.DateRented, c.DateReturned, c.Penalty FROM ";
    strcat(s1,customerID);
    char *query = strcat(s1," c LEFT JOIN Videos v ON v.VideoID = c.VideoID");

    if (mysql_query(con, query))
    {
         display_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        display_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

	printf("\n-----------------------------------------------------------------------------------------------------------\n");
	printf("%-15s%-15s%-15s%-15s%-15s\n","No","Title","Date Rented","Date Returned","Penalty Paid");
	printf("\n-----------------------------------------------------------------------------------------------------------\n");
	int j=1;
	while ((row = mysql_fetch_row(result)))
    {
        int i=0;
        printf("%-15d",j);
        for(i = 0; i < num_fields; i++)
        {
              printf("%-15s", row[i] ? row[i] : "NULL");
        }
        printf("\n");
        j++;
    }
	printf("\n-----------------------------------------------------------------------------------------------------------\n");
	mysql_free_result(result);
}*/

//function for randomly generating indices for the array whose corresponding elements are combined to form a video id
char *generateCustomerID()
{
    //using time as seed for random number generator
    srand(time(0));

    //array contains some characters used to develop the customer id
    char charArray[54] = {'A','x','1','u','B','2','C','a','7','D','E','b','3','F','G','h','H','4','5','I','l','J','K',
                          'c','L','5','f','M','9','N','z','O','P','w','Q','0','m','R','S','j','1','T','U','4','8',
                          'V','d','W','e','X','2','Y','4','Z'};
    char newId[7];
    int i;

    //generates a random index between 0 and 54 and retrieve the corresponding element. Then the element is copied in a new array
    for (i = 0; i < 7; i++) {
        int num = (rand() + 0) % (54 + 1);
        newId[i] = charArray[num];
    }

    static char id[12];
    memset(id, '\0', sizeof(id));
    strncpy(id, newId, 7);

    //returns an array which contains the customer id
    return id;
}

//function for randomly generating indices for the array whose corresponding elements are combined to form a video id
char *generateVideoID()
{
    //using time as seed for random number generator
    srand(time(0));

    //array contains some characters used to develop the video id
    char charArray[54] = {'A','x','1','u','B','2','C','a','7','D','E','b','3','F','G','h','H','4','5','I','l','J','K',
                          'c','L','5','f','M','9','N','z','O','P','w','Q','0','m','R','S','j','1','T','U','4','8',
                          'V','d','W','e','X','2','Y','4','Z'};
    char newId[8];
    int i;

    //generates a random index between 0 and 54 and retrieve the corresponding element. Then the element is copied in a new array
    for (i = 1; i < 7; i++) {
        int num = (rand() + 0) % (54 + 1);
        newId[i] = charArray[num];
    }
    newId[0] = 'V';

    static char id[12];
    memset(id, '\0', sizeof(id));
    strncpy(id, newId, 7);

    //returns an array which contains  the video id
    return id;
}









