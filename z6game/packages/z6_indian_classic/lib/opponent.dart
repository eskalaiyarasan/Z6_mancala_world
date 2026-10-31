/// Project: Z6 Mancala World
/// File: z6_indian_classic_opponent.dart

import 'package:flutter/material.dart';

import 'engine.dart';
import 'engine_interface.dart';

class Z6IndianClassicOpponent extends StatefulWidget {
  final int side;
  final String id;

  const Z6IndianClassicOpponent({
    super.key,
    required this.side,
    required this.id,
  });

  @override
  State<Z6IndianClassicOpponent> createState() =>
      _Z6IndianClassicOpponentState();
}

class _Z6IndianClassicOpponentState extends State<Z6IndianClassicOpponent> {
  bool _isThinking = false;
  late Z6IndianClassicEngine? _engine;

  @override
  void initState() {
    _engine = Z6IndianClassicInterface.get(widget.id);
    super.initState();
    Z6IndianClassicInterface.observe(
      widget.id,
      Z6IndianClassicEvent.turn,
      evaluateAndPlay,
    );
  }

  void evaluateAndPlay() async {
    if (_engine?.turn == widget.side) {
      setState(() => _isThinking = true);
      // Simulate realistic AI thinking delay
      await Future.delayed(const Duration(seconds: 20));

      List<int> validIndices = [];
      var pits = _engine?.sides[widget.side].pits;
      for (var pit in pits ?? []) {
        if (pit.active && (pit.value > 0)) {
          validIndices.add(pit.index);
        }
      }

      if (validIndices.isNotEmpty) {
        validIndices.shuffle();
        int chosenPitIndex = validIndices.first;
        Z6IndianClassicInterface.move(widget.id, widget.side, chosenPitIndex);
      }
      if (mounted) {
        setState(() => _isThinking = false);
      }
      _isThinking = false;
    }
  }

  @override
  Widget build(BuildContext context) {
    return _isThinking
        ? SizedBox(width: 200, child: Text("AI Thinking"))
        : SizedBox.shrink(); // Invisible background controller widget
  }

  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
  }
}
