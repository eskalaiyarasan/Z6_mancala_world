/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: z6_inidan_classic.dart
/// Author:  Kalaiyarasan
/// Date:  11/10/2026
/// Package:  Z6IndianClassic
/// 
/// Notice: All information contained herein is, and remains the property of 
/// Zerasix Gaming Studio LLP. The intellectual and technical concepts contained 
/// herein are proprietary to Zerasix Gaming Studio LLP and may be covered by 
/// Indian and foreign patents or trademarks, and are protected by trade secret 
/// or copyright law. Dissemination of this information or reproduction of this 
/// material is strictly forbidden unless prior written permission is obtained 
/// from Zerasix Gaming Studio LLP.
import 'dart:convert';
class NodeData {
  int n;
  int m;
  Pasu pasu;
  bool visible;

  NodeData({required this.n, required this.m, required this.pasu, required this.visible});

  factory NodeData.fromJson(Map<String, dynamic> json) {
    return NodeData(
      n: json['n'] ?? 0,
      m: json['m'] ?? 0,
      pasu: Pasu.fromJson(json['pasu'] ?? {}),
      visible: json['visible'] ?? true,
    );
  }

  Map<String, dynamic> toJson() => {
    'n': n,
    'm': m,
    'pasu': pasu.toJson(),
    'visible': visible,
  };
}

class Pasu {
  bool on;
  int? m; // Optional property since it is missing in node "1"

  Pasu({required this.on, this.m});

  factory Pasu.fromJson(Map<String, dynamic> json) {
    return Pasu(
      on: json['on'] ?? false,
      m: json['m'],
    );
  }

  Map<String, dynamic> toJson() => {
    'on': on,
    if (m != null) 'm': m,
  };
}

class Z6IndianClassicLevel {
static const String rawJson = '''{
    "1": {"n": 6, "m": 4, "pasu": {"on": false}, "visible": true},
    "2": {"n": 7, "m": 5, "pasu": {"on": true, "p": 4}, "visible": true},
    "3": {"n": 7, "m": 12, "pasu": {"on": true, "p":6}, "visible": true},
  }''';

static int get(String ch,  int level){
    String key = level.toString();
    Map<String, dynamic> decodedJson = jsonDecode(rawJson);
    if (decodedJson.containsKey(key)) {
      if( ( ch == "nSide" ) || (ch.codeUnitAt(0) == 'n'.codeUnitAt(0)) ){
        return decodedJson[key]['n'];
      } else if( ( ch == "nSeeds" ) || (ch.codeUnitAt(0) == 'm'.codeUnitAt(0)) ){
        return decodedJson[key]['m'];
      } else if( ( ch == "pasu" ) || (ch.codeUnitAt(0) == 'p'.codeUnitAt(0)) ){
         if( decodedJson[key]['pasu']['on'] )
              return decodedJson[key]['pasu']['p'];
      } else if( ( ch == "visible" ) || (ch.codeUnitAt(0) == 'v'.codeUnitAt(0)) ){
        return decodedJson[key]['visible']? 1 : 0 ;
      }
    }
    return -1;
}
