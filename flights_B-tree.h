/////////////////////// GOWTHAM GORREPATI /////////////////////
///////////////////////////// HEADER FILE //////////////////////////////

#ifndef BT19CSE033_A2_H
#define BT19CSE033_A2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define DB_SIZE 100
#define NAME_LEN 20
#define CLASS_LEN 20
#define STATUS_LEN 20
#define M 5
typedef enum{FAILURE, SUCCESS} status_code;
typedef enum{FALSE, TRUE} boolean;

typedef struct time_tag // assumed in 24 hour format
{
	unsigned int hour;
	unsigned int min;
} time;

// Contains all the fields required for any type of record
typedef struct dataType_tag
{
	char flight_name[NAME_LEN];// ASSUMING NAME WITHOUT SPACES
	unsigned int flight_id;
	unsigned int flight_capacity;
	time arr_time;
	time dep_time;
	char flight_class[CLASS_LEN];// ASSUMING NAME WITHOUT SPACES

	unsigned int key;

	char flight_status[STATUS_LEN];// ASSUMING NAME WITHOUT SPACES
	time delay_time;
} dataType;

typedef struct treeNode_tag
{
	int count;
	dataType val[M-1]; // M-1
    struct treeNode_tag *np[M]; // M
} treeNode;

typedef struct flight_DB_tag
{
	treeNode *root; // main tree
	treeNode *vroot; // tree for VIP
	treeNode *vvroot; // tree for VVIP
	treeNode *proot; // tree for public
} flight_DB;


// Function Declarations
void initialize (flight_DB *flp);
treeNode *makeTreeNode (char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], unsigned int key);
dataType *makeDataNode (char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], unsigned int key);
void freeTree (treeNode *root);
void freeDB (flight_DB *flp);

void printTime (unsigned int h, unsigned int m);
void visit (treeNode *nptr, int index);
void traverse (treeNode *root);
void printRec (flight_DB *flp, int select_flag);

unsigned int computeKey (unsigned int id, time arr);
int computeTimeDifference (dataType *ptr);
treeNode *selectRoot (flight_DB *flp, int select_flag);
void updateRoot (flight_DB *flp, treeNode *root, int select_flag);
void copyData (dataType *destination, dataType *source);
boolean comparator (dataType *aptr, dataType *bptr, int compare_flag);
boolean isKeyEqual (dataType *aptr, dataType *bptr);
boolean isNewKeyEqual (dataType *aptr, dataType *bptr);

status_code insertUpdateSubTree (flight_DB *flp, char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[]);
status_code insertUpdate (flight_DB *flp, char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], int select_flag, int compare_flag);
status_code pushDown (dataType* data, treeNode *root, dataType *median, treeNode **rightSub, int compare_flag);
void pushIn (dataType *data, treeNode *ptr, treeNode *rightSub, int k);
void split (dataType *data, treeNode *ptr, treeNode *newNp, int k, dataType *median, treeNode **rightSub);
status_code deleteSubTree (flight_DB *flp, unsigned int id);
status_code deleteTree (flight_DB *flp, unsigned int id, int select_flag);
status_code recursiveDelete (treeNode *ptr, unsigned int id);
void removeData (treeNode *ptr, int index);
void copyInPredecessor (treeNode *ptr, int index);
void restore (treeNode *ptr, int index);
void moveLeft (treeNode *ptr, int index);
void moveRight (treeNode *ptr, int index);
void combine (treeNode *ptr, int index);
int recursiveGetNumFlights (treeNode *root);
int getNumFlights (flight_DB *flp, int count_flag);
boolean isEmpty (flight_DB *flp);
boolean isFull (flight_DB *flp);
int getMaxStay (treeNode *root, int maxTime);
void traverseLongestStay (treeNode *root, int max);
void getFlightWithLongestStay (flight_DB *flp);
void insertIntoNewDB (flight_DB *flp, treeNode *root, int compare_flag);
void getSorted (flight_DB *flp, int compare_flag);
void getSortedOnArrivalTime (flight_DB *flp);
void getSortedOnDepartureTime (flight_DB *flp);
void getSortedOnStayTime (flight_DB *flp);
status_code deleteByKey (flight_DB *flp, unsigned int key, int select_flag);
status_code recursiveDeleteByKey (treeNode *ptr, unsigned int id);
void insertAndDelete (flight_DB *flp, dataType *ptr, unsigned int key, int flag);
void updateInSubList (treeNode *root, unsigned int key, time dt);
void searchInList (flight_DB *flp, treeNode *root, dataType *ptr);
void traverse_2 (flight_DB *flp, treeNode *root);
status_code updateFlightStatus (flight_DB *flp, flight_DB *fl2);
status_code newInsertUpdate (flight_DB *flp, char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], int select_flag, int compare_flag);
status_code newPushDown (dataType* data, treeNode *root, dataType *median, treeNode **rightSub, int compare_flag);
void updateInNewTree (flight_DB *flp, flight_DB *duptr, dataType *data);
void traverseNew (flight_DB *flp, flight_DB *dptr, treeNode *root);
void listUnique (flight_DB *flp);
void recursiveSearch (treeNode *root, time start, time end);
void searchFlight (flight_DB *flp, time start, time end, int select_flag);

#endif // BT19CSE033_A2_H
