#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;


class Component
{
    private:
        std::string id;
        int netlistId;
        std::string internal_name;
        std::string type;
        float defaultt;
        float min;
        float max;

    public:

    int getNetListId()
    {
        return netlistId;
    }
    
    std::string getInternalName()
    {
        return internal_name;
    }

    std::string getId()
    {
        return id;
    }

    std::string getType()
    {
        return type;
    }

    float getMin()
    {
        return min;
    }

    float getMax()
    {
        return max;
    }

    float getDefaultt()
    {
        return defaultt;
    }

    void setId(std::string id)
    {
        Component::id=id;
    }

    void setNetListId(int netlistId)
    {
        Component::netlistId=netlistId;
    }

    void setInternalName(std::string internal_name)
    {
        Component::internal_name=internal_name;
    }

    virtual void setNetlist(std::vector<std::string>& values)=0;

    void setDefaultMinMax( float defaultt , float min , float max)
    {
        Component::defaultt=defaultt;
        Component::min=min;
        Component::max=max;
    }

    void setType(std::string type)
    {
        Component::type=type;
    }

    virtual void print()
    {
        std::cout << "The id of the component : " << getId() << std::endl;
        std::cout << "The Limits are : "<< std::endl;
        std::cout << "MIN : " << getMin()<< std::endl;
        std::cout <<"MAX : " <<getMax() << std::endl;
        std::cout << "DEFAULT : " << getDefaultt() << std::endl;
        std::cout << "netlist_ID: " << getNetListId() << std::endl;
    };

};

class Resistor:public Component
{
    public: 
        Resistor()
        {
            /*
            * it is made 'shared_ptr' to allow the casting 
            */
            netlist= std::make_shared<Resistnace_netlist>();
        }

        std::string getT1()
        {
            return netlist->t1;
        }
        std::string getT2()
        {
            return netlist->t2;
        }

        void setNetlist(std::vector<std::string>& values)
        {
            netlist->t1=values[0];
            netlist->t2=values[1];
        }

        void print()
        {
            Component::print();
            std::cout << "t1: " << getT1() << std::endl;
            std::cout << "t2: " << getT2() << std::endl;
            std::cout<<std::endl;        
        }
    
    private:
        typedef struct
        {
            std::string t1;
            std::string t2;
        }Resistnace_netlist;

        std::shared_ptr<Resistnace_netlist> netlist;    
};

class Nmos:public Component
{
    public:
        Nmos()
        {
            netlist=std::make_shared<Nmos_netlist>();
        }
        

        void setNetlist(std::vector<std::string>& values)
        {
            netlist->drain=values[0];
            netlist->source=values[1];
            netlist->gate=values[2];
        }

        std::string getDrain()
        {
            return netlist->drain;
        }

        std::string getSource()
        {
            return netlist->source;
        }

        std::string getGate()
        {
            return netlist->gate;
        }

        void print()
        {
            Component::print();
            std::cout << "Drain: " << getDrain() << std::endl;
            std::cout << "Source: " << getSource() << std::endl;
            std::cout << "Gate: " << getGate() << std::endl;
            std::cout<<std::endl;

        }
    private:
        typedef struct
        {
            std::string drain;
            std::string gate;
            std::string source;
        }Nmos_netlist;

        std::shared_ptr<Nmos_netlist> netlist;

};

class Topology
{
    private:
        std::string id;
        std::vector<std::shared_ptr<Component>> components;

    public:

        std::string getId()
        {
            return id;
        }

        std::vector<std::shared_ptr<Component>> getComponents()
        {
            return components;
        }

        void setId(std::string id)
        {
            Topology::id=id;
        }

        void setComponents(std::vector<std::shared_ptr<Component>> components)
        {
            Topology::components=components;
        }

        void addComponent(std::shared_ptr<Component> component )
        {
            components.push_back(component);
        }

        void print()
        {
            std::cout << "The ID of the topology :  " << getId() << std::endl;
            std::cout << "The components : "<< std::endl;
            for(int i=0;i<components.size();i++)
            {
                components[i]->print();
            }
        }
};

class TopologyAPI
{
    public:

        TopologyAPI()
        {
            topologies = std::make_shared<std::vector<Topology>>();
        }

        bool readJSON(std::string fileName)
        {
            std::ifstream i(fileName + ".json");
            json j;
            i >> j;
            saveToMemory(j);
            return true;
        }

        std::vector<Topology>& queryTopologies()
        {
            return *topologies;
        }

