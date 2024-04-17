# Team-010-28-homework4

#### Team Members
Nate Stearley - stear137  
Ryan Hill     - hill1186  
Caleb Otto    - otto0232  
Sean O'Neill   - oneil786  

## Complete Project Overview
This program's goal is to simulate drones picking up passengers and moving them from one destination to another within the University of Minnesota Campus. Similar to the functionality of Uber, users book trips by specifying where they want to be picked up and where they want to be dropped off. This trip is added to a queue and a drone is then tasked with picking up the passengers and moving them to their destination. The user can specify one of three path finding algorithms when booking their trip which dictates how the drone finds the path it needs to take. Each drone is also equipped with a battery. This battery drains slowly if the drone is not over one of many charging stations around the University Campus. Along with adding batteries to Drone, we've also implemented data collection functionality. Our program collects data on the number of trips each drone makes, the distance each drone travels, how much battery each drone uses and recharges. This data is stored in a CSV file and, after the simulation ends, this data is interpreted and summarized to the user.

Link to a short YouTube video explaining the project: https://youtu.be/UUufq51yfnQ

## How to Run the Simulation
### Running the Program Through Git Cloning
Clone the repository and navigate to the project directory

To compile and run the program...
```bash
# Build the project
make -j

# Run the project (./build/web-app <port> <web folder>)
./build/bin/transit_service <PORT> apps/transit_service/web/
```

Navigate to http://127.0.0.1:<PORT> and you should see a visualization.

Navigate to http://127.0.0.1:<PORT>/schedule.html and you should see a page to schedule the trips.

*Note: <PORT> will depend on what port you want to use. If you use port 8082, then it will be http://127.0.0.1:8082.*


### Running the Program Through Docker
Sign into DockerHub and navigate to <https://hub.docker.com/repository/docker/sean786/complete_drone_sim/general>

After cloning the docker image it can be run by doing this
```bash
docker run --rm -it -p <PORT>:<PORT> <image_name>
```
*Note: PORT will depend on what port you want to use. <image_name> will be sean786/complete_drone_sim. If you use port 8082 then this it what it will look like*
```bash
docker run --rm -it -p 8082:8082 sean786/complete_drone_sim
```

Navigate to http://127.0.0.1:<PORT> and you should see a visualization.

Navigate to http://127.0.0.1:<PORT>/schedule.html and you should see a page to schedule the trips.

*Note: When starting the simulation ensure both pages get refreshed first before doing anything on the
schedule page. If the visualization page doesn't get refreshed before using the schedule page the program
will segfault due to entities not being generated in the visualization yet. If it appears a rechargeStation
was not created please just refresh the visualization page and it should be fine afterwords. Also if many electricdrones are added in quick succession the program may slow down for a bit while those drones get created and they may not may move depending on if trips were also being schedule*

## What does the Simulation do Specifically?
(individual features ie movement of entities etc)
This simulation populates a 3D map of the University of Minnesota campus with drones aimed at moving a user's robots from their starting point to their destination. After starting the simulation the user will have a visualization page and a schedule page. The schedule page is used to set up and queue trips. After a trip is queued a robot is spawned in the simulation and it is added to an IEntity list of awaiting trips. A drone then selects the trip if it is valid. For a trip to be valid the drone checks to see that if, on its current battery charge, it can make it to the robot, the robot's destination, and then to a charger. If it can then the trip is valid and the drone selects that trip as its current task. If not it checks the next possible task and leaves the invalid task to another drone. It's important to note that under current battery discharge rates and the amount of available drone chargers it is very unlikely that a trip would not be valid for any drone but it is possible. In the case that the trip is not valid the trip will remain in queue until the program ends to ensure that there is the highest chance that a drone is able to take the robot.

## New Feature Overview
Our team added two new features to this project. 
The first is adding batteries to the drones and adding recharge stations for drones to recharge at in between rides. The second feature we added is data collection functionality. This allows for the collection and storage of information related to the drones trips. 

### What Does it Do and How Does it Add to Existing Work?
These features allow the program to better simulate the barriers that exist in deploying a drone delivery service. Each drone is now equipped with a battery that drains as it flies and a drone is not able to complete a trip if it cannot deliver the robot to its destination and then move to a recharge station.
On creation of a drone it will randomly spawn at one of the multiple recharge stations in the simulation.
There are multiple recharge stations throughout the simulation map and each drone moves to a recharge station after it drops off a drone in order to complete its trip. For the data collection functionality it, more specifically, collects and stores the length, the amount of battery lost, and the amount of battery recharged during, and the time it took for a complete trip for every trip for each drone in the simulation. It then writes this data to a csv file for storage and later interpretation. This implementation adds two new factory classes that are tasked with creating the electric drones and recharge stations. It also adds two new singleton classes that are used to track all created recharge stations and collect drone information. Each drone now has a struct in which it records its current trip data. After the trip is finished the drone calls the dataCollection singleton in order to collect and write the data to a csv. To see the total of all of the results of the drones data we have implemented a
button on the schedule page to end the simulation gracefully and prints the final results of all of the
drones that made trips to the csv file.

### Why is it Significantly Interesting and What Design Patterns are Implemented?
These features are significantly interesting because they add a great deal of new functionality and also are built in a way that allows them to be expanded upon. The battery feature employs a decorator design pattern in order to add additional functionality to the drone class. The class ElectricDrone is built on top of the Drone class and gives the drone a battery rate that directly interfaces with the recharge stations. It is also used to check if a trip is possible in order to claim that trip as "in progress." The recharge stations inherit from IEntity and a singleton RechargeStationRegistry is used to track each recharge station and interface with the electric drone requests. A singleton is also used to create the data collection functionality. This singleton, DataCollection, writes each trip to a csv file after it is finished. Note, each time the simulation is ran if there is current data in the csv file it will get overwritten to make room for the new simulation data. If one wanted to keep previous simulation data they would need to rename the csv file.

### Sprint Retrospective
Our group completed three sprint retrospectives, these were on April 16th, 23rd, and 30th. Each involved us going over what we've worked on in the previous week and talking about what needed to get done for the next week. 
  
##### Sprint 1
We completed the DroneDeco and ElectricDrone class that adds limited battery functionality to the drone. We also decided on how we wanted to implement our isTripPossible() function and how we would eventually add the recharge stations. Our main focus over the next week was to complete all electric drone functionality including the ElectricDroneFactory class and to have all drones be able to recharge at recharge stations.
##### Sprint 2
We completed the rest of the functionality for the electric drone, including the ElectricDrone class and factory as well as the recharge station class and factory. Further testing revealed that it would be easiest to create a singleton to store all of our recharge stations in and query this singleton when information about any recharge station was needed. That and the data collection singleton was the focus of the next week.
##### Sprint 3
The majority of our project was complete. All classes were implemented and the simulation worked as described. The meeting and subsequent week was spent working on documentation, Google code styling, and completing the README. The UML also needed slight changes for the updates completed in the last week.

### UML Diagram

![CSCI 3081W Project Extension UML (2)](https://media.github.umn.edu/user/21124/files/a94bd950-47a1-4f9a-b99a-51641194a2ba)
