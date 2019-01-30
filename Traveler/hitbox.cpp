#include "hitbox.h"


void AABB::set(double f1, double f2, double f3, double f4, double f5, double f6) {
	/*if(f1 > f4) swap(f1, f4);
	if(f2 > f5) swap(f2, f5);
	if(f3 > f6) swap(f3, f6);*/
	x1 = f1;
	y1 = f2;
	z1 = f3;
	x2 = f4;
	y2 = f5;
	z2 = f6;
}

void AABB::get(double *f) {
	*f     = x1;
	*(f+1) = y1;
	*(f+2) = z1;
	*(f+3) = x2;
	*(f+4) = y2;
	*(f+5) = z2;
}

void AABB::getPos(double *f) {
	*f     = x;
	*(f+1) = y;
	*(f+2) = z;
}

void AABB::move(double xa, double ya, double za) {
	x += xa;
	y += ya;
	z += za;
}

void AABB::moveTo(double xto, double yto, double zto) {
	x = xto;
	y = yto;
	z = zto;
}

bool AABB::hitXaxis(AABB aabb) {
	if(x+x2<=aabb.x+aabb.x1 || x+x1>=aabb.x+aabb.x2) return false;
	return true;
}

bool AABB::hitYaxis(AABB aabb) {
	if(y+y2<=aabb.y+aabb.y1 || y+y1>=aabb.y+aabb.y2) return false;
	return true;
}

bool AABB::hitZaxis(AABB aabb) {
	if(z+z2<=aabb.z+aabb.z1 || z+z1>=aabb.z+aabb.z2) return false;
	return true;
}

bool AABB::collision(AABB aabb) {
	if(hitXaxis(aabb))
		if(hitYaxis(aabb))
			if(hitZaxis(aabb))
				return true;
	return false;
}

double AABB::speedInXaxis(AABB aabb, double maxSpeed) {
	if(hitYaxis(aabb)) {
		if(hitZaxis(aabb)) {
			if(x+x2<aabb.x+aabb.x2 && x+x2+maxSpeed>aabb.x+aabb.x1)
				return aabb.x+aabb.x1-(x+x2);
			if(x+x1>aabb.x+aabb.x1 && x+x1+maxSpeed<aabb.x+aabb.x2)
				return aabb.x+aabb.x2-(x+x1);
		}
	}
	return maxSpeed;
}

double AABB::speedInYaxis(AABB aabb, double maxSpeed) {
	if(hitXaxis(aabb)) {
		if(hitZaxis(aabb)) {
			if(y+y2<aabb.y+aabb.y2 && y+y2+maxSpeed>aabb.y+aabb.y1)
				return aabb.y+aabb.y1-(y+y2);
			if(y+y1>aabb.y+aabb.y1 && y+y1+maxSpeed<aabb.y+aabb.y2)
				return aabb.y+aabb.y2-(y+y1);
		}
	}
	return maxSpeed;
}

double AABB::speedInZaxis(AABB aabb, double maxSpeed) {
	if(hitYaxis(aabb)) {
		if(hitXaxis(aabb)) {
			if(z+z2<aabb.z+aabb.z2 && z+z2+maxSpeed>aabb.z+aabb.z1)
				return aabb.z+aabb.z1-(z+z2);
			if(z+z1>aabb.z+aabb.z1 && z+z1+maxSpeed<aabb.z+aabb.z2)
				return aabb.z+aabb.z2-(z+z1);
		}
	}
	return maxSpeed;
}