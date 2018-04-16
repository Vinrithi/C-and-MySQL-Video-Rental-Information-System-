#include <stdio.h>
#include <stdlib.h>
#include "mysql-connector/include/mysql.h"
#include "db_manipulation.h"

//The first connection is for the videorental databse whereas the second one is for the videorentalcustomers database
//Failure of connection will provide an error and an immediate termination of the program execution.
void createConnectionStream(MYSQL *db1_con , MYSQL *db2_con)
{
    if(mysql_real_connect(db1_con, "localhost", "vicente", "vincent96", "VideoRental", 0, NULL, 0) == NULL)
    {
          display_error(db1_con);
    }
    if (mysql_real_connect(db2_con, "localhost", "vicente", "vincent96", "VideoRentalCustomers", 0, NULL, 0) == NULL)
    {
          display_error(db2_con);
    }
}

/*function for creating two databases: videorental and videorentalcustomers.
  The connection function mysql_real_connect is provided by mysql.h which helps in connecting to mysql server
*/
void createDatabase()
{
    MYSQL *create_db_con = mysql_init(NULL);

    if(create_db_con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(create_db_con));
        exit(1);
    }

    /*parameters server connection stream, server host (in this case localhost),user,password, database,port,
					   unix socket,client flag (e.g. if executing multiple queries at once)*/
    if (mysql_real_connect(create_db_con, "localhost", "vicente", "vincent96", NULL, 0, NULL, 0) == NULL)
    {
          display_error(create_db_con);
    }

    if(mysql_query(create_db_con, "CREATE DATABASE IF NOT EXISTS VideoRental"))
    {
        display_error(create_db_con);
    }

    if(mysql_query(create_db_con, "CREATE DATABASE IF NOT EXISTS VideoRentalCustomers"))
    {
        display_error(create_db_con);
    }


    mysql_close(create_db_con);
}

//creates a table customers which is used to store customer records
//parameter - a connection to videorental database
void createTableCustomers(MYSQL *con)
{
    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS Customers (No INT NOT NULL AUTO_INCREMENT , CustomerID VARCHAR(20) NOT NULL , Surname VARCHAR(25) NOT NULL, OtherNames VARCHAR(70) NOT NULL,Telephone INT(15) NOT NULL , Address VARCHAR(20) NOT NULL , PRIMARY KEY (CustomerID), UNIQUE(No)) ENGINE = InnoDB")) {
       display_error(con);
    }
}

//creates a table videos which is used to store video records
//parameter - a connection to videorental database
void createTableVideos(MYSQL *con)
{
    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS Videos (No INT NOT NULL AUTO_INCREMENT, VideoID VARCHAR(20) NOT NULL,Title VARCHAR(70) NOT NULL, Copies INT(20) NOT NULL,Type VARCHAR(10) NOT NULL , Director VARCHAR(70) NOT NULL,Actor VARCHAR(70) NOT NULL, Genre VARCHAR(20) NOT NULL, Charges INT(10) NOT NULL,PRIMARY KEY (VideoID),UNIQUE(No)) ENGINE = InnoDB")) {
       display_error(con);
    }
}

//creates a table rented which is used to store rented video records
//parameter - a connection to videorental database
void createTableRented(MYSQL *con)
{
    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS Rented (No INT NOT NULL AUTO_INCREMENT, VideoID VARCHAR(20) NOT NULL,CustomerID VARCHAR(20) NOT NULL, DateRented DATE NOT NULL , Fee INT(20) NOT NULL, Deadline DATE NOT NULL , Penalty INT(10) NOT NULL DEFAULT '0' ,PRIMARY KEY(No)) ENGINE = InnoDB")) {
       display_error(con);
    }
}

//creates a table for each new customer with table name being customer id. The table then stores the rental history of the customer
//parameters - a connection to videorentalcustomers database, customer id
void createPerCustomerTable(MYSQL *con,char *cust_id)
{
    char s1[510] = "CREATE TABLE IF NOT EXISTS ";
    char s3[410] = " (No INT NOT NULL AUTO_INCREMENT, VideoID VARCHAR(20) NOT NULL, DateRented DATE NOT NULL, DateReturned DATE NULL, Penalty INT(10) NOT NULL DEFAULT '0', PRIMARY KEY(No)) ENGINE = InnoDB";

    strcat(s1,cust_id);
    char *query = strcat(s1,s3); //concatenates the string arrays to form a query executable by mysql.

    if (mysql_query(con, query)) {
       display_error(con);
    }
}

//displays a mysql error to the terminal or console and terminates the program immediately.
void display_error(MYSQL *con)
{
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
}

//closes connection stream to the databases
//parameters - conncetion stream to videorental database, conncetion stream to videorentalcustomers database
void closeConnections(MYSQL *db1_con , MYSQL *db2_con)
{
    mysql_close(db1_con);
    mysql_close(db2_con);
}
