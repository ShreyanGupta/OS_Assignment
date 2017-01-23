#include "labs/shell.h"
#include "labs/vgatext.h"
#include "labs/deque.h"

typedef long long ll;
#include <string.h>
#include <stdlib.h>
//
// initialize shellstate
//
void shell_init(shellstate_t& state){
    state.curr_pos = 0;
    state.end_pos = 0;
    state.execute = false;
    state.num_key = 0;
}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//
void shell_update(uint8_t scankey, shellstate_t& s){
    hoh_debug("Got: "<<unsigned(scankey));
    ++s.num_key;
    bool write_key_press = false;
    char write_key;
    switch(scankey){
        case 0x01 : // esc
            break;
        case 0x0e : // backspace
        	if(s.curr_pos == 0) break;
        	for(int i=s.curr_pos-1; i < s.end_pos; ++i) s.curr_cmd[i] = s.curr_cmd[i+1];
	        // memcpy(s.curr_cmd+s.curr_pos-1,s.curr_cmd+s.curr_pos,s.end_pos-s.curr_pos);
	    	s.curr_pos--;
	    	s.end_pos--;
            break;
        case 0x0f : // tab
            break;
        case 0x1c : // enter
            s.execute = true;
            break;
        case 0x1d : // ctrl
            break;
        case 0x2a : // shift
            break;
        case 0x48 :	// up 	 
        	break;
        case 0x50 : // down	 
        	break;
        case 0x4b : // left	 
        	--s.curr_pos;
        	if(s.curr_pos < 0) s.curr_pos = 0;
        	break;
        case 0x4d : // right
        	++s.curr_pos;
        	if(s.curr_pos > s.end_pos) s.curr_pos = s.end_pos;
        	break;

        case 0x02 : write_key_press=true; write_key = '1';	break;
        case 0x03 : write_key_press=true; write_key = '2';	break;
        case 0x04 : write_key_press=true; write_key = '3';	break;
        case 0x05 : write_key_press=true; write_key = '4';	break;
        case 0x06 : write_key_press=true; write_key = '5';	break;
        case 0x07 : write_key_press=true; write_key = '6';	break;
        case 0x08 : write_key_press=true; write_key = '7';	break;
        case 0x09 : write_key_press=true; write_key = '8';	break;
        case 0x0a : write_key_press=true; write_key = '9';	break;
        case 0x0b : write_key_press=true; write_key = '0';	break;
        case 0x0c : write_key_press=true; write_key = '-';	break;
        case 0x0d : write_key_press=true; write_key = '=';	break;
        case 0x10 : write_key_press=true; write_key = 'q';	break;
        case 0x11 : write_key_press=true; write_key = 'w';	break;
        case 0x12 : write_key_press=true; write_key = 'e';	break;
        case 0x13 : write_key_press=true; write_key = 'r';	break;
        case 0x14 : write_key_press=true; write_key = 't';	break;
        case 0x15 : write_key_press=true; write_key = 'y';	break;
        case 0x16 : write_key_press=true; write_key = 'u';	break;
        case 0x17 : write_key_press=true; write_key = 'i';	break;
        case 0x18 : write_key_press=true; write_key = 'o';	break;
        case 0x19 : write_key_press=true; write_key = 'p';	break;
        case 0x1a : write_key_press=true; write_key = '[';	break;
        case 0x1b : write_key_press=true; write_key = ']';	break;
        case 0x1e : write_key_press=true; write_key = 'a';	break;
        case 0x1f : write_key_press=true; write_key = 's';	break;
        case 0x20 : write_key_press=true; write_key = 'd';	break;
        case 0x21 : write_key_press=true; write_key = 'f';	break;
        case 0x22 : write_key_press=true; write_key = 'g';	break;
        case 0x23 : write_key_press=true; write_key = 'h';	break;
        case 0x24 : write_key_press=true; write_key = 'j';	break;
        case 0x25 : write_key_press=true; write_key = 'k';	break;
        case 0x26 : write_key_press=true; write_key = 'l';	break;
        case 0x27 : write_key_press=true; write_key = ';';	break;
        case 0x28 : write_key_press=true; write_key = '\'';break;
        case 0x29 : write_key_press=true; write_key = ' ';	break;
        case 0x2c : write_key_press=true; write_key = 'z';	break;
        case 0x2d : write_key_press=true; write_key = 'x';	break;
        case 0x2e : write_key_press=true; write_key = 'c';	break;
        case 0x2f : write_key_press=true; write_key = 'v';	break;
        case 0x30 : write_key_press=true; write_key = 'b';	break;
        case 0x31 : write_key_press=true; write_key = 'n';	break;
        case 0x32 : write_key_press=true; write_key = 'm';	break;
        case 0x33 : write_key_press=true; write_key = ',';	break;
        case 0x34 : write_key_press=true; write_key = '.';	break;
        case 0x35 : write_key_press=true; write_key = '/';	break;
        case 0x39 : write_key_press=true; write_key = ' ';	break;
    }
    if (write_key_press){
    	for(int i=s.end_pos; i > s.curr_pos; --i) s.curr_cmd[i] = s.curr_cmd[i-1];
    	// memcpy(s.curr_cmd+s.curr_pos+1,s.curr_cmd+s.curr_pos,s.end_pos-s.curr_pos);
    	s.curr_cmd[s.curr_pos] = write_key;
    	s.curr_pos++;
    	s.end_pos++;
    }
    s.curr_cmd[s.end_pos] = '\0';
    hoh_debug("Current pos: " << s.curr_pos << " end pos " << s.end_pos << " line " << s.curr_cmd);
    // for(int i=0; i<s.end_pos; ++i) hoh_debug(s.curr_cmd[i]);
}


