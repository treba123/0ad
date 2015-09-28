/* Copyright (C) 2011 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "precompiled.h"
#include "NetLanDiscovery.h"
#include "lib/external_libraries/enet.h"
#include "ps/CLogger.h"
//#include "network/NetMessages.h"

#define PS_BROADCAST_PORT				0x5070
#define PS_BROADCAST_PORT2				0x5071

LanDiscoverClient *DiscoverClient = NULL;

LanDiscoverClient::LanDiscoverClient()
{
	ENetAddress address;
	address.host = ENET_HOST_ANY;
	address.port = PS_BROADCAST_PORT2;
	
	socket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
	enet_socket_set_option(socket, ENET_SOCKOPT_REUSEADDR, 1);
	enet_socket_set_option(socket, ENET_SOCKOPT_NONBLOCK, 1);
	enet_socket_bind(socket, &address);
	
	shutdown = 0;
	
	int err = pthread_create(&broadcastReplyThread, NULL, &RunThread, this);
	if(err)
	{
		LOGERROR(L"Cannot create listener thread.\n");
	}
}

LanDiscoverClient::~LanDiscoverClient()
{
	shutdown = 1;
	pthread_join(broadcastReplyThread,NULL);
	if (socket) enet_socket_destroy(socket);
	LOGERROR(L"Closed lan lobby");
}

int LanDiscoverClient::SendBroadcast(){
	
	ENetAddress address;
	address.host = ENET_HOST_BROADCAST;
	address.port = PS_BROADCAST_PORT;
	
	ENetSocket sendSocket = enet_socket_create(ENET_SOCKET_TYPE_DATAGRAM);
	enet_socket_set_option(sendSocket, ENET_SOCKOPT_BROADCAST, 1);
	
	//broadcast
	char* message = "Ping";
	
	ENetBuffer buf;
	buf.data = message;
	buf.dataLength = sizeof(message);
	
	int err = enet_socket_send(sendSocket, &address, &buf, 1);
	LOGERROR(L"Broadcast: %d\n",err);	
	
	enet_socket_destroy(sendSocket);

	return err;
}

void* LanDiscoverClient::RunThread(void* data)
{
	debug_SetThreadName("LanDiscoverListenerThread");

	static_cast<LanDiscoverClient*>(data)->ListenToBroadcastReplies();

	return NULL;
}

void LanDiscoverClient::ListenToBroadcastReplies()
{
	ENetAddress address;
	ENetBuffer buf;
	wchar_t buffer[1000];
	buf.data = buffer;
	buf.dataLength = sizeof(buffer);
	
	while (!shutdown)
	{
		//memset(buffer,0,sizeof(buffer));
		int received = enet_socket_receive(socket, &address, &buf, 1);
		if(received)
		{
			//CStrW bla = CStr(buffer).FromUTF8();
			
			
			char ip[15];
			enet_address_get_host_ip(&address,ip,15);
			
			Game game;
			game.name = CStrW(buffer);
			game.ip = CStr(ip).FromUTF8();
			game.state = CStrW(&buffer[wcslen(buffer)+1]);
			//wcslen(GetState().c_str()) * sizeof(wchar_t)
			
			
			LOGERROR(L"broadcast received message: %ls with length %d from ip: %ls",buffer,received,game.ip.c_str());
			LOGERROR(L"Name: %ls len: %d",game.name.c_str(),wcslen(buffer));
			LOGERROR(L"State: %ls len %d",game.state.c_str(),wcslen(&buffer[wcslen(buffer)+1]));
		}
		else if(received < 0){
			LOGERROR(L"Net server: could not receive from port");
		}
		usleep(50000);
	}
}

CScriptValRooted LanDiscoverClient::GetGameList(ScriptInterface& scriptInterface)
{
	CScriptValRooted resGameList;
	scriptInterface.Eval("([])", resGameList);
	for(std::vector<Game>::iterator it = gameList.begin(); it != gameList.end(); ++it)
	{
		CScriptValRooted game;
		scriptInterface.Eval("({})", game);

		scriptInterface.SetProperty(game.get(), "name", it->name);
		scriptInterface.SetProperty(game.get(), "ip", it->ip);
		scriptInterface.SetProperty(game.get(), "state", it->state);
		scriptInterface.SetProperty(game.get(), "players", it->players);

		scriptInterface.CallFunctionVoid(resGameList.get(), "push", game);
	}

	return resGameList;
}

/*CScriptValRooted LanDiscoverClient::GetGameList(ScriptInterface& scriptInterface)
{
	CScriptValRooted gameList;
	scriptInterface.Eval("([])", gameList);
	for(std::vector<const glooxwrapper::Tag*>::const_iterator it = m_GameList.begin(); it != m_GameList.end(); ++it)
	{
		CScriptValRooted game;
		scriptInterface.Eval("({})", game);

		const char* stats[] = { "name", "ip", "state", "nbp", "tnbp", "players", "mapName", "niceMapName", "mapSize", "mapType", "victoryCondition" };
		short stats_length = 11;
		for (short i = 0; i < stats_length; i++)
			scriptInterface.SetProperty(game.get(), stats[i], wstring_from_utf8((*it)->findAttribute(stats[i]).to_string()));

		scriptInterface.CallFunctionVoid(gameList.get(), "push", game);
	}

	return gameList;
}*/

/*
void* ListenToBroadcastReplies(void*)//LANBROADCAST
{
	debug_SetThreadName("ListenToBroadcastReplies");
	
	LOGERROR(L"Listening for replies");
	
	ENetAddress addHost;
	addHost.host = ENET_HOST_ANY;
	addHost.port = PS_BROADCAST_PORT+1;
	
	ENetHost* host = enet_host_create(&addHost,32,2,0,0);
	if (!host)
	{
		LOGERROR(L"An error occurred while trying to create an ENet server host.\n");
	}
	
	ENetEvent event;
	while (enet_host_service (host, & event, 1000) > 0)
	{
		LOGERROR(L"Got host event.\n");
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				LOGERROR(L"A new client connected from %x:%u.\n",event.peer -> address.host,event.peer -> address.port);
				//event.peer -> data = "Client information";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				LOGERROR(L"A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet -> dataLength,
				event.packet -> data,
				event.peer -> data,
				event.channelID);
				enet_packet_destroy (event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				LOGERROR(L"%s disconected.\n", event.peer -> data);
				event.peer -> data = NULL;
		}
	}
	
	enet_host_destroy(host);

	return NULL;
}
*/
