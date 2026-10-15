
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
import 'theme_template.dart';
import 'honeycomb/honeycomb.dart';

class Z6ThemeFactory {
  static Z6ThemeTemplate produce(Z6Theme theme) {
    switch (theme) {
      case Z6Theme.Honeycomb:
        return Z6Honeycomb().generate();
      default:
        return Z6Honeycomb().generate();
    }
  } 
}
