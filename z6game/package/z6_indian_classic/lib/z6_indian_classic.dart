
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

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:z6_theme/theme_template.dart';
import 'package:z6_theme/theme_selection_widget.dart';
import 'package:z6_base/base_template.dart'

class Z6IndianClassicPit extends StatelessWidget {
    final int index ;
    final int  value;
    final int  side;
    final bool active;
    final Function(int pitIndex)? onPitTap;
    
    Z6IndianClassicPit({ 
        required this.index,
        required this.value,
        required this.side,
        required this.active, 
        required this.onPitTap
    });
    
    @override
  Widget build(BuildContext context) {
    // Get the current theme background color using Provider[cite: 2, 3]
    final theme = Provider.of<Z6ThemeProvider>(context).currentTheme;
    return Stack(
        children: [ 
            center( 
              child: (active)? theme.pit.normal(value): theme.pit.inactive()  ),
         Positioned.fill(
            child: Material(
              color: Colors.transparent,
              child: InkWell(
                onTap: () {
                  onPitTap(index);
                },
              ),
            ),
          ),
        ],
      );
  }
} 

