#include "camcontrollermanager.h"
#include "stdlib.h"

using namespace std::chrono_literals;
using  std::cout;

int main()
{
    std::vector<string> location{ "Home", "FHDortmund", "TUDortmund"};
    system("sudo modprobe bcm2835-v4l2");
    CamControllerManager m(location);
   //Enable Camera
    can_frame_types_t camframe;
    camframe.c_s_bit_id = COMMAND_DATA_BIT;
    camframe.topic_id = CAM_CTRL_ID_MODE;
    camframe.data[0] = 1;
    m.parseIncomingFrame(camframe);
   //Set Home location publishing state and rate
    can_frame_types_t frame;
    frame.c_s_bit_id = COMMAND_DATA_BIT;
    frame.topic_id = CAM_CTRL_ID_HOME_LOCATION;

//    while (true) {
        frame.data[0] = 2;
        frame.data[1] = 0;
        frame.data[2] = 200;
        m.parseIncomingFrame(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));

//        frame.data[0] = 1;
//        m.parseIncomingFrame(frame);

//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    }
        m.run();
}