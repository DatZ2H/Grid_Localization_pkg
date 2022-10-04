#include <grid_localization_pkg/grid_Loc_library.h>

#include <grid_localization_pkg/grid_loc.h>
#include <grid_localization_pkg/LocalizationControllerResultMessage0502.h>
#include <grid_localization_pkg/gridSetReadLocSrv.h>

#include <agv_define/action_status.h>
#include <agv_define/agvlib.h>

clientSock* gridLOC = new clientSock("172.20.235.62", 2112);
ros::Publisher LocalizationgridResultMessage_pub;
ros::Publisher gridLocalizationgridResul_pub;
grid_localization_pkg::grid_loc grid_loc;
grid_localization_pkg::LocalizationControllerResultMessage0502 LocalizationControllerResultMessage0502;
agv_define::action_status action_status;

void LocalizationgridResultMessage_Function_pub() {

    grid_loc.header.stamp = ros::Time::now();
    grid_loc.timestamp = gridLOC->Qrcode.FAMS;

    grid_loc.x = gridLOC->LlsResult.x;
    grid_loc.y = gridLOC->LlsResult.y;
    grid_loc.heading = gridLOC->LlsResult.heading;

    gridLOC->LlsResult.loc_status = gridLOC->LlsResult.loc_status;
    // gridLOC->LlsResult.map_match_status = msg->map_match_status;

    gridLOC->LlsResult.sync_timestamp_sec = gridLOC->LlsResult.sync_timestamp_sec;
    gridLOC->LlsResult.sync_timestamp_nsec = gridLOC->LlsResult.sync_timestamp_nsec;
    gridLOC->LlsResult.sync_timestamp_valid = gridLOC->LlsResult.sync_timestamp_valid;

    grid_loc.resolution = PARA_RESOLUTION_LENGTH;
    grid_loc.rowcode = gridLOC->Qrcode.code.rowcode;
    grid_loc.colcode = gridLOC->Qrcode.code.colcode;
    grid_loc.Yaw = gridLOC->LocResult.ANS;
    grid_loc.XMCL = gridLOC->Qrcode.pose.XMCL;
    grid_loc.YMCL = gridLOC->Qrcode.pose.YMCL;

    // grid_loc.x = gridLOC->LocResult.XMCL;
    // grid_loc.y = gridLOC->LocResult.YMCL;
    // grid_loc.heading = gridLOC->LocResult.ANS;

    // grid_loc.resolution = PARA_RESOLUTION_LENGTH;
    // grid_loc.XMCL = gridLOC->Qrcode.pose.XMCL;
    // grid_loc.YMCL = gridLOC->Qrcode.pose.YMCL;



    LocalizationgridResultMessage_pub.publish(grid_loc);
}
void gridLocalizationgridResul_Function_pub() {

    grid_loc.header.stamp = ros::Time::now();
    grid_loc.timestamp = gridLOC->Qrcode.FAMS;

    grid_loc.x = gridLOC->LlsResult.x;
    grid_loc.y = gridLOC->LlsResult.y;
    grid_loc.heading = gridLOC->LlsResult.heading;

    gridLOC->LlsResult.loc_status = gridLOC->LlsResult.loc_status;
    // gridLOC->LlsResult.map_match_status = msg->map_match_status;

    gridLOC->LlsResult.sync_timestamp_sec = gridLOC->LlsResult.sync_timestamp_sec;
    gridLOC->LlsResult.sync_timestamp_nsec = gridLOC->LlsResult.sync_timestamp_nsec;
    gridLOC->LlsResult.sync_timestamp_valid = gridLOC->LlsResult.sync_timestamp_valid;

    grid_loc.resolution = PARA_RESOLUTION_LENGTH;
    grid_loc.rowcode = gridLOC->Qrcode.code.rowcode;
    grid_loc.colcode = gridLOC->Qrcode.code.colcode;
    grid_loc.Yaw = gridLOC->LocResult.ANS;
    grid_loc.XMCL = gridLOC->Qrcode.pose.XMCL;
    grid_loc.YMCL = gridLOC->Qrcode.pose.YMCL;

    // grid_loc.x = gridLOC->LocResult.XMCL;
    // grid_loc.y = gridLOC->LocResult.YMCL;
    // grid_loc.heading = gridLOC->LocResult.ANS;

    // grid_loc.resolution = PARA_RESOLUTION_LENGTH;
    // grid_loc.XMCL = gridLOC->Qrcode.pose.XMCL;
    // grid_loc.YMCL = gridLOC->Qrcode.pose.YMCL;



    gridLocalizationgridResul_pub.publish(grid_loc);
}
bool ServiceCbFlexSetZoneSrv(grid_localization_pkg::gridSetReadLocSrv::Request& req,
    grid_localization_pkg::gridSetReadLocSrv::Response& res)
{

    gridLOC->call_state = req.CALL;

    if (gridLOC->tcp_read() == STATUS_GRID_RECV_DATA) {
        LocalizationgridResultMessage_Function_pub();
        ROS_INFO("LocalizationgridResultMessage_Function_pub....");
    }
    res.success = true;
}
void localizationcontroller_result_message_0502_CallBack(const grid_localization_pkg::LocalizationControllerResultMessage0502::ConstPtr& msg)
{
    LocalizationControllerResultMessage0502 = *msg;
    gridLOC->LlsResult.x = msg->x;
    gridLOC->LlsResult.y = msg->y;
    gridLOC->LlsResult.heading = msg->heading;

    gridLOC->LlsResult.loc_status = msg->loc_status;
    // gridLOC->LlsResult.map_match_status = msg->map_match_status;

    gridLOC->LlsResult.sync_timestamp_sec = msg->sync_timestamp_sec;
    gridLOC->LlsResult.sync_timestamp_nsec = msg->sync_timestamp_nsec;
    gridLOC->LlsResult.sync_timestamp_valid = msg->sync_timestamp_valid;


}
void control_charger_status_CallBack(const agv_define::action_status::ConstPtr& msg)
{
    action_status = *msg;
    if (msg->action_status.status == ActionStatus::SUCCEEDED) {
        LocalizationgridResultMessage_Function_pub();
        ROS_INFO("control_charger_status_CallBack....");
    }

}
int main(int argc, char** argv)
{

    ROS_INFO("grid_loc CONNECTING.");
    ros::init(argc, argv, "grid_loc");
    ros::NodeHandle nh;

    ros::ServiceServer Srv_gridSetReadLocSrv = nh.advertiseService("gridSetReadLocSrv", ServiceCbFlexSetZoneSrv);

    LocalizationgridResultMessage_pub = nh.advertise<grid_localization_pkg::grid_loc>("/LocalizationgridResultMessage_pub", 10);
    gridLocalizationgridResul_pub = nh.advertise<grid_localization_pkg::grid_loc>("/gridLocalizationgridResul_pub", 10);

    ros::Subscriber localizationcontroller_result_message_0502_sub = nh.subscribe("/localizationcontroller/out/localizationcontroller_result_message_0502", 10, localizationcontroller_result_message_0502_CallBack);
    ros::Subscriber control_charger_status_sub = nh.subscribe("control_charger_status", 10, control_charger_status_CallBack);

    ros::Rate loop_rate(100);



    while (ros::ok())
    {

        if (!gridLOC->connected)
        {
            ROS_INFO("grid_loc CONNECTING.....");
            gridLOC->connect();

        }
        while (ros::ok() && gridLOC->connected)
        {
            // int cout = 0;
            // int check = 0;

            // if (gridLOC->tcp_read() == STATUS_GRID_RECV_DATA) {
            //     gridLocalizationgridResul_Function_pub();
            // }
            // gridLOC->LlsResult.x = 1111;
            // gridLOC->LlsResult.y = 2222;
            // gridLOC->LlsResult.heading = 33333;



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