        /*
        * Returns true if the topology was found and deleted , else false
        */
        bool deleteTopology(std::string id)
        {
            for(int i=0;i<topologies->size();i++)
            {
                if((*topologies)[i].getId().compare(id)==0)
                {
                    topologies->erase(topologies->begin()+i);
                    return true;
                }
            }
            return false;
        }

        std::vector<std::shared_ptr<Component>> queryDevices(std::string id)
        {
            for(int i=0;i<topologies->size();i++)
            {
                if((*topologies)[i].getId().compare(id)==0)
                {
                    return (*topologies)[i].getComponents();
                }
            }
            throw std::invalid_argument("Query is Invalid !");
        }

        std::vector<std::shared_ptr<Component>> queryDevicesWithNetListNode(std::string toplogyId
        ,int netListId)
        {
            auto devices=queryDevices(toplogyId);
            for (int i=0;i<devices.size();i++)
            {
                if(devices[i]->getNetListId()!=netListId)
                {
                    devices.erase(devices.begin()+i);
                    return devices;
                }
            }
            throw std::invalid_argument("Query is Invalid !");
        }

        void writeJSON(std::string id)
        {
            Topology topology;
            for(int i=0;i<topologies->size();i++)
            {
                if((*topologies)[i].getId().compare(id)==0)
                {
                    topology=(*topologies)[i];
                }
            }
            json t_map;
            t_map["id"]=topology.getId();
            std::ofstream file(topology.getId()+".json");
            std::list<json> components;
            for(auto component : topology.getComponents())
            {
                json c_map;
                c_map["id"]=component->getId();
                c_map["type"]=component->getType();
                json limits;
                limits["default"]=component->getDefaultt();
                limits["min"]=component->getMin();
                limits["max"]=component->getMax();
                c_map[component->getInternalName()]=limits;
                limits.clear();
                limits["id"]=component->getNetListId();
                if(component->getType()=="resistor")
                {
                    Resistor r = static_cast<Resistor&>(*component);
                    limits["t1"]=r.getT1();
                    limits["t2"]=r.getT2();
                }
                else if (component->getType()=="nmos")
                {
                    Nmos n = static_cast<Nmos&>(*component);
                    limits["drain"]=n.getDrain();
                    limits["gate"]=n.getGate();
                    limits["source"]=n.getSource();
                }
                c_map["netlist"]=limits;
                components.push_back(c_map);
            }
            t_map["components"]=components;

            file << t_map;
        }
    
    private:
        std::shared_ptr<std::vector<Topology>> topologies;

        void saveToMemory(json j)
        {
            Topology t;
            t.setId(j["id"]);
            for (auto component : j["components"])
            {
                std::vector<std::string> values; 
                if(component["type"]=="resistor")
                {
                    std::shared_ptr<Resistor> r = std::make_shared<Resistor>();
                    r->setType(component["type"]);
                    r->setInternalName("resistance");
                    values.push_back(component["netlist"]["t1"]);
                    values.push_back(component["netlist"]["t2"]);
                    r->setDefaultMinMax(component[r->getInternalName()]["default"],
                    component[r->getInternalName()]["min"],
                    component[r->getInternalName()]["max"]);
                    r->setNetListId(component["netlist"]["id"]);
                    r->setNetlist(values);
                    r->setId(component["id"]);
                    t.addComponent(r);
                }
                else if(component["type"]=="nmos")
                {
                    std::shared_ptr<Nmos> r = std::make_shared<Nmos>();
                    r->setType(component["type"]);
                    r->setInternalName("m(l)");
                    values.push_back(component["netlist"]["drain"]);
                    values.push_back(component["netlist"]["source"]);
                    values.push_back(component["netlist"]["gate"]);
                    r->setDefaultMinMax(component[r->getInternalName()]["default"],
                    component[r->getInternalName()]["min"],
                    component[r->getInternalName()]["max"]);
                    r->setNetListId(component["netlist"]["id"]);
                    r->setNetlist(values);
                    r->setId(component["id"]);
                    t.addComponent(r);
                }
            }
            topologies->push_back(t);
        }

};


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
    API.readJSON("topology");
    API.readJSON("topology2");
    API.readJSON("topology3");

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
    API.writeJSON("top2");
    API.deleteTopology("top2");
    API.readJSON("top2");

    /*
    * QUERY ALL THE DEVICES LEFT ON MEMORY ('top2')
    */
    std::vector<Topology> topologies3 = API.queryTopologies();
    showTopologies(topologies2);
}