# flight-management-system-in-C-using-B-tree

5 way

A Flight Records Management System is implemented as a B tree, each record has fields like flight name, flight id, flight capacity, flight arrival time, flight departure time, flight class (VIP, VVIP, public), etc.

The combined pair of flight id and arrival time of the flight can be thought of as a key in the list and represents a unique record in a single list. The list of records are always sorted (after any operation) on the basis of key. Also kept are 3 sorted lists, one each for every class of the flight.

Opeartions:

insert / update
Inserts (if the entry is not present in the list) or updates an element (if the entry is present in the list)
I/p parameters: flight name, flight id, flight capacity, flight arrival time, flight departure time, flight class (VIP, VVIP, public).
O/p – Entry should be added/updated appropriately. The return parameter can specify if the insert operation is successful or not.

delete
Deletes an element/s
i/p parameters:
list (list of records from which the entry needs to be deleted)
flight id of the flight whose records need to be deleted.
o/p parameters: To know the operation is successful or not you have to print the appropriate message.

getNumFlights
i/p parameters: list (linked list of records)
o/p: number of flights in the list.

isEmpty
i/p parameters: list (linked list of records)
o/p: To know if the list is empty or not

isFull
i/p parameters: list (original linked list of records)
o/p: To know if the list is fully occupied or not

geFlightWithtLongeststay
i/p parameters: list (original linked list of records)
o/p : flight(s) with max stay time (departure time – arrival time)

getSortedOnArrivalTime
i/p parameters: list (original linked list of records)
o/p parameters : list sorted based on arrival time

getSortedOnDepartureTime
i/p parameters: list (original linked list of records)
o/p parameters : list sorted based on departure time

getSortedOnStayTime
i/p parameters: list (original linked list of records)
o/p parameters : list sorted based on stay time (departure time – arrival time)

UpdateFlightStatus 
• i/p paraments: : tree (original tree of records), tree2 (each record containing flight id, 
flight name, status (DELAY, ON_TIME, CANCELLED, PROMOTED), delay time. 
• Description : If status is DELAY, then delay_time gives us the time by which the 
flight is delayed for arrival. The stay time (difference between arrival and departure 
times) for the flight remains the same. If status is PROMOTED, it promotes the flight 
class, so the original flight class and promoted flight class to be provided. 
• o/p: Updates the original tree according to status. The Output must be sorted on key. 
Also updates the 3 trees maintained for each flight class.

list Unique 
• i/p parameters – A tree of records which contains duplicate entries (with same flight 
name and flight id) 
• Output – Remove duplicate entries if present. Retain the occurrence with minimum 
delay, in case of duplicates.

 searchFlight 
• i/p parameters – A tree of records and the range of start & end time (24-hours scale)
• Output – The flight details which have scheduled departure & arrival time withing the 
time range. The details should be printed according to flight class

