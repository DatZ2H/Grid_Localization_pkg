#include <grid_localization_pkg/grid_Loc_library.h>

#include <grid_localization_pkg/grid_loc.h>

 clientSock *gridLOC = new clientSock("172.20.235.62", 2112);
ros::Publisher LocalizationgridResultMessage_pub;
grid_localization_pkg::grid_loc grid_loc;

void LocalizationgridResultMessage_Function_pub(){

    grid_loc.header.stamp  = ros::Time::now();

    grid_loc.x = gridLOC->LocResult.XMCL;
    grid_loc.y = gridLOC->LocResult.YMCL;
    grid_loc.heading = gridLOC->LocResult.ANS;

    grid_loc.resolution = PARA_RESOLUTION_LENGTH; 
    grid_loc.XMCL = gridLOC->Qrcode.pose.XMCL;
    grid_loc.YMCL = gridLOC->Qrcode.pose.YMCL;

    LocalizationgridResultMessage_pub.publish(grid_loc);
}
int main(int argc, char **argv)
{

    ROS_INFO("grid_loc CONNECTING.");
    ros::init(argc, argv, "grid_loc");
    ros::NodeHandle nh;

    LocalizationgridResultMessage_pub = nh.advertise<grid_localization_pkg::grid_loc>("/LocalizationgridResultMessage_pub", 10);

    ros::Rate loop_rate(30);

    

    while (ros::ok())
    {
      
        if (!gridLOC->connected)
        {
            ROS_INFO("grid_loc CONNECTING.....");
            gridLOC->connect();
            
        }
        while (ros::ok() && gridLOC->connected)
        {
            /* code */


      if (gridLOC->tcp_read() == STATUS_GRID_RECV_DATA){

      
      LocalizationgridResultMessage_Function_pub();
      }



            ros::spinOnce();
            loop_rate.sleep();
        }
        loop_rate.sleep();
        ros::spinOnce();
    }
    gridLOC->disconnect();
    ros::spin();
    delete (gridLOC);

    return 0;
}
