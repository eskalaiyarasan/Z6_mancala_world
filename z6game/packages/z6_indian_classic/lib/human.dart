/// Project: Z6 Mancala World
/// File: z6_indian_classic_human.dart

import 'package:flutter/material.dart';

import 'board.dart';
import 'toss.dart';
import 'engine.dart';
import 'engine_interface.dart';

class Z6IndianClassicHuman extends StatefulWidget {
  final int level;
  final String player1;
  final String player2;

  const Z6IndianClassicHuman({
    super.key,
    required this.level,
    this.player1 = "Player 1",
    this.player2 = "Player 2",
  });

  @override
  State<Z6IndianClassicHuman> createState() => _Z6IndianClassicHumanState();
}

class _Z6IndianClassicHumanState extends State<Z6IndianClassicHuman> {
  late String id;
  @override
  void initState() {
    super.initState();
    id = Z6IndianClassicInterface.create(
      widget.level,
      widget.player1,
      widget.player2,
    );
    WidgetsBinding.instance.addPostFrameCallback((_) => _showTossDialog());
  }

  void _showTossDialog() {
    showDialog(
      context: context,
      barrierDismissible: false,
      builder: (context) => Z6IndianClassicToss(id: id),
    );
  }

  @override
  void dispose() {
    Z6IndianClassicInterface.destroy(id);
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(body: Z6IndianClassicBoard(id: id, side: 0, both: true));
  }
}
