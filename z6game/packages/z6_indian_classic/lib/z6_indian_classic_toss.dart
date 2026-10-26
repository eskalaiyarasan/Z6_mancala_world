/// Project: Z6 Mancala World
/// File: z6_indian_classic_toss.dart

import 'dart:math';
import 'package:flutter/material.dart';

class Z6IndianClassicToss extends StatefulWidget {
  final String player1;
  final String player2;
  final Function(String winner) onTossComplete;

  const Z6IndianClassicToss({
    super.key,
    required this.player1,
    required this.player2,
    required this.onTossComplete,
  });

  @override
  State<Z6IndianClassicToss> createState() => _Z6IndianClassicTossState();
}

class _Z6IndianClassicTossState extends State<Z6IndianClassicToss> {
  bool _isTossing = true;
  String? _tossWinner;

  @override
  void initState() {
    super.initState();
    _executeToss();
  }

  void _executeToss() {
    setState(() => _isTossing = true);
    Future.delayed(const Duration(seconds: 1), () {
      final random = Random();
      final winner = random.nextBool() ? widget.player1 : widget.player2;
      setState(() {
        _isTossing = false;
        _tossWinner = winner;
      });
      widget.onTossComplete(winner);
    });
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: const Text("Tossing for First Turn..."),
      content: SizedBox(
        height: 100,
        child: Center(
          child: _isTossing
              ? const CircularProgressIndicator()
              : Text(
                  "$_tossWinner won the toss!",
                  style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
                  textAlign: TextAlign.center,
                ),
        ),
      ),
      actions: [
        if (!_isTossing)
          TextButton(
            onPressed: () => Navigator.of(context).pop(),
            child: const Text("Start Game"),
          ),
      ],
    );
  }
}