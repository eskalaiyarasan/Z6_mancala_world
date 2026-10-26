/// Project: Z6 Mancala World
/// File: main.dart

import 'package:flutter/material.dart';
import 'z6_indian_classic_ai.dart';

class Z6IndianClassicPage extends StatelessWidget {
  const Z6IndianClassicPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Text('NewGame : play with Bot '),
                TextButton(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const Z6IndianClassicAI(level: 1)),
                    );
                  },
                  child: const Text('Easy'),
                ),
                TextButton(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const Z6IndianClassicAI(level: 2)),
                    );
                  },
                  child: const Text('Medium'),
                ),
                TextButton(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const Z6IndianClassicAI(level: 3)),
                    );
                  },
                  child: const Text('Hard'),
                ),
              ],
            ),
            const SizedBox(height: 20),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Text('NewGame : play with Human'),
                TextButton(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const Z6IndianClassicHuman(level: 1)),
                    );
                  },
                  child: const Text('Easy'),
                ),
                TextButton(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const Z6IndianClassicHuman(level: 2)),
                    );
                  },
                  child: const Text('Medium'),
                ),
                TextButton(
                  onPressed: () {
                    Navigator.push(
                      context,
                      MaterialPageRoute(builder: (context) => const Z6IndianClassicHuman(level: 3)),
                    );
                  },
                  child: const Text('Hard'),
                ),
              ],
            ),
          ],
        ),
      ),
    ); 
  }
}