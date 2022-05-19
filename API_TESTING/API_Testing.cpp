#include "pch.h"
#include "CppUnitTest.h"
#include "API.h"
#define MAIN_PATH "F:\\Programming\\MasterMicro\\TopologyAPI\\TopologyAPI_Testing\\API_Testing\\API_Testing\\topology"
#define MAIN_PATH2 "F:\\Programming\\MasterMicro\\TopologyAPI\\TopologyAPI_Testing\\API_Testing\\API_Testing\\topology2"
#define CREATED_PATH "F:\\Programming\\MasterMicro\\TopologyAPI\\TopologyAPI_Testing\\API_Testing\\API_Testing\\top1"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace APITesting
{
	TEST_CLASS(APITesting)
	{
	public:
		
		TEST_METHOD(Test_Successful_ReadJson)
		{
			/*
			* CREATE THE EXPECTED OBJECT
			*/
			std::shared_ptr<Resistor> resistor = std::make_shared<Resistor>();
			resistor->setType("resistor"); resistor->setId("res1"); resistor->setInternalName("resistance");
			resistor->setDefaultMinMax(100, 10, 1000); resistor->setNetListId(1);
			auto r_netlist = std::vector<std::string>{ "vdd", "n1" };
			resistor->setNetlist(r_netlist);
			std::shared_ptr<Nmos> nmos = std::make_shared<Nmos>();
			nmos->setType("nmos"); nmos->setId("m1"); nmos->setInternalName("m(l)"); nmos->setDefaultMinMax(1.5, 1, 2);
			auto n_netlist = std::vector<std::string>{ "n1", "vss", "vin" };
			nmos->setNetListId(2); nmos->setNetlist(n_netlist);
			Topology topology;
			auto components = std::vector<std::shared_ptr<Component>>{ resistor, nmos };
			topology.setId("top1"); topology.setComponents(components);

			TopologyAPI API;
			API.readJSON(MAIN_PATH);
			assert(API.queryTopologies()[0]==topology);
		}
		TEST_METHOD(Test_Successful_Delete)
		{
			TopologyAPI API;
			API.readJSON(MAIN_PATH);
			assert(API.queryTopologies().size() == 1);
			API.deleteTopology("top1");
			assert(API.queryTopologies().size() == 0);
		}
		TEST_METHOD(Test_Invalid_Delete)
		{
			TopologyAPI API;
			try
			{
				API.deleteTopology("top1");
			}
			catch (std::invalid_argument)
			{

			}
		}
		TEST_METHOD(Test_Read_Json_Invalid_Path)
		{
			TopologyAPI API;
			try
			{
				API.readJSON("");
			}
			catch(std::invalid_argument)
			{

			}
		}
		TEST_METHOD(Read_Write_Query_Delete_Sequence)
		{
			TopologyAPI API;
			API.readJSON(MAIN_PATH);
			auto original_topology = API.queryTopologies()[0];
			API.writeJSON("top1",CREATED_PATH);
			API.deleteTopology("top1");
			API.readJSON(CREATED_PATH);
			auto new_topology = API.queryTopologies()[0];
			assert(original_topology == new_topology);
		}
		TEST_METHOD(Test_Successful_Query_Devices)
		{
			/*
			* CREATE THE EXPECTED OBJECT
			*/
			std::shared_ptr<Resistor> resistor = std::make_shared<Resistor>();
			resistor->setType("resistor"); resistor->setId("res1"); resistor->setInternalName("resistance");
			resistor->setDefaultMinMax(100, 10, 1000); resistor->setNetListId(1);
			auto r_netlist = std::vector<std::string>{ "vdd", "n1" };
			resistor->setNetlist(r_netlist);
			std::shared_ptr<Nmos> nmos = std::make_shared<Nmos>();
			nmos->setType("nmos"); nmos->setId("m1"); nmos->setInternalName("m(l)"); nmos->setDefaultMinMax(1.5, 1, 2);
			auto n_netlist = std::vector<std::string>{ "n1", "vss", "vin" };
			nmos->setNetListId(2); nmos->setNetlist(n_netlist);
			auto components = std::vector<std::shared_ptr<Component>>{ resistor, nmos };

			TopologyAPI API;
			API.readJSON(MAIN_PATH);
			auto devices = API.queryDevices("top1");
			for (int i = 0 ; i < devices.size(); i++)
			{
				assert(*devices[i] == *components[i]);
			}
		}
		TEST_METHOD(Test_Invalid_Query_Devices)
		{
			try
			{
				TopologyAPI API;
				auto devices = API.queryDevices("top1");
			}
			catch (std::invalid_argument)
			{

			}

		}
		TEST_METHOD(Test_Successful_QueryDevices_NetList)
		{
			/*
			* CREATE THE EXPECTED OBJECT
			*/
			std::shared_ptr<Resistor> resistor = std::make_shared<Resistor>();
			resistor->setType("resistor"); resistor->setId("res1"); resistor->setInternalName("resistance");
			resistor->setDefaultMinMax(100, 10, 1000); resistor->setNetListId(1);
			auto r_netlist = std::vector<std::string>{ "vdd", "n1" };
			resistor->setNetlist(r_netlist);
			auto components = std::vector<std::shared_ptr<Component>>{ resistor};

			TopologyAPI API;
			API.readJSON(MAIN_PATH);
			auto devices = API.queryDevicesWithNetListNode("top1",1);
			for (int i = 0; i < devices.size(); i++)
			{
				assert(*devices[i] == *components[i]);
			}
		}
		TEST_METHOD(Test_Successful_Advanced_QueryDevices_NetList)
		{
			/*
			* CREATE THE EXPECTED OBJECT
			*/
			std::shared_ptr<Resistor> resistor = std::make_shared<Resistor>();
			resistor->setType("resistor"); resistor->setId("res2"); resistor->setInternalName("resistance");
			resistor->setDefaultMinMax(100, 10, 1000); resistor->setNetListId(1);
			auto r_netlist = std::vector<std::string>{ "vdd", "n1" };
			resistor->setNetlist(r_netlist);
			std::shared_ptr<Nmos> nmos = std::make_shared<Nmos>();
			nmos->setType("nmos"); nmos->setId("m2"); nmos->setInternalName("m(l)"); nmos->setDefaultMinMax(1.5, 1, 2);
			auto n_netlist = std::vector<std::string>{ "n1", "vss", "vin" };
			nmos->setNetListId(1); nmos->setNetlist(n_netlist);
			Topology topology;
			auto components = std::vector<std::shared_ptr<Component>>{ resistor, nmos };
			topology.setId("top2"); topology.setComponents(components);


			TopologyAPI API;
			API.readJSON(MAIN_PATH2);
			auto devices = API.queryDevicesWithNetListNode("top2", 1);
			for (int i = 0; i < devices.size(); i++)
			{
				assert(*devices[i] == *components[i]);
			}
		}
		TEST_METHOD(TestInvalidNetList)
		{
			try
			{
				TopologyAPI API;
				API.readJSON(MAIN_PATH);
				auto devices = API.queryDevicesWithNetListNode("top1", 13);
			}
			catch (std::invalid_argument)
			{

			}

		}



	};
}
