/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: theme_selection_widget.dart
/// Author:  Kalaiyarasan
/// Date:  11/10/2026
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
import 'package:z6_theme/theme_selection_widget.dart';
import 'package:z6_theme/theme_template.dart';
class Z6SettingScreen extends StatefulWidget {
  const Z6SettingScreen({super.key});

  @override
  State<Z6SettingScreen> createState() => _Z6SettingScreenState();
}

class _Z6SettingScreenState extends State<Z6SettingScreen> {
  Z6Delay _currentSpeed = Z6Delay.Z6xMEDIUM;
  bool _mute = false;
  Z6Theme _theme = Z6Theme.Honeycomb;

  void _applySetting() {
    Provider.of<Z6ThemeProvider>(context, listen: false).setTheme(_theme, _mute,_currentSpeed );
    Navigator.pop(context);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Settings')),
      body: Column(
        children: [
          Z6SpeedScreen(
            speed: _currentSpeed,
            onSpeedChanged: (newSpeed) {
              if (newSpeed != null) {
                setState(() {
                  _currentSpeed = newSpeed;
                });
              }
            },
          ),
          Z6ThemeSelectScreen(
            onThemeChanged: (newTheme) {
              if (newTheme != null) {
                setState(() {
                  _theme = newTheme;
                });
              }
            },
          ),
          SwitchListTile(
            title: const Text('Mute audio: '),
            value: _mute,
            onChanged: (bool? value) {
              setState(() {
                _mute = value ?? false;
              });
            },
          ),
          Row(
            children: [
              ElevatedButton(
                onPressed: _applySetting,
                child: const Text('Apply'),
              ),
              ElevatedButton(
                onPressed: () {
                  Navigator.pop(context);
                },
                child: const Text('Cancel'),
              ),
            ],
          ),
        ],
      ),
    );
  }
}
