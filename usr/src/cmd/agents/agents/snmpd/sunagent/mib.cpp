-- Copyright 09/19/96 Sun Microsystems, Inc. All Rights Reserved.
--pragma ident  "@(#)mib.cpp	1.1 96/09/19 Sun Microsystems"

                RFC1066-MIB { iso org(3) dod(6) internet(1) mgmt(2) 1 }

--***************************************************************************
--*     Copyright (c) 1988, 1989  Epilogue Technology Corporation
--*     All rights reserved.
--*
--*     This is unpublished proprietary source code of Epilogue Technology
--*     Corporation.
--*
--*     The copyright notice above does not evidence any actual or intended
--*     publication of such source code.
--***************************************************************************

-- $Header:   D:/snmpv2/agent/sun/mib.asv   2.2   20 Jun 1990 18:01:00  $

		FORCE-INCLUDE <stdio.h>
		FORCE-INCLUDE <sys/types.h>
		FORCE-INCLUDE <sys/socket.h>
		FORCE-INCLUDE <netinet/in.h>
		FORCE-INCLUDE <netinet/in_systm.h>
		FORCE-INCLUDE <netinet/ip.h>
		FORCE-INCLUDE <netinet/ip_icmp.h>
		FORCE-INCLUDE <net/if.h>
		FORCE-INCLUDE <netinet/if_ether.h>
		FORCE-INCLUDE <asn1.h>
		FORCE-INCLUDE <mib.h>
		FORCE-INCLUDE "general.h"
		FORCE-INCLUDE "snmp.h"
		FORCE-INCLUDE "snmpvars.h"

		-- MIB file for Sun OS 4.x
		-- set up MIB-wide defaults
		-- all functions can use %n for object name and
		-- %t for object type, %p for parent name,
		-- %d for name of default-bearing node and %% for %
		DEFAULT test-function	it_exists
		DEFAULT	set-function	set_%n
		DEFAULT	get-function	get_%n
		DEFAULT	next-function	std_next
		DEFAULT cookie		(char *)NULL

-- There are four view definitions:
-- 0x01 is a read-only view of the system group
-- 0x02 is a read-write view of the system group
-- 0x04 is a read-only view of the entire MIB
-- 0x08 is a read-write view of the entire MIB
-- For simplicity the defaults are set to cover the bulk of the MIB,
--   i.e. everything outside the system group.
		DEFAULT view-mask	0x0C
		DEFAULT write-mask	0x08

                DEFINITIONS ::= BEGIN

                IMPORTS
                        mgmt, OBJECT-TYPE, NetworkAddress, IpAddress,
                        Counter, Gauge, TimeTicks
                            FROM RFC1065-SMI;

                  mib        OBJECT IDENTIFIER ::= { mgmt 1 }

		  private    OBJECT IDENTIFIER ::= { internet 4 }
 	          enterprises   OBJECT IDENTIFIER ::= { private 1 }

                  system     OBJECT IDENTIFIER ::= { mib 1 }
                  interfaces OBJECT IDENTIFIER ::= { mib 2 }
                  at         OBJECT IDENTIFIER ::= { mib 3 }
                  ip         OBJECT IDENTIFIER ::= { mib 4 }
                  icmp       OBJECT IDENTIFIER ::= { mib 5 }
                  tcp        OBJECT IDENTIFIER ::= { mib 6 }
                  udp        OBJECT IDENTIFIER ::= { mib 7 }
