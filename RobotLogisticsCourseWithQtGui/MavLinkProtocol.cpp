#include "MavLinkProtocol.h"
#include <Driver/SerialPortLink.h>
#include <QDebug>

MAVLinkProtocol::MAVLinkProtocol() {

}

MAVLinkProtocol::~MAVLinkProtocol() {

}

void MAVLinkProtocol::setCurrentPortName(QString currentPortName) {
    serialPortName = currentPortName;
}

void MAVLinkProtocol::sendMessage(mavlink_message_t message) {
    // Create buffer
    static uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    // Rewriting header to ensure correct link ID is set
    static uint8_t messageKeys[256] = MAVLINK_MESSAGE_CRCS;
    //if (link->getId() != 0)
    mavlink_finalize_message_chan(&message, systemId, componentId, linkId,
            message.len, messageKeys[message.msgid]);
    // Write message into buffer, prepending start sign
    int len = mavlink_msg_to_send_buffer(buffer, &message);


    serialPortLink.setCurrentPortName(serialPortName);
    serialPortLink.writeBytes((const char*) buffer, len);  //send data and data length to be written at serial port

    /*
    [#Example]: for declearing new object, just for further REF

    SerialPortLink* serialLink = new SerialPortLink(serialPortName);

    */

}





void MAVLinkProtocol::sendHeartbeat() {
    mavlink_message_t beat;
    //default for now: systemId = 255; ComponentID = 0;
    mavlink_msg_heartbeat_pack(255, 0, &beat, MAV_TYPE_GCS,
            MAV_AUTOPILOT_INVALID, MAV_MODE_MANUAL_ARMED, 0, MAV_STATE_ACTIVE);
    sendMessage(beat);
}

bool MAVLinkProtocol::isConnected(){
return serialPortLink.isConnected();
}

