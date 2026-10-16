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
import 'theme_template.dart';
import 'theme_factory.dart';


class Z6ThemeProvider extends ChangeNotifier {
  // Default starting theme
  Z6ThemeTemplate _currentTheme = Z6ThemeFactory.produce(Z6Theme.Honeycomb); 

  Z6ThemeTemplate get currentTheme => _currentTheme;

  void setTheme(Z6Theme theme) {
    _currentTheme = Z6ThemeFactory.produce(theme);
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
      children[
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
