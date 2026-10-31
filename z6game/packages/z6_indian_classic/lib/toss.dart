/// Project: Z6 Mancala World
/// File: z6_indian_classic_toss.dart

import 'dart:math';

import 'package:flutter/material.dart';

import 'engine_interface.dart';

class Z6IndianClassicToss extends StatefulWidget {
  final String id;

  const Z6IndianClassicToss({super.key, required this.id});

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
    Future.delayed(const Duration(seconds: 1), () {
      if (!mounted) return;
      final random = Random();
      var engine = Z6IndianClassicInterface.get(widget.id);
      final winner = random.nextBool() ? engine?.player1 : engine?.player2;
      setState(() {
        _isTossing = false;
        _tossWinner = winner;
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: const Text("Tossing for First Turn"),
      content: SizedBox(
        height: 100,
        child: Center(
          child: _isTossing
              ? const CircularProgressIndicator()
              : Text(
                  "$_tossWinner won the toss!",
                  style: const TextStyle(
                    fontSize: 18,
                    fontWeight: FontWeight.bold,
                  ),
                  textAlign: TextAlign.center,
                ),
        ),
      ),
      actions: [
        if (!_isTossing)
          ElevatedButton(
            onPressed: () {
              if (_tossWinner != null) {
                Z6IndianClassicInterface.start(widget.id, _tossWinner ?? "AI");
              }
              Navigator.of(context).pop();
            },
            child: const Text("Start Game"),
          ),
      ],
    );
  }
}
