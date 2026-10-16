import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_soloud/flutter_soloud.dart';
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
      child: MaterialApp(
        title: 'Z6 Mancala World',
        home: const RootHomeScreen(),
      ),
    );
  }
}

class RootHomeScreen extends StatefulWidget {
  const RootHomeScreen({super.key});

  @override
  State<RootHomeScreen> createState() => _RootHomeScreenState();
}

class _RootHomeScreenState extends State<RootHomeScreen> {
  bool _isAudioInitialized = false;

  Future<void> _initAudio() async {
    if (_isAudioInitialized) return;
    try {
      await SoLoud.instance.init();
      setState(() {
        _isAudioInitialized = true;
      });
      debugPrint("SoLoud initialized successfully!");
    } catch (e) {
      debugPrint("Failed to initialize SoLoud: $e");
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            TextButton(
              onPressed: () async {
                // Initialize audio on user click to satisfy browser policies
                await _initAudio();
                if (!context.mounted) return;
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
