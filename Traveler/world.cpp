#include <math.h>
#include <GLES/gl.h>
#include "world.h"
#include "renderer.h"


const float G=6.63f; // pow 10^-34 !
Universe uni;
unsigned int textures[5];
Renderer path, path2, path3;
Renderer range;


////////////////Chunk
Chunk::Chunk() {
	renderer.init(36*2, true, true, true);
	updated=false;
	isin=false;
}
void Chunk::init() {
	//renderer.setTexture(Tex::blockmap, 128, 128);
	//renderer.setTextureFormat(128, 128);
	isin=true;
	updated=true;
	clear();
}
void Chunk::clear() {
	renderer.clear();
	numBlocks=0;
}
void Chunk::destroy() {
	isin=false;
}
void Chunk::updateRS() {
	float2 td1, td2, td3, td4;
	float3 cd1, cd2, cd3, cd4;
	float3 nd1, nd2, nd3, nd4, nd5, nd6;
	float3 v1, v2, v3, v4, v5, v6, v7, v8;
	renderer.clear();
	
	cd4=cd3=cd2=cd1={1, 1, 1};
	for (int i=0; i<numBlocks; i++) {
		if(blocks[i][0].ID==Bl::air)
		continue;
		
		float tex[8];
		renderer.getTexIndexPointer(blocks[i][0].ID-1, tex);
		td1={tex[0], tex[1]};
		td2={tex[2], tex[3]};
		td3={tex[4], tex[5]};
		td4={tex[6], tex[7]};
		
		nd1={0,-1,0};
		nd2={0,1,0};
		nd3={-1,0,0};
		nd4={1,0,0};
		nd5={0,0,-1};
		nd6={0,0,1};
		Block &t=blocks[i][0];
		/*v1={float(cx*16+t.x+t.x1), float(t.y+t.y1), float(cz*16+t.z+t.z2)};
		v2={float(cx*16+t.x+t.x2), float(t.y+t.y1), float(cz*16+t.z+t.z2)};
		v3={float(cx*16+t.x+t.x2), float(t.y+t.y1), float(cz*16+t.z+t.z1)};
		v4={float(cx*16+t.x+t.x1), float(t.y+t.y1), float(cz*16+t.z+t.z1)};
		v5={float(cx*16+t.x+t.x1), float(t.y+t.y2), float(cz*16+t.z+t.z2)};
		v6={float(cx*16+t.x+t.x2), float(t.y+t.y2), float(cz*16+t.z+t.z2)};
		v7={float(cx*16+t.x+t.x2), float(t.y+t.y2), float(cz*16+t.z+t.z1)};
		v8={float(cx*16+t.x+t.x1), float(t.y+t.y2), float(cz*16+t.z+t.z1)};
		
		//bottom
		renderer.texture2fq(td1, td2, td3, td4);
		renderer.color3fq(cd1, cd2, cd3, cd4);
		renderer.normal3fq(nd1, nd1, nd1, nd1);
		renderer.vertex3fq(v4, v3, v2, v1);
		//top
		renderer.texture2fq(td1, td2, td3, td4);
		renderer.color3fq(cd1, cd2, cd3, cd4);
		renderer.normal3fq(nd2, nd2, nd2, nd2);
		renderer.vertex3fq(v5, v6, v7, v8);
		//left
		renderer.texture2fq(td1, td2, td3, td4);
		renderer.color3fq(cd1, cd2, cd3, cd4);
		renderer.normal3fq(nd3, nd3, nd3, nd3);
		renderer.vertex3fq(v4, v1, v5, v8);
		//right
		renderer.texture2fq(td1, td2, td3, td4);
		renderer.color3fq(cd1, cd2, cd3, cd4);
		renderer.normal3fq(nd4, nd4, nd4, nd4);
		renderer.vertex3fq(v2, v3, v7, v6);
		//back
		renderer.texture2fq(td1, td2, td3, td4);
		renderer.color3fq(cd1, cd2, cd3, cd4);
		renderer.normal3fq(nd5, nd5, nd5, nd5);
		renderer.vertex3fq(v3, v4, v8, v7);
		//front
		renderer.texture2fq(td1, td2, td3, td4);
		renderer.color3fq(cd1, cd2, cd3, cd4);
		renderer.normal3fq(nd6, nd6, nd6, nd6);
		renderer.vertex3fq(v1, v2, v6, v5);*/
	}
	renderer.rendererUpdate();
}


