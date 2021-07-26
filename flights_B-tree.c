/////////////////////// BT19CSE033 GOWTHAM GORREPATI /////////////////////
/////////////////////////// DSPD-2 ASSIGNMENT-2 /////////////////////////
//////////////////////////   ASSUMPTIONS MADE   /////////////////////////
/*
	> I have used B Tree to implement.
	> TIME is in 24 hour format.
	> Strings are inputted without spaces.
	> FLIGHT ID is a non-zero integer <=9999.
	> Each flight record will have a single class (VIP , VVIP or public).
	> DEPARTURE TIME is greater than ARRIVAL TIME for any particular record and a flight arrives and departures on the same day.
	> KEY is taken as a combination of FLIGHT ID and ARRIVAL TIME (giving preference to FLIGHT ID). (except for listUnique)
	> I have assumed that malloc would never fail during the execution.
	> In the updateFlightStatus function, the combination of ID and ARRIVAL TIME are considered as key.
	> We assume that every record in the list2 of updateFlightStatus is present in the main list.
	> In the updateFlightStatus function, I have assumed that PUBLIC can be only promoted to VIP and VIP to only VVIP.
	> In listUnique function, key is the combination of Name and ID (giving preference to Name).
	> Please give length of name < 15 for the output table to not get disturbed.
	> I have focused on reusability of functions rather than space, so I have used the same struct flight_DB for all kinds of lists.
	> I have used comparator function to compare appropriate fields of a record.
	> select_flag will select proper list as per requirement.
	> I have referred B Trees from the book : "Data Structures and Program Design in C++" by Robert Kruse.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BT19CSE033_DSPD-2_ASSIGNMENT-2.h"

int srNo, yesFlag;

void initialize (flight_DB *flp)
{
	flp->root = NULL;
	flp->vroot = NULL;
	flp->vvroot = NULL;
	flp->proot = NULL;
}

treeNode *makeTreeNode (char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], unsigned int key)
{
	int i;
	treeNode *nptr;
	nptr = (treeNode *)malloc(sizeof(treeNode));
	if(nptr != NULL)
	{
		nptr->count=1;
		strcpy(nptr->val[0].flight_name, name);
		nptr->val[0].flight_id = id;
		nptr->val[0].flight_capacity = cap;
		nptr->val[0].arr_time.hour = arr.hour;
		nptr->val[0].arr_time.min = arr.min;
		nptr->val[0].dep_time.hour = dep.hour;
		nptr->val[0].dep_time.min = dep.min;
		nptr->val[0].delay_time.hour = delay_time.hour;
		nptr->val[0].delay_time.min = delay_time.min;
		strcpy(nptr->val[0].flight_class, fl_class);
		strcpy(nptr->val[0].flight_status, fl_status);
		nptr->val[0].key = key;
		for(i=0; i<M; i++)
		{
			nptr->np[i] = NULL;
		}
	}
	return nptr;
}

dataType *makeDataNode (char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], unsigned int key)
{
	int i;
	dataType *nptr;
	nptr = (dataType *)malloc(sizeof(dataType));
	if(nptr != NULL)
	{
		strcpy(nptr->flight_name, name);
		nptr->flight_id = id;
		nptr->flight_capacity = cap;
		nptr->arr_time.hour = arr.hour;
		nptr->arr_time.min = arr.min;
		nptr->dep_time.hour = dep.hour;
		nptr->dep_time.min = dep.min;
		nptr->delay_time.hour = delay_time.hour;
		nptr->delay_time.min = delay_time.min;
		strcpy(nptr->flight_class, fl_class);
		strcpy(nptr->flight_status, fl_status);
		nptr->key = key;
	}
	return nptr;
}

void freeTree (treeNode *root)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				freeTree(root->np[i]);
			freeTree(root->np[i+1]);
		}
		free(root);
		root = NULL;
	}
}

void freeDB (flight_DB *flp)
{
	treeNode *root;
	int i=0;
	for(i=0; i<4; i++)
	{
		root = selectRoot(flp, i); // deleting all lists of a database
		if(root != NULL)
		{
			freeTree(root);
		}
		updateRoot(flp, root, i);
	}
}


void printTime (unsigned int h, unsigned int m)
{
	if(h < 10)
		printf("0%u:",h);
	else
		printf("%u:",h);
	if(m < 10)
		printf("0%u",m);
	else
		printf("%u",m);
}

void visit (treeNode *nptr, int index)
{
	printf("\t\t%s\t\t%u\t%u\t\t", nptr->val[index].flight_name, nptr->val[index].flight_id, nptr->val[index].flight_capacity);
	printTime(nptr->val[index].arr_time.hour, nptr->val[index].arr_time.min);
	printf("\t\t");
	printTime(nptr->val[index].dep_time.hour, nptr->val[index].dep_time.min);
	printf("\t\t%s\t", nptr->val[index].flight_class);
	printTime(nptr->val[index].delay_time.hour, nptr->val[index].delay_time.min);
	printf("\n");
}

void traverse (treeNode *root)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				traverse(root->np[i]);
			printf("\n%d", srNo);
			srNo++;
			visit(root, i);
			traverse(root->np[i+1]);
		}
	}
}

void printRec (flight_DB *flp, int select_flag)
{
	treeNode *root;
	root = selectRoot(flp, select_flag);
	if(root == NULL)
	{
		printf("\nList is empty.\n\n");
	}
	else
	{
		printf("SERIAL NO.\tFLIGHT NAME\tID\tCAPACITY\tARRIVAL TIME\tDEPARTURE TIME\tCLASS\tDELAY TIME");
		srNo = 1;
		traverse(root);
	}
	updateRoot(flp, root, select_flag);
}


unsigned int computeKey (unsigned int id, time arr)
{
	unsigned int key;
	key = 10000*id + 100*arr.hour + arr.min;
	return key;
}

int computeTimeDifference (dataType *ptr)
{
	int retVal = 0;
	unsigned int hour_diff = ptr->dep_time.hour - ptr->arr_time.hour;
	if(ptr->dep_time.min < ptr->arr_time.min)
	{
		hour_diff--;
		retVal = 60;
	}
	retVal =  100*(hour_diff) + (ptr->dep_time.min-ptr->arr_time.min);
	return retVal;
}

treeNode *selectRoot (flight_DB *flp, int select_flag)
{
	// selects appropriate tree
	treeNode *root;
	switch (select_flag)
	{
		case 0: root = flp->root;
				break;
		case 1: root = flp->vroot;
				break;
		case 2: root = flp->vvroot;
				break;
		case 3: root = flp->proot;
				break;
		default : root = flp->root;
				break;
	}
	return root;
}

void updateRoot (flight_DB *flp, treeNode *root, int select_flag)
{
	// updates appropriate tree
	switch (select_flag)
	{
		case 0: flp->root = root;
				break;
		case 1: flp->vroot = root;
				break;
		case 2: flp->vvroot = root;
				break;
		case 3: flp->proot = root;
				break;
		default : flp->root = root;
				break;
	}
}

void copyData (dataType *destination, dataType *source)
{
	// to copy data
	strcpy(destination->flight_name, source->flight_name);
	destination->flight_id = source->flight_id;
	destination->flight_capacity = source->flight_capacity;
	destination->arr_time.hour = source->arr_time.hour;
	destination->arr_time.min = source->arr_time.min;
	destination->dep_time.hour = source->dep_time.hour;
	destination->dep_time.min = source->dep_time.min;
	destination->delay_time.hour = source->delay_time.hour;
	destination->delay_time.min = source->delay_time.min;
	strcpy(destination->flight_class, source->flight_class);
	strcpy(destination->flight_status, source->flight_status);
	destination->key = source->key;
}

// returns true if required contents of aptr < that of bptr
boolean comparator (dataType *aptr, dataType *bptr, int compare_flag)
{
   boolean ret_val = TRUE;
   switch(compare_flag) {
	   case 0: { // key
				   if(aptr->flight_id > bptr->flight_id)
					   ret_val = FALSE;
				   else if(aptr->flight_id == bptr->flight_id)
				   {
					   if(comparator(aptr, bptr, 1) == FALSE)
						   ret_val = FALSE;
				   }
			   }
			   break;
	   case 1: { // arrival time
					if(aptr->arr_time.hour > bptr->arr_time.hour)
					   ret_val = FALSE;
				    else if(aptr->arr_time.hour == bptr->arr_time.hour)
				    {
						if(aptr->arr_time.min > bptr->arr_time.min)
					   		ret_val = FALSE;
						if(aptr->arr_time.min == bptr->arr_time.min && aptr->flight_id >= bptr->flight_id)
							ret_val = FALSE; // for uniqueness
					}
			   }
			   break;
	   case 2: { // departure time
				   if(aptr->dep_time.hour > bptr->dep_time.hour)
					   ret_val = FALSE;
				   else if(aptr->dep_time.hour == bptr->dep_time.hour)
				   {
					   if(aptr->dep_time.min > bptr->dep_time.min)
						   ret_val = FALSE;
					   else if(aptr->dep_time.min == bptr->dep_time.min)
					   	   ret_val = comparator(aptr, bptr, 0); // for uniqueness
				   }
			   }
			   break;
	   case 3: { // stay time
				   if(aptr->dep_time.hour-aptr->arr_time.hour > bptr->dep_time.hour-bptr->arr_time.hour)
					   ret_val = FALSE;
				   else if(aptr->dep_time.hour-aptr->arr_time.hour == bptr->dep_time.hour-bptr->arr_time.hour)
				   {
					   if(aptr->dep_time.min-aptr->arr_time.min > bptr->dep_time.min-bptr->arr_time.min)
						    ret_val = FALSE;
					   else if(aptr->dep_time.min-aptr->arr_time.min == bptr->dep_time.min-bptr->arr_time.min)
					   	   	ret_val = comparator(aptr, bptr, 0); // for uniqueness
				   }
			   }
			   break;
	   case 4: { // delay time
				   if(aptr->delay_time.hour > bptr->delay_time.hour)
					   ret_val = FALSE;
				   else if(aptr->delay_time.hour == bptr->delay_time.hour)
				   {
					   if(aptr->delay_time.min > bptr->delay_time.min)
					   		ret_val = FALSE;
					   else if(aptr->delay_time.min == bptr->delay_time.min)
					   		ret_val = comparator(aptr, bptr, 0); // for uniqueness
				   }
			   }
			   break;
	   case 5: { // for listUnique, Name and ID and delay_time
				   if(strcmp(aptr->flight_name,bptr->flight_name) > 0)
					   ret_val = FALSE;
				   else if(strcmp(aptr->flight_name,bptr->flight_name) == 0)
				   {
					   if(aptr->flight_id > bptr->flight_id)
						   ret_val = FALSE;
					   else if(aptr->flight_id == bptr->flight_id)
					   {
						   if(aptr->delay_time.hour > bptr->delay_time.hour)
						   		ret_val = FALSE;
						   else if(aptr->delay_time.hour == bptr->delay_time.hour && aptr->delay_time.min >= bptr->delay_time.min)
						   {
							   ret_val = FALSE;
						   }
					   }
				   }
			   }
			   break;
		case 6: { // for listUnique, Name and ID
					if(strcmp(aptr->flight_name, bptr->flight_name) > 0)
					   ret_val = FALSE;
				    else if(strcmp(aptr->flight_name, bptr->flight_name) == 0)
				    {
					    if(aptr->flight_id >= bptr->flight_id)
						    ret_val = FALSE;
					}
				}
				break;
	   default : break;
   }
   return ret_val;
}

boolean isKeyEqual (dataType *aptr, dataType *bptr)
{
	boolean ret_val = FALSE;
	if(aptr->flight_id == bptr->flight_id && aptr->arr_time.hour == aptr->arr_time.hour && aptr->arr_time.min == aptr->arr_time.min)
		ret_val = TRUE;
	return ret_val;
}

boolean isNewKeyEqual (dataType *aptr, dataType *bptr)
{
	// for listUnique
	boolean retVal = FALSE;
	if(strcmp(aptr->flight_name, bptr->flight_name) == 0 && aptr->flight_id == bptr->flight_id)
		retVal = TRUE;
	return retVal;
}


status_code insertUpdateSubTree (flight_DB *flp, char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[])
{
	status_code sc = SUCCESS;
	int key = computeKey(id, arr);
	if(strcmp(fl_class, "VIP") == 0)
	{
		sc = insertUpdate(flp, name, id, cap, arr, dep, delay_time, fl_class, fl_status, 1, 0);
		deleteByKey(flp, key, 2);
		deleteByKey(flp, key, 3);
	}
	else if(strcmp(fl_class, "VVIP") == 0)
	{
		sc = insertUpdate(flp, name, id, cap, arr, dep, delay_time, fl_class, fl_status, 2, 0);
		deleteByKey(flp, key, 1);
		deleteByKey(flp, key, 3);
	}
	else if(strcmp(fl_class, "public") == 0)
	{
		sc = insertUpdate(flp, name, id, cap, arr, dep, delay_time, fl_class, fl_status, 3, 0);
		deleteByKey(flp, key, 1);
		deleteByKey(flp, key, 2);
	}
	else
	{
		sc = FAILURE;
	}
	return sc;
}

status_code insertUpdate (flight_DB *flp, char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], int select_flag, int compare_flag)
{
	status_code sc = SUCCESS;
	int key;
	dataType median;
	treeNode *root, *rightSub, *temp;
	root = selectRoot(flp, select_flag);
	if(getNumFlights(flp, select_flag) == DB_SIZE)
	{
		sc = FAILURE;
	}
	else
	{
		key = computeKey(id, arr);
		dataType *data = makeDataNode(name, id, cap, arr, dep, delay_time, fl_class, fl_status, key);
		sc = pushDown(data, root, &median, &rightSub, compare_flag);
		if(sc == FAILURE)
		{
		 	key = computeKey(id, arr);
			temp = makeTreeNode(name, id, cap, arr, dep, delay_time, fl_class, fl_status, key);
			copyData(&(temp->val[0]), &median);
			temp->np[0] = root;
			temp->np[1] = rightSub;
			root = temp;
			sc = SUCCESS;
		}
	}
	updateRoot(flp, root, select_flag);
	return sc;
}

status_code pushDown (dataType* data, treeNode *root, dataType *median, treeNode **rightSub, int compare_flag)
{
	status_code sc = SUCCESS;
	int k = 0; // position
	if(root == NULL)
	{
		copyData(median, data);
		*rightSub = NULL;
		sc = FAILURE;
	}
	else
	{
		// perform search
		int found = 0;
		while(k < root->count && comparator(&(root->val[k]), data, compare_flag) == TRUE)
		{
			k++;
		}
		if(compare_flag == 0 && k < root->count && root->val[k].key == data->key) // for key case
		{
			found = 1;
		} // else found = 0;


		if(found)
		{
			// update
			copyData(&(root->val[k]), data);
			// sc = SUCCESS;
		}
		else
		{
			dataType newData;
			treeNode *newNp;
			sc = pushDown(data, root->np[k], &newData, &newNp, compare_flag);
			if(sc == FAILURE)
			{
				if(root->count < M-1)
				{
					pushIn(&newData, root, newNp, k);
					sc = SUCCESS;
				}
				else
				{
					split(&newData, root, newNp, k, median, rightSub);
					// sc = FAILURE;
				}
			}
		}
	}
	return sc;
}

void pushIn (dataType *data, treeNode *ptr, treeNode *rightSub, int k)
{ // ptr is present node
	int j;
	for(j=ptr->count; j>k; j--)
	{
		copyData(&(ptr->val[j]), &(ptr->val[j-1]));
		ptr->np[j+1] = ptr->np[j];
	}
	copyData(&(ptr->val[k]), data);
	ptr->np[k+1] = rightSub;
	ptr->count++;
}

void split (dataType *data, treeNode *ptr, treeNode *newNp, int k, dataType *median, treeNode **rightSub)
{ // ptr is present node
	// k is position
	int i, mid = M/2;
	*rightSub = (treeNode*) malloc(sizeof(treeNode));
	if(k <= mid)
	{
		for(i = mid; i < M-1; i++)
		{
			copyData(&((*rightSub)->val[i-mid]), &(ptr->val[i]));
			(*rightSub)->np[i+1-mid] = ptr->np[i+1];
		}
		ptr->count = mid;
		(*rightSub)->count = (M-1) - mid;
		pushIn(data, ptr, newNp, k);
	}
	else
	{
		mid++;
		for(i = mid; i < M-1; i++)
		{
			copyData(&((*rightSub)->val[i-mid]), &(ptr->val[i]));
			(*rightSub)->np[i+1-mid] = ptr->np[i+1];
		}
		ptr->count = mid;
		(*rightSub)->count = (M-1) - mid;
		pushIn(data, *rightSub, newNp, k-mid);
	}
	copyData(median, &(ptr->val[ptr->count - 1]));
	(*rightSub)->np[0] = ptr->np[ptr->count];
	ptr->count--;
}

status_code deleteSubTree (flight_DB *flp, unsigned int id)
{
	status_code sc = SUCCESS;
	sc = deleteTree(flp, id, 1);
	sc = deleteTree(flp, id, 2);
	sc = deleteTree(flp, id, 3);
	return sc;
}

status_code deleteTree (flight_DB *flp, unsigned int id, int select_flag)
{
	treeNode *root = selectRoot(flp, select_flag);
	int successFlag = 0;
	status_code sc = SUCCESS, scc;
	while(sc == SUCCESS)
	{
		scc = recursiveDelete(root, id);
		if(scc == SUCCESS)
		{
			successFlag = 1;
		}
		sc = scc;
		if(root != NULL && root->count == 0) // empty root
		{
			treeNode *temp = root;
			root = root->np[0];
			free(temp);
		}
	}
	updateRoot(flp, root, select_flag);
	if(successFlag == 0)
	{
		sc = FAILURE;
	}
	else
	{
		sc = SUCCESS;
	}
	return sc;
}

status_code recursiveDelete (treeNode *ptr, unsigned int id)
{ // ptr is present node
	status_code sc;
	int k=0; // position
	if(ptr == NULL)
	{
		sc = FAILURE;
	}
	else
	{
		// perform search
		int found = 0;
		k=0;
		while(k < ptr->count && ptr->val[k].flight_id < id)
		{
			k++;
		}
		if(k < ptr->count && ptr->val[k].flight_id == id)
		{
			found = 1;
		} // else found = 0;

		if(found)
		{
			sc = SUCCESS;
			if(ptr->np[k] != NULL)
			{
				copyInPredecessor(ptr, k);
				recursiveDelete(ptr->np[k], ptr->val[k].flight_id);
			}
			else
			{
				removeData(ptr, k);
			}
		}
		else
		{
			sc = recursiveDelete(ptr->np[k], id);
		}
		if(ptr->np[k] != NULL && ptr->np[k]->count < (M-1)/2)
		{
			restore(ptr, k);
		}
	}
	return sc;
}

void removeData (treeNode *ptr, int index)
{ // ptr is present node
	int i;
	for(i = index; i < ptr->count - 1; i++)
	{
		copyData(&(ptr->val[i]), &(ptr->val[i+1]));
	}
	ptr->count--;
}

void copyInPredecessor (treeNode *ptr, int index)
{ // ptr is present node
	treeNode *leafNode = ptr->np[index];
	while(leafNode->np[leafNode->count] != NULL)
	{
		leafNode = leafNode->np[leafNode->count];
	}
	copyData(&(ptr->val[index]), &(leafNode->val[leafNode->count - 1]));
}

void restore (treeNode *ptr, int index)
{ // ptr is present node
	if(index == ptr->count)
	{
		if(ptr->np[index - 1]->count > (M-1)/2)
		{
			moveRight(ptr, index-1);
		}
		else
		{
			combine(ptr, index);
		}
	}
	else if(index == 0)
	{
		if(ptr->np[1]->count > (M-1)/2)
		{
			moveLeft(ptr, 1);
		}
		else
		{
			combine(ptr, 1);
		}
	}
	else
	{
		if(ptr->np[index - 1]->count > (M-1)/2)
		{
			moveRight(ptr, index-1);
		}
		else if(ptr->np[index + 1]->count > (M-1)/2)
		{
			moveLeft(ptr, index+1);
		}
		else
		{
			combine(ptr, index);
		}
	}
}

void moveLeft (treeNode *ptr, int index)
{ // ptr is present node
	treeNode *leftSub = ptr->np[index-1];
	treeNode *rightSub = ptr->np[index];
	copyData(&(leftSub->val[leftSub->count]), &(ptr->val[index-1]));
	leftSub->count++;
	leftSub->np[leftSub->count] = rightSub->np[0];
	copyData(&(ptr->val[index-1]), &(rightSub->val[0]));
	rightSub->count--;
	int i;
	for(i=0; i<rightSub->count; i++)
	{
		copyData(&(rightSub->val[i]), &(rightSub->val[i+1]));
		rightSub->np[i] = rightSub->np[i+1];
	}
	rightSub->np[rightSub->count] = rightSub->np[rightSub->count + 1];
}

void moveRight (treeNode *ptr, int index)
{ // ptr is present node
	treeNode *rightSub = ptr->np[index+1];
	treeNode *leftSub = ptr->np[index];
	rightSub->np[rightSub->count + 1] = rightSub->np[rightSub->count];
	int i;
	for(i = rightSub->count; i>0; i--)
	{
		copyData(&(rightSub->val[i]), &(rightSub->val[i-1]));
		rightSub->np[i] = rightSub->np[i-1];
	}
	rightSub->count++;
	copyData(&(rightSub->val[0]), &(ptr->val[index]));
	rightSub->np[0] = leftSub->np[leftSub->count];
	leftSub->count--;
	copyData(&(ptr->val[index]), &(leftSub->val[leftSub->count]));
}

void combine (treeNode *ptr, int index)
{ // ptr is present node
	int i;
	treeNode *leftSub = ptr->np[index-1];
	treeNode *rightSub = ptr->np[index];
	copyData(&(leftSub->val[leftSub->count]), &(ptr->val[index-1]));
	leftSub->count++;
	leftSub->np[leftSub->count] = rightSub->np[0];
	for(i=0; i < rightSub->count; i++)
	{
		copyData(&(leftSub->val[leftSub->count]), &(rightSub->val[i]));
		leftSub->count++;
		leftSub->np[leftSub->count] = rightSub->np[i+1];
	}
	ptr->count--;
	for(i = index - 1; i < ptr->count; i++)
	{
		copyData(&(ptr->val[i]), &(ptr->val[i+1]));
		ptr->np[i+1] = ptr->np[i+2];
	}
	free(rightSub);
}

int recursiveGetNumFlights (treeNode *root)
{
	int retVal = 0;
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				retVal += recursiveGetNumFlights(root->np[i]);
			retVal++;
			retVal += recursiveGetNumFlights(root->np[i+1]);
		}
	}
	return retVal;
}

int getNumFlights (flight_DB *flp, int count_flag)
{
	treeNode *root;
	int retVal = 0;
	root = selectRoot(flp, count_flag);
	if(root != NULL)
	{
		retVal = recursiveGetNumFlights(root);
	}
	updateRoot(flp, root, count_flag);
	return retVal;
}

boolean isEmpty (flight_DB *flp)
{
	boolean ret_val = TRUE;
	if( flp->root != NULL ) {
		ret_val = FALSE;
	}
	return ret_val;
}

boolean isFull (flight_DB *flp)
{
	boolean ret_val = FALSE;
	if(getNumFlights(flp, 0) == DB_SIZE)
		ret_val = TRUE;
	return ret_val;
}

int getMaxStay (treeNode *root, int maxTime)
{
	int retVal = maxTime, x, y, z;
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
			{
				x = getMaxStay(root->np[i], maxTime);
				if(x > maxTime)
					maxTime = x;
			}
			y = computeTimeDifference(&(root->val[i]));
			if(y > maxTime)
				maxTime = y;
			z = getMaxStay(root->np[i+1], maxTime);
			if(z > maxTime)
				maxTime = z;
		}
	}
	return maxTime;
}

void traverseLongestStay (treeNode *root, int max)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				traverseLongestStay(root->np[i], max);
			if(computeTimeDifference(&(root->val[i])) == max )
			{
				printf("Flight id : %u , Arrival Time : ",root->val[i].flight_id);
				printTime(root->val[i].arr_time.hour, root->val[i].arr_time.min);
				printf("\n");
			}
			traverseLongestStay(root->np[i+1], max);
		}
	}
}

void getFlightWithLongestStay (flight_DB *flp)
{
	int n = getNumFlights(flp, 0);
	if(n>0)
	{
		int max = getMaxStay(flp->root, 0);
		printf("Longest stay time is : ");
		printTime(max/100, max%100);
		printf("\n");
		traverseLongestStay(flp->root, max);
	}
}

// used by getSorted
void insertIntoNewDB (flight_DB *flp, treeNode *root, int compare_flag)
{ // flp is new DB
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				insertIntoNewDB(flp, root->np[i], compare_flag);
			status_code sc = insertUpdate(flp, root->val[i].flight_name, root->val[i].flight_id, root->val[i].flight_capacity, root->val[i].arr_time, root->val[i].dep_time, root->val[i].delay_time, root->val[i].flight_class, root->val[i].flight_status, 0, compare_flag);
			insertIntoNewDB(flp, root->np[i+1], compare_flag);
		}
	}
}

void getSorted (flight_DB *flp, int compare_flag)
{
	treeNode *root = flp->root;
	if(root != NULL)
	{
		flight_DB newDB;
		flight_DB *newDBptr = &newDB;
		initialize(newDBptr);
		insertIntoNewDB(newDBptr, root, compare_flag);
		printRec(newDBptr, 0);
		freeDB(newDBptr);
	}
}

void getSortedOnArrivalTime (flight_DB *flp)
{
	getSorted(flp, 1);
	return ;
}

void getSortedOnDepartureTime (flight_DB *flp)
{
	getSorted(flp, 2);
	return ;
}

void getSortedOnStayTime (flight_DB *flp)
{
	getSorted(flp, 3);
	return ;
}

status_code deleteByKey (flight_DB *flp, unsigned int key, int select_flag)
{
	treeNode *root = selectRoot(flp, select_flag);
	int successFlag = 0;
	status_code sc = SUCCESS, scc;
	while(sc == SUCCESS)
	{
		scc = recursiveDeleteByKey(root, key);
		if(scc == SUCCESS)
		{
			successFlag = 1;
		}
		sc = scc;
		if(root != NULL && root->count == 0) // empty root
		{
			treeNode *temp = root;
			root = root->np[0];
			free(temp);
		}
	}
	updateRoot(flp, root, select_flag);
	if(successFlag == 0)
	{
		sc = FAILURE;
	}
	else
	{
		sc = SUCCESS;
	}
	return sc;
}

status_code recursiveDeleteByKey (treeNode *ptr, unsigned int key)
{ // ptr is present node
	status_code sc;
	int k=0; // position
	if(ptr == NULL)
	{
		sc = FAILURE;
	}
	else
	{
		// perform search
		int found = 0;
		k=0;
		while(k < ptr->count && ptr->val[k].key < key)
		{
			k++;
		}
		if(k < ptr->count && ptr->val[k].key == key)
		{
			found = 1;
		} // else found = 0;

		if(found)
		{
			sc = SUCCESS;
			if(ptr->np[k] != NULL)
			{
				copyInPredecessor(ptr, k);
				recursiveDeleteByKey(ptr->np[k], ptr->val[k].key);
			}
			else
			{
				removeData(ptr, k);
			}
		}
		else
		{
			sc = recursiveDeleteByKey(ptr->np[k], key);
		}
		if(ptr->np[k] != NULL && ptr->np[k]->count < (M-1)/2)
		{
			restore(ptr, k);
		}
	}
	return sc;
}

void insertAndDelete (flight_DB *flp, dataType *ptr, unsigned int key, int flag)
{ // for updateFlightStatus
	if(flag == 1)
	{
		// update in VIP
		insertUpdate(flp, ptr->flight_name, ptr->flight_id, ptr->flight_capacity, ptr->arr_time, ptr->dep_time, ptr->delay_time, ptr->flight_class, ptr->flight_status, 1, 0);
		// delete in public
		deleteByKey(flp, key, 3);
	}
	else if(flag == 2)
	{
		// update in VVIP
		insertUpdate(flp, ptr->flight_name, ptr->flight_id, ptr->flight_capacity, ptr->arr_time, ptr->dep_time, ptr->delay_time, ptr->flight_class, ptr->flight_status, 2, 0);
		// delete in VIP
		deleteByKey(flp, key, 1);
	}
}

void updateInSubList (treeNode *root, unsigned int key, time dt)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				updateInSubList(root->np[i], key, dt);
			if(root->val[i].key == key)
			{
				strcpy(root->val[i].flight_status, "DELAY");
				root->val[i].delay_time.hour = dt.hour;
				root->val[i].delay_time.min = dt.min;
			}
			updateInSubList(root->np[i+1], key, dt);
		}
	}
}

void searchInList (flight_DB *flp, treeNode *root, dataType *ptr)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				searchInList(flp, root->np[i], ptr);
			if(ptr->key == root->val[i].key)
			{
				if(strcmp(ptr->flight_status, "DELAY") == 0)
				{
					strcpy(root->val[i].flight_status, "DELAY");
					root->val[i].delay_time.hour = ptr->delay_time.hour;
					root->val[i].delay_time.min = ptr->delay_time.min;
					if(strcmp(root->val[i].flight_class, "public") == 0)
					{
						updateInSubList(flp->proot, ptr->key, root->val[i].delay_time);
					}
					else if(strcmp(root->val[i].flight_class, "VIP") == 0)
					{
						updateInSubList(flp->vroot, ptr->key, root->val[i].delay_time);
					}
					else if(strcmp(root->val[i].flight_class, "VVIP") == 0)
					{
						updateInSubList(flp->vvroot, ptr->key, root->val[i].delay_time);
					}
				}
				else
				{
					if(strcmp(root->val[i].flight_class, "public") == 0)
					{
						strcpy(root->val[i].flight_class, "VIP");
						insertAndDelete(flp, &(root->val[i]), ptr->key, 1);
					}
					else if(strcmp(root->val[i].flight_class, "VIP") == 0)
					{
						strcpy(root->val[i].flight_class, "VVIP");
						insertAndDelete(flp, &(root->val[i]), ptr->key, 2);
					}
				}
			}
			searchInList(flp, root->np[i+1], ptr);
		}
	}
}

void traverse_2 (flight_DB *flp, treeNode *root)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				traverse_2(flp, root->np[i]);
			searchInList(flp, flp->root, &(root->val[i]));
			traverse_2(flp, root->np[i+1]);
		}
	}
}

status_code updateFlightStatus (flight_DB *flp, flight_DB *fl2)
{
	// assuming each record in list_2 is present in main list
	status_code sc = SUCCESS;
	treeNode *root;
	root = fl2->root;
	traverse_2(flp, root);
	return sc;
}

status_code newInsertUpdate (flight_DB *flp, char name[], unsigned int id, unsigned int cap, time arr, time dep, time delay_time, char fl_class[], char fl_status[], int select_flag, int compare_flag)
{
	status_code sc = SUCCESS;
	int key;
	dataType median;
	treeNode *root, *rightSub, *temp;
	root = selectRoot(flp, select_flag);
	if(getNumFlights(flp, select_flag) == DB_SIZE)
	{
		sc = FAILURE;
	}
	else
	{
		key = computeKey(id, arr);
		dataType *data = makeDataNode(name, id, cap, arr, dep, delay_time, fl_class, fl_status, key);
		sc = newPushDown(data, root, &median, &rightSub, compare_flag);
		if(sc == FAILURE)
		{
		 	key = computeKey(id, arr);
			temp = makeTreeNode(name, id, cap, arr, dep, delay_time, fl_class, fl_status, key);
			copyData(&(temp->val[0]), &median);
			temp->np[0] = root;
			temp->np[1] = rightSub;
			root = temp;
			sc = SUCCESS;
		}
	}
	updateRoot(flp, root, select_flag);
	return sc;
}

status_code newPushDown (dataType* data, treeNode *root, dataType *median, treeNode **rightSub, int compare_flag)
{
	status_code sc = SUCCESS;
	int k = 0;
	if(root == NULL)
	{
		copyData(median, data);
		*rightSub = NULL;
		sc = FAILURE;
	}
	else
	{
		// perform search
		int found = 0;
		while(k < root->count && comparator(&(root->val[k]), data, compare_flag) == TRUE)
		{
			k++;
		}
		if(k < root->count && isNewKeyEqual(&(root->val[k]), data) == TRUE && ( (data->delay_time.hour < root->val[k].delay_time.hour) || ((data->delay_time.hour == root->val[k].delay_time.hour) && (data->delay_time.min < root->val[k].delay_time.min)) ) )
		{
			found = 1;
		} // else found = 0;

		if(found)
		{
			// update
			copyData(&(root->val[k]), data);
			// sc = SUCCESS;
		}
		else
		{
			dataType newData;
			treeNode *newNp;
			sc = newPushDown(data, root->np[k], &newData, &newNp, compare_flag);
			if(sc == FAILURE)
			{
				if(root->count < M-1)
				{
					pushIn(&newData, root, newNp, k);
					sc = SUCCESS;
				}
				else
				{
					split(&newData, root, newNp, k, median, rightSub);
					// sc = FAILURE;
				}
			}
		}
	}
	return sc;
}

void updateInNewTree (flight_DB *flp, flight_DB *duptr, dataType *data)
{
	newInsertUpdate(duptr, data->flight_name, data->flight_id, data->flight_capacity, data->arr_time, data->dep_time, data->delay_time, data->flight_class, data->flight_status, 0, 6);
}

void traverseNew (flight_DB *flp, flight_DB *dptr, treeNode *root)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				traverseNew(flp, dptr, root->np[i]);
			updateInNewTree(flp, dptr, &(root->val[i]));
			traverseNew(flp, dptr, root->np[i+1]);
		}
	}
}

void listUnique (flight_DB *flp)
{
	flight_DB dummy;
	initialize(&dummy);
	treeNode *root = flp->root;
	traverseNew(flp, &dummy, root);
	treeNode *temp = root;
	flp->root = (&dummy)->root;
	freeTree(temp);
}

void recursiveSearch (treeNode *root, time start, time end)
{
	if(root != NULL)
	{
		int i;
		for(i=0; i<root->count; i++)
		{
			if(i==0)
				recursiveSearch(root->np[i], start, end);
			if( ( start.hour < root->val[i].arr_time.hour ) || ( start.hour == root->val[i].arr_time.hour && (start.min <= root->val[i].arr_time.min) ) )
			{
				if( ( root->val[i].dep_time.hour < end.hour ) || ( root->val[i].dep_time.hour == end.hour && (root->val[i].dep_time.min <= end.min) ) )
				{
					if(yesFlag == 0)
					{
						printf("\n\nSERIAL NO.\tFLIGHT NAME\tID\tCAPACITY\tARRIVAL TIME\tDEPARTURE TIME\tCLASS\tDELAY TIME");
						srNo = 1;
						yesFlag = 1;
					}
					printf("\n%d", srNo);
					srNo++;
					visit(root, i);
				}
			}
			recursiveSearch(root->np[i+1], start, end);
		}
	}
}

void searchFlight (flight_DB *flp, time start, time end, int select_flag)
{
	treeNode *root = selectRoot(flp, select_flag);
	if(root != NULL)
	{
		srNo = 1;
		recursiveSearch(root, start, end);
	}
	updateRoot(flp, root, select_flag);
}


int main()
{
	flight_DB my_fl, my_fl2, new_db;
	flight_DB *my_ptr = &my_fl, *my2 = &my_fl2, *newp = &new_db;
	initialize (my_ptr);
	initialize (my2);
	initialize (newp);
	// my2 for updateFlightStatus, newp for listUnique

	status_code sc=SUCCESS;
	unsigned int id=0, cap=0, key=0, del=0, hr=0, mi=0;
	time arr, dep, delay_time, start, end;
	arr.hour=0, arr.min=0, dep.hour=0, dep.min=0, delay_time.hour=0, delay_time.min=0;
	start.hour = end.hour = start.min = end.min = 0;
	int i=0, j=0, not_failed=0, num_rec=0, select=12, flag=1, num=0;
	char name[NAME_LEN], fl_class[CLASS_LEN], fl_status[STATUS_LEN];
	treeNode *nptr = NULL;

	while(flag)
	{
		id=0; cap=0; key=0; del=0; hr=0; mi=0;
		arr.hour=0; arr.min=0; dep.hour=0; dep.min=0; delay_time.hour=0; delay_time.min=0;
		start.hour = end.hour = start.min = end.min = 0;
		i=0; j=0; num_rec=0; num=0;
		name[0]='\0'; fl_class[0]='\0'; fl_status[0]='\0';

		select=12;
		printf("Please enter appropriate number each time (until you want to exit : 0) for the corresponding operation\n0. EXIT\n1. insert - to insert records\n2. delete - to delete records using id\n3. getNumFlights - get the number of flight records available\n4. isEmpty - to check if the database is empty\n5. isFull - to check if the database is fully occupied\n6. getFlightWithLongestStay - get flights with longest stay time\n7. getSortedOnArrivalTime - get the main list sorted on arrival time\n8. getSortedOnDepartureTime - get the main list sorted on departure time\n9. getSortedOnStayTime - get the main list sorted on stay time\n10. updateFlightStatus - to update the flight status\n11. listUnique\n12. to Print The Main list of records\n13. to Print The 3 Sorted Lists Of Classes\n14. searchFlight\n\n");
		scanf("%d", &select);
		printf("\n");
		switch (select)
		{
			case 0: { // Exit
						flag=0;
						printf("\nSUCCESS : Exited !!\n");
					}
					break;
			case 1: { // insertUpdate
						delay_time.hour=0;
						delay_time.min=0;
						printf("\nEnter the number of records to insert : ");
						scanf("%d",&num_rec);
						printf("\n");
						for(i=0; i<num_rec; i++)
						{
							printf("Enter the flight name ( without spaces ) : ");
							scanf("%s",name);

							printf("Enter the flight id ( non zero ) : ");
							scanf("%u",&id);

							printf("Enter the flight capacity : ");
							scanf("%u",&cap);

							printf("Enter the space separated hours(0to23) and minutes(0to59) of arrival time respectively in 24 hour format : ");
							scanf("%u%u",&arr.hour, &arr.min);

							printf("Enter the space separated hours(0to23) and minutes(0to59) of departure time respectively in 24 hour format : ");
							scanf("%u%u",&dep.hour, &dep.min);

							printf("Enter the flight class VIP or VVIP or public : ");
							scanf("%s",fl_class);

							char temp[STATUS_LEN];
							temp[0]='\0';

							if (insertUpdate(my_ptr, name, id, cap, arr, dep, delay_time, fl_class, temp, 0, 0) == SUCCESS)
							{
								printf("\nSUCCESS : List updated.\n\n");
								sc = insertUpdateSubTree(my_ptr, name, id, cap, arr, dep, delay_time, fl_class, temp);
							}
							else
							{
								printf("\nFAILURE : There is no space.\n\n");
							}
						}
					}
					break;
			case 2: { // delete
						printf("\nEnter the flight id whose details are to be deleted : ");
						scanf("%u", &del);

						if (deleteTree(my_ptr, del, 0) == SUCCESS)
						{
							printf("\nSUCCESS : Relevant records deleted.\n\n");
							sc = deleteSubTree(my_ptr, del);
						}
						else
						{
							printf("\nFAILURE : Flight details not present.\n\n");
						}
					}
					break;
			case 3: { // getNumFlights
						num=getNumFlights(my_ptr, 0);
						printf("\nSUCCESS: Number of flights are %d.\n\n", num);
					}
					break;
			case 4: { // isEmpty
						if(isEmpty(my_ptr) == TRUE)
							printf("\nSUCCESS : The list is empty.\n\n");
						else
							printf("\nSUCCESS : The list is non empty.\n\n");
					}
					break;
			case 5: { // isFull
						if(isFull(my_ptr) == TRUE)
							printf("\nSUCCESS : The list is fully occupied.\n\n");
						else
							printf("\nSUCCESS : The list is not fully occupied.\n\n");
					}
					break;
			case 6: { // getFlightWithLongestStay
						if(isEmpty(my_ptr) == TRUE)
						{
							printf("\nSUCCESS : The list is empty.\n\n");
						}
						else
						{
							printf("\nSUCCESS : Flight id and arrival time are as follows\n");
							getFlightWithLongestStay(my_ptr);
							printf("\n");
						}
					}
					break;
			case 7: { // getSortedOnArrivalTime
						if(isEmpty(my_ptr) == TRUE)
						{
							printf("\nThe list is empty.\n\n");
						}
						else
						{
							printf("\nSUCCESS : \n\n");
							getSortedOnArrivalTime(my_ptr);
						}
					}
					break;
			case 8: { // getSortedOnDepartureTime
						if(isEmpty(my_ptr) == TRUE)
						{
							printf("\nThe list is empty.\n\n");
						}
						else
						{
							printf("\nSUCCESS : \n\n");
							getSortedOnDepartureTime(my_ptr);
						}
					}
					break;
			case 9: { // getSortedOnStayTime
						if(isEmpty(my_ptr) == TRUE)
						{
							printf("\nThe list is empty.\n\n");
						}
						else
						{
							printf("\nSUCCESS : \n\n");
							getSortedOnStayTime(my_ptr);
						}
					}
					break;
			case 10:{ // updateFlightStatus
						initialize(my2);
						printf("\nEnter the number of records whose details are to be updated : ");
						scanf("%d",&num_rec);

						if( num_rec > DB_SIZE )
						{
							printf("\nFAILURE : Enter valid value.\n\n");
						}
						else
						{
							for (i=0; i < num_rec; i++)
							{
								printf("\nEnter the flight name ( without spaces ) : ");
								scanf("%s",name);

								printf("Enter the flight id ( non zero ) : ");
								scanf("%u",&id);

								printf("Enter the space separated hours(0to23) and minutes(0to59) of arrival time respectively in 24 hour format : ");
								scanf("%u%u",&arr.hour, &arr.min);

								printf("Enter the flight status : ");
								scanf("%s",fl_status);

								printf("Enter the space separated hours and minutes of delay time respectively (0 0 if status is not DELAY) : ");
								scanf("%u%u",&delay_time.hour, &delay_time.min);

								dep.hour = dep.min = 0;
								char temp[CLASS_LEN];
								temp[0]='\0';
								sc = insertUpdate(my2, name, id, 0, arr, dep, delay_time, temp, fl_status, 0, 0);
							}
							sc = updateFlightStatus(my_ptr, my2);
							printf("\n\nSUCCESS : \n");
							printRec(my_ptr, 0);
							printf("\n");
						}
						freeDB(my2);
					}
					break;
			case 11:{ // listUnique
						initialize(newp);
						printf("\nEnter the number of records (non-zero) to add : ");
						scanf("%d",&num_rec);

						if( num_rec > DB_SIZE )
						{
							printf("\nFAILURE : Enter valid value.\n\n");
						}
						else
						{
							for(i=0; i<num_rec; i++)
							{
								printf("\nEnter the flight name ( without spaces ) : ");
								scanf("%s",&name);

								printf("Enter the flight id ( non zero ) : ");
								scanf("%u",&id);

								printf("Enter the flight capacity : ");
								scanf("%u",&cap);

								printf("Enter the space separated hours(0to23) and minutes(0to59) of arrival time respectively in 24 hour format : ");
								scanf("%u%u",&arr.hour, &arr.min);

								printf("Enter the space separated hours(0to23) and minutes(0to59) of departure time respectively in 24 hour format : ");
								scanf("%u%u",&dep.hour, &dep.min);

								printf("Enter the flight class VIP or VVIP or public : ");
								scanf("%s",&fl_class);

								printf("Enter the space separated hours(0to23) and minutes(0to59) of delay time respectively (0 0 if status is not DELAY) : ");
								scanf("%u%u",&delay_time.hour, &delay_time.min);

								char temp[STATUS_LEN];
								temp[0]='\0';

								if (insertUpdate(newp, name, id, cap, arr, dep, delay_time, fl_class, temp, 0, 5) == SUCCESS)
								{
									printf("\nSUCCESS : List updated.\n\n");
								}
								else
								{
									printf("\nFAILURE : There is no space.\n\n");
								}
							}
							printf("\n");
							printf("\nSUCCESS : LIST BEFORE REMOVING DUPLICATES IS\n\n");
							printRec(newp, 0);
							listUnique(newp);
							printf("\nSUCCESS : LIST AFTER REMOVING DUPLICATES IS\n\n");
							printRec(newp, 0);
							freeDB(newp);
						}
					}
					break;
			case 12:{ // printRec
						printf("\n\nLIST OF RECORDS IS : \n\n");
						printRec(my_ptr, 0);
						printf("\n");
					}
					break;
			case 13:{ // printSubTrees
						printf("\n\nVIP list\n");
						printRec(my_ptr, 1);
						printf("\n\nVVIP list\n");
						printRec(my_ptr, 2);
						printf("\n\npublic list\n");
						printRec(my_ptr, 3);
						printf("\n\n");
					}
					break;
			case 14:{ // searchFlight (checks inclusive range)
						printf("Enter the space separated hours(0to23) and minutes(0to59) of start time respectively in 24 hour format : ");
						scanf("%u%u",&start.hour, &start.min);
						printf("Enter the space separated hours(0to23) and minutes(0to59) of end time respectively in 24 hour format : ");
						scanf("%u%u",&end.hour, &end.min);

						printf("\n\nVIP CLASS");
						yesFlag = 0;
						searchFlight(my_ptr, start, end, 1);
						if(yesFlag == 0)
							printf("\n\nNo match.\n\n");

						printf("\nVVIP CLASS");
						yesFlag = 0;
						searchFlight(my_ptr, start, end, 2);
						if(yesFlag == 0)
							printf("\n\nNo match.\n\n");

						printf("\nPublic CLASS");
						yesFlag = 0;
						searchFlight(my_ptr, start, end, 3);
						if(yesFlag == 0)
							printf("\n\nNo match.\n\n");

						printf("\n\n");
					}
					break;
			default:{
						flag=0;
					}
					break;
		}
	}
	freeDB(my_ptr);
	freeDB(my2);
	freeDB(newp);
	return 0;
}
