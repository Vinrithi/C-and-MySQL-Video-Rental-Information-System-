#ifndef DB_MANIPULATION_H_INCLUDED
#define DB_MANIPULATION_H_INCLUDED
#include "mysql-connector/include/mysql.h"

//provides for external invocations of the functions in different .c files

void createDatabase();
void createTableCustomers(MYSQL *con);
void display_error(MYSQL *con);
void createConnectionStream(MYSQL *db1_con , MYSQL *db2_con);
void closeConnections(MYSQL *db1_con , MYSQL *db2_con);
char *generateCustomerID();
char *generateVideoID();
void newCustomer(MYSQL *db1_con,MYSQL *db2_con);
void newVideo(MYSQL *db1_con);
void searchForVideo(MYSQL *con);
void rentVideo(MYSQL *con,MYSQL *con2);
void updateVideoDetails(MYSQL *con);
void deleteVideo(MYSQL *con);
void updateCustomerDetails(MYSQL *con);
void deleteCustomer(MYSQL *con);
void doPenalization(int penalty);
void returnVideo(MYSQL *con,MYSQL *con2);
void searchForRentedVideo(MYSQL *con,char *customerID);
void showAllVideos(MYSQL *con);
//void showRentalHistory(MYSQL *con);
#endif // DB_MANIPULATION_H_INCLUDED
