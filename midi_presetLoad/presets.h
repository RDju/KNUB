#include "memory.h"

aKnubPreset preset1 = {"ALLOFF",{0, 0, 0, 0, 0, 1},
  {{"DIST", {5, 254, 1}, 0, 0, 0}, 
  {"FILT", {254, 1, 1}, 0, 0, 0},
  {"LOVE", {134, 134, 1}, 0, 0, 0},
  {"WAH", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};

aKnubPreset preset2 = {"VERSE ",{0, 0, 0, 0, 0, 2},
  {{"DIST", {5, 254, 1}, 1, 1, 0}, 
  {"FILT", {254, 1, 1}, 1, 1, 0},
  {"LOVE", {134, 134, 1}, 1, 1, 0},
  {"WAH", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};

aKnubPreset preset3 = {"CHORUS ",{0, 0, 0, 0, 0, 3},
  
  {{"DIST", {100, 254, 1}, 1, 1, 0}, 
  {"FILT", {10, 0, 1}, 1, 1, 0},
  {"VOL", {134, 134, 1}, 1, 1, 0},
  {"WAH", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
};

aKnubPreset preset4 = {"SOLO   ",{0, 0, 0, 0, 0, 4},
  
  {{"DIST", {254, 254, 1}, 0, 1, 0}, 
  {"FILT", {254, 254, 1}, 0, 1, 0},
  {"VOL", {200, 255, 1}, 0, 1, 0},
  {"WAH", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};

aKnubPreset preset5 = {"CLEAN  ",{0, 0, 0, 0, 0, 5},
  {{"DIST", {5, 254, 1}, 0, 0, 0}, 
  {"FILT", {254, 1, 1}, 0, 0, 0},
  {"VOL", {134, 134, 1}, 0, 0, 0},
  {"WAH", {0, 255, 1}, 0, 0, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};

aKnubPreset preset6 = {"FUNK   ",{0, 0, 0, 0, 0, 6},
  {{"DIST", {200, 0, 1}, 0, 1, 0}, 
  {"FILT", {1, 0, 1}, 0, 1, 0},
  {"VOL", {134, 0, 1}, 0, 1, 0},
  {"WAH", {127, 127, 1}, 0, 1, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
  
};

aKnubPreset preset7 = {"WAH    ",{0, 0, 0, 0, 0, 7},
  
  {{"DIST", {160, 254, 1}, 0, 0, 0}, 
  {"FILT", {26, 0, 1}, 0, 0, 0},
  {"VOL", {109, 134, 1}, 0, 0, 0},
  {"WAH", {0, 255, 1}, 1, 1, 1}, 
  {"OUT5", {0, 255, 1}, 0, 0, 2},
  {"OUT6", {0, 255, 1}, 0, 0, 2},
  {"OUT7", {0, 255, 1}, 0, 0, 3},
  {"OUT8", {0, 255, 1}, 0, 0, 3}}
};

aKnubPreset preset8 = {"CRAZY  ",{0, 0, 0, 0, 0, 8},
  
  {{"DIST", {254, 110, 1}, 1, 1, 0}, 
  {"FILT", {254,  1, 1}, 1, 1, 0},
  {"VOL", {200, 255, 1}, 1, 1, 0},
  {"WAH", {0, 255, 1}, 1, 1, 1}, 
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
};


aKnubPreset currentPreset = {"       ", {0, 0, 0, 0, 0, 0}, 

  {{"       ", {0, 0, 0}, 0, 0, 0}, 
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0}, 
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0},
  {"       ", {0, 0, 0}, 0, 0, 0}}
};
