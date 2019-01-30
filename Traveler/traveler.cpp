#include <stdio.h>
#include <android_native_app_glue.h>
#include <glesTools.h>
#include <GLES/gl.h>
#include <math.h>
#include "renderer.h"
#include "world.h"
#include "glControls.h"


gtWindow mainWindow;
Renderer ru;


//配置OpenGLES
bool initGLES(gtWindow *window){
	//开启纹理
	glEnable(GL_TEXTURE_2D);
	//开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepthf(1.0f);
	//开启混合模式
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//窗口初配置
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glViewport(0, 0, window->width, window->height);
	glViewport(0, 0, window->width, window->height);
	glPerspectivef(120.0f, float(window->width) / window->height, 1.0f, 100000.0f);
	glMatrixMode(GL_MODELVIEW);
	
	//lightting
	float la[4]={0, 0, 0, 1};
	float lc[4]={1, 1, 1, 1};
	float lp[4]={0, 0, 1, 0};
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, la);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lc);
	glLightfv(GL_LIGHT0, GL_POSITION, lp);
}
//创建纹理
void createTextures(int num, GLuint *texArray)
{
	glGenTextures(num, texArray);
}
//加载纹理
int loadTexture(GLuint texid, const char *path)
{
	GLint width, height, total_bytes;
	GLubyte bpp;
	GLubyte *pixels=NULL;
	FILE *pFile;
	
	if((pFile=fopen(path, "rb"))==NULL) return 0;
	fseek(pFile, 18, SEEK_SET);
	fread(&width, sizeof(width), 1, pFile);
	fread(&height, sizeof(height), 1, pFile);
	fseek(pFile, 2, SEEK_CUR);
	fread(&bpp, sizeof(bpp), 1, pFile);
	fseek(pFile, 54, SEEK_SET);
	
	if(bpp==24) total_bytes=(width*3+(4-width*3%4)%4)*height;
	else if(bpp==32) total_bytes=width*4*height;
	else return 0;
	if((pixels=(GLubyte *)malloc(total_bytes))==NULL) {
		fclose(pFile);
		return 0;
	}
	if(fread(pixels, total_bytes, 1, pFile)<=0) {
		free(pixels);
		fclose(pFile);
		return 0;
	}
	int t;
	for(int i=0; i<total_bytes-3; i+=3) {
		t=pixels[i];
		pixels[i]=pixels[i+2];
		pixels[i+2]=t;
	}
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	if(bpp==24) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	else glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	free(pixels);
	return 1;
}

