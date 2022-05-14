#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;


class Component
{
    private:
        std::string id;
        std::string internal_name;
        std::string type;
        float defaultt;
        float min;
        float max;

    public:

    virtual ~Component()
    {
        
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
    };

};

class Resistor:public Component
{
    public: 
        Resistor()
        {
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

        std::string readJSON(std::string fileName)
        {
            std::ifstream i(fileName + ".json");
            json j;
            i >> j;
            saveToMemory(j);
            return j.dump();
        }

        std::vector<Topology> queryTopologies()
        {
            return *topologies;
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
}

int main()
{
    TopologyAPI API ;
    API.readJSON("topology");
    std::vector<Topology> topologies = API.queryTopologies();
    showTopologies(topologies);


}