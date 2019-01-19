Multi-hop project
=================
# Features
* Multi-hop network
* Transport and Application layers

# Transport layer:

## Node IDs
Generated after requesting a list of node IDs or manually assigned

## Multiple Hops
* Routing tables
* Duplicate packets

## Duplicate Packets
Nodes account for duplicate packets by storing the Source ID, and Seq Number of the last few packets.
A packet can only be forwarded N times. (2/3?)

## Packet
* Protocol Version?
* If using encryption
* Source ID (0 if not assigned yet)
* Destination ID (0 for broadcast)
* Checksum
* Packet length

Byte 0:
Encryption 1 bit, Data packet 1 bit, Non-Data type 1 bit, unused 5 bits

Byte 1: Checksum

Byte 2: IV (if encryption set otherwise next bytes are shifted down)

Byte 3: Source ID

Byte 4: Dest ID

Byte 5: Seq Num Incremented after every unique packet is sent

Bytes 6-63: Data

Length - already included

## Packet types
* Request list of nodes
* Ping node
* Data packet

Data packets have the data packet bit set.

Non-Data identifiers:

* Request list: 0
* Ping node: 1

### Node List Packet
The node list packet is an unencrypted broadcast packet that is used by nodes to discover each other.
The Source ID can be 0 if it has not been assigned.
The packet is not rebroadcast.
There are two subtypes of packets, the first is the request, which can be identified by a lack of data section.
The second type is the result, which has a data section as described below.
Multiple result packets may be sent if the list cannot be fit into a single packet.

Result data section:

Byte 0: number of nodes in this packet

Pairs of two bytes
Node ID, next node in path to node

### Ping Packet
A ping packet is sent to test if a node is still active.
Ping packets are responded to with a pong packet, which is a ping with a data section with a single 0 byte.

## encryption
* Include IV in packet
* ECB style encription

# Application layer protocols:

## Messaging
* User ID
* Channels (group messages)
* Direct messages

### Message Info
* Timestamp at sender end

### Messaging packets
* request channel list
* request users
* message (direct or channel)
* update channel messages
* break multiple messages up into multiple packets or limit size?

## File transfer
* Target uses User ID
* Send request, once accepted then starts transfer
* Allow pause and resume
* Stream and order packets
* checksums to validate files

# USART Interface:
