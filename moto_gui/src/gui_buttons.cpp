// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------
#include <ros/ros.h>
#include <ros/package.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <Qt>
#include <QPainter>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QMessageBox>
#include <QComboBox>
#include <QAction>
#include <QVariant>
#include <QStringList>
#include <QTextEdit>
#include "moto_gui/gui_buttons.h"
#include "dirent.h"

using namespace std;

namespace gui_btn
{
    gui_buttons::gui_buttons( QWidget* parent ) : rviz::Panel( parent )
    {
        
        // create layout for GUI buttons  
        QVBoxLayout* layout = new QVBoxLayout;




        // create robot list button /////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////
        QHBoxLayout* robot_list_layout = new QHBoxLayout;
        robot_list = new QComboBox;
        robot_list->addItem(tr(""));
        // QString robot_item="gp8";
        // robot_list->addItem(robot_item);
        // robot_item="gp12";
        // robot_list->addItem(robot_item);
        DIR *bot_dir;
        struct dirent *bot_entry;
        std::string package_name[3]={"motoman","abb","iiwa"};
        std::string src_path =ros::package::getPath("moto_gui")+"/../";
        std::string package_path;
        for(int i=0; i<3; i++)
        {
            if(i!=2)
            {
               package_path=src_path+package_name[i];
                if ((bot_dir = opendir (package_path.c_str())) != NULL) 
                {
                    /* print all the files and directories within directory */
                    while ((bot_entry = readdir (bot_dir)) != NULL)
                    { 
                        std::string s1 = bot_entry->d_name;
                        if (s1.find("_support")!= std::string::npos)
                        {
                            
                            int pos=s1.length()-8;
                            // std::string::size_type pos = s1.find('s');
                            robot_list->addItem(tr(s1.substr(0,pos).c_str()));
                        }
                    }
                    closedir (bot_dir);
                } 
                else 
                {
                    // could not open mold directory
                    std::string errormsg= "Could not find "+ package_name[i]+" robot directory...check the directory path";
                    ROS_ERROR(errormsg.c_str());  
                }
            }
            else
            {
                package_path = ros::package::getPath("iiwa_description")+"/urdf/";
                if ((bot_dir = opendir (package_path.c_str())) != NULL) 
                {
                    /* print all the files and directories within directory */
                    while ((bot_entry = readdir (bot_dir)) != NULL)
                    { 
                        std::string s1 = bot_entry->d_name;
                        if (s1.find("iiwa7.xacro")!= std::string::npos)
                        {
                            // std::string::size_type pos = s1.find('s');
                            QString bot= "iiwa7";
                            robot_list->addItem(bot);
                        }
                        else if (s1.find("iiwa14.xacro")!= std::string::npos)
                        {
                            // std::string::size_type pos = s1.find('s');
                            QString bot= "iiwa14";
                            robot_list->addItem("iiwa14");
                        }
                    }
                    closedir (bot_dir);
                } 
                else 
                {
                    // could not open mold directory
                    std::string errormsg= "Could not find "+ package_name[i]+" robot directory...check the directory path";
                    ROS_ERROR(errormsg.c_str());  
                }
            }


        }


        robot_list_layout->addWidget(new QLabel("Select Robot:"));
         robot_list_layout->addWidget(robot_list);
        layout->addLayout(robot_list_layout);


        // create Tool list button //////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        QHBoxLayout* tool_list_layout = new QHBoxLayout;
        tool_list = new QComboBox;
        tool_list->addItem(tr(""));
        QString tool_item="probe";
         tool_list->addItem(tool_item);
         tool_item="nozzle_tube";
         tool_list->addItem(tool_item);
        // DIR *tools_dir;
        // struct dirent *tool_entry;
        // package_path = ros::package::getPath("gen_utilities") + "/meshes/tools/";
        // if ((tools_dir = opendir (package_path.c_str())) != NULL) 
        // {
            // /* print all the files and directories within directory */
            // while ((tool_entry = readdir (tools_dir)) != NULL)
            // { 
                // std::string s1 = tool_entry->d_name;
                // if (s1.find(".stl")!= std::string::npos)
                // {
                    // std::string::size_type pos = s1.find('.');
                    // tool_list->addItem(tr(s1.substr(0,pos).c_str()));
                // }
            // }
            // closedir (tools_dir);
        // } 
        // else 
        // {
            // // could not open tools directory 
            // ROS_ERROR("Could not find tools directory...check the mold directory path");  
        // }

        tool_list_layout->addWidget(new QLabel("Select Tool:"));
         tool_list_layout->addWidget(tool_list);
        layout->addLayout(tool_list_layout);




        ////create button for launching feature selcting software///////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        object_import= new QPushButton("Object Import");
        layout->addWidget(object_import);

        //create button for Trajectory Simulation///////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        QHBoxLayout* simulation_layout = new QHBoxLayout;
        calculate= new QPushButton("Calculate");
        simulation_layout->addWidget(calculate);
        simulator= new QPushButton("Trajectory Simulation");
        simulation_layout->addWidget(simulator);
        layout->addLayout(simulation_layout);
        ///create controller list////////////////////////////
        ///////////////////////////////////////////////////
        QHBoxLayout* ctrl_list_layout = new QHBoxLayout;
        controller_list=new QComboBox;
        controller_list->addItem(tr(""));
        QString ctrl_item="yrc1000";
        controller_list->addItem(ctrl_item);
        ctrl_item="dx200";
        controller_list->addItem(ctrl_item);
        ctrl_item="fs100";
        controller_list->addItem(ctrl_item);

        ctrl_list_layout->addWidget(new QLabel("Select Controller:"));
        ctrl_list_layout->addWidget(controller_list);
        layout->addLayout(ctrl_list_layout);


        ////Create edit box for robot ip///////////////////////////////////
        ///////////////////////////////////////////////////////////////////
        QHBoxLayout* ping_list_layout = new QHBoxLayout;
        ping_list_layout->addWidget(new QLabel("robot_ip: "));
        ping1=new QLineEdit;
        ping1->setPlaceholderText("192");
        ping_list_layout->addWidget(ping1);
        ping_list_layout->addWidget(new QLabel("."));

        ping2=new QLineEdit;
        ping2->setPlaceholderText("168");
        ping_list_layout->addWidget(ping2);
        ping_list_layout->addWidget(new QLabel("."));

        ping3=new QLineEdit;
        ping3->setPlaceholderText("10");
        ping_list_layout->addWidget(ping3);
        ping_list_layout->addWidget(new QLabel("."));

        ping4=new QLineEdit;
        ping4->setPlaceholderText("31");
        ping_list_layout->addWidget(ping4);

        ping_check= new QPushButton("Connect");
        ping_list_layout->addWidget(ping_check);

        layout->addLayout(ping_list_layout);
        //create button for running the trajectory///////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////////////
        QHBoxLayout* stream_layout = new QHBoxLayout;
        robot_enable= new QPushButton("ENABLE");
        stream_layout->addWidget(robot_enable);
        run= new QPushButton("RUN");
        stream_layout->addWidget(run);
        layout->addLayout(stream_layout);

        // create notification box/////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////
        txt = new QTextEdit();
        txt->setText("Trajectory visualizer:\nStart by selecting the robot\n\nFor streaming purpose:\n Please select the controller and enter robot_ip\n");
        txt->setStyleSheet("font: 25pt;" "color: white;" "background-color: Black;");
        txt->setAlignment(Qt::AlignCenter);
        txt->setReadOnly(true);
        layout->addWidget(txt);


        setLayout(layout);
        connect(robot_list,SIGNAL(currentIndexChanged(int)), this, SLOT(select_robot_Clicked()));
        connect(tool_list,SIGNAL(currentIndexChanged(int)), this, SLOT(select_tool_Clicked()));
        connect(object_import,SIGNAL(clicked()), this, SLOT(object_import_Clicked()));

        connect(calculate,SIGNAL(clicked()), this, SLOT(calculate_Clicked()));
        connect(simulator,SIGNAL(clicked()), this, SLOT(simulator_Clicked()));

        connect(controller_list,SIGNAL(currentIndexChanged(int)), this, SLOT(select_ctrl_Clicked()));
        connect(ping_check,SIGNAL(clicked()), this, SLOT(ping_Clicked()));
        connect(robot_enable,SIGNAL(clicked()), this, SLOT(robot_enable_Clicked()));
        connect(run,SIGNAL(clicked()), this, SLOT(run_Clicked()));


        //As a class member, nh_ is created implicitly
        ctrl_pub_ = nh_.advertise<std_msgs::String>("gui_coord_ctrl", 10);
        display_sub_ = nh_.subscribe("rf_display", 10, &gui_buttons::displayCallback, this);
        coodinator_msgs_sub_ = nh_.subscribe("coodinator_msgs_to_gui", 10, &gui_buttons::updateDisplayCallbackByCoordinator, this);
        coodinator_data_sub_ = nh_.subscribe("coodinator_data_to_gui", 10, &gui_buttons::getDataFromCoordinator, this);



    }