double atr(double a) {
	return a*3.1415926/180.0;
}
double rta(double a) {
	return a/3.1415926*180.0;
}
double abs(double a) {
	return a<0? -a:a;
}

////////////////Body
void Body::init(float px, float py, float pz) {
	x=px; y=py; z=pz;
	r.init(4800, true, true, true);
	r.setTexture(Tex::blockmap, 256, 256);
	r.setTextureFormat(16, 16);
	
	float3 v1, v2, v3, v4;
	int w=10;
	float t[8];
	for(float i=-90; i<90; i+=w)
	for(float j=0; j<360; j+=w) {
		v1.x= cos(atr(j-w))*abs(cos(atr(i+w)))*R;
		v1.y=sin(atr(i+w))*R;
		v1.z= sin(atr(j-w))*abs(cos(atr(i+w)))*R;
		v2.x= cos(atr(j))*abs(cos(atr(i+w)))*R;
		v2.y=sin(atr(i+w))*R;
		v2.z= sin(atr(j))*abs(cos(atr(i+w)))*R;
		v3.x= cos(atr(j))*abs(cos(atr(i)))*R;
		v3.y=sin(atr(i))*R;
		v3.z= sin(atr(j))*abs(cos(atr(i)))*R;
		v4.x= cos(atr(j-w))*abs(cos(atr(i)))*R;
		v4.y=sin(atr(i))*R;
		v4.z= sin(atr(j-w))*abs(cos(atr(i)))*R;
		
		r.getTexIndexPointer(tex, t);
		r.texture2fq({t[0],t[1]}, {t[2],t[3]}, {t[4],t[5]}, {t[6],t[7]});
		r.color3fq({1,1,1}, {1,1,1}, {1,1,1}, {1,1,1});
		r.normal3fq(v1, v2, v3, v4);
		r.vertex3fq(v1, v2, v3, v4);
	}
	r.rendererUpdate();
}

void Body::set(float quality, float radius, float gradius) {
	m=quality;
	R=radius;
	rg=gradius;
}

void Body::meet(Body &a) {
	float dst, wd, hd, xx, yx, zx;
	float F, simi;
	dst=sqrt(pow(a.x-x,2) + pow(a.y-y,2) + pow(a.z-z,2));
	
	//超出引力范围则不考虑
	if(dst>a.rg+rg) return;
	
	//求吸引力
	//相撞则不计算吸引力
	if(dst>a.R+R) {
	F=G*a.m*m/(dst*dst);
	simi=F/dst;//相似比
	// F/dst=Fx/x
	// Fx=x* F/dst = x* simi;
	
	//求两天体加速度
	xa+=(a.x-x)*simi/m;
	ya+=(a.y-y)*simi/m;
	za+=(a.z-z)*simi/m;
	
	a.xa+=(x-a.x)*simi/a.m;
	a.ya+=(y-a.y)*simi/a.m;
	a.za+=(z-a.z)*simi/a.m;
	}
	/*if(a.x!=x)
	xs+=abs(a.x-x)/(a.x-x)* abs(a.x-x)/a.rg*a.g/q;
	if(a.y!=y)
	ys+=abs(a.y-y)/(a.y-y)* abs(a.y-y)/a.rg*a.g/q;
	if(a.z!=z)
	zs+=abs(a.z-z)/(a.z-z)* abs(a.z-z)/a.rg*a.g/q;
	//吸引对方
	if(x-a.x!=0)
	a.xs+=abs(x-a.x)/(x-a.x)* abs(x-a.x)/gr*g/a.q;
	if(y-a.y!=0)
	a.ys+=abs(y-a.y)/(y-a.y)* abs(y-a.y)/gr*g/a.q;
	if(z-a.z!=0)
	a.zs+=abs(z-a.z)/(z-a.z)* abs(z-a.z)/gr*g/a.q;
	*/
	//碰撞
	////s⁴=t²(xs²+ys²+zs²t²)
	//s²=√(
	
	
	//s=√(√(x²t²+z²t²)+y²t²)
	//√s=√(t²(x²+z²))+y²t²
	//√s=t√(x²+z²)+y²t²
	//y²t²+√(x²+z²)t-√s=0
	
	//(-4y²√s-(x²+z²))/4y²
	
	/*double distn;
	distn=sqrt(pow(sqrt(pow(x+xs-a.x, 2)+ pow(z+zs-a.z, 2)), 2)+pow(y+ys-a.y, 2));
	if(distn<ra+a.ra) {
		x+=xs; y+=ys; z+=zs;
		
	}
	*/
	
	
	/*float xt, yt, zt;
	xt=x+xs; yt=y+ys; zt=z+zs;
	if(x+r+xs<dist
	*/
}
void Body::flush() {
	xs+=xa; ys+=ya; zs+=za;
	x+=xs; y+=ys; z+=zs;
	xa=ya=za=0.0f;
}
void Body::draw() {
	glPushMatrix();
	glRotatef(-hdir, 1, 0, 0);
	glRotatef(-wdir, 0, 1, 0);
	glTranslatef(x, y, z);
	r.bind();
	r.rendering(GL_TRIANGLES);
	glPopMatrix();
}