////////////////MAIN
glc::button cmoveboard1(200, 180);
glc::button cbakground1(200, 180, 70);
glc::button cmoveboard2(200, 1100);
glc::button cbakground2(200, 1100, 60);
glc::Screen cscreen;
void android_main(struct android_app* state)
{
	glesInitWindow(&mainWindow, state);
	initGLES(&mainWindow);
	createTextures(1, textures);
	loadTexture(Tex::blockmap, "resources/glass.bmp");
	uni.init();
	ru.init(100);
	Player p;
	p.x=p.z=0;
	p.y=500;
	p.hdir=-90;
	
	float2 tdir;
	float3 tmove;
	float2 bBasePos={200, 180};
	cscreen.set(mainWindow.width, mainWindow.height);
	
	while (!glesShouldClose(&mainWindow))
	{
		//p.x=uni.star[0].x;//*
		//p.z=uni.star[0].z;//*
		
		glClear(GL_COLOR_BUFFER_BIT |	GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glRotatef(-90, 0,0,1);
		glRotatef(-p.hdir, 1, 0, 0);
		glRotatef(-p.wdir, 0, 1, 0);
		glTranslatef(-p.x, -p.y, -p.z);
		
		uni.flush();
		uni.draw();
		
		glDisable(GL_LIGHTING);
		glOrthoBegin( &mainWindow);
		ru.clear();
		cmoveboard1.draw(ru, 0.8, {1,0,1}, {0,0,1});
		cbakground1.draw(ru, 0.5, {1,1,1}, {1,1,1});
		cmoveboard2.draw(ru, 0.8, {1,1,0}, {0,1,0});
		cbakground2.draw(ru, 0.5, {1,1,1}, {1,1,1});
		ru.rendererUpdate();
		ru.rendering(GL_TRIANGLES);
		
		gtContact contact;
		//On moveboard Pressing
		/*moveboard.pressed();
		if(bakground.pressed())
		{mbFollow=true; ca=bakground.getcid();}
		if(ca!=-1)
		contact=getContactByID(ca);
		if(contact.id==-1)
		{mbFollow=false; ca=-1;}
		if(mbFollow)
		{xc=contact.x-mbbasepos.x; yc=contact.y-mbbasepos.y;}
		else
		{xc*=0.8; yc*=0.8;}
		if(xc>70) xc=70;
		if(xc<-70) xc=-70;
		if(yc>70) yc=70;
		if(yc<-70) yc=-70;
		moveboard.set(mbbasepos.x+xc, mbbasepos.y+yc);
		
		//On moveboard2 Pressing
		if(moveboard2.pressed()) {
			cc=moveboard2.getcid();
		}
		contact=getContactByID(cc);
		if((cc=contact.id)!=-1)
		{xc2=contact.x-mbbasepos.x;}
		else xc2*=0.8;
		if(xc2>60) xc2=60;
		if(xc2<-60) xc2=-60;
		moveboard2.set(mbbasepos.x+xc2, 1100);
		
		*/
		//On screen Pressing
		/*if(gt_isPtrue && gt_pcontact.id!=bakground.getcid() && gt_pcontact.id!=moveboard2.getcid() && cb==-1) {
			cb=gt_pcontact.id;
		}
		contact=getContactByID(cb);
		cb=contact.id;
		if(cb!=-1) {
			xpass=contact.x-contact.x0;
			ypass=contact.y-contact.y0;
		}else xpass=ypass=0;
		*/
		
		do {
			if(gt_isPtrue) {
				if(cbakground1.pressed(gt_pcontact)) {
					cmoveboard1.moveTo(gt_pcontact.x, gt_pcontact.y);
					break;
				}
				if(cbakground2.pressed(gt_pcontact)) {
					cmoveboard2.moveTo(gt_pcontact.x, cmoveboard2.p2.y-cmoveboard2.bsize);
					break;
				}
				if(cscreen.pressed(gt_pcontact)) {
					tdir.x=p.wdir; tdir.y=p.hdir;
					break;
				}
			}
		}while(false);
		
		if(cbakground1.fcid!=-1) {
			contact=getContactByID(cbakground1.fcid);
			tmove.x=contact.x-bBasePos.x;
			tmove.y=contact.y-bBasePos.y;
			if(tmove.x>70) tmove.x=70;
			if(tmove.x<-70) tmove.x=-70;
			if(tmove.y>70) tmove.y=70;
			if(tmove.y<-70) tmove.y=-70;
			//cmoveboard1.moveTo(bBasePos.x+tmove.x, bBasePos.y+tmove.y);
		}else {tmove.x*=0.8f; tmove.y*=0.8f;}
		cmoveboard1.moveTo(bBasePos.x+tmove.x, bBasePos.y+tmove.y);
		
		if(cbakground2.fcid!=-1) {
			contact=getContactByID(cbakground2.fcid);
			tmove.z=contact.x-bBasePos.x;
			if(tmove.z>70) tmove.z=70;
			if(tmove.z<-70) tmove.z=-70;
			//cmoveboard2.moveTo(bBasePos.x+tmove.z, cmoveboard2.p2.y-cmoveboard2.bsize);
		}else {tmove.z*=0.8f;}
		cmoveboard2.moveTo(bBasePos.x+tmove.z, cmoveboard2.p2.y-cmoveboard2.bsize);
		
		if(cscreen.fcid!=-1) {
			contact=getContactByID(cscreen.fcid);
			p.wdir=tdir.x-(contact.y-contact.y0)/5.0f;
			p.hdir=tdir.y+(contact.x-contact.x0)/5.0f;
		}
		do {
			if(gt_isRtrue) {
				if(cbakground1.released(gt_rcontact)) {
					//cmoveboard1.moveTo(bBasePos.x, bBasePos.y);
					break;
				}
				if(cbakground2.released(gt_rcontact)) {
					//cmoveboard2.moveTo(bBasePos.x, cmoveboard2.p2.y-cmoveboard2.bsize);
					break;
				}
				if(cscreen.released(gt_rcontact)) {
					tdir.x=p.wdir; tdir.y=p.hdir;
					break;
				}
			}
		}while(false);
		
		
		
		//使用三角函数移动
		//托盘左右移动
		p.x+=cos(p.wdir*M_PI/180.0)* (tmove.y/10);
		p.z-=sin(p.wdir*M_PI/180.0)* (tmove.y/10);
		//托盘上下移动
		p.x-= cos((p.wdir-90)*M_PI/180.0)* (tmove.x/10);
		p.z+= sin((p.wdir-90)*M_PI/180.0)* (tmove.x/10);
		p.y+=tmove.z/10.0;
		
		//p.wdir+=-ypass/5.0;
		//p.hdir+=xpass/5.0;
		
		glOrthoEnd();
		glEnable(GL_LIGHTING);
		
		glesPollEvents( &mainWindow);
	}
	
	glesTerminate(&mainWindow);
	//return 0;
}