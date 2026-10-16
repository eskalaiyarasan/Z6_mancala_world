import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'setting_page.dart';
import 'theme/theme_selection_widget.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  runApp(const MainApp());
}

class MainApp extends StatelessWidget {
  const MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => Z6SettingProvider()),
        ChangeNotifierProvider(create: (_) => Z6ThemeProvider()),
      ],
      child: const MaterialApp(
        title: 'Z6 Mancala World',
        home: RootHomeScreen(),
      ),
    );
  }
}

class RootHomeScreen extends StatelessWidget {
  const RootHomeScreen({super.key});

  @override
  Widget build(BuildContext context) {
    // Get the current theme background color using Provider[cite: 2, 3]
    final backgroundColor = Provider.of<Z6ThemeProvider>(context).currentTheme.color?.bg;

    return Scaffold(
      backgroundColor: backgroundColor, // Background color updates automatically[cite: 1, 2, 3]
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            TextButton(
              onPressed: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => const Z6SettingScreen()),
                );
              },
              child: const Text('Setting'),
            ),
          ],
        ),
      ),
    );
  }
}