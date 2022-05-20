#include "API.h"

/*
* Helper Function to print the topologies
*/
void showTopologies(std::vector<Topology>& topologies)
{
    for(int i=0;i<topologies.size();i++)
    {
        topologies[i].print();
    }
    std::cout<<std::endl;
}

/*
* Helper Function to print the Devices/components
*/
void showComponents(std::vector<std::shared_ptr<Component>>& components)
{
   for(auto d : components)
   {
       d->print();
   }
    std::cout<<"_________________________________"<<std::endl;

}

int main()
{
    TopologyAPI API ;
    /*
    * READ 'top1','top2','top3'
    */
    API.readJSON("JSON_FILES/topology");
    API.readJSON("JSON_FILES/topology2");
    API.readJSON("JSON_FILES/topology3");

    /*
    * QUERY ALL THE TOPOLOGIES ON MEMORY
    */
    std::vector<Topology> topologies = API.queryTopologies();
    showTopologies(topologies);

    /*
    *QUERY ALL THE DEVICES IN 'top3'
    */
    auto devices = API.queryDevices("top3");
    showComponents(devices);

   /*
   *QUERY THE DEVICES IN 'top3' AND NODE 2
   */
    auto selectedDevices = API.queryDevicesWithNetListNode("top3",2);
    showComponents(selectedDevices);

    /*
    * DELETE top1,top3
    */
    API.deleteTopology("top1");
    API.deleteTopology("top3");

    /*
    * QUERY ALL THE DEVICES LEFT ON MEMORY ('top2')
    */
    std::vector<Topology> topologies2 = API.queryTopologies();
    showTopologies(topologies2);

    /*
    * WRITE 'top2' TO DISK , DELETE IT FROM MEMORY
    * THEN READ IT FROM DISK
    */
    API.writeJSON("top2","JSON_FILES/top2");
    API.deleteTopology("top2");
    API.readJSON("JSON_FILES/top2");

    /*
    * QUERY ALL THE DEVICES LEFT ON MEMORY ('top2')
    */
    std::vector<Topology> topologies3 = API.queryTopologies();
    showTopologies(topologies2);
}