////////////////University
void Universe::init() {
	star[0].tex=(1<<16);
	star[0].R=50;
	star[0].m=1000;
	star[0].rg=2000;
	star[0].init(-1000, 0, 0);
	star[0].xs=3;
	star[0].zs=0;
	
	star[1].tex=(3<<16);
	star[1].R=50;
	star[1].m=1000;
	star[1].rg=2000;
	star[1].init(0, 0, -300);
	star[1].xs=-2;
	star[1].zs=-0;
	
	star[2].tex=(3<<16);
	star[2].R=50;
	star[2].m=1000;
	star[2].rg=2000;
	star[2].init(0, 0, 300);
	star[2].xs=2;
	star[2].zs=0;
	/*
	star[0].tex=(1<<16);
	star[0].R=50;
	star[0].m=1000;
	star[0].rg=10000;
	star[0].init(0*400, 0, 1.73*2/3*400);
	star[0].xs=2.5;
	star[0].zs=0;
	
	star[1].tex=(3<<16);
	star[1].R=50;
	star[1].m=1000;
	star[1].rg=10000;
	star[1].init(1*400, 0, -1.73/3*400);
	star[1].xs=-2.5*0.5;
	star[1].zs=-2.5*0.5*1.73;
	
	star[2].tex=(3<<16);
	star[2].R=50;
	star[2].m=1000;
	star[2].rg=10000;
	star[2].init(-1*400, 0, -1.73/3*400);
	star[2].xs=-2.5*0.5;
	star[2].zs=2.5*0.5*1.73;
	*/
	path.init(1000, false, true);
	path2.init(1000, false, true);
	path3.init(1000, false, true);
	range.init(1000, false, true);
	glLineWidth(5);
}
void Universe::flush() {
	for(int i=0; i<3; i++)
	for(int j=0; j<3; j++)
	if(i!=j) star[i].meet(star[j]);
	for(int i=0; i<3; i++)
		star[i].flush();
	
	static int vtxnum=0;
	if(vtxnum>=1000) {
	path.clear();
	path2.clear();
	path3.clear();
	vtxnum=0;
	}
	path.color3f(1, 0, 0);
	path.vertex3f(star[0].x, star[0].y, star[0].z);
	path2.color3f(0, 1, 0);
	path2.vertex3f(star[1].x, star[1].y, star[1].z);
	path3.color3f(0, 0, 1);
	path3.vertex3f(star[2].x, star[2].y, star[2].z);
	path.rendererUpdate();
	path2.rendererUpdate();
	path3.rendererUpdate();
	vtxnum++;
}
void Universe::draw() {
	glDisable(GL_LIGHTING);
	
	path.bind();
	path.rendering(GL_LINE_STRIP);
	path2.bind();
	path2.rendering(GL_LINE_STRIP);
	path3.bind();
	path3.rendering(GL_LINE_STRIP);
	range.clear();
	for(float i=0; i<360; i+=10) {
		range.color3f(1, 1, 1);
		range.vertex3f(star[0].x+ sin(atr(i))* star[0].rg, 0, star[0].z+ cos(atr(i))* star[0].rg);
		range.color3f(1, 1, 1);
		range.vertex3f(star[0].x+ sin(atr(i+10))* star[0].rg, 0, star[0].z+ cos(atr(i+10))* star[0].rg);
		
		range.color3f(1, 1, 1);
		range.vertex3f(star[1].x+ sin(atr(i))* star[1].rg, 0, star[1].z+ cos(atr(i))* star[1].rg);
		range.color3f(1, 1, 1);
		range.vertex3f(star[1].x+ sin(atr(i+10))* star[1].rg, 0, star[1].z+ cos(atr(i+10))* star[1].rg);
		
		range.color3f(1, 1, 1);
		range.vertex3f(star[2].x+ sin(atr(i))* star[2].rg, 0, star[2].z+ cos(atr(i))* star[2].rg);
		range.color3f(1, 1, 1);
		range.vertex3f(star[2].x+ sin(atr(i+10))* star[2].rg, 0, star[2].z+ cos(atr(i+10))* star[2].rg);
		
		range.rendererUpdate();
	}
	glLineWidth(2);
	range.rendering(GL_LINES);
	glLineWidth(5);
	glEnable(GL_LIGHTING);
	
	for(int i=0; i<3; i++)
	star[i].draw();
}

