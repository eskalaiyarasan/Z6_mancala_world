/// Project: Z6 Mancala World
/// File: main.dart

import 'package:flutter/material.dart';

import 'ai.dart';
import 'human.dart';

class Z6TempContainer extends StatelessWidget {
  final String value;
  final VoidCallback callbk;

  Z6TempContainer(this.value, this.callbk, {super.key});
  @override
  Widget build(BuildContext context) {
    return Container(
      margin: const EdgeInsets.only(bottom: 12.0), // Spacing between items
      decoration: BoxDecoration(
        color: Colors.grey[200],
        borderRadius: BorderRadius.circular(12.0),
        border: Border.all(color: Colors.blue, width: 1),
      ),
      child: TextButton(
        onPressed: () {
          callbk();
        },
        child: Text(value),
      ),
    );
  }
}

class Z6IndianClassicPage extends StatelessWidget {
  Z6IndianClassicPage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('New Game')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text('Play with Bot '),
                Z6TempContainer('Easy', () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => Z6IndianClassicAI(level: 1),
                    ),
                  );
                }),
                Z6TempContainer('Medium', () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => Z6IndianClassicAI(level: 2),
                    ),
                  );
                }),
                Z6TempContainer('Hard', () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => Z6IndianClassicAI(level: 3),
                    ),
                  );
                }),
              ],
            ),
            SizedBox(height: 20),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Text('Play with Human'),
                Z6TempContainer('Easy', () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => Z6IndianClassicHuman(level: 1),
                    ),
                  );
                }),
                Z6TempContainer('Medium', () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => Z6IndianClassicHuman(level: 2),
                    ),
                  );
                }),
                Z6TempContainer('Hard', () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => Z6IndianClassicHuman(level: 3),
                    ),
                  );
                }),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
