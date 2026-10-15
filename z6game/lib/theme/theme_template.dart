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
import 'package:flutter_soloud/flutter_soloud.dart';

enum Z6Theme { Honeycomb, Aqua }
enum Z6PitType { General, Master, Store }

class Z6ThemeSound {
  AudioSource? drop;
  AudioSource? scoop;
  AudioSource? capture;
  AudioSource? win;
  AudioSource? lose;
  AudioSource? other; 

  Z6ThemeSound({
    this.drop, 
    this.scoop,
    this.capture, 
    this.win,
    this.lose,  
    this.other,
  }); 

  static Future<Z6ThemeSound> fromPaths({
    required String dropPath,
    required String scoopPath,
    required String capturePath,
    required String winPath,
    required String losePath,
    required String otherPath,
  }) async {
    final results = await Future.wait([
      SoLoud.instance.loadAsset(dropPath, autoDispose: false),
      SoLoud.instance.loadAsset(scoopPath, autoDispose: false),
      SoLoud.instance.loadAsset(capturePath, autoDispose: false),
      SoLoud.instance.loadAsset(winPath, autoDispose: false),
      SoLoud.instance.loadAsset(losePath, autoDispose: false),
      SoLoud.instance.loadAsset(otherPath, autoDispose: false),
    ]);

    return Z6ThemeSound(
      drop: results[0],
      scoop: results[1],
      capture: results[2],
      win: results[3],
      lose: results[4],
      other: results[5],
    );
  }

  Future<void> playDrop() async { if (drop != null) SoLoud.instance.play(drop!); }
  Future<void> playScoop() async { if (scoop != null) SoLoud.instance.play(scoop!); }
  Future<void> playCapture() async { if (capture != null) SoLoud.instance.play(capture!); }
  Future<void> playWin() async { if (win != null) SoLoud.instance.play(win!); }
  Future<void> playLose() async { if (lose != null) SoLoud.instance.play(lose!); }
  Future<void> playOther() async { if (other != null) SoLoud.instance.play(other!); }

  void dispose() {
    if (drop != null) SoLoud.instance.disposeSource(drop!);
    if (scoop != null) SoLoud.instance.disposeSource(scoop!);
    if (capture != null) SoLoud.instance.disposeSource(capture!);
    if (win != null) SoLoud.instance.disposeSource(win!);
    if (lose != null) SoLoud.instance.disposeSource(lose!);
    if (other != null) SoLoud.instance.disposeSource(other!);
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
