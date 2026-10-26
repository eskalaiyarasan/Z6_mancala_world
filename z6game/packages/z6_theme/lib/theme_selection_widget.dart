/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: theme_selection_widget.dart
/// Author:  Kalaiyarasan
/// Date:  11/10/2026
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
import 'package:provider/provider.dart';
import 'theme_template.dart';
import 'theme_factory.dart';


class Z6SpeedScreen extends StatelessWidget {
  final Z6Delay speed;
  final ValueChanged<Z6Delay?> onSpeedChanged;
  
  const Z6SpeedScreen({
    super.key,
    required this.speed,
    required this.onSpeedChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [ 
        Text("Select the Animation speed"),
        DropdownMenu<Z6Delay>(
          initialSelection: speed,
          dropdownMenuEntries: const <DropdownMenuEntry<Z6Delay>>[
            DropdownMenuEntry(value: Z6Delay.Z6xIMMEDIATE, label: 'Super Fast'),
            DropdownMenuEntry(value: Z6Delay.Z6xFAST, label: 'Fast'),
            DropdownMenuEntry(value: Z6Delay.Z6xMEDIUM, label: 'Normal'),
            DropdownMenuEntry(value: Z6Delay.Z6xSLOW, label: 'Slow'),
            ],
          onSelected: (newSpeed) {
            onSpeedChanged(newSpeed);
          },
        ),
      ]
    );

  }
}

class Z6ThemeProvider extends ChangeNotifier {
  // Default starting theme
  Z6ThemeTemplate _currentTheme = Z6ThemeFactory.produce(Z6Theme.Honeycomb); 
  Z6Speed _currentSpeed = Z6Speed();  //Z6Delay.Z6xMEDIUM;
  Z6ThemeTemplate get currentTheme => _currentTheme;
  Z6Speed get currentSpeed => _currentSpeed;

  void setTheme(Z6Theme theme , bool mute , Z6Delay delay) {
    _currentTheme = Z6ThemeFactory.produce(theme);
    _currentSpeed.setParam(mute, delay);
    _currentTheme.sound?.mute = mute;
    notifyListeners(); // Tells all listening widgets to rebuild automatically
  }
}

class Z6ThemeSelectScreen extends StatelessWidget {
  final ValueChanged<Z6Theme?> onThemeChanged;
  Z6ThemeSelectScreen({
    super.key,
    required this.onThemeChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
          Text('Select Your Game Theme'),
          DropdownMenu<Z6Theme>(
            initialSelection: Provider.of<Z6ThemeProvider>(context, listen: false).currentTheme.theme,
            dropdownMenuEntries: const <DropdownMenuEntry<Z6Theme>>[
                DropdownMenuEntry(value: Z6Theme.Honeycomb, label: 'HoneyComb Theme'),
            ],
            onSelected: (theme) {
              onThemeChanged(theme);
            },
          )
      ]
    );
  }
}
