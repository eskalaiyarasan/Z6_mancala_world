/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: theme_template.dart
/// Author:  Kalaiyarasan
/// Date:  13/10/2026
/// Package:  Z6Theme
/// 
/// Notice: All information contained herein is, and remains the property of 
/// Zerasix Gaming Studio LLP. The intellectual and technical concepts contained 
/// herein are proprietary to Zerasix Gaming Studio LLP and may be covered by 
/// Indian and foreign patents or trademarks, and are protected by trade secret 
/// or copyright law. Dissemination of this information or reproduction of this 
/// material is strictly forbidden unless prior written permission is obtained 
/// from Zerasix Gaming Studio LLP.
import 'package:flutter/material.dart';
import 'package:audioplayers/audioplayers.dart';

enum Z6Theme { Honeycomb, Aqua }
enum Z6PitType { General, Master, Store }
enum Z6Delay { Z6xIMMEDIATE, Z6xFAST, Z6xMEDIUM, Z6xSLOW }

class Z6ThemeSound {
  bool? mute = false;
  String? drop;
  String? scoop;
  String? capture;
  String? win;
  String? lose;
  String? other; 
// Singleton player instance to play SFX easily
  static final AudioPlayer _player = AudioPlayer();

  Z6ThemeSound({
    this.drop, 
    this.scoop,
    this.capture, 
    this.win,
    this.lose,  
    this.other,
  }); 
   
  Future<void> _play_sound(String? s) async {
      bool isMuted = mute ?? false;
      if ( (s != null) && (!isMuted)){
        await _player.play(AssetSource(s!)); 
      }
  }
          

  Future<void> playDrop() async { await _play_sound(drop); }
  Future<void> playScoop() async { await _play_sound(scoop); }
  Future<void> playCapture() async { await _play_sound(capture); }
  Future<void> playWin() async { await _play_sound(win); }
  Future<void> playLose() async { await _play_sound(lose); }
  Future<void> playOther() async { await _play_sound(other); }

  void dispose() {
  }
}

abstract class Z6ThemePit {
  Widget normal(int value);
  Widget special(int value);
  Widget store(int value);
  Widget inactive();
}

class Z6ThemeColor {
  Color? bg;
  TextStyle? text;
  Color? empty;
  Color? fill;

  Z6ThemeColor({
    this.bg,
    this.text,
    this.empty,
    this.fill,
  });
}
    
class Z6ThemeTemplate {
  Z6ThemeSound? sound;
  Z6ThemePit? pit;
  Z6Theme? theme;
  Z6ThemeColor? color;  
  
  Z6ThemeTemplate({
    this.sound, 
    this.pit,
    this.theme, 
    this.color,
  });
}

class Z6Speed {
  bool mute = false; 
  Z6Delay _delay = Z6Delay.Z6xMEDIUM;  
  Z6Speed(){
    mute = false;
    _delay = Z6Delay.Z6xMEDIUM; 
  }
  bool get isMuted => mute;
  int get speed {
    return switch (_delay) {
      Z6Delay.Z6xIMMEDIATE => 1,   
      Z6Delay.Z6xFAST      => 100,
      Z6Delay.Z6xMEDIUM    => 600,
      Z6Delay.Z6xSLOW      => 1200,
      _                    => 800,
    };
  }

  void setParam( bool mm, Z6Delay delay) {
    mute = mm;
    _delay = delay;
  } 
}
abstract class Z6ThemeBlueprint {
  Z6ThemeTemplate generate();
}
