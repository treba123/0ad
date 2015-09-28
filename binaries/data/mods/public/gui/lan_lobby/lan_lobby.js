var lanDiscovery;

function init()
{
	//Engine.GetGUIObjectByName("pageConnecting").hidden = false;
	//setTimeout(getLanGames,0);
	//Engine.GetGUIObjectByName("connectionStatus").caption = Engine.DiscoverLanGames();
	
	return;
	
	Engine.StartLanGameDiscovery();
	lanDiscovery = true;
	Engine.SendLanDiscoveryBroadcast();
}

function stopLanDiscovery()
{
	return;
	lanDiscovery = false;
	Engine.StopLanGameDiscovery()
}

function getLanGames()
{
	return;
	
	
	if (!lanDiscovery) return;
	
	var gameList = Engine.GetLanDiscoveryGameList();
	
	var list_name = [];
	var list_ip = [];
	
	var c = 0;
	for each (var g in gameList)
	{
		if(!filterGame(g))
		{
			// Highlight games 'waiting' for this player, otherwise display as green
			var name = (g.state != 'waiting') ? '[color="0 125 0"]' + g.name + '[/color]' : '[color="orange"]' + g.name + '[/color]';
			list_name.push(name);
			list_ip.push(g.ip);
		}
		c++;
	}
	
	//Engine.GetGUIObjectByName("connectionStatus").caption = Engine.GetLanDiscoveryGameList();
	return;
}

/*function updateGameList()
{
	var gamesBox = Engine.GetGUIObjectByName("gamesBox");
	var gameList = Engine.GetGameList();
	// Store the game whole game list data so that we can access it later
	// to update the game info panel.
	g_GameList = gameList;

	// Sort the list of games to that games 'waiting' are displayed at the top
	g_GameList.sort(function (a,b) {
		return a.state == 'waiting' ? -1 : b.state == 'waiting' ? +1 : 0;
	});

	var list_name = [];
	var list_ip = [];
	var list_mapName = [];
	var list_mapSize = [];
	var list_mapType = [];
	var list_nPlayers = [];
	var list = [];
	var list_data = [];

	var c = 0;
	for each (var g in gameList)
	{
		if(!filterGame(g))
		{
			// Highlight games 'waiting' for this player, otherwise display as green
			var name = (g.state != 'waiting') ? '[color="0 125 0"]' + g.name + '[/color]' : '[color="orange"]' + g.name + '[/color]';
			list_name.push(name);
			list_ip.push(g.ip);
			list_mapName.push(g.niceMapName);
			list_mapSize.push(g.mapSize.split("(")[0]);
			list_mapType.push(toTitleCase(g.mapType));
			list_nPlayers.push(g.nbp + "/" +g.tnbp);
			list.push(g.name);
			list_data.push(c);
		}
		c++;
	}

	gamesBox.list_name = list_name;
	//gamesBox.list_ip = list_ip;
	gamesBox.list_mapName = list_mapName;
	gamesBox.list_mapSize = list_mapSize;
	gamesBox.list_mapType = list_mapType;
	gamesBox.list_nPlayers = list_nPlayers;
	gamesBox.list = list;
	gamesBox.list_data = list_data;

	if (gamesBox.selected >= gamesBox.list_name.length)
		gamesBox.selected = -1;

	// Update info box about the game currently selected
	updateGameSelection();
}*/