-- no egp, thank you! we're a host not a router...
--                egp        OBJECT IDENTIFIER ::= { mib 8 }
	          snmp       OBJECT IDENTIFIER ::= { mib 11 }

                  -- object types

                  -- the System group

                  sysDescr OBJECT-TYPE
                          SYNTAX  DisplayString (SIZE (0..255))
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function	get_string
			  DEFAULT cookie snmp_sysDescr
			  DEFAULT view-mask	0x0F
			  DEFAULT write-mask	0x0A
                          ::= { system 1 }

               sysObjectID OBJECT-TYPE
                       SYNTAX  OBJECT IDENTIFIER
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_object_identifier
		       DEFAULT cookie (char *)&snmp_sysObjectID
		       DEFAULT view-mask	0x0F
		       DEFAULT write-mask	0x0A
                       ::= { system 2 }

                  sysUpTime OBJECT-TYPE
                          SYNTAX  TimeTicks
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT view-mask	0x0F
			  DEFAULT write-mask	0x0A
                          ::= { system 3 }

               sysContact OBJECT-TYPE
                       SYNTAX  DisplayString (SIZE (0..255))
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_string
		       DEFAULT set-function set_sysContact
		       DEFAULT cookie (char *)snmp_sysContact
		       DEFAULT view-mask	0x0F
		       DEFAULT write-mask	0x0A
                       ::= { system 4 }

               sysName OBJECT-TYPE
                       SYNTAX  DisplayString (SIZE (0..255))
                       -- ACCESS  read-write
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT set-function null_set_proc
		       DEFAULT get-function get_sysName
		       DEFAULT view-mask	0x0F
		       DEFAULT write-mask	0x0A
                       ::= { system 5 }

               sysLocation OBJECT-TYPE
                       SYNTAX  DisplayString (SIZE (0..255))
                       -- ACCESS  read-only
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_string
		       DEFAULT set-function set_sysLocation
		       DEFAULT cookie (char *)snmp_sysLocation
		       DEFAULT view-mask	0x0F
		       DEFAULT write-mask	0x0A
                       ::= { system 6 }

               sysServices OBJECT-TYPE
                       SYNTAX  INTEGER (0..127)
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       -- Services are:
		       --	internet (e.g., IP gateways), but
		       --		only if ipforwarding is set.
		       --	end-to-end
		       --	applications
		       DEFAULT cookie (char *)0x48
		       DEFAULT view-mask	0x0F
		        DEFAULT write-mask	0x0A
                       ::= { system 7 }


               -- the Interfaces group

               ifNumber OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { interfaces 1 }

               -- the Interfaces table

               ifTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF IfEntry
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT test-function	iftable_test
		       DEFAULT set-function	null_set_proc
		       DEFAULT next-function	iftable_next
		       DEFAULT cookie		(char *)NULL
                       ::= { interfaces 2 }

               ifEntry OBJECT-TYPE
                       SYNTAX  IfEntry
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifTable 1 }

               IfEntry ::= SEQUENCE {
                   ifIndex
                       INTEGER,
                   ifDescr
                       DisplayString,
                   ifType
                       INTEGER,
                   ifMtu
                       INTEGER,
                   ifSpeed
                       Gauge,
                   ifPhysAddress
                       OCTET STRING,
                   ifAdminStatus
                       INTEGER,
                   ifOperStatus
                       INTEGER,
                   ifLastChange
                       TimeTicks,
                   ifInOctets
                       Counter,
                   ifInUcastPkts
                       Counter,
                   ifInNUcastPkts
                       Counter,
                   ifInDiscards
                       Counter,
                   ifInErrors
                       Counter,
                   ifInUnknownProtos
                       Counter,
                   ifOutOctets
                       Counter,
                   ifOutUcastPkts
                       Counter,
                   ifOutNUcastPkts
                       Counter,
                   ifOutDiscards
                       Counter,
                   ifOutErrors
                       Counter,
                   ifOutQLen
                       Gauge,
                   ifSpecific
                       OBJECT IDENTIFIER
               }

               ifIndex OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 1 }

               ifDescr OBJECT-TYPE
                       SYNTAX  DisplayString (SIZE (0..255))
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 2 }

               ifType OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   other(1),           -- none of the following
                                   regular1822(2),
                                   hdh1822(3),
                                   ddn-x25(4),
                                   rfc877-x25(5),
                                   ethernet-csmacd(6),
                                   iso88023-csmacd(7),
                                   iso88024-tokenBus(8),
                                   iso88025-tokenRing(9),
                                   iso88026-man(10),
                                   starLan(11),
                                   proteon-10Mbit(12),
                                   proteon-80Mbit(13),
                                   hyperchannel(14),
                                   fddi(15),
                                   lapb(16),
                                   sdlc(17),
                                   t1-carrier(18),
                                   cept(19),           -- european equivalent of T-1
                                   basicISDN(20),
                                   primaryISDN(21),
                                                       -- proprietary serial
                                   propPointToPointSerial(22),
                                   terminalServer-asyncPort(23),
                                   softwareLoopback(24),
                                   eon(25),            -- CLNP over IP
                                   ethernet-3Mbit(26),
                                   nsip(27),           -- XNS over IP
                                   slip(28)            -- generic SLIP
                           }
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 3 }

               ifMtu OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 4 }

               ifSpeed OBJECT-TYPE
                       SYNTAX  Gauge
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 5 }

               ifPhysAddress OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 6 }

               ifAdminStatus OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   up(1),        -- ready to pass packets
                                   down(2),
                                   testing(3)    -- in some test mode
                               }
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT set-function	set_%n
			 -- granga  12/19/94  bug - 1185563
                       DEFAULT test-function    test_%n
                       ::= { ifEntry 7 }

               ifOperStatus OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   up(1),        -- ready to pass packets
                                   down(2),
                                   testing(3)    -- in some test mode
                               }
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 8 }

               ifLastChange OBJECT-TYPE
                       SYNTAX  TimeTicks
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 9 }

               ifInOctets OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { ifEntry 10 }

               ifInUcastPkts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { ifEntry 11 }

               ifInNUcastPkts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { ifEntry 12 }

               ifInDiscards OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { ifEntry 13 }

               ifInErrors OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 14 }

               ifInUnknownProtos OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { ifEntry 15 }

               ifOutOctets OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { ifEntry 16 }

               ifOutUcastPkts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 17 }

               ifOutNUcastPkts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { ifEntry 18 }

               ifOutDiscards OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { ifEntry 19 }

               ifOutErrors OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 20 }

               ifOutQLen OBJECT-TYPE
                       SYNTAX  Gauge
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 21 }

               ifSpecific OBJECT-TYPE
                       SYNTAX  OBJECT IDENTIFIER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ifEntry 22 }

               -- the Address Translation group (deprecated)

               atTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF AtEntry
                       ACCESS  read-write
                       STATUS  deprecated
		       DEFAULT test-function	arptable_test
		       DEFAULT set-function	null_set_proc
		       DEFAULT next-function	arptable_next
		       DEFAULT cookie		(char *)NULL
                       ::= { at 1 }

               atEntry OBJECT-TYPE
                       SYNTAX  AtEntry
                       ACCESS  read-write
                       STATUS  deprecated
                       ::= { atTable 1 }

               AtEntry ::= SEQUENCE {
                   atIfIndex
                       INTEGER,
                   atPhysAddress
                       OCTET STRING,
                   atNetAddress
                       NetworkAddress
               }

               atIfIndex OBJECT-TYPE
                       SYNTAX  INTEGER
                       --ACCESS  read-write
                       ACCESS  read-only
                       STATUS  deprecated
                       ::= { atEntry 1 }

               atPhysAddress OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-write
                       STATUS  deprecated
		       DEFAULT set-function set_atPhysAddress
                       ::= { atEntry 2 }

               atNetAddress OBJECT-TYPE
                       SYNTAX  NetworkAddress
                       --ACCESS  read-write
                       ACCESS  read-only
                       STATUS  deprecated
                       ::= { atEntry 3 }

                  -- the IP group

                  ipForwarding OBJECT-TYPE
                          SYNTAX  INTEGER {
                        gateway(1), -- entity forwards datagrams
                        host(2)     -- entity does NOT forward datagrams
                                  }
                          ACCESS  read-write
                          STATUS  mandatory
			  DEFAULT test-function	test_%n
                          ::= { ip 1 }

                  ipDefaultTTL OBJECT-TYPE
                          SYNTAX  INTEGER
                          -- ACCESS  read-write
                          ACCESS  read-only
                          STATUS  mandatory
                          ::= { ip 2 }

                  ipInReceives OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 3 }

                  ipInHdrErrors OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 4 }

                  ipInAddrErrors OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 5 }

                  ipForwDatagrams OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 6 }

                  ipInUnknownProtos OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 7 }

                  ipInDiscards OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 8 }

                  ipInDelivers OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 9 }

                  ipOutRequests OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 10 }

                  ipOutDiscards OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 11 }

                  ipOutNoRoutes OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 12 }

                  ipReasmTimeout OBJECT-TYPE
                          SYNTAX  INTEGER
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 13 }

                  ipReasmReqds OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 14 }

                  ipReasmOKs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 15 }

                  ipReasmFails OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 16 }

                  ipFragOKs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 17 }

                  ipFragFails OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 18 }

                  ipFragCreates OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_ip
                          ::= { ip 19 }

               -- the IP Interface table

               ipAddrTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF IpAddrEntry
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT test-function	ipadtable_test
		       DEFAULT set-function	null_set_proc
		       DEFAULT next-function	ipadtable_next
		       DEFAULT cookie		(char *)NULL
                       ::= { ip 20 }

               ipAddrEntry OBJECT-TYPE
                       SYNTAX  IpAddrEntry
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { ipAddrTable 1 }

               IpAddrEntry ::= SEQUENCE {
                   ipAdEntAddr
                       IpAddress,
                   ipAdEntIfIndex
                       INTEGER,
                   ipAdEntNetMask
                       IpAddress,
                   ipAdEntBcastAddr
                       INTEGER,
                   ipAdEntReasmMaxSize
                       INTEGER (0..65535)
               }

               ipAdEntAddr OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { ipAddrEntry 1 }

               ipAdEntIfIndex OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { ipAddrEntry 2 }

               ipAdEntNetMask OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { ipAddrEntry 3 }

               ipAdEntBcastAddr OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)1
                       ::= { ipAddrEntry 4 }

               ipAdEntReasmMaxSiz OBJECT-TYPE
                       SYNTAX  INTEGER (0..65535)
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)65535
                       ::= { ipAddrEntry 5 }

               -- the IP Routing table

               ipRoutingTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF IpRouteEntry
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function	get_%n
		       DEFAULT set-function	set_%n
		       DEFAULT test-function	test_%p
		       DEFAULT next-function	next_%p
		       DEFAULT cookie	(char *)0
                       ::= { ip 21 }

               ipRouteEntry OBJECT-TYPE
                       SYNTAX  IpRouteEntry
                       ACCESS  read-write
                       STATUS  mandatory
                       ::= { ipRoutingTable 1 }

               IpRouteEntry ::= SEQUENCE {
                   ipRouteDest
                       IpAddress,
                   ipRouteIfIndex
                       INTEGER,
                   ipRouteMetric1
                       INTEGER,
                   ipRouteMetric2
                       INTEGER,
                   ipRouteMetric3
                       INTEGER,
                   ipRouteMetric4
                       INTEGER,
                   ipRouteNextHop
                       IpAddress,
                   ipRouteType
                       INTEGER,
                   ipRouteProto
                       INTEGER,
                   ipRouteAge
                       INTEGER,
                   ipRouteMask
                       IpAddress,
                  ipRouteMetric5
                      INTEGER,
                  ipRouteInfo
                      OBJECT IDENTIFIER
               }

		--************************************************************
		-- Routing table entries have locator values in the range 101-150
		-- ***DO NOT CHANGE THE LOCATORS WITHOUT MAKING CORESPONDING
		--    CHANGES TO THE C CODE IN MODULE IPRTE.C !!!
		--************************************************************

               ipRouteDest OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT locator	101
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 1 }

               ipRouteIfIndex  OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT locator	102
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 2 }

               ipRouteMetric1 OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)-1
		       DEFAULT locator	103
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 3 }

               ipRouteMetric2 OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)-1
		       DEFAULT locator	104
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 4 }

               ipRouteMetric3 OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)-1
		       DEFAULT locator	105
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 5 }

               ipRouteMetric4 OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)-1
		       DEFAULT locator	106
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 6 }

               ipRouteNextHop OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT locator	107
                       ::= { ipRouteEntry 7 }

               ipRouteType OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   other(1),      -- none of the following

                                   invalid(2),    -- an invalidated route

                                                  -- route to directly
                                   direct(3),     -- connected (sub-)network

                                                  -- route to a non-local
                                   remote(4)     -- host/network/sub-network
                           }
		       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT locator	108
                       ::= { ipRouteEntry 8 }

               ipRouteProto OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   other(1),     -- none of the following

                                                 -- non-protocol information
                                                 --   e.g., manually
                                   local(2),     --   configured entries

                                                 -- set via a network
                                   netmgmt(3),   --   management protocol

                                                 -- obtained via ICMP,
                                   icmp(4),      --   e.g., Redirect

                                                 -- the following are
                                                 -- gateway routing protocols
                                   egp(5),
                                   ggp(6),
                                   hello(7),
                                   rip(8),
                                   is-is(9),
                                   es-is(10),
                                   ciscoIgrp(11),
                                   bbnSpfIgp(12),
                                   ospf(13)
                           }
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT locator	109
                       ::= { ipRouteEntry 9 }

               ipRouteAge OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
		       DEFAULT locator	110
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 10 }

               ipRouteMask OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT locator	111
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 11 }

               ipRouteMetric5 OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)-1
		       DEFAULT locator	112
		       DEFAULT set-function	null_set_proc
                       ::= { ipRouteEntry 12 }

          ipRouteInfo OBJECT-TYPE
              SYNTAX  OBJECT IDENTIFIER
              ACCESS  read-only
              STATUS  mandatory
              DEFAULT locator	113
              ::= { ipRouteEntry 13 }

               -- the IP Address Translation tables

               ipNetToMediaTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF IpNetToMediaEntry
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT test-function	n2mtable_test
		       DEFAULT set-function	null_set_proc
		       DEFAULT next-function	n2mtable_next
		       DEFAULT cookie		(char *)NULL
                       ::= { ip 22 }

               ipNetToMediaEntry OBJECT-TYPE
                       SYNTAX  IpNetToMediaEntry
                       ACCESS  read-write
                       STATUS  mandatory
                       ::= { ipNetToMediaTable 1 }

               IpNetToMediaEntry ::= SEQUENCE {
                   ipNetToMediaIfIndex
                       INTEGER,
                   ipNetToMediaPhysAddress
                       OCTET STRING,
                   ipNetToMediaNetAddress
                       IpAddress,
                   ipNetoToMediaType
                       INTEGER
               }

               ipNetToMediaIfIndex OBJECT-TYPE
                       SYNTAX  INTEGER
                       --ACCESS  read-write
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_n2mIfIndex
                       ::= { ipNetToMediaEntry 1 }

               ipNetToMediaPhysAddress OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_n2mPhysAddress
		       DEFAULT set-function set_n2mPhysAddress
                       ::= { ipNetToMediaEntry 2 }

               ipNetToMediaNetAddress OBJECT-TYPE
                       SYNTAX  IpAddress
                       -- ACCESS  read-write
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_n2mNetAddress
                       ::= { ipNetToMediaEntry 3 }

               ipNetToMediaType OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   other(1),      -- none of the following
                                   invalid(2),    -- an invalidated mapping
                                   dynamic(3),    -- connected (sub-)network
                                   static(4)
                           }
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT set-function set_ipNetToMediaType
		       DEFAULT cookie (char *)3
                       ::= { ipNetToMediaEntry 4 }

          ipRoutingDiscards OBJECT-TYPE
              SYNTAX  Counter
              ACCESS  read-only
              STATUS  mandatory
	      DEFAULT get-function get_cookie
	      DEFAULT cookie (char *)0
              ::= { ip 23 }

                  -- the ICMP group

                  icmpInMsgs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
                          ::= { icmp 1 }

                  icmpInErrors OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
                          ::= { icmp 2 }

                  icmpInDestUnreachs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_UNREACH
                          ::= { icmp 3 }

                  icmpInTimeExcds OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_TIMXCEED
                          ::= { icmp 4 }

                  icmpInParmProbs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_PARAMPROB
                          ::= { icmp 5 }

                  icmpInSrcQuenchs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_SOURCEQUENCH
                          ::= { icmp 6 }

                  icmpInRedirects OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_REDIRECT
                          ::= { icmp 7 }

                  icmpInEchos OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_ECHO
                          ::= { icmp 8 }

                  icmpInEchoReps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_ECHOREPLY
                          ::= { icmp 9 }

                  icmpInTimestamps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_TSTAMP
                          ::= { icmp 10 }

                  icmpInTimestampReps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_TSTAMPREPLY
                          ::= { icmp 11 }

                  icmpInAddrMasks OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_MASKREQ
                          ::= { icmp 12 }

                  icmpInAddrMaskReps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpin
			  DEFAULT cookie (char *)ICMP_MASKREPLY
                          ::= { icmp 13 }

                  icmpOutMsgs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
                          ::= { icmp 14 }

                  icmpOutErrors OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
                          ::= { icmp 15 }

                  icmpOutDestUnreachs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_UNREACH
                          ::= { icmp 16 }

                  icmpOutTimeExcds OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_TIMXCEED
                          ::= { icmp 17 }

                  icmpOutParmProbs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_PARAMPROB
                          ::= { icmp 18 }

                  icmpOutSrcQuenchs OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_SOURCEQUENCH
                          ::= { icmp 19 }

                  icmpOutRedirects OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_REDIRECT
                          ::= { icmp 20 }

                  icmpOutEchos OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_ECHO
                          ::= { icmp 21 }

                  icmpOutEchoReps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_ECHOREPLY
                          ::= { icmp 22 }

                  icmpOutTimestamps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_TSTAMP
                          ::= { icmp 23 }

                  icmpOutTimestampReps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_TSTAMPREPLY
                          ::= { icmp 24 }

                  icmpOutAddrMasks OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_MASKREQ
                          ::= { icmp 25 }

                  icmpOutAddrMaskReps OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function icmpout
			  DEFAULT cookie (char *)ICMP_MASKREPLY
                          ::= { icmp 26 }

                  -- the TCP group

                    tcpRtoAlgorithm OBJECT-TYPE
                            SYNTAX  INTEGER {
                            other(1),    -- none of the following
                            constant(2), -- a constant rto
                            rsre(3),     -- MIL-STD-1778, Appendix B
                            vanj(4)      -- Van Jacobson's algorithm [11]
                                    }
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_ints
                            ::= { tcp 1 }
  
                    tcpRtoMin OBJECT-TYPE
                            SYNTAX  INTEGER
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_ints
                            ::= { tcp 2 }
  
                    tcpRtoMax OBJECT-TYPE
                            SYNTAX  INTEGER
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_ints
                            ::= { tcp 3 }
  
                    tcpMaxConn OBJECT-TYPE
                            SYNTAX  INTEGER
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_ints
                            ::= { tcp 4 }
  
                    tcpActiveOpens OBJECT-TYPE
                            SYNTAX  Counter
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_uints
                            ::= { tcp 5 }
  
                    tcpPassiveOpens OBJECT-TYPE
                            SYNTAX  Counter
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_uints
                            ::= { tcp 6 }
  
                    tcpAttemptFails OBJECT-TYPE
                            SYNTAX  Counter
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_uints
                            ::= { tcp 7 }
  
                    tcpEstabResets OBJECT-TYPE
                            SYNTAX  Counter
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_uints
                            ::= { tcp 8 }
  
                    tcpCurrEstab OBJECT-TYPE
                            SYNTAX  Gauge
                            ACCESS  read-only
                            STATUS  mandatory
                            ::= { tcp 9 }
  
                    tcpInSegs OBJECT-TYPE
                            SYNTAX  Counter
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_uints
                            ::= { tcp 10 }
  
                    tcpOutSegs OBJECT-TYPE
                            SYNTAX  Counter
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_uints
                            ::= { tcp 11 }
  
                    tcpRetransSegs OBJECT-TYPE
                            SYNTAX  Counter
                            ACCESS  read-only
                            STATUS  mandatory
			    DEFAULT get-function get_tcp_uints
                            ::= { tcp 12 }
  
               -- the TCP connections table

               tcpConnTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF TcpConnEntry
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT test-function	tcptable_test
		       DEFAULT set-function	null_set_proc
		       DEFAULT next-function	tcptable_next
		       DEFAULT cookie		(char *)NULL
                       ::= { tcp 13 }

               tcpConnEntry OBJECT-TYPE
                       SYNTAX  TcpConnEntry
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { tcpConnTable 1 }

               TcpConnEntry ::= SEQUENCE {
                   tcpConnState
                       INTEGER,
                   tcpConnLocalAddress
                       IpAddress,
                   tcpConnLocalPort
                       INTEGER (0..65535),
                   tcpConnRemAddress
                       IpAddress,
                   tcpConnRemPort
                       INTEGER (0..65535)
               }

               tcpConnState OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   closed(1),
                                   listen(2),
                                   synSent(3),
                                   synReceived(4),
                                   established(5),
                                   finWait1(6),
                                   finWait2(7),
                                   closeWait(8),
                                   lastAck(9),
                                   closing(10),
                                   timeWait(11),
	                           deleteTCB(12)
                               }
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function	get_tcpConnState
                       ::= { tcpConnEntry 1 }

               tcpConnLocalAddress OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_tcptab_localaddr
                       ::= { tcpConnEntry 2 }

               tcpConnLocalPort OBJECT-TYPE
                       SYNTAX  INTEGER (0..65535)
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_tcptab_localport
                       ::= { tcpConnEntry 3 }

               tcpConnRemAddress OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_tcptab_remoteaddr
                       ::= { tcpConnEntry 4 }

               tcpConnRemPort OBJECT-TYPE
                       SYNTAX  INTEGER (0..65535)
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_tcptab_remoteport
                       ::= { tcpConnEntry 5 }

               -- additional TCP variables

               tcpInErrs OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_tcp_uints
                       ::= { tcp 14 }

               tcpOutRsts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function get_tcp_uints
                       ::= { tcp 15 }

                  -- the UDP group
                  udpInDatagrams OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  -- DEFAULT get-function get_cookie
			  DEFAULT get-function get_udp_uints
			  DEFAULT cookie (char *)0
                          ::= { udp 1 }

                   udpNoPorts OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  -- DEFAULT get-function get_cookie
			  DEFAULT get-function get_udp_uints
			  DEFAULT cookie (char *)0
                          ::= { udp 2 }

                   udpInErrors OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  DEFAULT get-function get_udp_uints
                          ::= { udp 3 }

                  udpOutDatagrams OBJECT-TYPE
                          SYNTAX  Counter
                          ACCESS  read-only
                          STATUS  mandatory
			  -- DEFAULT get-function get_cookie
			  DEFAULT get-function get_udp_uints
			  DEFAULT cookie (char *)0
                          ::= { udp 4 }

               -- the UDP listener table

               udpTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF UdpEntry
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT test-function	udptable_test
		       DEFAULT set-function	null_set_proc
		       DEFAULT next-function	udptable_next
		       DEFAULT cookie		(char *)NULL
                       ::= { udp 5 }

               udpEntry OBJECT-TYPE
                       SYNTAX  UdpEntry
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { udpTable 1 }

               UdpEntry ::= SEQUENCE {
                   udpLocalAddress
                       IpAddress,
                   udpLocalPort
                       INTEGER (0..65535)
               }

               udpLocalAddress OBJECT-TYPE
                       SYNTAX  IpAddress
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_udptab_ipaddr
                       ::= { udpEntry 1 }

               udpLocalPort OBJECT-TYPE
                       SYNTAX  INTEGER (0..65535)
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_udptab_localport
                       ::= { udpEntry 2 }
 
              -- the SNMP group

               snmpInPkts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInPkts
                       ::=  { snmp 1 }

               snmpOutPkts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutPkts
                       ::=  { snmp 2 }

               snmpInBadVersions OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInBadVersions
                       ::=  { snmp 3 }

               snmpInBadCommunityNames OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInBadCommunityNames
                       ::=  { snmp 4 }

               snmpInBadCommunityUses OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInBadCommunityUses
                       ::=  { snmp 5 }

               snmpInASNParseErrs OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInASNParseErrs
                       ::=  { snmp 6 }


               snmpInTooBigs OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInTooBigs
                       ::=  { snmp 8 }

               snmpInNoSuchNames OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInNoSuchNames
                       ::=  { snmp 9 }

               snmpInBadValues OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInBadValues
                       ::=  { snmp 10 }

               snmpInReadOnlys OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInReadOnlys
                       ::=  { snmp 11 }

               snmpInGenErrs OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInGenErrs
                       ::=  { snmp 12 }

               snmpInTotalReqVars OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInTotalReqVars
                       ::=  { snmp 13 }

               snmpInTotalSetVars OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInTotalSetVars
                       ::=  { snmp 14 }

               snmpInGetRequests OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInGetRequests
                       ::=  { snmp 15 }

               snmpInGetNexts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInGetNexts
                       ::=  { snmp 16 }

               snmpInSetRequests OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInSetRequests
                       ::=  { snmp 17 }

               snmpInGetResponses OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInGetResponses
                       ::=  { snmp 18 }

               snmpInTraps OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpInTraps
                       ::=  { snmp 19 }

               snmpOutTooBigs OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutTooBigs
                       ::=  { snmp 20 }

               snmpOutNoSuchNames OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutNoSuchNames
                       ::=  { snmp 21 }

               snmpOutBadValues OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutBadValues
                       ::=  { snmp 22 }

               snmpOutGenErrs OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutGenErrs
                       ::=  { snmp 24 }

               snmpOutGetRequests OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutGetRequests
                       ::=  { snmp 25 }

               snmpOutGetNexts OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutGetNexts
                       ::=  { snmp 26 }

               snmpOutSetRequests OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutSetRequests
                       ::=  { snmp 27 }

               snmpOutGetResponses OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutGetResponses
                       ::=  { snmp 28 }

               snmpOutTraps OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT get-function	get_ulong
		       DEFAULT cookie	(char *)&snmp_stats.snmpOutTraps
                       ::=  { snmp 29 }

	       snmpEnableAuthenTraps OBJECT-TYPE
                       SYNTAX  INTEGER {
                                   enabled(1),
                                   disabled(2)
                               }
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function	get_snmpEnableAuthTraps
		       DEFAULT set-function	set_snmpEnableAuthTraps
		       DEFAULT cookie		(char *)0
                       ::=  { snmp 30 }


