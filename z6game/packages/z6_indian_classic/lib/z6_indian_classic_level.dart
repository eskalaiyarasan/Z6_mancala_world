/// Project: Z6 Mancala World
/// File: z6_indian_classic_level.dart

import 'dart:convert';

class Z6IndianClassicLevel {
  static const String rawJson = '''{
    "1": {"n": 6, "m": 4, "pasu": {"on": false}, "visible": true},
    "2": {"n": 7, "m": 5, "pasu": {"on": true, "p": 4}, "visible": true},
    "3": {"n": 7, "m": 12, "pasu": {"on": true, "p": 6}, "visible": true}
  }''';

  static int get(String ch, int level) {
    String key = level.toString();
    Map<String, dynamic> decodedJson = jsonDecode(rawJson);
    if (decodedJson.containsKey(key)) {
      if ((ch == "nSide") || (ch.codeUnitAt(0) == 'n'.codeUnitAt(0))) {
        return decodedJson[key]['n'];
      } else if ((ch == "nSeeds") || (ch.codeUnitAt(0) == 'm'.codeUnitAt(0))) {
        return decodedJson[key]['m'];
      } else if ((ch == "pasu") || (ch.codeUnitAt(0) == 'p'.codeUnitAt(0))) {
        if (decodedJson[key]['pasu']['on'] ?? false) {
          return decodedJson[key]['pasu']['p'] ?? 0;
        }
      } else if ((ch == "visible") || (ch.codeUnitAt(0) == 'v'.codeUnitAt(0))) {
        return decodedJson[key]['visible'] ? 1 : 0;
      }
    }
    return -1;
  }
}