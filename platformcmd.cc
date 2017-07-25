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

#ifdef __linux__
#define COLOR_RESET "\033[0m"
#define COLOR_ERROR "\033[31m"
#define COLOR_SUCCESS "\033[32m"
#else 
#define COLOR_RESET ""
#define COLOR_ERROR ""
#define COLOR_SUCCESS ""
#endif

typedef const boost::shared_ptr<const platform_msgs::msgs::platform>
    PlatformPtr;

int main(int _argc, char **_argv)
{
  platform_msgs::msgs::platform msg;
  msg.set_time_msec(0);
  
  float x, y, h;
  char c;
  opterr = 0;
  
  while ((c = getopt (_argc, _argv, "x:y:h:")) != -1) {
    switch (c){
    case 'x':
      x = std::stof(optarg);
      msg.set_x(x);
      break;
    case 'y':
      y = std::stof(optarg);
      msg.set_y(y);
      break;
    case 'h':
      h = std::stof(optarg);
      if(h == 0){
        srand (static_cast <unsigned> (time(0)));
        std::cout<<"Input height is 0: height will be randomly selected.\n";
        h = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/4))+0.5;
        std::cout<<"Height has been randomly set to: "<<h<<"\n";
      }
      msg.set_h(h);
      break;
    case '?':
      if (optopt == 'h' || optopt == 'x' || optopt == 'y')
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
