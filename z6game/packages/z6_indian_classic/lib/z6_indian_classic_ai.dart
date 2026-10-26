/// Project: Z6 Mancala World
/// File: z6_indian_classic_ai.dart

import 'package:flutter/material.dart';
import 'z6_indian_classic_board.dart';
import 'z6_indian_classic_toss.dart';
import 'z6_indian_classic_opponent.dart';

class Z6IndianClassicAI extends StatefulWidget {
  final int level;
  final String currentUserName;

  const Z6IndianClassicAI({
    super.key,
    required this.level,
    this.currentUserName = "Player 1",
  });

  @override
  State<Z6IndianClassicAI> createState() => _Z6IndianClassicAIState();
}

class _Z6IndianClassicAIState extends State<Z6IndianClassicAI> {
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
        player1: widget.currentUserName,
        player2: "Computer",
        onTossComplete: (winner) async {
          await _boardKey.currentState?.start(winner);
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          Z6IndianClassicBoard(
            key: _boardKey,
            level: widget.level,
            player1: widget.currentUserName,
            player2: "Computer",
          ),
          // Embedded AI Opponent Listener interacting via GlobalKey hooks
          Z6IndianClassicOpponent(
            aiSide: 1,
            isAiTurn: () => _boardKey.currentState?.isTurn(1) ?? false,
            getAiPitValues: () => _boardKey.currentState?.getSidePitValues(1) ?? [],
            onMakeMove: (pitIndex) {
              _boardKey.currentState?.handleAiPitTap(1, pitIndex);
            },
          ),
        ],
      ),
    );
  }
}