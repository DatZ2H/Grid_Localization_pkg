#include <Grid_Localization_pkg/Grid_Loc_library.h>

#include <Grid_Localization_pkg/grid_loc.h>

 clientSock *Flexisoft = new clientSock("172.20.3.53", 2112);
ros::Publisher LocalizationGridResultMessage_pub;
Grid_Localization_pkg::grid_loc grid_loc;

void LocalizationGridResultMessage_Function_pub(){
    grid_loc.x = Flexisoft->Qrcode.pose.XMCL;
    grid_loc.y = Flexisoft->Qrcode.pose.YMCL;
    grid_loc.heading = Flexisoft->Qrcode.pose.ANS;
    LocalizationGridResultMessage_pub.publish(grid_loc);
}
int main(int argc, char **argv)
{

    ROS_INFO("FLEXISOFT CONNECTING.");
    ros::init(argc, argv, "safety_function");
    ros::NodeHandle nh;

    LocalizationGridResultMessage_pub = nh.advertise<Grid_Localization_pkg::grid_loc>("/LocalizationGridResultMessage_pub", 10);

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
      LocalizationGridResultMessage_Function_pub();



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