void MAVLinkProtocol::receiveBytes(QByteArray b)
{
//    receiveMutex.lock();
    mavlink_message_t message;
    mavlink_status_t status;

    // Cache the link ID for common use.
   // int linkId = link->getId();

    static int mavlink09Count = 0;
    static int nonmavlinkCount = 0;
    static bool decodedFirstPacket = false;
    static bool warnedUser = false;
    static bool checkedUserNonMavlink = false;
    static bool warnedUserNonMavlink = false;

    // FIXME: Add check for if link->getId() >= MAVLINK_COMM_NUM_BUFFERS
    for (int position = 0; position < b.size(); position++) {
        unsigned int decodeState = mavlink_parse_char(linkId, (uint8_t)(b[position]), &message, &status);

        if ((uint8_t)b[position] == 0x55) mavlink09Count++;
        if ((mavlink09Count > 100) && !decodedFirstPacket && !warnedUser)
        {
            warnedUser = true;
            // Obviously the user tries to use a 0.9 autopilot
            // with QGroundControl built for version 1.0
//            emit protocolStatusMessage(tr("MAVLink Protocol"), tr("There is a MAVLink Version or Baud Rate Mismatch. "
//                                                                  "Your MAVLink device seems to use the deprecated version 0.9, while QGroundControl only supports version 1.0+. "
//                                                                  "Please upgrade the MAVLink version of your autopilot. "
//                                                                  "If your autopilot is using version 1.0, check if the baud rates of QGroundControl and your autopilot are the same."));
        }

        if (decodeState == 0 && !decodedFirstPacket)
        {
            nonmavlinkCount++;
            if (nonmavlinkCount > 2000 && !warnedUserNonMavlink)
            {
                //2000 bytes with no mavlink message. Are we connected to a mavlink capable device?
                if (!checkedUserNonMavlink)
                {
//                    link->requestReset();
                    checkedUserNonMavlink = true;
                }
                else
                {
                    warnedUserNonMavlink = true;
//                    emit protocolStatusMessage(tr("MAVLink Protocol"), tr("There is a MAVLink Version or Baud Rate Mismatch. "
//                                                                          "Please check if the baud rates of QGroundControl and your autopilot are the same."));
                }
            }
        }
        if (decodeState == 1)
        {
            decodedFirstPacket = true;

            if(message.msgid == MAVLINK_MSG_ID_PING)
            {
                // process ping requests (tgt_system and tgt_comp must be zero)
                mavlink_ping_t ping;
                mavlink_msg_ping_decode(&message, &ping);
                if(!ping.target_system && !ping.target_component)
                {
                    mavlink_message_t msg;
                    mavlink_msg_ping_pack(systemId, componentId, &msg, ping.time_usec, ping.seq, message.sysid, message.compid);
                    sendMessage(msg);
                }
            }

            if(message.msgid == MAVLINK_MSG_ID_RADIO_STATUS)
            {
                // process telemetry status message
                mavlink_radio_status_t rstatus;
                mavlink_msg_radio_status_decode(&message, &rstatus);

//                emit radioStatusChanged(link, rstatus.rxerrors, rstatus.fixed, rstatus.rssi, rstatus.remrssi,
//                    rstatus.txbuf, rstatus.noise, rstatus.remnoise);
            }

            // Log data

//            if (!_logSuspendError && !_logSuspendReplay && _tempLogFile.isOpen()) {
//                uint8_t buf[MAVLINK_MAX_PACKET_LEN+sizeof(quint64)];

//                // Write the uint64 time in microseconds in big endian format before the message.
//                // This timestamp is saved in UTC time. We are only saving in ms precision because
//                // getting more than this isn't possible with Qt without a ton of extra code.
//                quint64 time = (quint64)QDateTime::currentMSecsSinceEpoch() * 1000;
//                qToBigEndian(time, buf);

//                // Then write the message to the buffer
//                int len = mavlink_msg_to_send_buffer(buf + sizeof(quint64), &message);

//                // Determine how many bytes were written by adding the timestamp size to the message size
//                len += sizeof(quint64);

//                // Now write this timestamp/message pair to the log.
//                QByteArray b((const char*)buf, len);
//                if(_tempLogFile.write(b) != len)
//                {
//                    // If there's an error logging data, raise an alert and stop logging.
//                    emit protocolStatusMessage(tr("MAVLink Protocol"), tr("MAVLink Logging failed. Could not write to file %1, logging disabled.").arg(_tempLogFile.fileName()));
//                    _stopLogging();
//                    _logSuspendError = true;
//                }
//            }

            // ORDER MATTERS HERE!
            // If the matching UAS object does not yet exist, it has to be created
            // before emitting the packetReceived signal

//            UASInterface* uas = UASManager::instance()->getUASForId(message.sysid);

            // Check and (if necessary) create UAS object
            if (uas == NULL && message.msgid == MAVLINK_MSG_ID_HEARTBEAT)
            {
                // ORDER MATTERS HERE!
                // The UAS object has first to be created and connected,
                // only then the rest of the application can be made aware
                // of its existence, as it only then can send and receive
                // it's first messages.

                // Check if the UAS has the same id like this system
                if (message.sysid == getSystemId())
                {
//                    emit protocolStatusMessage(tr("MAVLink Protocol"), tr("Warning: A second system is using the same system id (%1)").arg(getSystemId()));
                }

                // Create a new UAS based on the heartbeat received
                // Todo dynamically load plugin at run-time for MAV
                // WIKISEARCH:AUTOPILOT_TYPE_INSTANTIATION

                // First create new UAS object
                // Decode heartbeat message
                mavlink_heartbeat_t heartbeat;
                // Reset version field to 0
                heartbeat.mavlink_version = 0;
                mavlink_msg_heartbeat_decode(&message, &heartbeat);

                // Check if the UAS has a different protocol version
                if (m_enable_version_check && (heartbeat.mavlink_version != MAVLINK_VERSION))
                {
                    // Bring up dialog to inform user
                    if (!versionMismatchIgnore)
                    {
                        emit protocolStatusMessage(tr("MAVLink Protocol"), tr("The MAVLink protocol version on the MAV and QGroundControl mismatch! "
                                                                              "It is unsafe to use different MAVLink versions. "
                                                                              "QGroundControl therefore refuses to connect to system %1, which sends MAVLink version %2 (QGroundControl uses version %3).").arg(message.sysid).arg(heartbeat.mavlink_version).arg(MAVLINK_VERSION));
                        versionMismatchIgnore = true;
                    }

                    // Ignore this message and continue gracefully
                    continue;
                }

                // Create a new UAS object
                uas = QGCMAVLinkUASFactory::createUAS(this, link, message.sysid, &heartbeat);

            }

            // Increase receive counter
            totalReceiveCounter[linkId]++;
            currReceiveCounter[linkId]++;

            // Determine what the next expected sequence number is, accounting for
            // never having seen a message for this system/component pair.
            int lastSeq = lastIndex[message.sysid][message.compid];
            int expectedSeq = (lastSeq == -1) ? message.seq : (lastSeq + 1);

            // And if we didn't encounter that sequence number, record the error
            if (message.seq != expectedSeq)
            {

                // Determine how many messages were skipped
                int lostMessages = message.seq - expectedSeq;

                // Out of order messages or wraparound can cause this, but we just ignore these conditions for simplicity
                if (lostMessages < 0)
                {
                    lostMessages = 0;
                }

                // And log how many were lost for all time and just this timestep
                totalLossCounter[linkId] += lostMessages;
                currLossCounter[linkId] += lostMessages;
            }

            // And update the last sequence number for this system/component pair
            lastIndex[message.sysid][message.compid] = expectedSeq;

            // Update on every 32th packet
            if ((totalReceiveCounter[linkId] & 0x1F) == 0)
            {
                // Calculate new loss ratio
                // Receive loss
                float receiveLoss = (double)currLossCounter[linkId]/(double)(currReceiveCounter[linkId]+currLossCounter[linkId]);
                receiveLoss *= 100.0f;
                currLossCounter[linkId] = 0;
                currReceiveCounter[linkId] = 0;
                emit receiveLossChanged(message.sysid, receiveLoss);
            }

            // The packet is emitted as a whole, as it is only 255 - 261 bytes short
            // kind of inefficient, but no issue for a groundstation pc.
            // It buys as reentrancy for the whole code over all threads
//            emit messageReceived(link, message);

            // Multiplex message if enabled
//            if (m_multiplexingEnabled)
//            {
//                // Get all links connected to this unit
//                QList<LinkInterface*> links = _linkMgr->getLinks();

//                // Emit message on all links that are currently connected
//                foreach (LinkInterface* currLink, links)
//                {
//                    // Only forward this message to the other links,
//                    // not the link the message was received on
//                    if (currLink != link) sendMessage(currLink, message, message.sysid, message.compid);
//                }
//            }
        }
    }
}
