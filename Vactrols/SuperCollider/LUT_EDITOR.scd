(

n=255;
o = 0;
~curvIndx = 0;
~selIndx = 0;

~duino = NetAddr.new("192.168.0.8", 10000);

//working LUT for DAC : 

~val = [1 , 4 ,  5 ,  6 ,  7 ,  8 ,  8 ,  9 ,  9 ,  9 ,  9 ,  10 ,  10 ,  10 ,  10 ,  10 ,  10 ,  10 ,  11 ,  11 ,  11 ,  11 ,  11 ,  11 ,  11 ,  11 ,  12 ,  12 ,  12 ,  12 ,  12 ,  12 ,  12 ,  12 ,  13 ,  13 ,  13 ,  13 ,  13 ,  14 ,  14 ,  14 ,  15 ,  15 ,  15 ,  15 ,  15 ,  15 ,  15 ,  15 ,  16 ,  16 ,  16 ,  16 ,  16 ,  16 ,  16 ,  16 ,  16 ,  16 ,  17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  17 ,  18 ,  18 ,  18 ,  18 ,  18 ,  18 ,  18 ,  18 ,  19 ,  19 ,  19 ,  19 ,  19 ,  19 ,  20 ,  20 ,  20 ,  20 ,  20 ,  20 ,  20 ,  20 ,  20 ,  20 ,  20 ,  21 ,  21 ,  21 ,  21 ,  21 ,  22 ,  22 ,  22 ,  22 ,  22 ,  22 ,  23 ,  23 ,  23 ,  23 ,  24 ,  24 ,  24 ,  24 ,  24 ,  24 ,  24 ,  24 ,  24 ,  24 ,  25 ,  25 ,  25 ,  26 ,  26 ,  26 ,  26 ,  26 ,  27 ,  27 ,  27 ,  27 ,  27 ,  28 ,  28 ,  28 ,  28 ,  29 ,  29 ,  29 ,  29 ,  30 ,  30 ,  30 ,  30 ,  30 ,  30 ,  31 ,  31 ,  31 ,  31 ,  32 ,  32 ,  32 ,  32 ,  33 ,  33 ,  33 ,  33 ,  33 ,  33 ,  33 ,  33 ,  34 ,  35 ,  35 ,  36 ,  36 ,  36 ,  37 ,  37 ,  38 ,  39 ,  39 ,  40 ,  40 ,  41 ,  41 ,  42 ,  42 ,  43 ,  43 ,  43 ,  44 ,  45 ,  46 ,  46 ,  47 ,  48 ,  48 ,  49 ,  50 ,  51 ,  52 ,  53 ,  53 ,  54 ,  55 ,  56 ,  57 ,  58 ,  60 ,  62 ,  63 ,  65 ,  66 ,  68 ,  69 ,  71 ,  72 ,  74 ,  76 ,  78 ,  80 ,  83 ,  86 ,  88 ,  91 ,  97 ,  99 ,  102 ,  108 ,  111 ,  113 ,  116 ,  122 ,  128 ,  131 ,  134 ,  139 ,  146 ,  153 ,  158 ,  171 ,  179 ,  187 ,  200 ,  207 ,  237 ,  239 ,  242 ,  246 ,  250 ,  253 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255];


~norms = Array.fill(255, {arg v; ~val[v]/255});

~outVals = Array.newClear(255);

~curves = Array.newClear(16);

~linear = Array.fill(255, {arg i; i/255});

~vactrol = Array.fill(255, {0.0});
~vactrol2 = Array.fill(255, {0.0});


~vacArray = Array.newClear(2);

~vacArray.put(0, ~vactrol);
~vacArray.put(1, ~vactrol2);

~resp = OSCresponder(~duino, '/O2A/vac1', {arg time, resp, mess;
	mess.postln;
	~vactrol.put(mess[1], mess[2]/1024);
	
	

}).add;


~resp1 = OSCresponder(~duino, '/O2A/done', {arg time, resp, mess;
	/*
	if(o==0, {t.value = ~vactrol;
	t.refresh;}, {l.value = ~vactrol;
	l.refresh;});
	*/
	
	AppClock.sched(0.0, {~curves.value = ~vactrol});
	AppClock.sched(0.0, {~d2.valueAction_(1);});	

}).add;


~resp2 = OSCresponder(~duino, '/O2A/doneVac', {arg time, resp, mess;


	AppClock.sched(0.0, {~d2.valueAction_(2);});
	//~duino.sendMsg("/O2A/doPot2");

}).add;

~resp3 = OSCresponder(~duino, '/O2A/doneVac2', {arg time, resp, mess;


	AppClock.sched(0.0, {~d2.valueAction_(3);});

}).add;



w = Window.new("LUT EDITOR", Rect(20, 20, 1050, 680));
w.onClose = {~resp.remove; ~resp1.remove; ~resp2.remove; ~resp3.remove;};
w.view.background_(Color.white);
w.front;


p = MultiSliderView(w,Rect(10,10,255*4+2,600)); 
p.elasticMode = true;
p.thumbSize = 1;
p.value=~linear;

p.strokeColor_(Color.black);
//t.backGround = Color.new255(255, 255, 255, 0);
p.drawLines = true;
p.drawRects = false;
p.background_(Color.clear);


~sendRout = Routine({
				
			255.do({
				arg i;	
				m.index_(i);
				m.doAction;

			0.02.wait;			
			});
			
});

 ~curves = MultiSliderView(w,Rect(10,10,255*4+2,600)).thumbSize_(1).drawLines_(true).elasticMode_(true).strokeColor_(Color.new255(0, 0, 0)).background_(Color.gray(0.8).alpha_(0.3));


m = MultiSliderView(w,Rect(10,10,255*4+2,600)); 
m.elasticMode = true;
m.thumbSize = 1;
m.value=~norms;

m.showIndex = true;
m.action = { arg q;
	
	~val.put(q.index, (q.value.at(q.index)*255).asInteger);
	~duino.sendMsg("/O2A/lookupVal", q.index, (q.value.at(q.index)*255).asInteger);	
	
};	

m.strokeColor_(Color.red);
m.drawLines = true;
m.drawRects = false;
m.background_(Color.clear);

~d1 = Button(w, Rect(10, 620, 150, 30));
~d1.states = [["update LUT", Color.black, Color.yellow]];
~d1.action = {~d2.valueAction_(0);
		
		~sendRout.reset.play(AppClock);		
		
};

~p1 = Button(w, Rect(170, 620, 150, 30));
~p1.states = [["do POT", Color.black, Color.yellow]];
~p1.action = {~d2.valueAction_(0);
		
		~duino.sendMsg("/O2A/doPot");		
		
};

~t1 = Button(w, Rect(330, 620, 150, 30));
~t1.states = [["TELL ME", Color.black, Color.blue]];
~t1.action = { ~duino.sendMsg("/O2A/tellMe");};

~d2 = Button(w, Rect(490, 620, 150, 30));
~d2.states = [["WAIT....", Color.black, Color.grey], ["DONE", Color.black, Color.green], ["VAC  done", Color.black, Color.yellow], ["VAC2 done", Color.black, Color.blue]];
~d2.action = {arg but;};

~p1 = Button(w, Rect(650, 620, 150, 30));
~p1.states = [["PRINT LUT", Color.black, Color.green]];
~p1.action = {~val.size.do({arg i; (~val[i]+","+" ").post});};

/*
~z1 = Button(w, Rect(650, 650, 150, 30));
~z1.states = [["compute LUT", Color.black, Color.blue]];
~z1.action = {
             255.do({
             
                arg i;
                ~val[i]= ~linear[i] - ~curves[1][i];
             
             
             }) ;
             AppClock.sched(0.0, {m.value = ~val});
                

}
*/
)
