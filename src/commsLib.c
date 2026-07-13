#include "libPlx/commsLib.h"
#include <string.h>

#include "libSerial/serialLib.h"

extern uint32_t HAL_GetTick();

// 100 milliseconds in systicks
static const int T100MS = 100;

/// @brief full auto-send  begins after three timeouts have occurred
static const char TIMEOUT_COUNT = 3;
/// \@brief an array containing values being one more than incoming index for all observations or zero
// if no incoming index

static uint8_t obsIndex[PLX_MAX_OBS];

static PLX_SensorInfo info; ///< sensor info structure for getting idents

static int infoCount; ///< byte counter into info structure

static int timeoutCounter; ///< count how many times we have timed out at 100MS interval

static uint32_t timeoutTimer; ///< Timeout timer value

static enum {
    SCAN,             ///< Waiting for a start byte
    START,            ///< Currently seen a start byte - in first frame
    START_BADCHAR,    ///< Skipping first frame as bad character present
    INPACKET,         ///< Currently scanning input data
    INPACKET_BADCHAR, ///< Currently skipping input data for one PLx observation
    SEND              ///< Send our data
} inPacketState = SCAN;

void sendPlxInfo(struct usart_ctl *uc, int address, int reading)
{
    PLX_SensorInfo info;
    ConvToPLXAddr(address, &info);
    ConvToPLXInstance(libPLXgetNextInstance(address), &info);
    ConvToPLXReading(reading, &info);
    sendInfo(uc, &info);
}

// send a PLX_SensorInfo structure to the usart.
void sendInfo(struct usart_ctl *uc, PLX_SensorInfo *info)
{
    for (int i = 0; i < sizeof(PLX_SensorInfo); ++i)
        PutCharSerial(uc, info->bytes[i]);
}

void resetPLX()
{
    memset(obsIndex, 0, PLX_MAX_OBS); // zero incoming obsevation index
    infoCount = -1;                   ///< counting bytes when we are copying them across
    inPacketState = SCAN;

    timeoutTimer = HAL_GetTick() + T100MS;
    timeoutCounter = 0;
}

/// @brief Return a flag indicating that the scanner is running
/// @return
static char inline isRunning()
{
    return inPacketState == START || inPacketState == START_BADCHAR || inPacketState == INPACKET || inPacketState == INPACKET_BADCHAR;
}

void libPLXpollData(struct usart_ctl *instance)
{
    // check to see if we have any incoming data, copy and append if so, if no data then create our own frames.

    // locally compute if we have timeoutFlag.
    // wait three cycles without start, then every 100mS
    // send another automatic frame from local data
    uint8_t timeoutFlag = 0;

    if (HAL_GetTick() > timeoutTimer)
    {
        timeoutTimer += T100MS;
        if (timeoutCounter < TIMEOUT_COUNT)
            ++timeoutCounter;
        else
            timeoutFlag = 1;
    }

    // poll the input
    while (PollSerial(instance))
    {
        uint8_t c = GetCharSerial(instance);
        timeoutFlag = 0; // cancel any timeout, as we are getting characters
        uint32_t now = HAL_GetTick();

        // code added to read in all data, extract the observation index maximum for each type of data
        if (c == PLX_Start)
        {
            // tell the caller we have seen a start frame
            timeoutCounter = 0;
            timeoutTimer = now + T100MS;

            inPacketState = START; // indicate we are inside a packet, have seen start
            infoCount = 0;
            memset(obsIndex, 0, PLX_MAX_OBS); // zero incoming observation index table
        }
        else if (c == PLX_Stop)
        {
            // Stop character while running, switch to sending data
            if (isRunning())
                inPacketState = SEND;
            else
            {
                // break out only if not sending our data
                inPacketState = SCAN;
                break;
            }
        }
        else if (isRunning())
        {
            info.bytes[infoCount++] = c;

            // invalid data byte : greater or equal to 0x40 (PLX_Stop) and in the middle of a data frame
            if (c > PLX_Stop)
            {
                if (inPacketState == START)
                    inPacketState = START_BADCHAR;
                else
                    inPacketState = INPACKET_BADCHAR;
            }
            // process the sensor info field : discover maximum observation index
            if (infoCount == sizeof(PLX_SensorInfo))
            {
                infoCount = 0;
                int addr = ConvPLXAddr(&info);
                // make sure the numbers make sense
                if ((addr < PLX_MAX_OBS) && (info.Instance < PLX_MAX_INST) && (obsIndex[addr] <= info.Instance))
                {
                    // make the "next" observation index one more than the current observation
                    obsIndex[addr] = info.Instance + 1;
                    switch (inPacketState)
                    {
                    case START:

                        PutCharSerial(instance, PLX_Start);
                        inPacketState = INPACKET;
                    case INPACKET:
                        // only forward data that makes sense..
                        sendInfo(instance, &info);
                        libPLXcallbackRecievedData(&info);
                        break;
                    // failure during start packet, so try starting next time
                    case START_BADCHAR:
                        inPacketState = START;
                        break;
                    // failure during non-start packet, just start scanning next time. -
                    case INPACKET_BADCHAR:
                        inPacketState = INPACKET;
                    default:
                        // nothing
                        break;
                    }
                }
            }
        }
    }
    // sort out auto-sending : if there has been no traffic for three cycles of 10Hz,
    // start sending data ourselves
    if (timeoutFlag)
    {
        PutCharSerial(instance, PLX_Start);
        memset(obsIndex, 0, PLX_MAX_OBS); // zero incoming obsevation index
        inPacketState = SEND;
        // and then do the sending step
    }

    if (inPacketState == SEND)
    {
        inPacketState = SCAN;

        // call user data send
        libPLXcallbackSendUserData(instance);
        PutCharSerial(instance, PLX_Stop);
    }
}

int libPLXgetNextInstance(enum PLX_Observations obs)
{
    if (obs < 0 || obs >= PLX_MAX_OBS)
        return 0;

    return obsIndex[obs]++;
}
