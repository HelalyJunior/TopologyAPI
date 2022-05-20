

# TOPOLOGY API

![alt text](https://github.com/HelalyJunior/TopologyAPI/blob/main/IMAGES/cover.png)

The library Provides the functionality to **access**, **manage** and **store** device topologies, given different ***JSON*** files each includes *one* topology, storing them in desk and applying different queries on them.
## Description
- A topology is a set of electronic components that are connected together.
- The **API** supports the following operations:
    1. Read topology from ***JSON*** file and store it in memory.
    2. Write topology into a ***JSON*** file and store it on the disk.
    3. Query the list of topologies stored in memory.
    4. Query the devices of a certain topology given an id.
    5. Query the devices that are connected to each other through a given node in a 		 certain topology.
    6. Delete  a topology from memory given an id.
## Development
- During Development I used : 
  1 - *Visual Studio Code* / *Visual Studio 2019* as ***IDE***
  2 - *Microsoft Code Analysis tool* for ***testing***
  3 - *nlohmann-json* which is a library that help parsing ***JSON*** files
  4 - *Visual Paradigm Online* to create the ***class diagram***

## Why C++?
**C++** is a powerful general-purpose programming language. It can be used to develop different  types of application such as games , **APIs** and so on. **C++** also supports writing the code in different paradigms including functional programming and **object-oriented programming** which makes it very powerful and extremely flexible and that's the reason **C++** is my go-to language.

## Class Documentation

The Figure below can show the class diagram that can clearly show the connection between the classes in the system.
![alt text](https://github.com/HelalyJunior/TopologyAPI/blob/main/IMAGES/Class-Diagram.jpg)
##### Note that the NodeList has an id.

The `Nmos_netlist` and The `Resistance_netlist` structs aren't shown in the diagram but their implementation can been shown here :

    typedef  struct
    {
	    std::string  t1;
	    std::string  t2;
    }Resistnace_netlist;

    typedef  struct
    { 
	    std::string  drain;  
	    std::string  gate;
	    std::string  source;    
    }Nmos_netlist;

## API Documentation
Here will be a detailed explanation on each function provided by the **API** and how to use it.

    bool readJSON(std::string  path)

- **Description**:
	-  Loads the topology from the **JSON** file into memory.
- **Parameters**: 
	-  `path`: the path of the given ***JSON*** file.
- **Return**:  
	- `true` on success
- **Throw**:
	-   `std::invalid_argument` if the path is wrong or the ***JSON*** content is invalid.
##
     std::vector<Topology>&  queryTopologies()
- **Description**: 
	- Gets all the topologies stored in the memory.
- **Parameters**:
     -  `NONE`
- **Return**:  
	- `std::vector<Topology>` : A copy of all the topologies store in the memory.
##
     
    bool  deleteTopology(std::string  id)

- **Description**: 
	- Deletes a topology from the memory.
- **Parameters**:
     -  `id` : The id of the topology to be removed
- **Return**:  
	- `true` on success
- **Throw**:
	-   `std::invalid_argument` if the id is invalid.
##
     std::vector<std::shared_ptr<Component>> queryDevices(std::string  id)

- **Description**: 
	- Gets all the devices stored in this topology.
- **Parameters**:
     -  `id` : The id of the topology that contains the devices
- **Return**:  
	- `std::vector<std::shared_ptr<Component>>`  : A vector of pointers that points to objects of type `Component`. It was designed in this manner so that the user can latter on use `static_cast`to cast to a `Resistor`object or `Nmos`object. *****Don't*** worry about memory management , we handle this for you.**
- **Throw**:
	-   `std::invalid_argument` if the id is invalid.
##
     std::vector<std::shared_ptr<Component>> queryDevicesWithNetListNode(std::string  toplogyId,int  netListId)

- **Description**: 
	- Gets the devices stored in this topology **and** connected to the given node.
- **Parameters**:
     -  `toplogyId` : The id of the topology that contains the devices.
     - `netList id` : The id of the node that the device is connected to.
- **Return**:  
	- `std::vector<std::shared_ptr<Component>>`  : A vector of pointers that points to objects of type `Component`. It was designed in this manner so that the user can latter on use `static_cast`to cast to a `Resistor`object or `Nmos`object. *****Don't*** worry about memory management , we handle this for you.**
- **Throw**:
	-   `std::invalid_argument` if either id is invalid.
##
    void  writeJSON(std::string  id,std::string  path)
- **Description**: 
	- Writes a topology from the memory into the disk.
- **Parameters**:
     -  `id` : The id of the topology
     - `path`: The path and the name of the file to be created. **CHECK** [`examples.cpp](https://github.com/HelalyJunior/TopologyAPI/blob/main/Examples.cpp)`
- **Return**:  
	- `void`
- **Throw**:
	-   `std::invalid_argument` if the id is invalid.

## Testing
Test cases can be seen from [here](https://github.com/HelalyJunior/TopologyAPI/blob/main/API_TESTING/API_Testing.cpp)
*Microsoft's Code Analysis* tool was used to automatically test the **API**.
![alt text](https://github.com/HelalyJunior/TopologyAPI/blob/main/IMAGES/tests.png)

## How to use the API?
All you need to do to use the API is to include the header file into your files.

    #include "API.h"
But to even make the compilation of your files faster , you can **pre-compile** the library one time and use the binarized version later on which will significantly make the program faster. All you need to do is to include the `pch.h` file into your file instead of `API.h` as follows :

    #include "pch.h"
and then in the terminal compile the `pch.h` file , it will generate another file of nearly **50 MB** of size.

    g++ pch.h
## Enjoy !