#include <avr/pgmspace.h>

//prog_uchar PROGMEM 
// PROGMEM prog_uchar redLUT[] = {
// 0 ,  0,  4 ,  9 ,  11 ,  13 ,  15 ,  16 ,  17 ,  19 ,  20 ,  
// 21 ,  22 ,  24 ,  25 ,  26 ,  27 ,  27 ,  28 ,  29 ,  29 ,  30 ,  31 ,  31 ,  
// 32 ,  33 ,  33 ,  34 ,  34 ,  35 ,  35 ,  36 ,  36 ,  37 ,  37 ,  37 ,  38 ,  
// 38 ,  39 ,  39 ,  39 ,  39 ,  40 ,  40 ,  41 ,  41 ,  42 ,  42 ,  42 ,  43 ,  
// 43 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  44 ,  44 ,  45 ,  45 ,  46 ,  46 ,  
// 46 ,  46 ,  46 ,  46 ,  46 ,  47 ,  47 ,  47 ,  47 ,  47 ,  47 ,  48 ,  48 ,  
// 48 ,  48 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  49 ,  50 ,  50 ,  50 ,  50 ,  
// 50 ,  50 ,  50 ,  50 ,  50 ,  51 ,  51 ,  51 ,  51 ,  51 ,  51 ,  52 ,  52 ,  
// 52 ,  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  53 ,  53 ,  54 ,  54 ,  54 ,  54 ,  
// 54 ,  55 ,  55 ,  55 ,  55 ,  55 ,  56 ,  56 ,  56 ,  56 ,  57 ,  57 ,  57 ,  
// 58 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  60 ,  60 ,  60 ,  61 ,  61 ,  61 ,  
// 61 ,  62 ,  62 ,  62 ,  63 ,  63 ,  63 ,  64 ,  64 ,  64 ,  64 ,  65 ,  65 ,  
// 65 ,  66 ,  66 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  68 ,  69 ,  69 ,  70 ,  
// 70 ,  71 ,  72 ,  72 ,  73 ,  74 ,  74 ,  75 ,  76 ,  76 ,  77 ,  78 ,  78 ,  
// 79 ,  80 ,  81 ,  82 ,  83 ,  84 ,  85 ,  86 ,  87 ,  88 ,  89 ,  90 ,  90 ,  
// 91 ,  92 ,  93 ,  94 ,  96 ,  96 ,  97 ,  99 ,  100 ,  102 ,  103 ,  105 ,  107 ,  
// 109 ,  110 ,  112 ,  115 ,  117 ,  120 ,  122 ,  125 ,  127 ,  131 ,  134 ,  138 ,  
// 141 ,  143 ,  146 ,  149 ,  152 ,  155 ,  161 ,  164 ,  167 ,  172 ,  176 ,  188 ,  
// 203 ,  215 ,  224 ,  233 ,  238 ,  241 ,  244 ,  247 ,  249 ,  251 ,  252 ,  254 ,  
// 255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  255 ,  
// 255 ,  255 };

PROGMEM prog_uchar redLUT[] = { 3 ,  6 ,  8 ,  10 ,  11 ,  12 ,  14 ,  15 ,  16 ,  16 ,  17 ,  
  18 ,  19 ,  19 ,  20 ,  20 ,  21 ,  21 ,  22 ,  22 ,  23 ,  23 ,  24 ,  24 ,  24 ,  
  25 ,  25 ,  25 ,  26 ,  26 ,  26 ,  27 ,  27 ,  27 ,  28 ,  28 ,  28 ,  29 ,  29 ,  
  29 ,  29 ,  30 ,  30 ,  30 ,  30 ,  30 ,  31 ,  31 ,  31 ,  31 ,  32 ,  32 ,  32 ,  
  33 ,  33 ,  33 ,  33 ,  33 ,  34 ,  34 ,  34 ,  35 ,  35 ,  37 ,  37 ,  38 ,  38 ,  
  39 ,  39 ,  39 ,  39 ,  39 ,  40 ,  40 ,  40 ,  41 ,  41 ,  41 ,  41 ,  41 ,  42 ,  
  42 ,  42 ,  42 ,  43 ,  43 ,  43 ,  43 ,  44 ,  44 ,  44 ,  45 ,  45 ,  45 ,  46 ,  
  46 ,  46 ,  47 ,  47 ,  48 ,  48 ,  48 ,  49 ,  49 ,  49 ,  50 ,  50 ,  51 ,  51 ,  
  52 ,  52 ,  52 ,  53 ,  53 ,  53 ,  54 ,  54 ,  54 ,  55 ,  55 ,  56 ,  56 ,  56 ,  
  57 ,  58 ,  58 ,  58 ,  59 ,  59 ,  59 ,  60 ,  60 ,  61 ,  61 ,  62 ,  62 ,  62 ,  
  63 ,  63 ,  64 ,  64 ,  65 ,  65 ,  65 ,  66 ,  66 ,  66 ,  67 ,  67 ,  68 ,  68 ,  
  69 ,  69 ,  70 ,  71 ,  71 ,  71 ,  72 ,  73 ,  73 ,  73 ,  74 ,  75 ,  75 ,  75 ,  
  76 ,  76 ,  77 ,  77 ,  78 ,  79 ,  79 ,  80 ,  80 ,  81 ,  82 ,  83 ,  83 ,  84 ,  
  84 ,  85 ,  86 ,  86 ,  87 ,  88 ,  89 ,  89 ,  90 ,  90 ,  91 ,  92 ,  93 ,  94 ,  
  94 ,  95 ,  96 ,  97 ,  98 ,  98 ,  99 ,  100 ,  101 ,  102 ,  103 ,  104 ,  106 ,  
  107 ,  108 ,  109 ,  111 ,  112 ,  113 ,  115 ,  116 ,  118 ,  119 ,  121 ,  122 ,  
  124 ,  127 ,  129 ,  130 ,  132 ,  134 ,  136 ,  138 ,  140 ,  142 ,  145 ,  148 ,  
  151 ,  158 ,  165 ,  173 ,  180 ,  185 ,  190 ,  199 ,  208 ,  212 ,  215 ,  219 ,  
  225 ,  230 ,  235 ,  240 ,  244 ,  250 ,  252 ,  253 ,  254 ,  255 ,  255 ,  255 ,  255 };