    void gui_buttons::sendUpdate(std::string msgdata)
    {
        if( ros::ok() && ctrl_pub_ )
        {
            std_msgs::String msg;
            msg.data = msgdata;
            ctrl_pub_.publish(msg);
        }
    }   

    void gui_buttons::send_callback_on_gui(std::string msgdata)
    {
    	txt->setText(QString::fromStdString(msgdata));
    	txt->setAlignment(Qt::AlignCenter);
    }

    void gui_buttons::select_robot_Clicked()
    {
        int robot_id = robot_list->currentIndex();
        std::string out_string;
        std::stringstream ss;
        ss << robot_id;
        out_string = robot_list->itemText(robot_id).toUtf8().constData();


        sendUpdate("selected_robot_" + out_string);
        if (out_string.size()!=0)
        {
            send_callback_on_gui("selected robot is " + out_string);    
        }
    }

    void gui_buttons::select_ctrl_Clicked()
    {
        int ctrl_id = controller_list->currentIndex();
        std::string out_string;
        std::stringstream ss;
        ss << ctrl_id;
        out_string = controller_list->itemText(ctrl_id).toUtf8().constData();
        sendUpdate("selected_ctrl_" + out_string);
        if (out_string.size()!=0)
        {
            send_callback_on_gui("selected controller is " + out_string);
            nh_.setParam("controller",out_string);    
        }
    }