////////////////Player
void Player::rendering() {
	//init player model
	float3 cd1, cd2, cd3, cd4;
	float3 v1, v2, v3, v4, v5, v6, v7, v8;
	float3 nd1, nd2, nd3, nd4, nd5, nd6;
	cd4=cd3=cd2=cd1={1, 1, 1};
	
	v1={x+x1, y+y1, z+z2};
	v2={x+x2, y+y1, z+z2};
	v3={x+x2, y+y1, z+z1};
	v4={x+x1, y+y1, z+z1};
	v5={x+x1, y+y2, z+z2};
	v6={x+x2, y+y2, z+z2};
	v7={x+x2, y+y2, z+z1};
	v8={x+x1, y+y2, z+z1};
	
	nd1={0,-1,0};
	nd2={0,1,0};
	nd3={-1,0,0};
	nd4={1,0,0};
	nd5={0,0,-1};
	nd6={0,0,1};
	
	r.clear();
	//bottom
	r.color3fq(cd1, cd2, cd3, cd4);
	r.normal3fq(nd1, nd1, nd1, nd1);
	r.vertex3fq(v4, v3, v2, v1);
	//top
	r.color3fq(cd1, cd2, cd3, cd4);
	r.normal3fq(nd2, nd2, nd2, nd2);
	r.vertex3fq(v5, v6, v7, v8);
	//left
	r.color3fq(cd1, cd2, cd3, cd4);
	r.normal3fq(nd3, nd3, nd3, nd3);
	r.vertex3fq(v4, v1, v5, v8);
	//right
	r.color3fq(cd1, cd2, cd3, cd4);
	r.normal3fq(nd4, nd4, nd4, nd4);
	r.vertex3fq(v2, v3, v7, v6);
	//back
	r.color3fq(cd1, cd2, cd3, cd4);
	r.normal3fq(nd5, nd5, nd5, nd5);
	r.vertex3fq(v3, v4, v8, v7);
	//front
	r.color3fq(cd1, cd2, cd3, cd4);
	r.normal3fq(nd6, nd6, nd6, nd6);
	r.vertex3fq(v1, v2, v6, v5);
	
	r.rendererUpdate();
	r.rendering(GL_TRIANGLES);
}