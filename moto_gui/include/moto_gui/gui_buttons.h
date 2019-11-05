// Author: Yi-Wei, Chen
// Email: yiweiche@usc.edu
// ------------------------------------
#ifndef PLUGIN_CLASS
#define PLUGIN_CLASS

#include <vector>

#ifndef Q_MOC_RUN
# include <ros/ros.h>
# include <rviz/panel.h>
#endif

#include <std_msgs/String.h>

// #include <QTextEdit>

class QTextEdit;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;
class QSlider;

namespace gui_btn
{

// BEGIN_TUTORIAL
// Here we declare our new subclass of rviz::Panel.  Every panel which
// can be added via the Panels/Add_New_Panel menu is a subclass of
// rviz::Panel.
//
// InterfacePanel will show a text-entry field to set the output topic
// and a 2D control area.  The 2D control area is implemented by the
// DriveWidget class, and is described there.
class gui_buttons: public rviz::Panel
{
// This class uses Qt slots and is a subclass of QObject, so it needs
// the Q_OBJECT macro.
Q_OBJECT
public:
  // QWidget subclass constructors usually take a parent widget
  // parameter (which usually defaults to 0).  At the same time,
  // pluginlib::ClassLoader creates instances by calling the default
  // constructor (with no arguments).  Taking the parameter and giving
  // a default of 0 lets the default constructor work and also lets
  // someone using the class for something else to pass in a parent
  // widget as they normally would with Qt.
  gui_buttons( QWidget* parent = 0 );

  // Now we declare overrides of rviz::Panel functions for saving and
  // loading data from the config file.  Here the data is the
  // topic name.
  virtual void load( const rviz::Config& config );
  virtual void save( rviz::Config config ) const;

  // Next come a couple of public Qt slots.
public Q_SLOTS:
  // In this example setTopic() does not get connected to any signal
  // (it is called directly), but it is easy to define it as a public
  // slot instead of a private function in case it would be useful to
  // some other user.
  void setTopic( const QString& topic );

protected Q_SLOTS:
  void sendUpdate(std::string);
  void send_callback_on_gui(std::string);

  void select_robot_Clicked();
  void select_ctrl_Clicked();
  void ping_Clicked();
  void select_tool_Clicked();

  void object_import_Clicked();
  void calculate_Clicked();
  void simulator_Clicked();
  void robot_enable_Clicked();
  void run_Clicked();



  ///////////////////////////////////////////////////

  void displayCallback(const std_msgs::String &);
  void updateDisplayCallbackByCoordinator(const std_msgs::String &);
  void getDataFromCoordinator(const std_msgs::String &);





protected:
  void createLabelLayout(QVBoxLayout *, std::string, std::string);
  void userPrompt(QString, int);


  QTextEdit* txt;
  QLineEdit* ping1;
  QLineEdit* ping2;
  QLineEdit* ping3;
  QLineEdit* ping4;
  QPushButton* ping_check;
  QComboBox* robot_list;
  QComboBox* controller_list;
  QComboBox* tool_list;
  QPushButton* object_import;
  QPushButton* calculate;
  QPushButton* simulator;
  QPushButton* robot_enable;
  QPushButton* run;

/////////////////////////////////////////////////////////////////////


  std::map<std::string, QLabel*> id_labels_;

  QString output_topic_;

  ros::Publisher ctrl_pub_;
  ros::Subscriber display_sub_;
  ros::Subscriber coodinator_msgs_sub_;
  ros::Subscriber coodinator_data_sub_;


  ros::NodeHandle nh_;
};

} // end namespace

#endif // INTERFACE_PANEL_H



