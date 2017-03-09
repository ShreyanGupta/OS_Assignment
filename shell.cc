#include "labs/shell.h"
#include "labs/vgatext.h"

#include <stdlib.h>
#include <string.h>

#define SIZE 24
#define h1 "help          : Displays all the commands."
#define h2 "clear         : Clears the window"
#define h3 "fibbo       x : Prints the value of fibbo(x)"
#define h4 "facto       x : Prints the factorial of the number x"
#define h5 "echo        x : Prints the input x"
#define h6 "cursorcolor x : Changes color of cursor"
#define h7 "corfacto    x : Calls the coroutine to find factorial of x"
#define h8 "fiberfacto  x : Calls the fiber to find factorial of x"

typedef long long ll;
//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  	char temp[] = "";
  	state.line.push(temp);

    char tempr[3] = "$ ";
    state.renderline.push(tempr);
    state.curr_cmd = state.line.get_i(0);
    state.curr_pos = 0;
    state.end_pos = 0;
    state.line_pos = 0;
    state.execute = false;
    state.num_key = 0;
    state.cursor_color = 6;
    state.coroutine_run = false;
    state.coroutine_x = -1;

    fiberstate &f = state.fs;
    for (int i = 0; i < 6; i++)
    {
      f.started[i] = false;
      f.running[i] = false;
      f.x[i] = -1;
      f.answer[i] = 1;
    }
    f.total_fiber = 0;
    f.curr_fiber = 0;
}

