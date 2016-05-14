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
#include <vector>
#include <string>
#include "scriptinterface/ScriptInterface.h"

typedef struct Game {
   CStrW name;
   CStrW ip;
   CStrW state;
   CStrW players;
} Game;

class LanDiscoveryServer
{
	NONCOPYABLE(LanDiscoveryServerIntstance);
public:
	LanDiscoveryServer();
	virtual ~LanDiscoveryServer();
	
private:
	
	int socket;
	int shutdown;
	pthread_t broadcastListenerThread;
	
	static void* RunThread(void*);
	void ListenToBroadcast();
};

class LanDiscoveryClient
{
	NONCOPYABLE(LanDiscoveryClient);
public:
	LanDiscoveryClient();
	virtual ~LanDiscoveryClient();

	int SendBroadcast();
	/*CScriptValRooted GetGameList(ScriptInterface&);*/
	
	
private:
	
	int socket;
	int shutdown;
	std::vector<Game> gameList;
	pthread_t broadcastReplyThread;
	
	static void* RunThread(void*);
	void ListenToBroadcastReplies();
};

extern LanDiscoveryServer *LanDiscoveryServerIntstance;
extern LanDiscoveryClient *LanDiscoveryClientIntstance;
