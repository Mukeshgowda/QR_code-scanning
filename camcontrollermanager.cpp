
/**
 * @file camcontrollermanager.h CamController Manager class definition
 * @author Vladimir Poliakov
 */

#include "camcontrollermanager.h"
#include "locationpublisherservice.h"

using std::vector;
using std::make_unique;
using std::unique_ptr;
using std::thread;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::string;
using cv::Mat;
using std::cout;

using namespace CanCommunication;

CamControllerManager::CamControllerManager(const vector<string>& loc) :
    _scanner(),
    _capture(0),
    location(loc)
{
    addPublisher(make_unique<LocationPublisherService>(location[0], CAM_CTRL_ID_HOME_LOCATION, this));
    addPublisher(make_unique<LocationPublisherService>(location[1], CAM_CTRL_ID_LOCATION_1, this));
    addPublisher(make_unique<LocationPublisherService>(location[2], CAM_CTRL_ID_LOCATION_2, this));
    setPublisherState(CAM_CTRL_ID_LOCATION_1, PublisherService::Publishing, 200);
    setPublisherState(CAM_CTRL_ID_LOCATION_2, PublisherService::Publishing, 200);
}

void CamControllerManager::run()
{
    can_bind_socket_pd(&_canSocket);
    std::cout<<"Creating socket thread\n";
    thread recvTh([&]() {
        auto frame = can_receive_data_frame_pd(&_canSocket);
        parseIncomingFrame(frame);
    });

    while(true) {
        if(isRgbSensorEnabled()) {
            //Capture a new frame
            Mat frame;
            _capture.cv::VideoCapture::open(0);
            //Check for camera availability
            if(!_capture.isOpened())
            {
               std::cout<<"ERROR OPENING CAMERA\n";
            }
            _capture >> frame;
            //Forward the frame for scan
            _scanner.scan(frame, location);
        }
        sleep_for(milliseconds(100));
    }
}

void CamControllerManager::parseIncomingFrame(const can_frame_types_t &frame)
{
    // Parsing incoming command frames
    if (frame.c_s_bit_id == COMMAND_DATA_BIT) {
        switch (frame.topic_id) {
        // Set CamController mode
        case CAM_CTRL_ID_MODE :
            if (frame.data[ModeCommand::StateByte]) {
                setAdaptive(false);
                setRgbSensorEnabled(frame.data[ModeCommand::StateByte]
                        | ModeCommand::RgbEnabledMask);
                //setDepthSensorEnabled(frame.data[ModeCommand::StateByte]
                  //      | ModeCommand::DepthEnabledMask);
            } else {
                setAdaptive(true);
            }
            break;

        // For any location topic read the command parameters
        // and start correspoinding service
        case CAM_CTRL_ID_HOME_LOCATION :
        case CAM_CTRL_ID_LOCATION_1 : {
                PublisherService::State state;
            	unsigned int period = 0;
            // Read the state and period
            	switch (frame.data[PublisherCommand::StateByte]) {
            	case PublisherCommand::SingleTransmission:
                	state = PublisherService::SingleTransmission;
                	break;
            	case PublisherCommand::Publishing:
                	state = PublisherService::Publishing;
                	period= static_cast<unsigned int>(frame.data[PublisherCommand::PeriodMsb] << sizeof(uint8_t)
                        	               | frame.data[PublisherCommand::PeriodLsb]);
	                if (frame.data[PublisherCommand::TimeUnitsByte] == PublisherCommand::Seconds) period *= 1000;
  	                break;
                default:
                	state = PublisherService::Idle;
          	        break;
            }

            // Change publisher service's state
            setPublisherState(frame.topic_id, state, period);
            break;
        }
        default:
            break;
        }
    }
}