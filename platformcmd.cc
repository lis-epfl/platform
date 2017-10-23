/*
 * Copyright (c) 2017 Arthur Gay
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo_client.hh>
#include <iostream>
#include <unistd.h>
#include "platform.pb.h"
#include "platformcmd.hh"

#ifdef __linux__
#define COLOR_RESET "\033[0m"
#define COLOR_ERROR "\033[31m"
#define COLOR_SUCCESS "\033[32m"
#else 
#define COLOR_RESET ""
#define COLOR_ERROR ""
#define COLOR_SUCCESS ""
#endif

#ifndef MAX_HEIGHT
#define MAX_HEIGHT 	1.3
#endif
#ifndef MIN_HEIGHT
#define MIN_HEIGHT 	0.8
#endif
#ifndef CENTER_X
#define CENTER_X	100.0
#endif
#ifndef LENGTH_X
#define LENGTH_X	3.0
#endif
#ifndef CENTER_Y
#define CENTER_Y	0.0
#endif
#ifndef LENGTH_Y
#define LENGTH_Y	3.0
#endif

#define MIN_X		((CENTER_X)-(LENGTH_X)/2)
#define MAX_X		((CENTER_X)+(LENGTH_X)/2)
#define MIN_Y		((CENTER_Y)-(LENGTH_Y)/2)
#define MAX_Y		((CENTER_Y)+(LENGTH_Y)/2)
#define MIN_H		(MIN_HEIGHT)
#define MAX_H 		(MAX_HEIGHT)

typedef const boost::shared_ptr<const platform_msgs::msgs::platform>
    PlatformPtr;

int main(int _argc, char **_argv)
{
  platform_msgs::msgs::platform msg;
  msg.set_time_msec(0);
  
  float x = 0, y = 0, h = 1;
  char c;
  opterr = 0;
  
  while ((c = getopt (_argc, _argv, "x:y:H:advh")) != -1) {
    switch (c){
	case 'h':
std::cout <<
"Usage: platform [OPTION]..." 
<< std::endl <<
"Interact with the platform plugin of a running gazebo server with the"
<< std::endl <<
"platform plugin to set the coordinates and the height of the platform."
<< std::endl << std::endl <<
"  -x X                       set the x coordinate to X"
<< std::endl <<
"  -y Y                       set the y coordinate to Y"
<< std::endl <<
"  -H h                       set the height to h, if h=0 then height is"
<< std::endl <<
"                              set at random"
<< std::endl <<
"  -a                         random position (x, y)"
<< std::endl <<
"  -d                         used for debuging: dump all and exit"
<< std::endl <<
"  -h                         display this help and exit"
<< std::endl <<
"  -v                         output version information and exit"
<< std::endl << std::endl <<
"Examples:"
<< std::endl <<
"Choose coordinates and height"
<< std::endl << 
"    platform -x 100 -y 0 -h 1"
<< std::endl << 
"Change position, keep height"
<< std::endl << 
"    platform -x 100 -y 0"
<< std::endl << 
"Change to random height, keep position"
<< std::endl << 
"    platform -h 0"
<< std::endl << 
"Random position but keep height (order of options is significant)"
<< std::endl << 
"    platform -a"
<< std::endl << 
"Random height and position"
<< std::endl << 
"    platform -a -h 0"
<< std::endl << std::endl <<
"Position ranges for -a switch and height range for -h 0 option:"
<< std::endl
<< "MIN_X=" << MIN_X << "\t MAX_X=" << MAX_X << std::endl
<< "MIN_Y=" << MIN_Y << "\t MAX_Y=" << MAX_Y << std::endl
<< "MIN_H=" << MIN_H << "\t MAX_H=" << MAX_H << std::endl
<< std::endl << std::endl
;

	case 'v':
		std::cout << PROGRAM_NAME << " "
			<< PROGRAM_VERSION
			<< std::endl
			<< "Source: " << PROGRAM_SOURCE
			<< std::endl;
		exit(0);
		break;
	case 'a':
		srand (static_cast <unsigned> (time(0)));
        x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(MAX_X-MIN_X))) + MIN_X;
		msg.set_x(x);
        y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(MAX_Y-MIN_Y))) + MIN_Y;
		msg.set_y(y);
		h = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(MAX_H-MIN_H))) + MIN_H;
		msg.set_h(h);
		break;
	case 'd':
		std::cout << "x=" << x << ", y=" << y << ", h=" << h << std::endl;
		std::cout << "MIN_X=" << MIN_X << "\t MAX_X=" << MAX_X << std::endl;
		std::cout << "MIN_Y=" << MIN_Y << "\t MAX_Y=" << MAX_Y << std::endl;
		std::cout << "MIN_H=" << MIN_H << "\t MAX_H=" << MAX_H << std::endl;
		exit(0);
		break;
    case 'x':
      x = std::stof(optarg);
      msg.set_x(x);
      break;
    case 'y':
      y = std::stof(optarg);
      msg.set_y(y);
      break;
    case 'H':
      h = std::stof(optarg);
      if(h == 0){
        srand (static_cast <unsigned> (time(0)));
		h = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(MAX_H-MIN_H))) + MIN_H;
		msg.set_h(h);
        std::cout<<"Height has been randomly set to: "<<h<<"\n";
      }
      msg.set_h(h);
      break;
    case '?':
      if (optopt == 'H' || optopt == 'x' || optopt == 'y')
        fprintf (stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint (optopt))
        fprintf (stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf (stderr,
                 "Unknown option character `\\x%x'.\n",
                 optopt);
      return 1;
    default:
      abort();
    }
  }
  
  // Load gazebo
  std::cout<<"Loading gazebo...\n";
  gazebo::client::setup(_argc, _argv);

  // Create our node for communication
  std::cout<<"Initializing node...\n";
  gazebo::transport::NodePtr node(new gazebo::transport::Node());
  node->Init();

  // Advertise to Gazebo platform topic	
  std::cout<<"Advertising topic...\n";
  gazebo::transport::PublisherPtr pub = node->Advertise<platform_msgs::msgs::platform>("~/platform");
  
  std::cout << "Connecting to running gazebo...\n";
  if(!pub->WaitForConnection(gazebo::common::Time(1))){
	std::cout 
		<< COLOR_ERROR 
		<< "Error: "
		<< COLOR_RESET
		<< "gazebo is not running!\n" ;
	return 1;
  }

  // Send
  std::cout<<"Publishing message: ";
  pub->Publish(msg);
  
  // Make sure to shut everything down.
  gazebo::client::shutdown();
  
  std::cout 
		<< COLOR_SUCCESS
		<< "\bOK!\n"
		<< COLOR_RESET;
  return 0;
}
