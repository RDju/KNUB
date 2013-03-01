(

n=255;
o = 0;
~curvIndx = 0;
~selIndx = 0;

~duino = NetAddr.new("192.168.0.8", 10000);

//working LUT for DAC : 

~val = [0 , 77 , 92 , 99 , 103 , 106 , 108 , 112 , 114 , 116 , 118 , 119 , 121 , 122 , 123 , 124 , 126 , 127 , 128 , 129 , 130 , 131 , 132 , 133 , 134 , 134 , 135 , 137 , 137 , 138 , 139 , 139 , 140 , 141 , 142 , 143 , 143 , 144 , 145 , 145 , 146 , 146 , 147 , 148 , 148 , 148 , 149 , 149 , 150 , 150 , 150 , 151 , 152 , 152 , 152 , 153 , 153 , 153 , 154 , 154 , 155 , 155 , 156 , 156 , 156 , 157 , 157 , 157 , 157 , 158 , 158 , 158 , 159 , 159 , 159 , 159 , 160 , 160 , 160 , 161 , 161 , 161 , 161 , 162 , 162 , 162 , 163 , 163 , 164 , 164 , 165 , 165 , 165 , 165 , 166 , 166 , 167 , 167 , 167 , 168 , 168 , 168 , 168 , 169 , 169 , 169 , 169 , 169 , 170 , 170 , 170 , 170 , 171 , 171 , 171 , 171 , 172 , 172 , 173 , 173 , 174 , 174 , 174 , 174 , 175 , 175 , 175 , 176 , 176 , 176 , 176 , 177 , 177 , 177 , 178 , 178 , 178 , 179 , 179 , 179 , 180 , 180 , 180 , 180 , 181 , 181 , 181 , 181 , 181 , 182 , 182 , 182 , 182 , 183 , 183 , 183 , 184 , 184 , 185 , 185 , 185 , 185 , 185 , 186 , 186 , 186 , 187 , 187 , 188 , 188 , 189 , 189 , 189 , 190 , 190 , 190 , 191 , 191 , 191 , 192 , 192 , 194 , 194 , 194 , 194 , 194 , 195 , 196 , 196 , 196 , 197 , 197 , 197 , 197 , 197 , 198 , 198 , 198 , 199 , 199 , 200 , 200 , 200 , 201 , 201 , 202 , 202 , 202 , 203 , 203 , 203 , 203 , 204 , 205 , 205 , 206 , 206 , 206 , 207 , 207 , 208 , 209 , 209 , 209 , 210 , 211 , 211 , 212 , 212 , 213 , 213 , 214 , 215 , 216 , 217 , 218 , 219 , 219 , 220 , 221 , 222 , 223 , 224 , 225 , 226 , 228 , 229 , 231 , 232 , 234 , 235 , 238 , 240 , 241 , 244 , 255];


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
	~vactrol.put(mess[1], mess[2]/255);
	
	

}).add;


~resp1 = OSCresponder(~duino, '/O2A/done', {arg time, resp, mess;
	/*
	if(o==0, {t.value = ~vactrol;
	t.refresh;}, {l.value = ~vactrol;
	l.refresh;});
	*/
	
	AppClock.sched(0.0, {~curves[~curvIndx].value = ~vactrol.reverse;});
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


2.do({arg i; ~curves.put(i, MultiSliderView(w,Rect(10,10,255*4+2,600)).thumbSize_(1).drawLines_(true).elasticMode_(true).strokeColor_(Color.new255(0, 0, 0)).background_(Color.gray(0.8).alpha_(0.3));)});


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
