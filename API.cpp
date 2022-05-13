#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;


class Attribute
{
    private:
        std::string type;
        float defaultt;
        float min;
        float max;
    public:
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

        std::string getType()
        {
            return type;
        }

        void setAttribute(std::string type , float defaultt , float min , float max)
        {
            Attribute::type=type;
            Attribute::defaultt=defaultt;
            Attribute::min=min;
            Attribute::max=max;
        }


    };

class Component
{
    private:
        std::string id;
        std::string type;
        Attribute attributes;
        std::map<std::string,std::string> netlist;

    public:

    std::string getId()
    {
        return id;
    }

    std::string getType()
    {
        return type;
    }

    Attribute getAttributes()
    {
        return attributes;
    }

    std::map<std::string,std::string> getNetList()
    {
        return netlist;
    }

    void setId(std::string id)
    {
        Component::id=id;
    }

    void setType(std::string type)
    {
        Component::type = type;
    }

    void setNetlist(std::map<std::string,std::string> netlist)
    {
        Component::netlist=netlist;
    }

    void setAttribute(Attribute attributes)
    {
        Component::attributes=attributes;
    }

};

class Topology
{
    private:
        std::string id;
        std::vector<Component> components;

    public:
        std::string getId()
        {
            return id;
        }

        std::vector<Component> getComponents()
        {
            return components;
        }

        void setId(std::string id)
        {
            Topology::id=id;
        }

        void setComponents(std::vector<Component> Component)
        {
            Topology::components=components;
        }

        void addComponent(Component component )
        {
            components.push_back(component);
        }
};

class TopologyAPI
{
    private:
        std::vector<Topology> topologies;

        void saveToMemory(json j)
        {
            Topology t;
            t.setId(j["id"]);
            for (auto component : j["components"])
            {
                Component c;
                std::map<std::string,std::string> netlist;

                for(json::iterator it = component["netlist"].begin(); it != component["netlist"].end(); ++it)
                {
                    netlist[it.key()]=it.value();
                }
                c.setNetlist(netlist);
                c.setId(component["id"]);
                c.setType(component["type"]);
                Attribute a;
                for (json::iterator it = component.begin(); it != component.end(); ++it)
                {
                    if(it.key() !="id" && it.key()!="type" && it.key()!="netlist")
                    {
                        a.setAttribute(it.key(),it.value()["default"]
                        ,it.value()["min"],it.value()["max"]);
                    }
                }
                c.setAttribute(a);
                t.addComponent(c);
            }
            topologies.push_back(t);
        }

    public:
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
            return topologies;
        }


};


int main()
{
    TopologyAPI API ;
    API.readJSON("topology");
    std::vector<Topology> topologies = API.queryTopologies();

    for(auto topology : topologies)
    {
        std::cout << "The id " +  topology.getId() << std::endl;
        std::cout << "The components are  "<< std::endl;

        for(auto component : topology.getComponents())
        {
            Attribute a =component.getAttributes();
            std::cout << "The id " +  component.getId() << std::endl;
            std::cout << "The attributes are  "<< std::endl;
            std::cout << a.getType() << std::endl;
            std::cout << a.getMin()<< std::endl;
            std::cout <<a.getMax() << std::endl;
            std::cout <<  a.getDefaultt() << std::endl;
            for(auto h : component.getNetList())
            {
                std::cout <<h.first;
                std::cout << " " << h.second << std::endl;
            }
        }

    }

}