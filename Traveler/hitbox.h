#pragma once


//碰撞检测
class AABB
{
public:
	double x1, y1, z1, x2, y2, z2;
	double x, y, z;

	void set(double f1, double f2, double f3, double f4, double f5, double f6);
	void get(double *f);
	void getPos(double *f);
	void move(double xa, double ya, double za);
	void moveTo(double xto, double yto, double zto);
	bool hitXaxis(AABB aabb);
	bool hitYaxis(AABB aabb);
	bool hitZaxis(AABB aabb);
	bool collision(AABB aabb);
	double speedInXaxis(AABB aabb, double maxSpeed);
	double speedInYaxis(AABB aabb, double maxSpeed);
	double speedInZaxis(AABB aabb, double maxSpeed);
};