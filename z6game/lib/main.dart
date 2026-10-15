import 'package:flutter/material.dart';
import 'package:flutter_soloud/flutter_soloud.dart';
import 'setting_page.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  try {
    await SoLoud.instance.init();
  } catch (e) {
    debugPrint("Failed to initialize SoLoud: $e");
  }
  runApp(MainApp());
}

class MainApp extends StatelessWidget {
  MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        body: Center(
          child: TextButton(
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const Z6SettingScreen()),
              );
            },
            child: const Text('Setting'),
          ),
        ),
      ),
    );
  }
}
