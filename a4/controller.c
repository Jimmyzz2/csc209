#include <stdio.h>
#include "controller.h"
/* Checks if the message has a valid type. The gateway should only receive
 * HANDSHAKE or UPDATE messages from sensors
 */
int is_valid_type(struct cignal *cig) {
    if ((cig->hdr.type == HANDSHAKE) || (cig->hdr.type == UPDATE)) {
        return 1;
    }
    return -1;
}

/* Returns 1 if the gateway seen this device before?
 */
int is_registered(int id, int *device_record) {
    if (device_record[id - LOWEST_ID] == 1) {
        return 1;
    }
    return -1;
}

/* Add a new device to the device_record.  Return the new device id.
 * Note that device ids will never be "de-registered" so they cannot be reused.
 */
int register_device(int *device_record) {
    for (int i = 0; i < MAXDEV; i++) {
        if (device_record[i] == 0) {
            device_record[i] = 1;
            return i + LOWEST_ID;
        }
    }
    return -1;
}

/* Turns on or off the cooler or dehumidifier based on the
 * current temperature or humidity.j
 */
void adjust_fan(struct cignal *cig) {
    cig->hdr.type = 3;
    if (cig->hdr.device_type == TEMPERATURE) {
        if (cig->value >= TEMPSET) {
            cig->cooler = ON;
        } else {
            cig->cooler = OFF;
        }
    } else if (cig->hdr.device_type == HUMIDITY) {
        if (cig->value >= HUMSET) {
            cig->dehumid = ON;
        } else {
            cig->dehumid = OFF;
        }
    }
}


/* Check each field of the incoming header to ensure that it is valid, print
 * information process about the state of the sensor using the printf messages
 * below, and adjust the fan.
 * 
 * Error messages must be printed to stderr, but the contents of the messages
 * are unspecified so you can choose good messages.
 * 
 * Print the following before returning from handling a valid event.
 * printf("********************END EVENT********************\n\n");
 * 
 * Print the following after getting a value from the relevant sensor.
 * printf("Temperature: %.4f --> Device_ID: %d\n", YOUR VARIABLES HERE);
 * printf("Humidity: %.4f --> Device_ID: %d\n", YOUR VARIABLES HERE);
 */

int process_message(struct cignal *cig, int *device_record) {
    // check header
    if ((cig->hdr.device_id > 99 || cig->hdr.device_id < 11) && cig->hdr.device_id != -1) {
        fprintf(stderr, "Received wrong device_id %d! The message is discarded...", cig->hdr.device_id);
        return -1;
    }
    if ((!is_valid_type(cig))) {
        fprintf(stderr, "Received wrong type %d! The message is discarded...", cig->hdr.device_type);
        return -1;
    }
    if ((cig->hdr.type == UPDATE && cig->hdr.device_type != TEMPERATURE && cig->hdr.device_type != HUMIDITY)) {
        fprintf(stderr, "Received wrong device_type for sensor update! The message is discarded...");
        return -1;
    }
    // valid message
    // case handshake
    if (cig->hdr.device_id == -1 && cig->hdr.type == HANDSHAKE) {
        int id = register_device(device_record);
        if (id == -1) {
            fprintf(stderr, "Unable to register device! The message is discarded...\n");
            return -1;
        }
        cig->hdr.device_id = id;
        printf("********************END EVENT********************\n\n");
        return 0;
    }
    // case sensor update
    if (cig->hdr.type == UPDATE && is_registered(cig->hdr.device_id, device_record)) {
        if (cig->hdr.device_type == TEMPERATURE) {
            printf("Temperature: %.4f --> Device_ID: %d\n",cig->value, cig->hdr.device_id);
        }
        if (cig->hdr.device_type == HUMIDITY) {
            printf("Humidity: %.4f --> Device_ID: %d\n", cig->value, cig->hdr.device_id);
        }
        adjust_fan(cig);
        printf("********************END EVENT********************\n\n");
        return 0;
    }
    return -1;



    
    fprintf(stderr, "Received corrupted cignal! The message is discarded...\n");
    return -1;
}
