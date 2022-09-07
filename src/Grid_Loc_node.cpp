#include <Grid_Localization_pkg/Grid_Loc_library.h>

 clientSock *Flexisoft = new clientSock("172.20.3.53", 2112);




int main(int argc, char **argv)
{

    ROS_INFO("FLEXISOFT CONNECTING.");
    ros::init(argc, argv, "safety_function");
    ros::NodeHandle nh;

    ros::Rate loop_rate(30);

    

    while (ros::ok())
    {
      
        if (!Flexisoft->connected)
        {
            ROS_INFO("FLEXISOFT CONNECTING.....");
            Flexisoft->connect();
            
        }
        while (ros::ok() && Flexisoft->connected)
        {
            /* code */

      Flexisoft->tcp_read();



            ros::spinOnce();
            loop_rate.sleep();
        }
        loop_rate.sleep();
        ros::spinOnce();
    }
    Flexisoft->disconnect();
    ros::spin();
    delete (Flexisoft);

    return 0;
}
