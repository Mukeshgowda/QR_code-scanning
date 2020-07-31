#ifndef LOCATIONPUBLISHERSERVICE_H
#define LOCATIONPUBLISHERSERVICE_H

#include "publisherservice.h"

class CamControllerManager;

/** @addtogroup CamController
 * @{
 */

/**
 * @brief Location publisher service
 *
 * Location publisher service perfroms automatic
 * publishing activity of the current position
 * of the location it is assigned to.
 * Each time publish() is called,
 * service gathers information about the
 * location and sends it via CAN.
 */
class LocationPublisherService : public PublisherService
{
public:
    /**
     * @brief Class constructor 
     * @param locationName name of the location
     * @param id topic identity number
     * @param manager pointer to the manager
     */
    LocationPublisherService(std::string locationName, int id, CamControllerManager *manager);
    
    /** @brief Class destructor */
    virtual ~LocationPublisherService() = default;
    
    /**
     * @brief Publishing function
     * 
     * Gather infromation about the current position
     * of the location and transmit it via CAN
     */
    virtual void publish();

private:
    CamControllerManager *_manager;
    std::string _locationName;
};

/** @} */
#endif // LOCATIONPUBLISHERSERVICE_H