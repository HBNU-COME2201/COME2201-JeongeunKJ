#include <iostream>
#include <string>
#include <vector>

#include "agent.hpp"
#include "mmanager.hpp"
#include "stochastic_decorator.hpp"
#include "simple_decorator.hpp"
#include "simple_decorator_2.hpp"

#include "tinyxml2.h"

#include "special_agent.hpp"

class InitManager
{
public:
    InitManager(){ }
    //initManager에서 path"test.xml"를 넘겨받음
    InitManager(std::string path) 
    //initManager객체를 생성하면서 path를 전달받고 이를 통해 tinyXml2의 xmldocument에 전달 후 xml을 엶
    {
        doc.LoadFile( path.c_str() );
        
        double x, y, heading, speed, drange;

        //CAgent객체를 생성
        {
            tinyxml2::XMLElement* agListElem = doc.FirstChildElement( "scenario" )->FirstChildElement( "AgentList" );      
            for(tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
                agElem != NULL; agElem = agElem->NextSiblingElement())
            {
                agElem->QueryDoubleAttribute("x", &x);
                agElem->QueryDoubleAttribute("y", &y);
                agElem->QueryDoubleAttribute("heading", &heading);
                agElem->QueryDoubleAttribute("speed", &speed);
                agElem->QueryDoubleAttribute("drange", &drange);

                CAgent* ag = new CAgent(x, y, heading, speed, drange);
                ag = new StochasticDecorator(ag);
                ag = new SimpleDecorator2(ag);
                ag = new SimpleDecorator(ag);


                m_agentList.push_back(ag);
                //m_agentList에 CAgent객체를 추가
            }
        }

        //CSpecialAgent를 생성

        {
            tinyxml2::XMLElement* agListElem = doc.FirstChildElement( "scenario" )->FirstChildElement( "SAgentList" );      
            for(tinyxml2::XMLElement* agElem = agListElem->FirstChildElement();
                agElem != NULL; agElem = agElem->NextSiblingElement())
            {
                agElem->QueryDoubleAttribute("x", &x);
                agElem->QueryDoubleAttribute("y", &y);
                agElem->QueryDoubleAttribute("heading", &heading);
                agElem->QueryDoubleAttribute("speed", &speed);
                agElem->QueryDoubleAttribute("drange", &drange);

                CAgent* ag = new CSpecialAgent(x, y, heading, speed, drange);
                ag = new StochasticDecorator(ag);
                ag = new SimpleDecorator2(ag);
                ag = new SimpleDecorator(ag);
                //m_agentList에 CAgent객체를 추가
                m_agentList.push_back(ag);
            }
        }
    }

    //CAgent 객체를 저장하는 가변배열의 참조값을 반환
    std::vector<CAgent*>& get_agent_list()
    {
        return m_agentList; //ㅂㅐ열의 이름
    }    

private:
    tinyxml2::XMLDocument doc;
    std::vector<CAgent*> m_agentList;
};

int main(int argc, char** argv)
{// Clinet Code  

    // XMLElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
    // XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement( "attributeApproach" );
	// attributeApproachElement->QueryIntAttribute( "v", &v0 );
    InitManager init_manager("test.xml");

    /* Simulation Engine */
    CManeuverManager mmanager;

    for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter )
        mmanager.register_publisher(*iter); 

    /*
    CAgent* p_agent1 = new CAgent(10,  0, 0, 1, 10);
    p_agent1 = new StochasticDecorator(p_agent1);
    p_agent1 = new SimpleDecorator(p_agent1);
    
    mmanager.register_publisher(p_agent1);

    CAgent* p_agent2 = new CAgent(10,  30, 180, 1, 10);
    mmanager.register_publisher(p_agent2);
    */

    /* Simulation Engine Initialization */
    double sim_time;
    double time_step = 1;

    for(sim_time = 0; sim_time < 30; sim_time += time_step)
    {   
        //p_agent1->maneuver(time_step);
        //p_agent2->maneuver(time_step);

        //p_agent1->detect(p_agent2);
        //p_agent2->detect(p_agent1);

        mmanager.svc(time_step);

        std::cout << "----" << std::endl;
        for(std::vector<CAgent*>::iterator iter = init_manager.get_agent_list().begin();
        iter != init_manager.get_agent_list().end(); ++iter )
        
        /*클래스들(SimpleDecorator, 2,CSpecialAgent등)이 CAgent를 상속받고,
        CAgent에 virtual로 선언이 되어 있는 detect()나 toString()을 재정의 하고 있다. 
        따라서 CAgent 인스턴스로 생성가능하고, 클라이언트 코드의 변경 없이도 
        CAgent의 실제 객체에 따라 동작이 달라진다.*/
            std::cout << "Time: " << sim_time << ", " <<**iter << std::endl;
            // (*iter) => CAgent*
            // *(*iter) => CAgent
        

    }
    return 0;
}
