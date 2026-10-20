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
import 'learn_template.dart';
import 'learn_factory.dart';



class Z6LearnSelectScreen extends StatelessWidget {
  final ValueChanged<Z6Learn?> onThemeChanged;
  Z6LearnSelectScreen({
    super.key,
    required this.onLearnChanged,
  });

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
          Text('Select Your Game to Learn: '),
          DropdownMenu<Z6Learn>(
            initialSelection: Provider.of<Z6ThemeProvider>(context, listen: false).currentTheme.theme,
            dropdownMenuEntries: const <DropdownMenuEntry<Z6Theme>>[
                DropdownMenuEntry(value: Z6Theme.Honeycomb, label: 'HoneyComb Theme'),
            ],
            onSelected: (theme) {
              onLearnChanged(theme);
            },
          )
      ]
    );
  }
}