//
// do computation
//
void shell_step(shellstate_t& s){
    if(!s.execute) return;
    int x = 0;
    while (s.curr_cmd[x] != ' ')
	x++;
    char blah[x+1];
    for (int i = 0; i < x; i++)
	blah[i] = s.curr_cmd[i];
    blah[x] = '\0';
    char echo[4] = "echo";
    char fibbo[5] = "fibbo"
    char facto[5] = "facto";
    char clear[5] = "clear";
    if (strcmp(blah,clear) == 0)
    {
	
    }
    else
    {
	// find next space!
	x++;
	int y = x;
	while (s.curr_cmd[x] != '\0')
	    x++;
	if (strcmp(blah,echo) == 0)
	{
	    for (int i = y; i <= x; i++)
		s.output[i-y] = s.cur_cmd[i];
	}
	else if (s.curr_cmd[x] == '-')
	{
	    s.output = "ERROR! Negative Input"
	}
	else
	{
	// convert str to long long.
	    ll input = 0;
	    for (int i = y; i < x; i++)
		input = 10*input + s.curr_cmd[i];
	    ll ans;
	    if (strcmp(blah,fibbo) == 0)
		ans = fibbo(input);
	    else
		ans = facto(input);
	    // update output! TODO
	}
    }
    s.execute = false;
  //
  //one way:
  // if a function is enabled in stateinout
  //   call that function( with arguments stored in stateinout) ;
//stateinout.args[0] = 5;
//stateinout.args[1] = 5;
  //
}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
    render.num_key = shell.num_key;
    // TODO : curr_cmd is the next line, and output is its next.
    // add these to render.Lines.
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
	return (a.num_key == b.num_key && a.curr_pos == b.curr_pos);
	// TODO : add equality of Deque here. a.Deque == b.Deque
}

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);

//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){


	//const char *p = "What is your problem?";
	// cursor had bg=7, fg=0
	// all others have white fg, black bg.
	for (int i = 0; i < 23; i++)
		drawtext(0,i,state.Lines.get_i(i),w,0,7,vgatext_base);
	//for(int loc=0;*p;loc++,p++){
	//	writecharxy(loc, 1, *p, 7, 0, w, h, vgatext_base);
	// MENU :
	// STATUS BAR :
	char *sbar = "Number of keys pressed = ";
	strcat(sbar,itoa(state.num_key));
	drawtext(0,24,sbar,w,0,7,vgatext_base);
//	for (int ptr = 0; *sbar; ptr++,sbar++)
//		writecharxy(ptr, 24, *sbar, 0, 7, w, h, vgatext_base);
    	// vgatext::writechar(loc,*p,0,7,vgatext_base);
    }
  // this is just an example:
  //
  // Please create your own user interface
  //
  // You may also have simple command line user interface
  // or menu based interface or a combination of both.
  //

}


//
//
// helper functions
//
//

ll facto(ll x)
{
	ll ans = 1;
	for (ll i = 2; i <= x; i++)
		ans *= i;
	return ans;
}

ll fibbo(ll x)
{
	ll a = 1;
	ll b = 1;
	ll count = 0;
	while (count < x)
	{
		ll c = b;
		b += a;
		a = c;
		count++
	}
	return b;
}

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  vgatext::writechar(y*w+x,c,bg,fg,vgatext_base);
}

static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int y=y0;y<y1;y++){
    for(int x=x0;x<x1;x++){
      writecharxy(x,y,0,bg,fg,w,h,vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){

  writecharxy(x0,  y0,  0xc9, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y0,  0xbb, bg,fg, w,h,vgatext_base);
  writecharxy(x0,  y1-1,0xc8, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y1-1,0xbc, bg,fg, w,h,vgatext_base);

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y0, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y1-1, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x0,y, 0xba, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x1-1,y, 0xba, bg,fg, w,h,vgatext_base);
  }
}

static void drawtext(int x,int y, char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
    if(!str[i]){
      break;
    }
  }
}

static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%16);
    number=number/16;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}

