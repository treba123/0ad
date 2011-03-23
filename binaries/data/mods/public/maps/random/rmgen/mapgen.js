var g_Map;

var g_Environment = {
	SkySet: "default",
	SunColour: {r: 1.5, g: 1.5, b: 1.5, a: 0},
	SunElevation: 0.785398,
	SunRotation: 5.49779,
	TerrainAmbientColour: {r: 0.313726, g: 0.376471, b: 0.521569, a: 0},
	UnitsAmbientColour: {r: 0.501961, g: 0.501961, b: 0.501961, a: 0},
	Water: {
		WaterBody: {
			Type: "default",
			Colour: {r: 0.3, g: 0.35, b: 0.7, a: 0},
			Height: 5,
			Shininess: 150,
			Waviness: 8,
			Murkiness: 0.45,
			Tint: {r: 0.28, g: 0.3, b: 0.59, a: 0},
			ReflectionTint: {r: 0.28, g: 0.3, b: 0.59, a: 0},
			ReflectionTintStrength: 0.0
		}
	}
};

var g_Camera = {
	Position: {x: 100, y: 150, z: -100},
	Rotation: 0,
	Declination: 0.523599
};

/////////////////////////////////////////////////////////////////////////////////////

function InitMap()
{
	if (g_MapSettings === undefined || g_MapSettings == {})
	{	// If settings missing, warn and use some defaults
		warn("InitMapGen: settings missing");
		g_MapSettings = {
			Size : 13,
			BaseTerrain: "grass1_spring",
			BaseHeight: 0,
			PlayerData : [ {}, {} ]
		};
	}
	
	// Create new map
	log("Creating new map...");
	var terrain = createTerrain(g_MapSettings.BaseTerrain);
	
	g_Map = new Map(g_MapSettings.Size * TILES_PER_PATCH, g_MapSettings.BaseHeight);
	g_Map.initTerrain(terrain);
}

function ExportMap()
{	// Wrapper for engine function
	log("Saving map...");
	
	// Get necessary data from map
	var data = g_Map.getMapData();
	
	// Add environment and camera settings
	g_Environment.Water.WaterBody.Height = SEA_LEVEL - 0.1;
	data.Environment = g_Environment;
	data.Camera = g_Camera;
	
	RMS.ExportMap(data);
}
