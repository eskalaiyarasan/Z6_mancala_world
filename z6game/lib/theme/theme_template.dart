/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: theme_template.dart
/// Author:  Kalaiyarasan
/// Date:  13/10/2026
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

class Z6ThemeSound {
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


  Future<void> playDrop() async { if (drop != null) await _player.play(AssetSource(drop!)); }
  Future<void> playScoop() async { if (scoop != null) await _player.play(AssetSource(scoop!)); }
  Future<void> playCapture() async { if (capture != null) await _player.play(AssetSource(capture!)); }
  Future<void> playWin() async { if (win != null) await _player.play(AssetSource(win!)); }
  Future<void> playLose() async { if (lose != null) await _player.play(AssetSource(lose!)); }
  Future<void> playOther() async { if (other != null) await _player.play(AssetSource(other!)); }

  void dispose() {
  }
}

class Z6ThemePit {
  Widget? normal;
  Widget? special;
  Widget? store;
  Widget? inactive;

  Z6ThemePit({
    this.normal, 
    this.special,
    this.store, 
    this.inactive,
  });
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

abstract class Z6ThemeBlueprint {
  Z6ThemeTemplate generate();
}
