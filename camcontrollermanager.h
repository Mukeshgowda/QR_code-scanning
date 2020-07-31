/**
 * @file camcontrollermanager.h CamController Manager class declaration
 * @author Vladimir Poliakov
 */

#ifndef CAMCONTROLLER_H
#define CAMCONTROLLER_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>

#include "publishingnode.h"
#include "qrscanner.h"
#include "ControllerAreaNetwork/ControllerAreaNetwork.h"

using std::vector;
class LocationPublisherService;

/** @addtogroup CamController
 * @{
 */

/** CAN communication defaults */
namespace CanCommunication {

/** Publisher command CAN frame */
namespace PublisherCommand {

/** CAN frame data byte order */
enum ByteOrder {
    StateByte = 0x0,
    PeriodMsb = 0x1,
    PeriodLsb = 0x2,
    TimeUnitsByte = 0x3
};

/** CAN frame publisher state byte possible values*/
enum PublisherState {
    Idle = 0x0,
    SingleTransmission = 0x1,
    Publishing = 0x2
};

/** CAN frame time units byte possible values*/
enum TimeUnits {
    Milliseconds = 0x0,
    Seconds = 0x1
};

}

/** CamController mode command CAN frame */
namespace ModeCommand{

/** CAN frame CamController mode byte order */
static constexpr uint8_t StateByte = 0x0;

/** CAN frame RGB sensor enabled state byte mask */
static constexpr uint8_t RgbEnabledMask = 0x1;

/** CAN frame Depth sensor enabled state byte mask */
static constexpr uint8_t DepthEnabledMask = 0x2;

}

}

/**
 * @brief CamController Manager
 * 
 * CamController Manager is the core class of the application
 * that controls the configuration of the CamController
 * and manages publishing services
 */
class CamControllerManager : public PublishingNode
{
public:
    /** Class constructor */
    CamControllerManager(const vector<string>& loc);
    
    /**
     * @brief Main execution loop
     * 
     * Initializes CamController, CAN communication
     * and performs main task loop, including
     * CAN frame receiving and parsing,
     * publisher services management,
     * video frame aquisition and activation
     * of scanner and obstacle detetion components.
     */
    [[ noreturn ]] void run();
    
    /** 
     * @brief Parse and handle incoming frame
     * @param frame reference to incoming frame
     */
    void parseIncomingFrame(const can_frame_types_t &frame);

    /** 
     * @brief Send outgoing frame
     * @param frame outgoing frame
     */
    inline void sendOutgoingFrame(can_frame_types_t frame) {
        can_transmit_data_frame_pd(&frame, &_canSocket);
    }

    /** 
     * @brief Adaptive state setter
     * @param b new state
     * @sa isAdaptive()
     */
    inline void setAdaptive(bool b) {
        _adaptive = b;
    }

    /** 
     * @brief Adaptive state getter
     * 
     * Adaptive state refers to automatic handling
     * of RGB and Depth Sensors' states.
     * 
     * @return is adaptive behavior allowed
     * @todo implement adaptive behavior
     * @sa setAdaptive()
     */
    inline bool isAdaptive() {
        return _adaptive;
    }

    /** 
     * @brief RGB sensor state setter
     * @param b new state
     * @sa isRgbSensorEnabled()
     */
    inline void setRgbSensorEnabled(bool b) {
        _rgbSensorEnabled = b;
    }

    /** 
     * @brief RGB sensor state getter
     * 
     * RGB sensor state is used for 
     * both QR scanning and obstacle detection.
     * When disabled, the manager deactivates
     * corersponding kernel module.
     * 
     * @note QR code scanning is only possible
     * when RGB sensor is enabled.
     * 
     * @return is RGB sensor enabled
     * @todo Implement RGB sensor activation/deactivation
     * @sa setRgbSensorEnabled()
     */
    inline bool isRgbSensorEnabled() {
        return _rgbSensorEnabled;
    }

    /** 
     * @brief Depth sensor state setter
     * @param b new state
     * @sa isDepthSensorEnabled()
     */
    inline void setDepthSensorEnabled(bool b) {
        _depthSensorEnabled = b;
    }

    /** 
     * @brief Depth sensor state getter
     * 
     * When disabled, the manager deactivates
     * corersponding kernel module and sends
     * a notification to the operator.
     * 
     * @return is depth sensor enabled
     * @todo Implement Depth sensor activation/deactivation
     * @sa setDepthSensorEnabled()
     */
    inline bool isDepthSensorEnabled() {
        return _depthSensorEnabled;
    }

    /** 
     * @brief QR Scanner getter
     * @return reference to the QR Scanner
     */
    QRScanner & qrScanner() {
        return _scanner;
    }

private:
    QRScanner _scanner;
    can_socket_t _canSocket;
    bool _adaptive;
    bool _rgbSensorEnabled;
    bool _depthSensorEnabled;
    cv::VideoCapture _capture;
    std::vector<string> location;
};

/** @} */

#endif // CAMCONTROLLER_H