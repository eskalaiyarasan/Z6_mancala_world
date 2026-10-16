/// Copyright (c) 2026 Zerasix Gaming Studio LLP. All rights reserved.
/// 
/// Project: Z6 Mancala World
/// File: honeycomb.dart
/// Author:  Kalaiyarasan
/// Date:  13/10/2026
/// 
/// Notice: All information contained herein is, and remains the property of 
/// Zerasix Gaming Studio LLP. The intellectual and technical concepts contained 
/// herein are proprietary to Zerasix Gaming Studio LLP and may be covered by 
/// Indian and foreign patents or trademarks, and are protected by trade secret 
/// or copyright law. Dissemination of this information or reproduction of this 
/// material is strictly forbidden unless prior written permission is obtained 
/// from Zerasix Gaming Studio LLP.
import 'package:flutter/material.dart';
import 'package:hexagon/hexagon.dart';
import '../theme_template.dart';
import 'package:liquid_bottle/liquid_bottle.dart';

const TextStyle honeyText = TextStyle(
  fontFamily: 'Fredoka', 
  color: Colors.black,
  fontWeight: FontWeight.bold,
  fontSize: 18,
  letterSpacing: 0.5,
);

class Z6HoneycombGhostCell extends StatelessWidget {
  const Z6HoneycombGhostCell({super.key});

  @override
  Widget build(BuildContext context) {
    return HexagonWidget.flat(
      width: 100,
      color: Colors.brown,
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: const [
            Text('GHOST', style: honeyText),
          ],
        ),
      ),
    );
  }
}

class Z6HoneycombWorkerCell extends StatelessWidget {
  final int number;
  const Z6HoneycombWorkerCell(this.number, {super.key});

  @override
  Widget build(BuildContext context) {
    return HexagonWidget.flat(
      width: 100,
      color: Colors.limeAccent,
      child: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(number.toString(), style: honeyText),
            const SizedBox(height: 4),
            for (int i = 0; i < 10; i++)
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  for (int j = 0; j < 10; j++)
                    Container(
                      width: 5,
                      height: 5,
                      color: number > i * 10 + j ? Colors.amber : Colors.limeAccent,
                    ),
                ],
              ),
          ],
        ),
      ),
    );
  }
}

class Z6HoneycombQueenCell extends StatelessWidget {
  final int number;
  const Z6HoneycombQueenCell(this.number, {super.key});

  @override
  Widget build(BuildContext context) {
    return HexagonWidget.flat(
      width: 110,
      color: Colors.grey,
      child: Z6HoneycombWorkerCell(number),
    );
  }
}

class Z6HoneyBottle extends StatelessWidget {
  final int number;
  const Z6HoneyBottle(this.number, {super.key});

  @override
  Widget build(BuildContext context) {
    final int clampedValue = number.clamp(0, 200);
    final double progress = clampedValue / 200.0;

    return Scaffold(
      appBar: AppBar(title: Text(number.toString(), style: honeyText)),
      body: LiquidBottleSlider(
        bottleType: BottleType.standards.first,
        value: progress,
      ),
    );
  }
}

class Z6Honeycomb implements Z6ThemeBlueprint {
  @override
  Z6ThemeTemplate generate() {
    Z6ThemeSound sound = Z6ThemeSound(drop: 'assert/sounds/bee_drop.mp3',
                        scoop: 'assert/sounds/bee_scoop.mp3',
                        capture: 'assert/sounds/bee_capture.mp3',
                        win: 'assert/sounds/bee_win.mp3',
                        lose: 'assert/sounds/bee_lose.mp3',
                        other: 'assert/sounds/bee_other.mp3',
                        );
    Z6ThemePit pit = Z6ThemePit(
      normal: Z6HoneycombWorkerCell(0), 
      special: Z6HoneycombQueenCell(0), 
      store: Z6HoneycombWorkerCell(0), 
      inactive: Z6HoneycombGhostCell(),
    ); 
    Z6ThemeColor color = Z6ThemeColor(
      bg: Colors.yellow, 
      text: honeyText,
      empty: Colors.yellow, 
      fill: Colors.amber,
    );
               
    return Z6ThemeTemplate(
      sound: sound, 
      pit: pit, 
      theme: Z6Theme.Honeycomb, 
      color: color,
    );
  }
}
