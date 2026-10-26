/// Project: Z6 Mancala World
/// File: z6_indian_classic_opponent.dart

import 'package:flutter/material.dart';

class Z6IndianClassicOpponent extends StatefulWidget {
  final int aiSide;
  final bool Function() isAiTurn;
  final List<int> Function() getAiPitValues;
  final Function(int pitIndex) onMakeMove;

  const Z6IndianClassicOpponent({
    super.key,
    required this.aiSide,
    required this.isAiTurn,
    required this.getAiPitValues,
    required this.onMakeMove,
  });

  @override
  State<Z6IndianClassicOpponent> createState() => _Z6IndianClassicOpponentState();
}

class _Z6IndianClassicOpponentState extends State<Z6IndianClassicOpponent> {
  bool _isThinking = false;

  void _evaluateAndPlay() async {
    if (_isThinking) return;

    if (widget.isAiTurn()) {
      setState(() => _isThinking = true);
      // Simulate realistic AI thinking delay
      await Future.delayed(const Duration(milliseconds: 900));

      final pitValues = widget.getAiPitValues();
      List<int> validIndices = [];
      
      for (int i = 0; i < pitValues.length; i++) {
        if (pitValues[i] > 0) {
          validIndices.add(i);
        }
      }

      if (validIndices.isNotEmpty) {
        validIndices.shuffle();
        int chosenPitIndex = validIndices.first;
        widget.onMakeMove(chosenPitIndex);
      }

      if (mounted) {
        setState(() => _isThinking = false);
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    _evaluateAndPlay();
    return const SizedBox.shrink(); // Invisible background controller widget
  }
}