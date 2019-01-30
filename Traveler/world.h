#pragma once
#include "renderer.h"
#include "hitbox.h"


////////////////resources
namespace Bl
{
	enum {
		air,
		wall,
		wood,
		box
	};
}
namespace Tex
{
	enum {
		blockmap,
		font
	};
}
////////////////
////////////////Block
class Block
{
public:
	char ID;
	char lon, lat, dep;
};

////////////////Chunk
class Chunk
{
public:
	int cx, cz;
	bool updated;
	bool isin;
	int numBlocks;
	Renderer renderer;
	Block blocks[16][256];
	
	
	Chunk();
	void init();
	void clear();
	void destroy();
	void updateRS();
};

////////////////Body
class Body
{
	public:
	float x, y, z;//position
	float wdir, hdir;//direction
	float xs, ys, zs;//speed
	float xa, ya, za;//add speed
	float m;//quality
	float R;//bosy radius
	float rg;//gravity range
	
	int tex;
	Renderer r;
	
	Body() {
		x=y=z=0.0f;
		wdir=hdir=0.0f;
		xs=ys=zs=0.0f;
		xa=ya=za=0.0f;
		m=0.0f;
	}
	void init(float px, float py, float pz);
	void set(float quality, float radius, float gradius);
	void meet(Body &a);
	void flush();
	void draw();
};

////////////////University
class Universe
{
	public:
	Body star[5];
	
	void init();
	void flush();
	void draw();
};

////////////////Player
class Player : public AABB
{
	public:
	double wdir, hdir;
	double gravity, xspeed, yspeed, zspeed, maxxspeed, maxyspeed, maxzspeed;
	double xfriction, yfriction, zfriction;
	bool onGround, inWall;
	Renderer r;
	
	Player():wdir(0), hdir(0),
		gravity(0), xspeed(0), yspeed(0), zspeed(0),  maxxspeed(1), maxyspeed(1), maxzspeed(1), xfriction(1), yfriction(1), zfriction(1),
	onGround(0), inWall(0) {}
	void rendering();
};

extern unsigned int textures[5];
extern Universe uni;
extern Renderer path;
extern Renderer range;