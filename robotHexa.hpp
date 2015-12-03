//| This file is a part of the ERC ResiBots project.
//| Copyright 2015, ISIR / Universite Pierre et Marie Curie (UPMC)
//| Main contributor(s): Jean-Baptiste Mouret, mouret@isir.fr
//|                      Antoine Cully, cully@isir.upmc.fr
//|
//| This software is governed by the CeCILL license under French law
//| and abiding by the rules of distribution of free software.  You
//| can use, modify and/ or redistribute the software under the terms
//| of the CeCILL license as circulated by CEA, CNRS and INRIA at the
//| following URL "http://www.cecill.info".
//|
//| As a counterpart to the access to the source code and rights to
//| copy, modify and redistribute granted by the license, users are
//| provided only with a limited warranty and the software's author,
//| the holder of the economic rights, and the successive licensors
//| have only limited liability.
//|
//| In this respect, the user's attention is drawn to the risks
//| associated with loading, using, modifying and/or developing or
//| reproducing the software by the user in light of its specific
//| status of free software, that may mean that it is complicated to
//| manipulate, and that also therefore means that it is reserved for
//| developers and experienced professionals having in-depth computer
//| knowledge. Users are therefore encouraged to load and test the
//| software's suitability as regards their requirements in conditions
//| enabling the security of their systems and/or data to be ensured
//| and, more generally, to use and operate it in the same conditions
//| as regards security.
//|
//| The fact that you are presently reading this means that you have
//| had knowledge of the CeCILL license and that you accept its terms.

#ifndef ROBOTHEXA_HPP
#define ROBOTHEXA_HPP

#include <stdlib.h>
#include <math.h>
#include <dynamixel/dynamixel.hpp>
#include "controllerDuty.hpp"

#include <imu_razor/imu_razor.hpp>


#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <rgbdslam/XtionDisplacement.h>
#include <rgbdslam/SferesCmd.h>


#define DEFAULT_SPEED 150
#define READ_DURATION 0.005f
#define SAMPLING_FREQUENCY 20

class RobotHexa
{
public:
    typedef unsigned char byte_t;

    RobotHexa()
    {
        init();
    }
    void init();

    void relax()
    {
        std::cout << "relax..." << std::endl;
        for (size_t i = 0; i < _actuators_ids.size(); ++i)
        {
            _controller.send(dynamixel::ax12::TorqueEnable(_actuators_ids[i], false));
            _controller.recv(READ_DURATION, _status);
        }
        for (size_t i = 0; i < _wheels_ids.size(); ++i)
        {
            _controller.send(dynamixel::ax12::TorqueEnable(_wheels_ids[i], false));
            _controller.recv(READ_DURATION, _status);
        }

        std::cout << "done" << std::endl;
    }
    void enable()
    {
        for (size_t i = 0; i < _actuators_ids.size(); ++i)
        {
            _controller.send(dynamixel::ax12::TorqueEnable(_actuators_ids[i], true));
            _controller.recv(READ_DURATION, _status);
        }
        usleep(1e5);
    }
    void reset();
    void transfer(ControllerDuty& controller, float duration,int transfer_number);
    size_t nb_actuators() const
    {
        return _actuators_ids.size();
    }
    size_t nb_wheels() const
    {
        return _wheels_ids.size();
    }
    void close_usb_controllers()
    {

            _imu.close_serial();

            _controller.close_serial();
    }


    const std::vector<float>& get_contact(int i)
    {
        switch (i)
        {
        case 0:
            return _behavior_contact_0;
            break;
        case 1:
            return _behavior_contact_1;
            break;
        case 2:
            return _behavior_contact_2;
            break;
        case 3:
            return _behavior_contact_3;
            break;
        case 4:
            return _behavior_contact_4;
            break;
        case 5:
            return _behavior_contact_5;
            break;

        }
        assert(false);
        return _behavior_contact_0;



    }
    void  initRosNode(  int argc ,char** argv);
    std::vector<std::vector<float> >& get_angles()
    {


        return _imu_angles;

    }
    void send_ros_start(int nbrun,int nbtrans);
    void send_ros_stop(int nbrun,int nbtrans);
    void posCallback(const rgbdslam::XtionDisplacement& msg);
    float covered_distance()
    {
        return _covered_distance;
    }
    float slam_duration()
    {
      return _slam_duration;
    }
  std::vector<float> final_pos(){return _final_pos;}
  float final_angle(){return _final_angle;}

protected:




    void write_contact(std::string const name);
    void write_angle(std::string const name);
    void contactSmoothing(int length);
    void _read_contacts();
    dynamixel::Usb2Dynamixel _controller;
    float _covered_distance;
  float _slam_duration;
  imu::Usb2Imu _imu;

  std::vector<float> _final_pos;
  float _final_angle;
    std::vector<std::vector<float> > _imu_angles;

    dynamixel::Status _status;
    std::vector<byte_t> _wheels_ids;
    std::vector<float> _behavior_contact_0;
    std::vector<float> _behavior_contact_1;
    std::vector<float> _behavior_contact_2;
    std::vector<float> _behavior_contact_3;
    std::vector<float> _behavior_contact_4;
    std::vector<float> _behavior_contact_5;
    //std::vector<float> _behavior_smooth_contact;
    std::vector<byte_t> _actuators_ids;
    boost::shared_ptr<ros::NodeHandle> _node_p;
    ros::Publisher  _chatter_pub;
    ros::Subscriber _sub;
};

#endif
