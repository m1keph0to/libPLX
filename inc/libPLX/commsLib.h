#include "libPlx/plx.h"

struct usart_ctl;

#if defined __cplusplus
extern "C"
{
#endif
    /// @brief Send information from a PLX sensor to the USART.
    /// @param instance USART to send to
    /// @param info Single sensor info
    void sendInfo(struct usart_ctl *instance, PLX_SensorInfo *info);

    /// @brief Send a formatted message
    /// @param uc USART control handle
    /// @param address PLX message type
    /// @param reading observation value
    void sendPlxInfo(struct usart_ctl *uc, int address, int reading);

    /// @brief Reset the PLX receive function
    extern void resetPLX();

    /// @brief Function to call to poll input serial data and send to usart
    /// @param instance USART to send to and poll from
    extern void libPLXpollData(struct usart_ctl *instance);

    /// @brief Get the next index value for this observation
    /// @param  obs The observation code
    /// @return index or 0 if out of range
    extern int libPLXgetNextInstance(enum PLX_Observations obs);

    /// @brief Function provided by user to append extra readings within
    /// outgoing PLX packet
    extern void libPLXcallbackSendUserData(struct usart_ctl *instance);

    /// @brief Function called when data has been recieved
    /// @param info pointer to sensor info.
    extern void libPLXcallbackRecievedData(PLX_SensorInfo *info);

#if defined __cplusplus
}
#endif
