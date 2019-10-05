# api2019
## Algorithms and principles of computer science final course project
### Overview
The task is to construct a single file C program for monitoring entities and relationships between them. Entities have a name and relationships have a type, an origin and a destinations (they are not symmetrical). An new entity or relationship can be added to the system and existing ones can be removed. The system should also implement a command 'report' which returns, for each type of monitored relationship, the entity which receives most relationships of that type.
These are the commands the system should implement:
* addent "name" : start monitoring entity "name", if "name" is already being monitored, do nothing
* addrel "origin" "destination" "type" : add relationship "type" between "origin" and "destination", if already existing or if one of the two entities is not being monitored by the system, do nothing
* delent "name" : stop monitoring entity "name", also remove all relationships that used to connect to "name"
* delrel "origin" "destination" "type" : remove relationship "type" between "origin" and "destination"
* report : for each type of relationship monitored by the system, print the name of the entity receiving most instanced of that relationship type and the number of instances, in case of ties, print all names in alphabetical order

in.txt and out.txt are example input and outputs.

To run the program, compile with

    gcc -Wall -o compiled_program source.c
and run with

    ./compiled_program

### Solution comments
I used a red black binary tree to store all the monitored entities, each entity also has two linked lists storing all outgoing and incomming relationships. There is a list storing all information necessary to produce a report which is updated progressively with each command.

### Possible improvements
I started implementing functions to print the binary tree on console, it proved more difficult than I imagined and wasn't really necessary, but would be a nice thing to have.
