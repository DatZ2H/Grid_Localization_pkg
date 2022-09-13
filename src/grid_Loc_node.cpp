#include <Grid_Localization_pkg/Grid_Loc_library.h>

#include <Grid_Localization_pkg/grid_loc.h>

 clientSock *GridLOC = new clientSock("172.20.3.53", 2112);
ros::Publisher LocalizationGridResultMessage_pub;
Grid_Localization_pkg::grid_loc grid_loc;

void LocalizationGridResultMessage_Function_pub(){

    grid_loc.header.stamp  = ros::Time::now();

    grid_loc.x = GridLOC->LocResult.XMCL;
    grid_loc.y = GridLOC->LocResult.YMCL;
    grid_loc.heading = GridLOC->LocResult.ANS;

    grid_loc.resolution = PARA_RESOLUTION_LENGTH; 
    grid_loc.XMCL = GridLOC->Qrcode.pose.XMCL;
    grid_loc.YMCL = GridLOC->Qrcode.pose.YMCL;

    LocalizationGridResultMessage_pub.publish(grid_loc);
}
int main(int argc, char **argv)
{

    ROS_INFO("Grid_loc CONNECTING.");
    ros::init(argc, argv, "Grid_loc");
    ros::NodeHandle nh;

    LocalizationGridResultMessage_pub = nh.advertise<Grid_Localization_pkg::grid_loc>("/LocalizationGridResultMessage_pub", 10);

    ros::Rate loop_rate(30);

    

    while (ros::ok())
    {
      
        if (!GridLOC->connected)
        {
            ROS_INFO("grid_loc CONNECTING.....");
            GridLOC->connect();
            
        }
        while (ros::ok() && GridLOC->connected)
        {
            /* code */

      GridLOC->tcp_read();
      LocalizationGridResultMessage_Function_pub();



            ros::spinOnce();
            loop_rate.sleep();
        }
        loop_rate.sleep();
        ros::spinOnce();
    }
    GridLOC->disconnect();
    ros::spin();
    delete (GridLOC);

    return 0;
}