    void gui_buttons::ping_Clicked()
    {

        QString ping;
        std::string robot_ping;
        ping= ping1->text() + "." + ping2->text() + "." + ping3->text() + "." + ping4->text();
        robot_ping=ping.toUtf8().constData();

        if (robot_ping.size()!=0)
        {
            send_callback_on_gui("Robot_ip is " + robot_ping);
            nh_.setParam("robot_ip",robot_ping);    
        }
    }



    void gui_buttons::select_tool_Clicked()
    {
        int tool_id = tool_list->currentIndex();
        std::string out_string;
        std::stringstream ss;
        ss << tool_id;
        out_string = tool_list->itemText(tool_id).toUtf8().constData();
        sendUpdate("selected_tool_" + out_string);
        if (out_string.size()==0)
        {
            out_string = "flange";    
        }
        send_callback_on_gui("selected tool is " + out_string);    
    }
    void gui_buttons::object_import_Clicked()
    {
        sendUpdate("object_import_sequence");
        send_callback_on_gui("Loading separate window for feature selection...");
    }

    void gui_buttons::calculate_Clicked()
    {
        sendUpdate("calculate_sequence");
        send_callback_on_gui("Calculating Inverse Kinematic...");
    }

    void gui_buttons::simulator_Clicked()
    {
        sendUpdate("Trajectory_simulation_sequence");
        send_callback_on_gui("Simulating trajectory on display window...");
    }


    void gui_buttons::robot_enable_Clicked()
    {
        sendUpdate("robot_enable_sequence");
        send_callback_on_gui("Streaming trajectory on display window...");
    }


    void gui_buttons::run_Clicked()
    {
        sendUpdate("robot_running_sequence");
        send_callback_on_gui("Streaming trajectory on display window...");
    }


    void gui_buttons::displayCallback(const std_msgs::String &msg)
    {
        txt->setText(msg.data.c_str());
        txt->setAlignment(Qt::AlignCenter);
    }

    void gui_buttons::updateDisplayCallbackByCoordinator(const std_msgs::String &msg)
    {
        txt->setText(msg.data.c_str());
        txt->setAlignment(Qt::AlignCenter);
    }

    void gui_buttons::getDataFromCoordinator(const std_msgs::String &msg_data)
    {

    }

    void gui_buttons::userPrompt(QString object, int id)
    {
        QString text = object + " change needed. Change to ID #" + QString::number(id);
        QMessageBox::information(this, "Operator Attention Required", text);
    }

    void gui_buttons::save( rviz::Config config ) const
    {
        rviz::Panel::save( config );
    }

    // Load all configuration data for this panel from the given Config object.

    void gui_buttons::load( const rviz::Config& config )
    {
        rviz::Panel::load( config );
        int auto_state;
        int skip_state;
    }

} // end namespace

// Tell pluginlib about this class.  Every class which should be
// loadable by pluginlib::ClassLoader must have these two lines
// compiled in its .cpp file, outside of any namespace scope.
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(gui_btn::gui_buttons,rviz::Panel)