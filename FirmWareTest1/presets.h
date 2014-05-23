
#include "memory.h"

/*aKnubPreset preset1 = {"VERSE ",{0, 0, 0, 0, 0, 1},
  {{"DIST", {5, 254, 1, 1, 1, 0}}, //???: pourquoi 254 ?
  {"FILT", {254, 1, 1, 1, 1, 0}},
  {"LOVE", {134, 134, 1, 1, 1, 0}},
  {"WAH", {0, 255, 1, 0, 0, 1}}, 
  {"OUT5", {0, 255, 1, 0, 0, 2}},
  {"OUT6", {0, 255, 1, 0, 0, 2}},
  {"OUT7", {0, 255, 1, 0, 0, 3}},
  {"OUT8", {0, 255, 1, 0, 0, 3}}}
  
};

aKnubPreset preset2 = {"CHORUS ",{0, 0, 0, 0, 0, 2},
  
  {{"DIST", {100, 254, 1, 1, 1, 0}}, 
  {"FILT", {10, 0, 1, 1, 1, 0}},
  {"VOL", {134, 134, 1, 1, 1, 0}},
  {"WAH", {0, 255, 1, 0, 0, 1}}, 
  {"OUT5", {0, 255, 1, 0, 0, 2}},
  {"OUT6", {0, 255, 1, 0, 0, 2}},
  {"OUT7", {0, 255, 1, 0, 0, 3}},
  {"OUT8", {0, 255, 1, 0, 0, 3}}}
};

aKnubPreset preset3 = {"SOLO   ",{0, 0, 0, 0, 0, 3},
  
  {{"DIST", {254, 110, 1}, 0, 1, 0}, 
  {"FILT", {250, 254, 1}, 0, 1, 0},
  {"VOL", {134, 255, 1}, 0, 1, 0},
  {"WAH", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};


aKnubPreset preset4 = {"INTRO1 ",{0, 0, 0, 0, 0, 4},
  {{"VOL", {100, 0, 1}, 1, 1, 0}, 
  {"TONE", {100, 0, 1}, 1, 1, 0},
  {"SUST", {100, 0, 1}, 1, 1, 0},
  {"OUT4", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};

aKnubPreset preset5 = {"VERSE2 ",{0, 0, 0, 0, 0, 5},
  
  {{"VOL", {160, 254, 1}, 1, 1, 0}, 
  {"TONE", {26, 0, 1}, 1, 1, 0},
  {"SUS", {109, 134, 1}, 1, 1, 0},
  {"OUT4", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
};

aKnubPreset preset6 = {"SOLO2  ",{0, 0, 0, 0, 0, 6},
  
  {{"VOL", {100, 110, 1}, 0, 1, 0}, 
  {"TONE", {254,  254, 1}, 0, 1, 0},
  {"SUST", {230, 255, 1}, 0, 1, 0},
  {"OUT4", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};



aKnubPreset emptyPreset = {"       ", {0, 0, 0, 0, 0, 0}, 

  {{"       ", {0, 0, 0}, 0, 0, 0}, 
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0}, 
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0}}
};*/


aKnubPreset currentPreset = {"       ", {0, 0, 0, 0, 0, 0}, 

  {{"       ", {0, 0, 0, 0, 0, 0}}, 
  {"       ", {0, 0, 0, 0, 0, 0}},
  {"       ", {0, 0, 0, 0, 0, 0}},
  {"       ", {0, 0, 0, 0, 0, 0}}, 
  {"       ", {0, 0, 0, 0, 0, 0}},
  {"       ", {0, 0, 0, 0, 0, 0}},
  {"       ", {0, 0, 0, 0, 0, 0}},
  {"       ", {0, 0, 0, 0, 0, 0}}}
};
