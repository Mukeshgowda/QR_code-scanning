#include "camcontrollermanager.h"
#include "locationpublisherservice.h"
#include "ControllerAreaNetwork/ControllerAreaNetwork.h"
#include "iostream"
using std::string;
using std::cout;

LocationPublisherService::LocationPublisherService(string locationName, int id, CamControllerManager *manager) :
    PublisherService(id), _manager{manager}, _locationName{locationName}
{
}

void LocationPublisherService::publish()
{
    auto loc = _manager->qrScanner().find(_locationName);
    // build the frame
    can_frame_types_t frame;
    frame.priority_id =PRIORITY_MEDIUM;
    frame.c_s_bit_id =SENSOR_DATA_BIT;
    frame.topic_id = static_cast<topic_id_can>(id());
    frame.dlc = 4;
    frame.data[0] = uint8_t((loc.first >> 8) & 0xFF);
    frame.data[1] = uint8_t((loc.first) & 0xFF);
    frame.data[2] = uint8_t((loc.second >> 8) & 0xFF);
    frame.data[3] = uint8_t((loc.second) & 0xFF);

    _manager->sendOutgoingFrame(frame);

    std::cout << "location " << _locationName << " at " << loc.first << " " << loc.second << std::endl;
}