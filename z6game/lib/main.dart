/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: main.dart
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
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'setting_page.dart';
import 'package:z6_theme/theme_selection_widget.dart';
import 'package:z6_indian_classic/z6_indian_classic_page.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  runApp(MainApp());
}

class MainApp extends StatelessWidget {
   MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => Z6ThemeProvider()),
      ],
      child: MaterialApp(
        title: 'Z6 Mancala World',
        home: RootHomeScreen(),
      ),
    );
  }
}

class RootHomeScreen extends StatelessWidget {
  RootHomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    // Get the current theme background color using Provider[cite: 2, 3]
    final backgroundColor = Provider.of<Z6ThemeProvider>(context).currentTheme.color?.bg;

    return Scaffold(
      backgroundColor: backgroundColor, // Background color updates automatically[cite: 1, 2, 3]
      body: Center(
        child: ListView(
          padding:  EdgeInsets.all(16.0),
          children: [
            ListTile(
              onTap: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => Z6IndianClassicPage()),
                );
              },
              title:  Text('Z6-Indian-Classic'),
              trailing:  Icon(Icons.arrow_forward_ios, size: 16),
              leading: Text( '🇮🇳', style: TextStyle(fontSize: 24), // Adjust size to match standard icons
              ),
            ), 
            ListTile(
              onTap: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => Z6SettingScreen()),
                );
              },
              title: Text('Settings'),
              trailing:  Icon(Icons.arrow_forward_ios, size: 16),
              leading:  Icon(Icons.settings),
            ),
            
          ],
        ),
      ),
    );
  }
}