char memcmp1(char* s1, char* s2, int len)
{
	int x1 = 0;
	int x2 = 0;
	while (s1[x1] != '\0' && s2[x2] != '\0'){
		if (s1[x1] != s2[x2]) return 1;
    ++x1; ++x2;
  }
	return (s1[x1] != s2[x2]);
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

void shellstate_t::insert_answer(int ans)
{
  // to add a new line to renderline, containing answer.
  hoh_debug(" ----------------  INSERT ANSWER CALLED : " << renderline.get_i(0) << "|");
  coroutine_run = false;

  char curr_cmd_copy[81] = "                                                                                ";
  copy_string(curr_cmd_copy,renderline.get_i(0));
  // hoh_debug("Address of curr_cmd_copy : " << curr_cmd_copy);

  hoh_debug("curr_cmd_copy = " << curr_cmd_copy << "|");
  
  char ans_str[] = "COR: Answer = ";

  char *tempr = renderline.get_i(0);
  copy_string(tempr,ans_str);

  hoh_debug("Pushing this : " << curr_cmd_copy);
  renderline.push(curr_cmd_copy);

  char *temp1 = renderline.get_i(1);
  int_to_string(ans,temp1+14);

          for(int i=0; i< renderline.size(); ++i){
            hoh_debug("line " << i << " : " << renderline.get_i(i));
          }
}

void shellstate_t::insert_answer_fiber(int ans, int curr)
{
  hoh_debug(" ----------------  INSERT ANSWER FIBER CALLED : " << renderline.get_i(0) << "|");
  hoh_debug("ans = " << ans << ", curr = " << curr);
  coroutine_run = false;

  char curr_cmd_copy[81] = "                                                                                ";
  copy_string(curr_cmd_copy,renderline.get_i(0));
  // hoh_debug("Address of curr_cmd_copy : " << curr_cmd_copy);

  hoh_debug("curr_cmd_copy = " << curr_cmd_copy << "|");
  
  char ans_str[40];
  char s1[] = "Answer to facto fiber                 ";
  char s2[] = "Answer to fibbo fiber                 ";
  if (curr < 3)
    copy_string(ans_str,s1); 
  else
  {
    copy_string(ans_str,s2);
    curr -= 3;
  }
  ans_str[22] = (char)((int)'0' + curr);
  ans_str[23] = ' ';
  ans_str[24] = '\0';

  char *tempr = renderline.get_i(0);
  copy_string(tempr,ans_str);

  hoh_debug("Pushing this : " << curr_cmd_copy);
  renderline.push(curr_cmd_copy);

  char *temp1 = renderline.get_i(1);
  int_to_string(ans,temp1+24);

          for(int i=0; i< renderline.size(); ++i){
            hoh_debug("line " << i << " : " << renderline.get_i(i));
          }  
}

void shell_update(uint8_t scankey, shellstate_t& s){
    s.execute = false;
    hoh_debug("Got: "<< unsigned(scankey));
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
        {
          char *temp = s.line.get_i(0);
          copy_string(temp,(const char*)s.curr_cmd);

          char *tempr = s.renderline.get_i(0);
          tempr[0] = '$'; tempr[1] = ' ';
          copy_string(tempr+2,s.curr_cmd);

          // int i=0;
          // while(*(s.curr_cmd+i) != '\0'){
          //   temp[i] = s.curr_cmd[i];
          //   ++i;
          // }
          // temp[i] = '\0';
          char temp2[1] = "";
          s.line.push(temp2);
          s.curr_pos = s.end_pos = s.line_pos = 0;
          s.curr_cmd = s.line.get_i(0);
          s.execute = true;
          // s.enter_pressed = true;
          break;
        }
        case 0x1d : // ctrl
            break;
        case 0x2a : // shift
            break;
        case 0x48 :	// up
        	if(s.line_pos == s.line.size()-1) break;
        	s.curr_cmd = s.line.get_i(++s.line_pos);
          	s.curr_pos = s.end_pos = 0;
        	while(*(s.curr_cmd + s.end_pos) != '\0'){
        		++s.end_pos;
        		++s.curr_pos;
        	}
        	break;
        case 0x50 : // down
        	if(s.line_pos == 0) break;
        	s.curr_cmd = s.line.get_i(--s.line_pos);
          	s.curr_pos = s.end_pos = 0;
        	while(*(s.curr_cmd + s.end_pos) != '\0'){
        		++s.end_pos;
        		++s.curr_pos;
        	}
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

      if (!s.execute)
      {
        char *tempr = s.renderline.get_i(0);
        tempr[0] = '$'; tempr[1] = ' ';
        copy_string(tempr+2,s.curr_cmd);
      }

    // hoh_debug("Current pos: " << s.curr_pos << " end pos " << s.end_pos << " line size " << s.renderline.size() << " curr line " << s.curr_cmd);
    // for(int i=0; i<s.renderline.size(); ++i){
    //   hoh_debug("line " << i << " : " << s.renderline.get_i(i));
    // }

}


//
// do computation
//
void shell_step(shellstate_t& s){
    if(!s.execute)
     return;
    int x = 0;
    // finding the command name:
    char *comm_exec = s.line.get_i(1);
    while (comm_exec[x] != ' ' && comm_exec[x] != '\0')
    	x++;
    char blah[x+1];
    for (int i = 0; i < x; i++)
    blah[i] = comm_exec[i];
    blah[x] = '\0';
    char echo[5] = "echo";
    char fibbonacci[6] = "fibbo";
    char factorial[6] = "facto";
    char clear[6] = "clear";
    char help[5] = "help";
    char cur_color[] = "cursorcolor";
    char cor_facto[] = "corfacto";
    char fiber_facto[] = "fiberfacto";
    char fiber_fibbo[] = "fiberfibbo";
    char output[1<<9];

          bool print_out = true;
          // bool add_line = true;

    if (memcmp1(blah,clear,6) == 0)
    {
      // executing clear
      s.renderline.clear();
      print_out = false;
      // add_line = true;
    }
    else if (memcmp1(blah,help,5) == 0)
    {
      // executing help
      s.renderline.push(h1);
      s.renderline.push(h2);
      s.renderline.push(h3);
      s.renderline.push(h4);
      s.renderline.push(h5);
      s.renderline.push(h6);
      s.renderline.push(h7);
      s.renderline.push(h8);
    }
    else
    {
      // COMMANDS WITH AN INPUT
    	// find next space!
    	x++;
    	int y = x;
    	while (comm_exec[x] != '\0')
    	    x++;
    	if (memcmp1(blah,echo,5) == 0)
    	{
    	    for (int i = y; i <= x; i++)
    	   	 output[i-y] = comm_exec[i];
    	}
    	else if (comm_exec[y] == '-')
    	{
    	    char err[22] =  "ERROR! Negative Input";
    	    memcpy(output,err,22);
    	}
    	else
    	{
    	// convert str to long long.
    	    ll input = 0;
    	    for (int i = y; i < x; i++)
    		    input = 10*input + (int)(comm_exec[i] - '0');
    	    ll ans;


    	    if (memcmp1(blah,fibbonacci,6) == 0)
    		    ans = fibbo(input);
    	    else if(memcmp1(blah,factorial,6) == 0)
    		    ans = facto(input);
          else if (memcmp1(blah,cur_color,12) == 0 && input > 0 && input < 8)
          {
            s.cursor_color = input;
            print_out = false;
            // add_line = true;
          }
          else if (memcmp1(blah,cor_facto,9) == 0)
          {
            // ans = -1;
            if (!s.coroutine_run)
            {
              s.coroutine_x = input;
              s.coroutine_run = true;
            }
            else
            {
              ans = -1;
              char err[] = "ERROR! Coroutine already running.";
              memcpy(output,err,sizeof(err));              
            }
            print_out = false;
            // add_line = true;

          }
          else if (memcmp1(blah, fiber_facto, 11) == 0 || memcmp1(blah, fiber_fibbo, 11) == 0)
          {
            // if (!s.fs.running)
            // {
            //   s.fs.x = input;
            //   s.fs.started = true;
            // }
            // else
            // {
            //   ans = -1;
            //   char err[] = "ERROR! Fiber already running.";
            //   memcpy(output,err,sizeof(err));                            
            // }
            // print_out = false;


            // code for 2.3 :
            if (s.fs.total_fiber < 5)
            {
              // insert to relevant slot
              int first_or_last3 = (memcmp1(blah, fiber_facto, 11) == 0) ? 0 : 3;

                bool found = false;
                for (int i = first_or_last3; i < first_or_last3+3; i++)
                {
                  if (!(s.fs.started[i] || s.fs.running[i]))
                  {
                    s.fs.started[i] = true;
                    s.fs.x[i] = input;

                    hoh_debug("ASSIGNED FIBER TO " << i);

                    found = true;
                    s.fs.total_fiber += 1;
                    break;
                  }
                }
                if (!found)
                {
                  ans = -1;
                  char err[] = "ERROR! 3 Fibers of this function running already!";
                  memcpy(output,err,sizeof(err));
                }
                else
                  print_out = false;

            }
            else
            {
              ans = -1;
              char err[] = "ERROR! 5 Fibers running already!";
              memcpy(output,err,sizeof(err));
            }
          }
      		else
      		{
      			ans = -1;
      			char err[23] = "ERROR! Invalid Command";
      			memcpy(output,err,23);
      		}
    	    // update output! TODO
    	    if (ans >= 0)
    	    {
    	    	// s.output = itoa(ans);
            int_to_string(ans,output);
            hoh_debug("answer = " << blah);
    	    }
    	}
      if (print_out) s.renderline.push(output);
    }
    // if (add_line)
    // {
      char temp[] = "$ ";
      s.renderline.push(temp);
      s.execute = false;      
    // }
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
void shell_render(const shellstate_t& s, renderstate_t& r){
  r.line = &(s.renderline);
  r.curr_pos = s.curr_pos;
  // int i=2;
  // while(*temp != '\0'){
  // 	curr_cmd[i] = *temp;
  // 	++i; ++temp;
  // }
  // curr_cmd[i] = '\0';

  r.cursor_color = s.cursor_color;
  r.coroutine_run = s.coroutine_run;
  r.fiber_run = (s.fs.total_fiber > 0);
  r.num_key = s.num_key;
  // TODO : curr_cmd is the next line, and output is its next.
  // add these to render.Lines.

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
	// return false;
	return ((a.num_key == b.num_key 
    && a.curr_pos == b.curr_pos 
    && a.coroutine_run == b.coroutine_run
    && a.fiber_run == b.fiber_run));
	// TODO : add equality of Deque here. a.Deque == b.Deque
}

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void draw_const_text(int x,int y,const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& r, int w, int h, addr_t vgatext_base){

	//const char *p = "What is your problem?";
	// cursor had bg=7, fg=0
	// all others have white fg, black bg.
    const char *temp = "                                                                                ";
  for (int i=0; i<SIZE; ++i) {
    draw_const_text(0,i,temp,w,0,0,w,h,vgatext_base);
  }

	for (int i = r.line->size() - 1; i >= 0 ; --i)
  {
    // check for size more than 80.
		draw_const_text(0,r.line->size()-1-i,r.line->read_i(i),w,7,0,w,h,vgatext_base);
  }


  writecharxy(2+r.curr_pos,r.line->size()-1,r.line->read_i(0)[r.curr_pos+2],0,r.cursor_color,w,h,vgatext_base);
	//for(int loc=0;*p;loc++,p++){
	//	writecharxy(loc, 1, *p, 7, 0, w, h, vgatext_base);
	// MENU :
	// STATUS BAR :

  char sbar[80] = "Number of keys pressed = ";
  int_to_string(r.num_key,sbar+25);
	drawtext(0,24,sbar,w,0,r.cursor_color,w,h,vgatext_base);

//	for (int ptr = 0; *sbar; ptr++,sbar++)
//		writecharxy(ptr, 24, *sbar, 0, 7, w, h, vgatext_base);
    	// vgatext::writechar(loc,*p,0,7,vgatext_base);
   // }
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
		count++;
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

static void draw_const_text(int x,int y,const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
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

void copy_string(char *to, const char *from){
	while(*from != '\0'){
		*to = *from;
		++to; ++from;
	}
	*to = '\0';
}

void int_to_string(int x, char *y2){
  char *y1 = y2;
  int s = 0;
  while(x != 0){
    *y2 = x%10 + 48;
    ++y2;
    x /= 10;
    ++s;
  }
  *(y2) = '\0';
  --y2;
  hoh_debug("number = " << x << ", size " << s << " " << (y2-y1) << " " << *y1 << " " << *y2);
  for(int i=0; i<s/2; ++i){
    char temp = *y1;
    *y1 = *y2;
    *y2 = temp;
    ++y1; --y2;
  }

}
