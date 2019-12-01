# 使用方法简述
需要三台 MSP430F5529，分别作为音乐播放、歌词显示、点阵控制，各台的 P1.2 和 P1.3 需要接在一起。  
- 音乐播放  
  DAC_OUT 接 P-AMP_IN；VO+ 和 VO- 接扬声器  
  在 main.c 中定义宏 `AS_WAV_PLAYBAC`  
- 歌词显示  
  在 main.c 中定义宏 `AS_LYRIC_DISPLAY`  
- 点阵控制  
  点阵供电 5V；DIN，CS，CLK 分别接 P6.1，P6.2，P6.3  
  在 main.c 中定义宏 `AS_LED_DISPLAY`  
