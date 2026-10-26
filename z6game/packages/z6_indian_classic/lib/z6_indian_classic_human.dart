/// Project: Z6 Mancala World
/// File: z6_indian_classic_human.dart

import 'package:flutter/material.dart';
import 'z6_indian_classic_board.dart';
import 'z6_indian_classic_toss.dart';

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
  final GlobalKey<Z6IndianClassicBoardState> _boardKey = GlobalKey<Z6IndianClassicBoardState>();

  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addPostFrameCallback((_) => _showTossDialog());
  }

  void _showTossDialog() {
    showDialog(
      context: context,
      barrierDismissible: false,
      builder: (context) => Z6IndianClassicToss(
        player1: widget.player1,
        player2: widget.player2,
        onTossComplete: (winner) async {
          await _boardKey.currentState?.start(winner);
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Z6IndianClassicBoard(
        key: _boardKey,
        level: widget.level,
        player1: widget.player1,
        player2: widget.player2,
      ),
    );
  }
}