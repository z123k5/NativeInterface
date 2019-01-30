#ifndef _GLCONTROLS_H_
#define _GLCONTROLS_H_


#include <glesTools.h>
#include "renderer.h"


//OpenGLES辅助库:控件
namespace glc
{
	class Screen
	{
		public:
		float2 p1, p2;
		int fcid;
		
		Screen(): fcid(-1) {}
		void set(int sizew, int sizeh) {
			p1.x=0; p1.y=0;
			p2.x=sizew; p2.y=sizeh;
		}
		bool pressed(gtContact c) {
			if(fcid!=-1) return false;
			if((c.x-p1.x)*(c.x-p2.x)<=0.0f)
			if((c.y-p1.y)*(c.y-p2.y)<=0.0f) {
				fcid=c.id;
				return true;
			}
			return false;
		}
		
		bool cntInside(gtContact c) {
			if((c.x-p1.x)*(c.x-p2.x)<=0.0f)
			if((c.y-p1.y)*(c.y-p2.y)<=0.0f)
			return true;
			return false;
		}
		
		bool released(gtContact c) {
			if(fcid==-1) return false;
			if(fcid==c.id) {
				fcid=-1;
				return true;
			}
			return false;
		}
	};
	class button//按钮
	{
	public:
		float2 p1, p2;//按钮是个矩形，由两个点构成
		int bsize;
		int fcid;//被按下的触点id
		
		button():fcid(-1) {}
		button(float x, float y, int size=40) {//按钮xy位置，按钮大小
			p1.x=x-size; p1.y=y-size;
			p2.x=x+size; p2.y=y+size;
			fcid=-1;
			bsize=size;
		}
		void set(float x, float y, int size=40) {
			p1.x=x-size; p1.y=y-size;
			p2.x=x+size; p2.y=y+size;
		}
		
		
		bool pressed(gtContact c) {
			if(fcid!=-1) return false;
			if((c.x-p1.x)*(c.x-p2.x)<=0.0f)
			if((c.y-p1.y)*(c.y-p2.y)<=0.0f) {
				fcid=c.id;
				return true;
			}
			return false;
		}
		
		bool cntInside(gtContact c) {
			if((c.x-p1.x)*(c.x-p2.x)<=0.0f)
			if((c.y-p1.y)*(c.y-p2.y)<=0.0f)
			return true;
			return false;
		}
		
		bool released(gtContact c) {
			if(fcid==-1) return false;
			if(fcid==c.id) {
				fcid=-1;
				return true;
			}
			return false;
		}
		
		//跟随触点
		bool follow(gtContact c) {
			
		}
		
		int getcid() {
			return fcid;
		}
		
		void move(int xadd, int yadd) {
			p1.x+=xadd;
			p2.x+=xadd;
			p1.y+=yadd;
			p2.y+=yadd;
		}
		
		void moveTo(int xto, int yto) {
			p1.x=xto-bsize;
			p2.x=xto+bsize;
			p1.y=yto-bsize;
			p2.y=yto+bsize;
		}
		
		void draw(Renderer &r, float alpha, float3 color1, float3 color2) {//把绘图数据传给渲染器，透明度，前景色，背景色
			float c[4];
			c[3]=alpha;
			if(fcid==-1) {
				c[0]=color1.x;
				c[1]=color1.y;
				c[2]=color1.z;
			}else{
				c[0]=color2.x;
				c[1]=color2.y;
				c[2]=color2.z;
			}
			
			r.color4f(c[0], c[1], c[2], c[3]); r.vertex2f(p1.x, p1.y);
			r.color4f(c[0], c[1], c[2], c[3]); r.vertex2f(p1.x, p2.y);
			r.color4f(c[0], c[1], c[2], c[3]); r.vertex2f(p2.x, p1.y);
			r.color4f(c[0], c[1], c[2], c[3]); r.vertex2f(p2.x, p2.y);
			r.color4f(c[0], c[1], c[2], c[3]); r.vertex2f(p2.x, p1.y);
			r.color4f(c[0], c[1], c[2], c[3]); r.vertex2f(p1.x, p2.y);
		}
	};
}



#endif