--  **********************************************************************
--  SUN EXTENSIONS
--  **********************************************************************
		sun		OBJECT IDENTIFIER ::= { enterprises 42 }
		products	OBJECT IDENTIFIER ::= { sun 2 }
		sunMib		OBJECT IDENTIFIER ::= { sun 3 }

		sunSystem	OBJECT IDENTIFIER ::= { sunMib 1 }
		sunInterfaces	OBJECT IDENTIFIER ::= { sunMib 2 }
		sunAt		OBJECT IDENTIFIER ::= { sunMib 3 }
		sunIp		OBJECT IDENTIFIER ::= { sunMib 4 }
		sunIcmp		OBJECT IDENTIFIER ::= { sunMib 5 }
		sunTcp		OBJECT IDENTIFIER ::= { sunMib 6 }
		sunUdp		OBJECT IDENTIFIER ::= { sunMib 7 }
		sunSnmp		OBJECT IDENTIFIER ::= { sunMib 11 }
		sunProcesses	OBJECT IDENTIFIER ::= { sunMib 12 }
		sunHostPerf	OBJECT IDENTIFIER ::= { sunMib 13 }

--  **********************************************************************
--  SUN SYSTEM GROUP
--  **********************************************************************

		agentDescr	OBJECT-TYPE
				SYNTAX	DisplayString (SIZE (0..255))
				ACCESS	read-only
				STATUS	mandatory
				DEFAULT	get-function	get_string
				DEFAULT	cookie "Sun Microsystems SNMP Agent"
				DEFAULT	view-mask	0x0F
				DEFAULT	write-mask	0x0A
			::= { sunSystem 1 }
				
		hostID	OBJECT-TYPE
				SYNTAX  OCTET STRING (SIZE (4))
				ACCESS  read-only
				STATUS	mandatory
				DEFAULT	view-mask	0x0F
				DEFAULT	write-mask	0x0A
			::= { sunSystem 2 }
				
		motd	OBJECT-TYPE
				SYNTAX	DisplayString (SIZE (0..255))
				ACCESS  read-only
				STATUS	mandatory
				DEFAULT	view-mask	0x0F
				DEFAULT	write-mask	0x0A
			::= { sunSystem 3 }
				
                unixTime OBJECT-TYPE
	                        SYNTAX  Counter
        	                ACCESS  read-only
                	        STATUS  mandatory
				DEFAULT	view-mask	0x0F
				DEFAULT	write-mask	0x0A
                       ::= { sunSystem 4 }

               -- the Sun Processes group

               -- the Sun Process table table

               sunProcessTable OBJECT-TYPE
                       SYNTAX  SEQUENCE OF psEntry
                       ACCESS  read-only
                       STATUS  mandatory
		       DEFAULT test-function	pstable_test
		       DEFAULT next-function	pstable_next
		       DEFAULT cookie		(char *)NULL
                       ::= { sunProcesses 1 }

               psEntry OBJECT-TYPE
                       SYNTAX  PsEntry
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { sunProcessTable 1 }

               PsEntry ::= SEQUENCE {
                   psProcessID
                       INTEGER,
                   psProcessName,
                       OCTET STRING
                   psProcessSize
                       INTEGER,
                   psProcessCpuTime
                       INTEGER,
                   psProcessState
                       OCTET STRING,
                   psProcessWaitChannel
                       OCTET STRING,
                   psProcessTTY
                       OCTET STRING,
                   psProcessUserName
                       OCTET STRING,
                   psProcessUserID
                       INTEGER,
                   psProcessName
                       OCTET STRING,
                   psProcessStatus
                       INTEGER
               }

               psProcessID OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 1 }

               psParentProcessID OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 2 }

               psProcessSize OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 3 }

               psProcessCpuTime OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 4 }

               psProcessState OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 5 }

               psProcessWaitChannel OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 6 }

               psProcessTTY OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 7 }

               psProcessUserName OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 8 }

               psProcessUserID OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 9 }

               psProcessName OBJECT-TYPE
                       SYNTAX  OCTET STRING
                       ACCESS  read-only
                       STATUS  mandatory
                       ::= { psEntry 10 }

               psProcessStatus OBJECT-TYPE
                       SYNTAX  INTEGER
                       ACCESS  read-write
                       STATUS  mandatory
		       DEFAULT get-function get_cookie
		       DEFAULT cookie (char *)0
                       ::= { psEntry 11 }

              -- the Sun Hostperf group

               rsCpuTime1 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 1 }

               rsCpuTime2 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 2 }

               rsCpuTime3 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 3 }

               rsCpuTime4 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 4 }

               rsDiskXfer1 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 5 }

               rsDiskXfer2 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 6 }

               rsDiskXfer3 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 7 }

               rsDiskXfer4 OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 8 }

               rsVPagesIn OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 9 }

               rsVPagesOut OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 10 }

               rsVSwapIn OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 11 }

               rsVSwapOut OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 12 }

               rsVIntr OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 13 }

               rsIfInPackets OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 14 }

               rsIfOutPackets OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 15 }

               rsIfInErrors OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 16 }

               rsIfOutErrors OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 17 }

               rsIfCollisions OBJECT-TYPE
                       SYNTAX  Counter
                       ACCESS  read-only
                       STATUS  mandatory
                       ::=  { sunHostPerf 18 }

               END
